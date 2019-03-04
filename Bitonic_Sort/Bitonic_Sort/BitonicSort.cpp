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
	//��ȫ���뵽2�Ĵη�
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

	//����D3D�豸�Լ�������
	unsigned createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//֧��Direct2D�ĸ�ʽ
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
	unsigned numDriverTypes = ARRAYSIZE(driverTypes);

	//���Եȼ�����
	D3D_FEATURE_LEVEL featureLevels[]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1
	};
	unsigned numFeatureLevels = ARRAYSIZE(featureLevels);

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
