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

/*
	Specific check
	Always answer with warlist status (yes/no)
*/
bool CChillerBotReply::DoYouWarMe(int NameLen, int MsgLen)
{
	if(str_find_nocase(m_pMessage, "?") ||
		str_find_nocase(m_pMessage, "do you") ||
		str_find_nocase(m_pMessage, "are we") ||
		str_find_nocase(m_pMessage, "u war") ||
		str_find_nocase(m_pMessage, "war me?") ||
		str_find_nocase(m_pMessage, "am i") ||
		str_find_nocase(m_pMessage, "is i") ||
		str_find_nocase(m_pMessage, "im your") ||
		str_find_nocase(m_pMessage, "what") ||
		str_find_nocase(m_pMessage, "show") ||
		str_find_nocase(m_pMessage, "wat"))
	{
		if(str_find_nocase(m_pMessage, "me enem") || str_find_nocase(m_pMessage, "i enem") || str_find_nocase(m_pMessage, "me is enem") || str_find_nocase(m_pMessage, "i is enem") ||
			str_find_nocase(m_pMessage, "i u enem") || str_find_nocase(m_pMessage, "i ur enem") || str_find_nocase(m_pMessage, "i your enem") ||
			str_find_nocase(m_pMessage, "im u enem") || str_find_nocase(m_pMessage, "im ur enem") || str_find_nocase(m_pMessage, "im your enem") ||
			str_find_nocase(m_pMessage, "i am u enem") || str_find_nocase(m_pMessage, "i am ur enem") || str_find_nocase(m_pMessage, "i am your enem") ||
			str_find_nocase(m_pMessage, "me war") || str_find_nocase(m_pMessage, "i war") || str_find_nocase(m_pMessage, "me is war") || str_find_nocase(m_pMessage, "i is war") ||
			str_find_nocase(m_pMessage, "peace") ||
			(str_find_nocase(m_pMessage, "war me?") && MsgLen < NameLen + 15) ||
			str_find_nocase(m_pMessage, "u war me") ||
			str_find_nocase(m_pMessage, "me friend") || str_find_nocase(m_pMessage, "i friend") || str_find_nocase(m_pMessage, "me is friend") || str_find_nocase(m_pMessage, "i is friend") ||
			str_find_nocase(m_pMessage, "me frint") || str_find_nocase(m_pMessage, "i frint") || str_find_nocase(m_pMessage, "me is frint") || str_find_nocase(m_pMessage, "i is frint") ||
			str_find_nocase(m_pMessage, "are we in war") || str_find_nocase(m_pMessage, "we war") || str_find_nocase(m_pMessage, "we peace") || str_find_nocase(m_pMessage, "we good") ||
			str_find_nocase(m_pMessage, "i enem") || str_find_nocase(m_pMessage, "i peace") || str_find_nocase(m_pMessage, "i frien") || str_find_nocase(m_pMessage, "i frin") ||
			str_find_nocase(m_pMessage, "me enem") || str_find_nocase(m_pMessage, "me peace") || str_find_nocase(m_pMessage, "me frien") || str_find_nocase(m_pMessage, "me frin") ||
			str_find_nocase(m_pMessage, "colo") || str_find_nocase(m_pMessage, "cole") || str_find_nocase(m_pMessage, "collo") || str_find_nocase(m_pMessage, "colla") || str_find_nocase(m_pMessage, "cola") ||
			str_find_nocase(m_pMessage, "red") || str_find_nocase(m_pMessage, "green") || str_find_nocase(m_pMessage, "orange") || str_find_nocase(m_pMessage, "black") || str_find_nocase(m_pMessage, "reason"))
		{
			char aWarReason[128];
			if(m_WarList.IsWarlist(m_pMessageAuthor) || m_WarList.IsTraitorlist(m_pMessageAuthor))
			{
				m_WarList.GetWarReason(m_pMessageAuthor, aWarReason, sizeof(aWarReason));
				if(aWarReason[0])
					WriteReplyBufFormat("%s you have war because: %s", m_pMessageAuthor, aWarReason);
				else
					WriteReplyBufFormat("%s you are on my warlist.", m_pMessageAuthor);
				return true;
			}
			else if(m_WarList.IsWarClanlist(m_pMessageAuthorClan))
			{
				WriteReplyBufWithPing("your clan is on my warlist.");
				return true;
			}
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				const char *pLoopName = m_Context.m_pfnGetClientName(i, m_Context.m_pUser);
				if(pLoopName[0] == '\0')
					continue;
				if(str_comp(pLoopName, m_pMessageAuthor))
					continue;

				// TODO: this loop is a waste of clock cycles just pass the msg author client id to the reply lib
				if(m_WarList.IsWarClanmate(i))
				{
					WriteReplyBufFormat("%s i might kill you because i war member of your clan", m_pMessageAuthor);
					return true;
				}
			}
			WriteReplyBufFormat("%s your name is neither on my friend nor enemy list.", m_pMessageAuthor);
			return true;
		}
	}
	return false;
}
