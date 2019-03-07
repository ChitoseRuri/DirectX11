#include "PreCompiler.h"
#include "BitonicSort.h"


int main()
{
	BitonicSort bs;
	unsigned size;
	cin >> size;
	auto vat = bs.getRandomVector(size, 0, 20000);
	bs.sort(vat);
	for (auto & var : vat)
	{
		printf("%d\n", var);
	}
	_getch();
	return 0;
}
