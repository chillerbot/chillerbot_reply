#include <chillerbot_reply/chillerbot_reply.h>
#include <chillerbot_reply/clan.h>
#include <chillerbot_reply/langparser.h>
#include <chillerbot_reply/text_helper.h>
#include <ddnet_base/base/str.h>

using namespace ddnet_base;
using namespace LangParser;

void CWarListWrapper::OnInit(const CChillerBotReplyContext *pContext)
{
	m_pContext = pContext;
}

void CWarListWrapper::GetWarReason(const char *pName, char *pReason, int ReasonSize)
{
	m_pContext->m_pfnGetWarReason(pName, pReason, ReasonSize, m_pContext->m_pUser);
}

void CWarListWrapper::GetWarClansStr(char *pBuf, size_t BufLen)
{
	m_pContext->m_pfnGetWarClansStr(pBuf, BufLen, m_pContext->m_pUser);
}

bool CWarListWrapper::IsWar(const char *pName, const char *pClan)
{
	return m_pContext->m_pfnIsWar(pName, pClan, m_pContext->m_pUser);
}

bool CWarListWrapper::IsWarlist(const char *pName)
{
	return m_pContext->m_pfnIsWarlist(pName, m_pContext->m_pUser);
}

bool CWarListWrapper::IsTeamlist(const char *pName)
{
	return m_pContext->m_pfnIsTeamlist(pName, m_pContext->m_pUser);
}

bool CWarListWrapper::IsTraitorlist(const char *pName)
{
	return m_pContext->m_pfnIsTraitorlist(pName, m_pContext->m_pUser);
}

bool CWarListWrapper::IsWarClanlist(const char *pClan)
{
	return m_pContext->m_pfnIsWarClanlist(pClan, m_pContext->m_pUser);
}

bool CWarListWrapper::IsTeamClanlist(const char *pClan)
{
	return m_pContext->m_pfnIsTeamClanlist(pClan, m_pContext->m_pUser);
}

bool CWarListWrapper::IsWarClanmate(const char *pClan)
{
	return m_pContext->m_pfnIsWarClanmate(pClan, m_pContext->m_pUser);
}

bool CWarListWrapper::IsWarClanmate(int ClientId)
{
	return m_pContext->m_pfnIsWarClanmateId(ClientId, m_pContext->m_pUser);
}

CChillerBotReplyContext::CChillerBotReplyContext()
{
	Reset();
}

void CChillerBotReplyContext::Reset()
{
	m_ActiveTee = 0;
	m_IsDummyConnected = false;
}

const char *CChillerBotReply::Name()
{
	return m_Context.m_aOwnTees[0].m_pName;
}

const char *CChillerBotReply::DummyName()
{
	return m_Context.m_aOwnTees[1].m_pName;
}

bool CChillerBotReply::IsDummyConnected() const
{
	return m_Context.m_IsDummyConnected;
}

void CChillerBotReply::WriteReplyBuf(const char *pMessage)
{
	str_copy(m_pReplyBuf, pMessage, m_ReplyBufLen);
}

void CChillerBotReply::WriteReplyBufWithPing(const char *pMessage)
{
	str_format(m_pReplyBuf, m_ReplyBufLen, "%s %s", m_pMessageAuthor, pMessage);
}

void CChillerBotReply::WriteReplyBufFormat(const char *pFormat, ...)
{
	va_list Args;
	va_start(Args, pFormat);
	str_format_v(m_pReplyBuf, m_ReplyBufLen, pFormat, Args);
	va_end(Args);
}

bool CChillerBotReply::LineShouldHighlight(const char *pLine, const char *pName)
{
	const char *pHL = str_utf8_find_nocase(pLine, pName);

	if(pHL)
	{
		int Length = str_length(pName);

		if(Length > 0 && (pLine == pHL || pHL[-1] == ' ') && (pHL[Length] == 0 || pHL[Length] == ' ' || pHL[Length] == '.' || pHL[Length] == '!' || pHL[Length] == ',' || pHL[Length] == '?' || pHL[Length] == ':'))
			return true;
	}

	return false;
}

bool CChillerBotReply::CanIJoinYourClan()
{
	if(str_find_nocase(m_pMessage, "clan") &&
		(str_find_nocase(m_pMessage, "enter") ||
			str_find_nocase(m_pMessage, "join") ||
			str_find_nocase(m_pMessage, "let me") ||
			str_find_nocase(m_pMessage, "beitreten") ||
			str_find_nocase(m_pMessage, " in ") ||
			str_find_nocase(m_pMessage, "can i") ||
			str_find_nocase(m_pMessage, "can me") ||
			str_find_nocase(m_pMessage, "me you") ||
			str_find_nocase(m_pMessage, "me is") ||
			str_find_nocase(m_pMessage, "into")))
	{
		const char *pClan = m_Context.m_aOwnTees[0].m_pClan;
		const char *pDummyClan = m_Context.m_aOwnTees[1].m_pClan;
		char aResponse[512];
		if(ChillerBotReply::HowToJoinClan(pClan, aResponse, sizeof(aResponse)) ||
			(m_Context.m_IsDummyConnected && ChillerBotReply::HowToJoinClan(pDummyClan, aResponse, sizeof(aResponse))))
		{
			WriteReplyBufWithPing(aResponse);
			return true;
		}
	}
	return false;
}

bool CChillerBotReply::ListClanWars()
{
	if((str_find_nocase(m_pMessage, "clan") || str_find_nocase(m_pMessage, "klan")) &&
		(IsQuestionWhoWhichWhat(m_pMessage) || str_find(m_pMessage, "?")))
	{
		if(str_find_nocase(m_pMessage, "war") || str_find_nocase(m_pMessage, "enemy") || str_find_nocase(m_pMessage, "kill") || str_find_nocase(m_pMessage, "against") || str_find_nocase(m_pMessage, "bad"))
		{
			char aClans[256];
			m_Context.m_pfnGetWarClansStr(aClans, sizeof(aClans), m_Context.m_pUser);
			if(aClans[0])
				WriteReplyBufFormat("%s I war those clans: %s", m_pMessageAuthor, aClans);
			else
				WriteReplyBufFormat("%s I currently do not war any clans.", m_pMessageAuthor);
			return true;
		}
	}
	return false;
}

bool CChillerBotReply::IsWarName()
{
	// check if a player has war or not
	const char *pDoYou = nullptr;
	if(!pDoYou && (pDoYou = str_find_nocase(m_pMessage, "you war ")))
		pDoYou = pDoYou + str_length("you war ");
	if(!pDoYou && (pDoYou = str_find_nocase(m_pMessage, "you in war with ")))
		pDoYou = pDoYou + str_length("you in war with ");
	// "hast du war mit"
	// "hast du eig war mit"
	// "hast du eigentlich war mit"
	// "hast du überhaupt war mit"
	// "hast du einen war mit"
	if(!pDoYou && (pDoYou = LangParser::StrFindOrder(m_pMessage, 2, "hast du ", "war mit ")))
		pDoYou = pDoYou + str_length("war mit ");
	if(pDoYou)
		if(WhyWar(pDoYou, true))
			return true;

	// check war reason for others
	const char *pWhy = str_find_nocase(m_pMessage, "why has ");
	if(pWhy)
		pWhy = pWhy + str_length("why has ");
	if(!pWhy)
		if((pWhy = str_find_nocase(m_pMessage, "why")))
			pWhy = pWhy + str_length("why");
	if(!pWhy)
		if((pWhy = str_find_nocase(m_pMessage, "warum hat ")))
			pWhy = pWhy + str_length("warum hat ");
	if(!pWhy)
		if((pWhy = str_find_nocase(m_pMessage, "warum")))
			pWhy = pWhy + str_length("warum");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "stop");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "do not");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "don't");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "do u");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "do you");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "is u");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "is you");
	if(!pWhy)
	{
		pWhy = str_find_nocase(m_pMessage, "is war");
	}
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "are u");
	if(!pWhy)
		pWhy = str_find_nocase(m_pMessage, "are you");
	if(pWhy)
	{
		const char *pKill = nullptr;
		pKill = str_find_nocase(pWhy, "kill "); // why do you kill foo?
		if(pKill)
			pKill = pKill + str_length("kill ");
		else if((pKill = str_find_nocase(pWhy, "kil "))) // why kil foo?
			pKill = pKill + str_length("kil ");
		else if((pKill = str_find_nocase(pWhy, "killed "))) // why killed foo?
			pKill = pKill + str_length("killed ");
		else if((pKill = str_find_nocase(pWhy, "block "))) // why do you block foo?
			pKill = pKill + str_length("block ");
		else if((pKill = str_find_nocase(pWhy, "blocked "))) // why do you blocked foo?
			pKill = pKill + str_length("blocked ");
		else if((pKill = str_find_nocase(pWhy, "help "))) // why no help foo?
			pKill = pKill + str_length("help ");
		else if((pKill = str_find_nocase(pWhy, "war with "))) // why do you have war with foo?
			pKill = pKill + str_length("war with ");
		else if((pKill = str_find_nocase(pWhy, "war "))) // why war foo?
			pKill = pKill + str_length("war ");
		else if((pKill = str_find_nocase(pWhy, "wared "))) // why wared foo?
			pKill = pKill + str_length("wared ");
		else if((pKill = str_find_nocase(pWhy, "waring "))) // why waring foo?
			pKill = pKill + str_length("waring ");
		else if((pKill = str_find_nocase(pWhy, "bully "))) // why bully foo?
			pKill = pKill + str_length("bully ");
		else if((pKill = str_find_nocase(pWhy, "bullying "))) // why bullying foo?
			pKill = pKill + str_length("bullying ");
		else if((pKill = str_find_nocase(pWhy, "bullied "))) // why bullied foo?
			pKill = pKill + str_length("bullied ");
		else if((pKill = str_find_nocase(pWhy, "freeze "))) // why freeze foo?
			pKill = pKill + str_length("freeze ");
		else if((pKill = str_find_nocase(pWhy, "warlist "))) // is warlist foo?
			pKill = pKill + str_length("warlist ");
		else if((pKill = str_find_nocase(pWhy, "enemi "))) // is enemi foo?
			pKill = pKill + str_length("enemi ");
		else if((pKill = str_find_nocase(pWhy, "enemy "))) // is enemy foo?
			pKill = pKill + str_length("enemy ");

		if(pKill && WhyWar(pKill))
			return true;

		// "why foo war?"
		// chop off the "war" at the end
		char aWhy[128];
		str_copy(aWhy, pWhy, sizeof(aWhy));

		int CutOffWar = -1;
		if((CutOffWar = LangParser::StrFindIndex(aWhy, " war")) != -1)
			aWhy[CutOffWar] = '\0';
		else if((CutOffWar = LangParser::StrFindIndex(aWhy, " kill")) != -1)
			aWhy[CutOffWar] = '\0';

		// trim
		int Trim = 0;
		while(aWhy[Trim] == ' ')
			Trim++;

		if(CutOffWar != -1)
			if(WhyWar(aWhy + Trim))
				return true;

		if(pKill)
		{
			char aStripped[128];
			TextHelper::StripSpacesAndPunctuationAndOwnName(pKill, Name(), DummyName(), aStripped, sizeof(aStripped));
			if(!TextHelper::IsEmptyStr(aStripped))
			{
				WriteReplyBufFormat("%s: '%s' is not on my warlist.", m_pMessageAuthor, aStripped);
				return true;
			}
		}
	}
	return false;
}

bool CChillerBotReply::Reply(const CChillerBotReplyChatMessage *pMsg, char *pReplyBuf, size_t ReplyBufLen)
{
	m_pMessage = pMsg->m_pMessage;
	m_pMessageAuthor = pMsg->m_pAuthor;
	m_pMessageAuthorClan = pMsg->m_pAuthorClan;
	m_pReplyBuf = pReplyBuf;
	m_ReplyBufLen = ReplyBufLen;

	int MsgLen = str_length(pMsg->m_pMessage);
	int NameLen = 0;

	const char *pName = m_Context.m_aOwnTees[0].m_pName;
	// const char *pClan = m_Context.m_aOwnTees[0].m_pClan;
	const char *pDummyName = m_Context.m_aOwnTees[1].m_pName;
	// const char *pDummyClan = m_Context.m_aOwnTees[1].m_pClan;

	if(LineShouldHighlight(pMsg->m_pMessage, pName))
		NameLen = str_length(pName);
	else if(m_Context.m_IsDummyConnected && LineShouldHighlight(pMsg->m_pMessage, pDummyName))
		NameLen = str_length(pDummyName);

	// ping without further context
	if(MsgLen < NameLen + 2)
	{
		str_format(pReplyBuf, ReplyBufLen, "%s ?", pMsg->m_pAuthor);
		return true;
	}

	if(CanIJoinYourClan())
		return true;
	if(ListClanWars())
		return true;
	// check war for others "is war name?"
	if(IsWarName())
		return true;
	// why? (check war for self)
	if(WhyDoYouKillMe(NameLen, MsgLen))
		return true;

	if(!str_comp_nocase(pMsg->m_pMessage, "lib"))
	{
		WriteReplyBuf("lab");
		return true;
	}
	if(str_find(pMsg->m_pMessage, "lib"))
	{
		WriteReplyBuf("lab xd");
		return true;
	}
	return false;
}
