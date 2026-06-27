#pragma once

namespace TextHelper
{
	int GetSuffixLen(const char *pStr, const char *pSuffix);
	void StripSpacesAndPunctuationAndOwnName(const char *pStr, const char *pName, const char *pDummyName, char *pStripped, int SizeOfStripped);
	bool IsEmptyStr(const char *pStr);
}
