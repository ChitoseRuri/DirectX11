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

// �������Ҫ���õĿ�
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
#define _1DSize	512u
#define _2DSize	262144u
#define _3DSize 134217728u

class BitonicSort
{
private:
	struct CB
	{
		UINT level;
		UINT descendMask;
		UINT matrixWidth;
		UINT matrixHeight;
	};

	// ʹ��ģ�����(C++11)��������
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::mt19937 m_RandEngine;				//�������

	// Direct3D 11
	ComPtr<ID3D11Device> m_D3dDevice;							// D3D11�豸
	ComPtr<ID3D11DeviceContext> m_D3dImmediateContext;			// D3D11�豸������

	// Direct3D 11.1
	ComPtr<ID3D11Device1> m_D3dDevice1;							// D3D11.1�豸
	ComPtr<ID3D11DeviceContext1> m_D3dImmediateContext1;		// D3D11.1�豸������

	ComPtr<ID3D11Buffer> m_ConstantBuffer;						// ����������
	ComPtr<ID3D11Buffer> m_TypedBuffer1;						// �����ͻ�����1
	ComPtr<ID3D11Buffer> m_TypedBuffer2;						// �����ͻ�����2
	ComPtr<ID3D11Buffer> m_TypedBufferCopy;						// ���ڿ����������ͻ�����
	ComPtr<ID3D11UnorderedAccessView> m_DataUAV1;				// �����ͻ�����1��Ӧ�����������ͼ
	ComPtr<ID3D11UnorderedAccessView> m_DataUAV2;				// �����ͻ�����2��Ӧ�����������ͼ
	ComPtr<ID3D11ShaderResourceView> m_DataSRV1;				// �����ͻ�����1��Ӧ����ɫ����Դ��ͼ
	ComPtr<ID3D11ShaderResourceView> m_DataSRV2;				// �����ͻ�����2��Ӧ����ɫ����Դ��ͼ
	ComPtr<ID3D11ComputeShader> m_BitonicSort_CS;				// ����ʹ�õ���ɫ��
	ComPtr<ID3D11ComputeShader> m_MatrixTranspose_CS;			// ת��ʹ�õ���ɫ��

public:
	BitonicSort();
	BitonicSort(const ComPtr<ID3D11Device> & pID3D11Device, const ComPtr<ID3D11DeviceContext> & pD3d11DeviceContext);
	~BitonicSort();

	//����һ�������vector
	std::vector<UINT> getRandomVector(_In size_t size, _In UINT min, _In UINT max);
	void sort(_Both std::vector<UINT> & vIn);

private:
	//��ʼ��Direct3D
	void inline initDirect3D();
	//��ʼ��shader
	void inline initComputeShader();
	//���û�����
	void inline setConstants(_In const CB & cb);
	//����������ͼ
	void inline createDesc(_In void * pData, _In size_t size, _In size_t count);
	
	//0��ת������
	void sort1D(_Both std::vector<UINT> & vIn, _In size_t size);
	//1��ת������
	void sort2D(_Both std::vector<UINT> & vIn, _In size_t size);
	//2��ת������
	void sort3D(_Both std::vector<UINT> & vIn, _In size_t size);
};