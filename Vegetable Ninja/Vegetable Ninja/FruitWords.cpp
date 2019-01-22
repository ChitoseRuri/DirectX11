#include "FruitWords.h"

wchar_t * DX11Game::CharToWchar(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if (nSize <= 0)
	{
		return NULL;
	}
	WCHAR *pwszDst = new WCHAR[nSize + 1];
	if (NULL == pwszDst)
	{
		return NULL;
	}
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;
	if (pwszDst[0] == 0xFEFF) // skip Oxfeff  
	{
		for (int i = 0; i < nSize; i++)
		{
			pwszDst[i] = pwszDst[i + 1];
		}
	}
	return pwszDst;
}
