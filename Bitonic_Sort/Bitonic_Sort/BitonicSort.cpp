#include "BitonicSort.h"

BitonicSort::BitonicSort()
{
	initDirect3D();
	initComputeShader();
}

BitonicSort::BitonicSort(const ComPtr<ID3D11Device>& pID3D11Device, const ComPtr<ID3D11DeviceContext>& pD3d11DeviceContext):
	m_D3dDevice(pID3D11Device),
	m_D3dImmediateContext(pD3d11DeviceContext)
{
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

auto BitonicSort::sort(_In std::vector<int>& vIn)
{
	size_t count = vIn.size();
	size_t tarCount = 2;
	//补全对齐到2的次方
	while (tarCount < count)
	{
		tarCount *= 2;
	}
	vIn.resize(tarCount, INT_MAX);
	const size_t tarSize = tarCount * sizeof(int);

	HR(CreateTypedBuffer(m_D3dDevice.Get(), vIn.data(), tarSize,
		m_TypedBuffer1.GetAddressOf(), false, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), vIn.data(), tarSize,
		m_TypedBuffer2.GetAddressOf(), false, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), vIn.data(), tarSize,
		m_TypedBufferCopy.GetAddressOf(), false, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), vIn.data(), tarSize,
		m_ConstantBuffer.GetAddressOf(), false, true));
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
