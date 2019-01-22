#include "Random.h"

namespace Random
{
	void Setup()
	{
		srand(time(nullptr));
	}

	int INT::Next(int begin, int end)
	{
		if (end > begin)
		{
			return (rand() << 16 | rand() & 0x7FFFFFFF) % (end - begin) + begin;
		}
		else if (end == begin)
		{
			return begin;
		}
		return 0;
	}

	bool INT::ArrayD(int begin, int end, int * arry, size_t size)
	{
		if (arry && size && end > begin && end - begin >= size)
		{
			arry[0] = (rand() << 16 | rand() & 0x7FFFFFFF) % (end - begin) + begin;
			for (size_t i = 1; i < size; i++)
			{
				int diff = 0;
				int temp = 0;
				while (diff < i)
				{
					temp = (rand() << 16 | rand() & 0x7FFFFFFF) % (end - begin) + begin;
					for (diff = 0; diff < i; diff++)
					{
						if (arry[diff] == temp)
						{
							break;
						}
					}
				}
				arry[i] = temp;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool INT::ArrayE(int begin, int end, int * arry, size_t size)
	{
		if (arry && size && end > begin)
		{
			for (size_t i = 0; i < size; i++)
			{
				arry[i] = (rand() << 16 | rand() & 0x7FFFFFFF) % (end - begin) + begin;
			}
			return  true;
		}
		else
		{
			return false;
		}
	}

	float FLOAT::Next(float begin, float end, int accuracy)
	{
		if (end > begin)
		{
			float Begin = begin;
			float power = pow(10, accuracy);
			begin *= power;
			end *= power;
			return (rand() << 16 | rand() & 0x7FFFFFFF) % static_cast<int>(end - begin) / power + Begin;
		}
		else if (end == begin)
		{
			return begin;
		}
		return 0.0f;
	}
}

