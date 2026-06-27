#include <chillerbot_reply/chillerbot_reply.h>
#include <chillerbot_reply/clan.h>
#include <chillerbot_reply/langparser.h>
#include <ddnet_base/base/str.h>

using namespace ddnet_base;
using namespace LangParser;

CChillerBotReplyContext::CChillerBotReplyContext()
{
	Reset();
}

void CChillerBotReplyContext::Reset()
{
	m_ActiveTee = 0;
	m_IsDummyConnected = false;
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

bool CChillerBotReply::Reply(const CChillerBotReplyChatMessage *pMsg, char *pReplyBuf, size_t ReplyBufLen)
{
	m_pMessage = pMsg->m_pMessage;
	m_pMessageAuthor = pMsg->m_pAuthor;
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
