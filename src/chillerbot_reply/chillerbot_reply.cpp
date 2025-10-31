#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>
#include <ddnet_base/base/system.h>

CChillerBotReplyContext::CChillerBotReplyContext()
{
	Reset();
}

void CChillerBotReplyContext::Reset()
{
	m_ActiveTee = 0;
	m_IsDummyConnected = false;
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

bool CChillerBotReply::Reply(const CChillerBotReplyChatMessage *pMsg, char *pReplyBuf, size_t ReplyBufLen)
{
	int MsgLen = str_length(pMsg->m_pMessage);
	int NameLen = 0;

	const char *pName = m_Context.m_aOwnTees[0].m_pName;
	const char *pClan = m_Context.m_aOwnTees[0].m_pClan;
	const char *pDummyName = m_Context.m_aOwnTees[1].m_pName;
	const char *pDummyClan = m_Context.m_aOwnTees[1].m_pClan;

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

	if(!str_comp_nocase(pMsg->m_pMessage, "hellow"))
	{
		str_copy(pReplyBuf, "yellow", ReplyBufLen);
		return true;
	}
	if(str_find(pMsg->m_pMessage, "hellow"))
	{
		str_copy(pReplyBuf, "yellow xd", ReplyBufLen);
		return true;
	}
	return false;
}
