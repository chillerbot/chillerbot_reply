#pragma once

namespace TextHelper
{
	/*
		GetSuffixLen

		if pStr ends with pSuffix return length of pSuffix
	*/
	int GetSuffixLen(const char *pStr, const char *pSuffix);

	/*
		StripSpacesAndPunctuationAndOwnName

		If the input pStr "yourname: hello, world  ??! ? !?"
		is given the pStripped will point to "hello, world"
	*/
	void StripSpacesAndPunctuationAndOwnName(const char *pStr, const char *pName, const char *pDummyName, char *pStripped, int SizeOfStripped);

	/*
		IsEmptyStr

		returns true if pStr is null "" or " " or "			 "
	*/
	bool IsEmptyStr(const char *pStr);
}
