#include <chillerbot_reply/text_helper.h>
#include <ddnet_base/base/dbg.h>
#include <ddnet_base/base/str.h>

using namespace ddnet_base;

namespace TextHelper
{

	int GetSuffixLen(const char *pStr, const char *pSuffix)
	{
		if(str_endswith(pStr, pSuffix))
			return str_length(pSuffix);
		return 0;
	}

	void StripSpacesAndPunctuationAndOwnName(const char *pStr, const char *pName, const char *pDummyName, char *pStripped, int SizeOfStripped)
	{
		if(!pStripped)
			return;
		dbg_assert(SizeOfStripped < 512, "too big to strip");
		if(pStr == pStripped)
		{
			pStripped[0] = '\0';
			return;
		}
		if(!pStr)
		{
			pStripped[0] = '\0';
			return;
		}
		if(!pStripped)
		{
			pStripped[0] = '\0';
			return;
		}
		if(SizeOfStripped < 1)
		{
			pStripped[0] = '\0';
			return;
		}
		char aBuf[512];
		str_copy(aBuf, pStr, sizeof(aBuf));
		char aPuncts[][4] = {"?", "!", ".", ",", "¿", " "};
		for(int i = 0; i < 10; i++) // strip up to 10 alternating punctuations
			for(auto const &Punc : aPuncts)
				while(str_endswith(aBuf, Punc)) // cut off punctuation and spaces
					aBuf[str_length(aBuf) - str_length(Punc)] = '\0';
		int Offset = 0;
		char aName[128];
		if(pName[0])
		{
			str_format(aName, sizeof(aName), "%s: ", pName);
			if(!Offset && str_startswith(pStr, aName))
				Offset = str_length(aName);
			str_format(aName, sizeof(aName), "%s ", pName);
			if(!Offset && str_startswith(pStr, aName))
				Offset = str_length(aName);
			str_format(aName, sizeof(aName), "%s", pName);
			if(!Offset && str_startswith(pStr, aName))
				Offset = str_length(aName);
		}
		if(pDummyName[0])
		{
			str_format(aName, sizeof(aName), "%s: ", pDummyName);
			if(!Offset && str_startswith(pStr, aName))
				Offset = str_length(aName);
			str_format(aName, sizeof(aName), "%s ", pDummyName);
			if(!Offset && str_startswith(pStr, aName))
				Offset = str_length(aName);
			str_format(aName, sizeof(aName), "%s", pDummyName);
			if(!Offset && str_startswith(pStr, aName))
				Offset = str_length(aName);
		}
		if(Offset >= str_length(aBuf))
			pStripped[0] = '\0';
		else
			str_copy(pStripped, aBuf + Offset, SizeOfStripped);
	}

	bool IsEmptyStr(const char *pStr)
	{
		if(!pStr)
			return true;
		for(int i = 0; pStr[i] != '\0'; i++)
			if(pStr[i] != ' ' && pStr[i] != 0x7)
				return false;
		return true;
	}

}
