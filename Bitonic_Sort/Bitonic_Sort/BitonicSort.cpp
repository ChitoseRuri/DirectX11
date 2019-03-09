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

std::vector<UINT> BitonicSort::getRandomVector(_In size_t size, _In UINT min, _In UINT max)
{
	m_RandEngine.seed(std::random_device()());
	std::uniform_int_distribution<UINT> randRange(min, max);
	std::vector<UINT> outVector(size);
	std::generate(outVector.begin(), outVector.end(), [&] {return randRange(m_RandEngine); });
	return outVector;
}

void BitonicSort::sort(_In std::vector<UINT>& vIn)
{
	const size_t count = vIn.size();//ԭ����Ԫ�ظ���
	size_t tarCount = 2;
	//��ȫ���뵽2�Ĵ���
	while (tarCount < count)
	{
		tarCount *= 2;
	}
	vIn.resize(tarCount, UINT_MAX);//��vector���䵽2�Ĵ���
	createDesc(vIn.data(), tarCount * sizeof(UINT), tarCount);
	if (tarCount <= _2DSize)
	{
		sort2D(vIn, tarCount);
	}
	else
	{
		MessageBox(nullptr, L"There are too many numbers for Bitonic Sort to sort.", L"Warning", 0);
		exit(0);
	}
	//��GPU���ݴ�CPU���ɷ��ʻ��浼��ɷ��ʻ���
	m_D3dImmediateContext->CopyResource(m_TypedBufferCopy.Get(), m_TypedBuffer1.Get());
	//�����ݴ�GPU���浼����
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_D3dImmediateContext->Map(m_TypedBufferCopy.Get(), 0, D3D11_MAP_READ, 0, &mappedData));
	//��vector�Ĵ�С�ظ���ԭ����С
	vIn.resize(count);
	//�ѵ����������ݸ��Ƶ�vector��
	const size_t srcSize = count * sizeof(int);
	memcpy_s(vIn.data(), srcSize, mappedData.pData, srcSize);
}

inline void BitonicSort::initDirect3D()
{
	HRESULT hr = S_OK;

	//����D3D�豸�Լ�������
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//֧��Direct2D�ĸ�ʽ
	//�����Debugģʽ������Direct11ͼ��Debug
#if defined(_DEBUG) || defined(DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif  
	//������������
	D3D_DRIVER_TYPE driverTypes[] { 
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE 
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	//���Եȼ�����
	D3D_FEATURE_LEVEL featureLevels[]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	for (auto & d3dDriverType : driverTypes)
	{
		//����Ӳ���Ƿ�֧��Direct3D11.1
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels,
			D3D11_SDK_VERSION, m_D3dDevice.GetAddressOf(), &featureLevel, m_D3dImmediateContext.GetAddressOf());

		if (hr == E_INVALIDARG)
		{
			// Direct3D 11.0 ��API������D3D_FEATURE_LEVEL_11_1������������Ҫ�������Եȼ�11.0�Լ����µİ汾
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
	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(����DXGI1.1)�Ľӿ���
	ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(����DXGI1.2)���еĽӿ���

	//�Ȼ�ȡD3D�豸��DXGI����
	HR(m_D3dDevice.As(&dxgiDevice));
	HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void **>(dxgiFactory1.GetAddressOf())));

	//�����豸�Ƿ�֧��IDXGIFactory2�Ľӿ�
	hr = dxgiFactory1.As(&dxgiFactory2);
	//���������˵��֧��D3D11.1
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

inline void BitonicSort::createDesc(_In void * pData, _In size_t size, _In size_t count)
{
	HR(CreateTypedBuffer(m_D3dDevice.Get(), pData, size,
		m_TypedBuffer1.GetAddressOf(), false, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), nullptr, size,
		m_TypedBuffer2.GetAddressOf(), false, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), nullptr, size,
		m_TypedBufferCopy.GetAddressOf(), true, true));

	HR(CreateTypedBuffer(m_D3dDevice.Get(), nullptr, sizeof(CB),//����CB�Ĵ�С��֤��16�ֽڴ���
		m_ConstantBuffer.GetAddressOf(), false, true));

	//������ɫ����Դ��ͼ
	D3D11_SHADER_RESOURCE_VIEW_DESC	srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_UINT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = count;
	HR(m_D3dDevice->CreateShaderResourceView(m_TypedBuffer1.Get(), &srvDesc, m_DataSRV1.GetAddressOf()));
	HR(m_D3dDevice->CreateShaderResourceView(m_TypedBuffer2.Get(), &srvDesc, m_DataSRV2.GetAddressOf()));

	//�������������ͼ
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R32_UINT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = count;
	HR(m_D3dDevice->CreateUnorderedAccessView(m_TypedBuffer1.Get(), &uavDesc, m_DataUAV1.GetAddressOf()));
	HR(m_D3dDevice->CreateUnorderedAccessView(m_TypedBuffer2.Get(), &uavDesc, m_DataUAV2.GetAddressOf()));

	m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
	m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV1.GetAddressOf(), nullptr);
}

void BitonicSort::sort1D(_Both std::vector<UINT>& vIn, _In size_t size)
{
	const auto & tarCount = size;

	// �������ݽ�������������level <= BLOCK_SIZE ���������
	CB cb{};
	for (UINT level = 2, xIndex = (tarCount + BITONIC_BLOCK_SIZE - 1) / BITONIC_BLOCK_SIZE;
		level <= tarCount && level <= BITONIC_BLOCK_SIZE; level *= 2)
	{
		cb.level = cb.descendMask = level;
		setConstants(cb);
		m_D3dImmediateContext->Dispatch(xIndex, 1, 1);
	}

	// ������������ߣ���>=���Ҷ�Ϊ2�Ĵ��ݣ�
	UINT matrixWidth = 2;
	while (matrixWidth * matrixWidth < tarCount)
	{
		matrixWidth *= 2;
	}
	UINT matrixHeight = tarCount / matrixWidth;
	cb.matrixHeight = matrixHeight;
	cb.matrixWidth = matrixWidth;

	const UINT tranWidth = matrixWidth / TRANSPOSE_BLOCK_SIZE;
	const UINT tranHeight = matrixHeight / TRANSPOSE_BLOCK_SIZE;
	const UINT eachSize = tarCount / BITONIC_BLOCK_SIZE;

	// ����level > BITONIC_BLOCK_SIZE ���������
	ComPtr<ID3D11ShaderResourceView> pNullSRV;
	for (UINT level = BITONIC_BLOCK_SIZE * 2; level <= tarCount; level *= 2)
	{
		// ����ﵽ��ߵȼ�����Ϊȫ��������
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
		// �Ƚ���ת�ã��������������Buffer2
		m_D3dImmediateContext->CSSetShader(m_MatrixTranspose_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, pNullSRV.GetAddressOf());
		m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV2.GetAddressOf(), nullptr);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, m_DataSRV1.GetAddressOf());
		m_D3dImmediateContext->Dispatch(tranWidth, tranHeight, 1);

		// ��Buffer2����������
		m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->Dispatch(eachSize, 1, 1);

		// ����ת�û������������������Buffer1
		cb.level = matrixWidth;
		cb.descendMask = level;
		setConstants(cb);
		m_D3dImmediateContext->CSSetShader(m_MatrixTranspose_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, pNullSRV.GetAddressOf());
		m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV1.GetAddressOf(), nullptr);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, m_DataSRV2.GetAddressOf());
		m_D3dImmediateContext->Dispatch(tranWidth, tranHeight, 1);

		// ��Buffer1����ʣ��������
		m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->Dispatch(eachSize, 1, 1);
	}
}

void BitonicSort::sort2D(_Both std::vector<UINT>& vIn, _In size_t size)
{
	const auto & tarCount = size;

	// �������ݽ�������������level <= BLOCK_SIZE ���������
	CB cb{};
	for (UINT level = 2, xIndex = (tarCount + BITONIC_BLOCK_SIZE - 1) / BITONIC_BLOCK_SIZE;
		level <= tarCount && level <= BITONIC_BLOCK_SIZE; level *= 2)
	{
		cb.level = cb.descendMask = level;
		setConstants(cb);
		m_D3dImmediateContext->Dispatch(xIndex, 1, 1);
	}

	//������������ߣ���>=���Ҷ�Ϊ2�Ĵ��ݣ�
	UINT matrixWidth = 2;
	while (matrixWidth * matrixWidth < tarCount)
	{
		matrixWidth *= 2;
	}
	UINT matrixHeight = tarCount / matrixWidth;
	cb.matrixHeight = matrixHeight;
	cb.matrixWidth = matrixWidth;

	const UINT tranWidth = matrixWidth / TRANSPOSE_BLOCK_SIZE;
	const UINT tranHeight = matrixHeight / TRANSPOSE_BLOCK_SIZE;
	const UINT eachSize = tarCount / BITONIC_BLOCK_SIZE;

	// ����level > BITONIC_BLOCK_SIZE ���������
	ComPtr<ID3D11ShaderResourceView> pNullSRV;
	for (UINT level = BITONIC_BLOCK_SIZE * 2; level <= tarCount; level *= 2)
	{
		// ����ﵽ��ߵȼ�����Ϊȫ��������
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
		// �Ƚ���ת�ã��������������Buffer2
		m_D3dImmediateContext->CSSetShader(m_MatrixTranspose_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, pNullSRV.GetAddressOf());
		m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV2.GetAddressOf(), nullptr);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, m_DataSRV1.GetAddressOf());
		m_D3dImmediateContext->Dispatch(tranWidth, tranHeight, 1);

		// ��Buffer2����������
		m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->Dispatch(eachSize, 1, 1);

		// ����ת�û������������������Buffer1
		cb.level = matrixWidth;
		cb.descendMask = level;
		setConstants(cb);
		m_D3dImmediateContext->CSSetShader(m_MatrixTranspose_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, pNullSRV.GetAddressOf());
		m_D3dImmediateContext->CSSetUnorderedAccessViews(0, 1, m_DataUAV1.GetAddressOf(), nullptr);
		m_D3dImmediateContext->CSSetShaderResources(0, 1, m_DataSRV2.GetAddressOf());
		m_D3dImmediateContext->Dispatch(tranWidth, tranHeight, 1);

		// ��Buffer1����ʣ��������
		m_D3dImmediateContext->CSSetShader(m_BitonicSort_CS.Get(), nullptr, 0);
		m_D3dImmediateContext->Dispatch(eachSize, 1, 1);
	}
}

void BitonicSort::sort3D(_Both std::vector<UINT>& vIn, _In size_t size)
{
}
