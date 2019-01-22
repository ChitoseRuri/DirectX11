#pragma once
#include <ctime>
#include <cmath>
#include <windows.h>

namespace Random
{
	void Setup();

	class INT
	{
	public:
		//���һ������
		static int Next(int begin, int end);
		//���һ�黥����ȵ�����
		static bool ArrayD(int begin, int end, int * arry, size_t size);
		//���һ�������ȵ�����
		static bool ArrayE(int begin, int end, int * arry, size_t size);
	};

	class FLOAT
	{
	public:
		//accuracy:С����ľ�ȷ��
		static float Next(float begin, float end, int accuracy);
	};
}