#include <chillerbot_reply/chillerbot_reply.h>
#include <chillerbot_reply/langparser.h>
#include <chillerbot_reply/text_helper.h>
#include <ddnet_base/base/str.h>
#include <polynet/protocol.h>

using namespace ddnet_base;
using namespace polynet;
using namespace TextHelper;
using namespace LangParser;

/*
	Wide range check
	Only answer if on warlist
*/
bool CChillerBotReply::WhyDoYouKillMe(int NameLen, int MsgLen)
{
	if(MsgLen - NameLen < 10 && str_find_nocase(m_pMessage, "got dj?"))
		return false;
	char aStripped[128];
	TextHelper::StripSpacesAndPunctuationAndOwnName(m_pMessage, Name(), DummyName(), aStripped, sizeof(aStripped));
	if(LangParser::IsQuestionWhy(m_pMessage) || (str_find_nocase(m_pMessage, "?") && str_length(aStripped) < NameLen + 4) ||
		((str_find_nocase(aStripped, "what") || str_find_nocase(aStripped, "wat") || str_find_nocase(aStripped, "warum") || str_find_nocase(aStripped, "why") || str_find_nocase(aStripped, "waht")) && str_length(aStripped) < 8) ||
		((str_find_nocase(aStripped, "what") || str_find_nocase(aStripped, "wat") || str_find_nocase(aStripped, "was") || str_find_nocase(aStripped, "waht")) && str_find_nocase(aStripped, "problem")) ||
		((str_find_nocase(m_pMessage, "stop") || str_find_nocase(m_pMessage, "help")) && (m_WarList.IsWarlist(m_pMessageAuthor) || m_WarList.IsTraitorlist(m_pMessageAuthor))))
	{
		char aWarReason[128];
		if(m_WarList.IsWarlist(m_pMessageAuthor) || m_WarList.IsTraitorlist(m_pMessageAuthor))
		{
			m_WarList.GetWarReason(m_pMessageAuthor, aWarReason, sizeof(aWarReason));
			if(aWarReason[0])
				WriteReplyBufFormat("%s has war because: %s", m_pMessageAuthor, aWarReason);
			else
				WriteReplyBufFormat("%s you are on my warlist.", m_pMessageAuthor);
			return true;
		}
		else if(m_WarList.IsWarClanlist(m_pMessageAuthorClan))
		{
			WriteReplyBufFormat("%s your clan is on my warlist.", m_pMessageAuthor);
			return true;
		}
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			const char *pLoopName = m_Context.m_pfnGetClientName(i, m_Context.m_pUser);
			if(pLoopName[0] == '\0')
				continue;
			if(str_comp(pLoopName, m_pMessageAuthor))
				continue;

			if(m_WarList.IsWarClanmate(i))
			{
				WriteReplyBufFormat("%s i might kill you because i war member of your clan", m_pMessageAuthor);
				return true;
			}
		}
	}
	return false;
}
