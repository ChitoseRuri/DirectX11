#include "BitonicSort.h"

#define BITONIC_BLOCK_SIZE 512
#define TRANSPOSE_BLOCK_SIZE 16

BitonicSort::BitonicSort()
{
	initDirect3D();
	initComputeShader();
}

BitonicSort::BitonicSort(const ComPtr<ID3D11Device>& pID3D11Device, const ComPtr<ID3D11DeviceContext>& pD3d11DeviceContext):
	m_D3dDevice(pID3D11Device),
	m_D3dImmediateContext(pD3d11DeviceContext)
{
	assert(pID3D11Device);
	assert(pD3d11DeviceContext);
	initComputeShader();
}

BitonicSort::~BitonicSort()
{
}

std::vector<int> BitonicSort::getRandomVector(_In size_t size, _In int min, _In int max)
{
	m_RandEngine.seed(std::random_device()());
	std::uniform_int_distribution<int> randRange(min, max);
	std::vector<int> outVector(size);
	std::generate(outVector.begin(), outVector.end(), [&] {return randRange(m_RandEngine); });
	return outVector;
}

void BitonicSort::sort(_In std::vector<int>& vIn)
{
	size_t count = vIn.size();//原来的元素个数
	size_t tarCount = 2;
	//补全对齐到2的次幂
	while (tarCount < count)
	{
		tarCount *= 2;
	}
	vIn.resize(tarCount, INT_MAX);//把vector扩充到2的次幂
	const size_t tarSize = tarCount * sizeof(int);

	HR(CreateTypedBuffer(m_D3dDevice.Get(), vIn.data(), tarSize,
		m_TypedBuffer1.GetAddressOf(), false, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), nullptr, tarSize,
		m_TypedBuffer2.GetAddressOf(), false, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), nullptr, tarSize,
		m_TypedBufferCopy.GetAddressOf(), true, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), nullptr, sizeof(CB),//这里CB的大小保证是16字节次幂
		m_ConstantBuffer.GetAddressOf(), false, true));

	//创建着色器资源视图
	D3D11_SHADER_RESOURCE_VIEW_DESC	srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_UINT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = tarCount;
	HR(m_D3dDevice->CreateShaderResourceView(m_TypedBuffer1.Get(), &srvDesc, m_DataSRV1.GetAddressOf()));
	HR(m_D3dDevice->CreateShaderResourceView(m_TypedBuffer2.Get(), &srvDesc, m_DataSRV2.GetAddressOf()));

	//创建无序访问视图
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R32_UINT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = tarCount;
	HR(m_D3dDevice->CreateUnorderedAccessView(m_TypedBuffer1.Get(), &uavDesc, m_DataUAV1.GetAddressOf()));
	HR(m_D3dDevice->CreateUnorderedAccessView(m_TypedBuffer2.Get(), &uavDesc, m_DataUAV2.GetAddressOf()));

	m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
	m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV1.GetAddressOf(), nullptr);

	// 按行数据进行排序，先排序level <= BLOCK_SIZE 的所有情况
	CB cb{};
	const unsigned times = (tarCount + BITONIC_BLOCK_SIZE - 1) / BITONIC_BLOCK_SIZE;
	for (unsigned level = 2; level <= tarCount && level <= BITONIC_BLOCK_SIZE; level *= 2)
	{
		cb.level = cb.descendMask = level;
		setConstants(cb);
		m_D3dImmediateContext->Dispatch(times, 1, 1);
	}

	//计算相近矩阵宽高（宽>=高且都为2的次幂）
	unsigned matrixHeight = 2, matrixWidth = 2;
	while (matrixHeight * matrixWidth < tarCount)
	{
		matrixWidth *= 2;
	}
	matrixHeight = tarCount / matrixWidth;
	cb.matrixHeight = matrixHeight;
	cb.matrixWidth = matrixWidth;

	const unsigned tranWidth = matrixWidth / TRANSPOSE_BLOCK_SIZE;
	const unsigned tranHeight = matrixHeight / TRANSPOSE_BLOCK_SIZE;
	const unsigned eachSize = tarCount / BITONIC_BLOCK_SIZE;

	// 排序level > BLOCK_SIZE 的所有情况
	ComPtr<ID3D11ShaderResourceView> pNullSRV;
	for (unsigned level = BITONIC_BLOCK_SIZE * 2; level <= tarCount; level *= 2)
	{
		// 如果达到最高等级，则为全递增序列
		if (level == tarCount)
		{
			cb.level = level / matrixWidth;
			cb.descendMask = level;
			setConstants(cb);
		}
		else
		{
			cb.level = cb.descendMask = level / matrixWidth;
			setConstants(cb);
		}
		// 先进行转置，并把数据输出到Buffer2
		m_D3dImmediateContext->CSSetShader(m_MatrixTranspose_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, pNullSRV.GetAddressOf());
		m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV2.GetAddressOf(), nullptr);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, m_DataSRV1.GetAddressOf());
		m_D3dImmediateContext->Dispatch(tranWidth, tranHeight, 1);

		// 对Buffer2排序列数据
		m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->Dispatch(eachSize, 1, 1);

		// 接着转置回来，并把数据输出到Buffer1
		cb.level = matrixWidth;
		cb.descendMask = level;
		setConstants(cb);
		m_D3dImmediateContext->CSSetShader(m_MatrixTranspose_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, pNullSRV.GetAddressOf());
		m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV1.GetAddressOf(), nullptr);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, m_DataSRV1.GetAddressOf());
		m_D3dImmediateContext->Dispatch(tranWidth, tranHeight, 1);

		// 对Buffer1排序剩余行数据
		m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->Dispatch(eachSize, 1, 1);
	}

	//把CPU不可访问的GPU数据导入可访问缓存
	m_D3dImmediateContext->CopyResource(m_TypedBufferCopy.Get(), m_TypedBuffer1.Get());
	//把数据从GPU里面导出来
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_D3dImmediateContext->Map(m_TypedBufferCopy.Get(), 0, D3D11_MAP_READ, 0, &mappedData));
	//把vector的大小回复正常
	vIn.resize(count);
	//把导出来的数据复制到vector中
	const size_t srcSize = count * sizeof(int);
	memcpy_s(vIn.data(), srcSize, mappedData.pData, srcSize);
}

inline void BitonicSort::initDirect3D()
{
	HRESULT hr = S_OK;

	//创建D3D设备以及上下文
	unsigned createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//支持Direct2D的格式
	//如果是Debug模式，开启Direct11图形Debug
#if defined(_DEBUG) || defined(DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif  
	//驱动类型数组
	D3D_DRIVER_TYPE driverTypes[] { 
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE 
	};
	unsigned numDriverTypes = ARRAYSIZE(driverTypes);

	//特性等级数组
	D3D_FEATURE_LEVEL featureLevels[]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1
	};
	unsigned numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	for (auto & d3dDriverType : driverTypes)
	{
		//尝试硬件是否支持Direct3D11.1
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels,
			D3D11_SDK_VERSION, m_D3dDevice.GetAddressOf(), &featureLevel, m_D3dImmediateContext.GetAddressOf());

		if (hr == E_INVALIDARG)
		{
			// Direct3D 11.0 的API不承认D3D_FEATURE_LEVEL_11_1，所以我们需要尝试特性等级11.0以及以下的版本
			hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[0], numFeatureLevels,
				D3D11_SDK_VERSION, m_D3dDevice.GetAddressOf(), &featureLevel, m_D3dImmediateContext.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			break;
		}
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		exit(0);
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		exit(0);
	}

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(包含DXGI1.1)的接口类
	ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(包含DXGI1.2)特有的接口类

	//先获取D3D设备的DXGI工厂
	HR(m_D3dDevice.As(&dxgiDevice));
	HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void **>(dxgiFactory1.GetAddressOf())));

	//检查该设备是否支持IDXGIFactory2的接口
	hr = dxgiFactory1.As(&dxgiFactory2);
	//如果包含，说明支持D3D11.1
	if (!dxgiFactory2)
	{
		HR(m_D3dDevice.As(&m_D3dDevice1));
		HR(m_D3dImmediateContext.As(&m_D3dImmediateContext1));
	}
}

inline void BitonicSort::initComputeShader()
{
	ComPtr<ID3DBlob>blob;
	HR(CreateShaderFromFile(L"HLSL\\BitonicSort_CS.cso", L"HLSL\\BitonicSort_CS.hlsl", "CS", "CS_5_0", blob.GetAddressOf()));
	HR(m_D3dDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_BitonicSort_CS.GetAddressOf()));
	HR(CreateShaderFromFile(L"HLSL\\MatrixTranspose_CS.cso", L"HLSL\\MatrixTranspose_CS.hlsl", "CS", "CS_5_0", blob.GetAddressOf()));
	HR(m_D3dDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_MatrixTranspose_CS.GetAddressOf()));
}

inline void BitonicSort::setConstants(_In const CB & cb)
{
	m_D3dImmediateContext->UpdateSubresource(m_ConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	m_D3dImmediateContext->CSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
}
