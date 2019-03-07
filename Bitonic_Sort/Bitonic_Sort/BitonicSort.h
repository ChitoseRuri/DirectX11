#pragma once
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3d11_1.h>
#include <random>
#include <d2d1.h>
#include <dwrite.h>
#include <string>
#include <sstream>
#include <vector>
#include "d3dUtil.h"
#include "DXTrace.h"

// 添加所有要引用的库
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

#define _In
#define _Out
#define _Both

class BitonicSort
{
private:
	struct CB
	{
		unsigned level;
		unsigned descendMask;
		unsigned matrixWidth;
		unsigned matrixHeight;
	};

	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::mt19937 m_RandEngine;				//随机引擎

	// Direct3D 11
	ComPtr<ID3D11Device> m_D3dDevice;							// D3D11设备
	ComPtr<ID3D11DeviceContext> m_D3dImmediateContext;			// D3D11设备上下文

	// Direct3D 11.1
	ComPtr<ID3D11Device1> m_D3dDevice1;							// D3D11.1设备
	ComPtr<ID3D11DeviceContext1> m_D3dImmediateContext1;		// D3D11.1设备上下文

	ComPtr<ID3D11Buffer> m_ConstantBuffer;						// 常量缓冲区
	ComPtr<ID3D11Buffer> m_TypedBuffer1;						// 有类型缓冲区1
	ComPtr<ID3D11Buffer> m_TypedBuffer2;						// 有类型缓冲区2
	ComPtr<ID3D11Buffer> m_TypedBufferCopy;						// 用于拷贝的有类型缓冲区
	ComPtr<ID3D11UnorderedAccessView> m_DataUAV1;				// 有类型缓冲区1对应的无序访问视图
	ComPtr<ID3D11UnorderedAccessView> m_DataUAV2;				// 有类型缓冲区2对应的无序访问视图
	ComPtr<ID3D11ShaderResourceView> m_DataSRV1;				// 有类型缓冲区1对应的着色器资源视图
	ComPtr<ID3D11ShaderResourceView> m_DataSRV2;				// 有类型缓冲区2对应的着色器资源视图
	ComPtr<ID3D11ComputeShader> m_BitonicSort_CS;				// 排序使用的着色器
	ComPtr<ID3D11ComputeShader> m_MatrixTranspose_CS;			// 转置使用的着色器

public:
	BitonicSort();
	BitonicSort(const ComPtr<ID3D11Device> & pID3D11Device, const ComPtr<ID3D11DeviceContext> & pD3d11DeviceContext);
	~BitonicSort();

	//生成一个随机数vector
	std::vector<int> getRandomVector(_In size_t size, _In int min, _In int max);
	void sort(_Both std::vector<int> & vIn);

private:
	void inline initDirect3D();
	void inline initComputeShader();
	void inline setConstants(_In const CB & cb);
};