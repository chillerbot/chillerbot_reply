#include <chillerbot_reply/chillerbot_reply.h>

// TODO: replace holy LLM code with ddnet styled utf8 code

/* Decode one UTF-8 code point. Returns false if invalid UTF-8. */
static bool utf8_next(const unsigned char **s, uint32_t *Cp)
{
	const unsigned char *p = *s;

	if(p[0] < 0x80)
	{
		*Cp = p[0];
		*s = p + 1;
		return true;
	}

	if((p[0] & 0xE0) == 0xC0)
	{
		if((p[1] & 0xC0) != 0x80)
			return false;

		*Cp = ((uint32_t)(p[0] & 0x1F) << 6) |
		      ((uint32_t)(p[1] & 0x3F));

		if(*Cp < 0x80)
			return false; /* overlong */
		*s = p + 2;
		return true;
	}

	if((p[0] & 0xF0) == 0xE0)
	{
		if((p[1] & 0xC0) != 0x80 ||
			(p[2] & 0xC0) != 0x80)
			return false;

		*Cp = ((uint32_t)(p[0] & 0x0F) << 12) |
		      ((uint32_t)(p[1] & 0x3F) << 6) |
		      ((uint32_t)(p[2] & 0x3F));

		if(*Cp < 0x800)
			return false; /* overlong */
		if(*Cp >= 0xD800 && *Cp <= 0xDFFF)
			return false; /* surrogate */
		*s = p + 3;
		return true;
	}

	if((p[0] & 0xF8) == 0xF0)
	{
		if((p[1] & 0xC0) != 0x80 ||
			(p[2] & 0xC0) != 0x80 ||
			(p[3] & 0xC0) != 0x80)
			return false;

		*Cp = ((uint32_t)(p[0] & 0x07) << 18) |
		      ((uint32_t)(p[1] & 0x3F) << 12) |
		      ((uint32_t)(p[2] & 0x3F) << 6) |
		      ((uint32_t)(p[3] & 0x3F));

		if(*Cp < 0x10000 || *Cp > 0x10FFFF)
			return false;
		*s = p + 4;
		return true;
	}

	return false;
}

/* Unicode Cyrillic ranges */
static bool is_cyrillic_codepoint(uint32_t Cp)
{
	return (Cp >= 0x0400 && Cp <= 0x04FF) || /* Cyrillic */
	       (Cp >= 0x0500 && Cp <= 0x052F) || /* Cyrillic Supplement */
	       (Cp >= 0x1C80 && Cp <= 0x1C8F) || /* Cyrillic Extended-C */
	       (Cp >= 0x2DE0 && Cp <= 0x2DFF) || /* Cyrillic Extended-A */
	       (Cp >= 0xA640 && Cp <= 0xA69F) || /* Cyrillic Extended-B */
	       (Cp >= 0x1E030 && Cp <= 0x1E08F); /* Cyrillic Extended-D */
}

/* Returns true if the UTF-8 string is non-empty and all chars are Cyrillic */
static bool is_cyrillic_string(const char *pStr)
{
	const unsigned char *p = (const unsigned char *)pStr;
	uint32_t Cp;
	int NumCyrillic = 0;

	while(*p)
	{
		if(!utf8_next(&p, &Cp))
			return false; /* invalid UTF-8 */

		if(is_cyrillic_codepoint(Cp))
		{
			NumCyrillic++;
			if(NumCyrillic > 4)
				return true;
		}
	}

	return false;
}

bool CChillerBotReply::EnglishPlease()
{
	if(NoRussian())
		return true;
	return false;
}

bool CChillerBotReply::NoRussian()
{
	if(is_cyrillic_string(m_pMessage))
		WriteReplyBufWithPing("english please - я не говорю по-русски");
	return false;
}
