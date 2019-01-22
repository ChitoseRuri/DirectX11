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
		//随机一个整数
		static int Next(int begin, int end);
		//随机一组互不相等的整数
		static bool ArrayD(int begin, int end, int * arry, size_t size);
		//随机一组可能相等的整数
		static bool ArrayE(int begin, int end, int * arry, size_t size);
	};

	class FLOAT
	{
	public:
		//accuracy:小数点的精确度
		static float Next(float begin, float end, int accuracy);
	};
}