#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>

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
	// const char *pHL = str_utf8_find_nocase(pLine, pName);

	// if(pHL)
	// {
	// 	int Length = str_length(pName);

	// 	if(Length > 0 && (pLine == pHL || pHL[-1] == ' ') && (pHL[Length] == 0 || pHL[Length] == ' ' || pHL[Length] == '.' || pHL[Length] == '!' || pHL[Length] == ',' || pHL[Length] == '?' || pHL[Length] == ':'))
	// 		return true;
	// }

	return false;
}

bool CChillerBotReply::Reply(const CChillerBotReplyChatMessage *pMsg, char *pReplyBuf, size_t ReplyBufLen)
{
	if(!str_comp_nocase(pMsg->m_pMessage, "hellow"))
	{
		str_copy(pReplyBuf, "yellow", ReplyBufLen);
		return true;
	}
	if(str_comp_nocase(pMsg->m_pMessage, "hellow"))
	{
		str_copy(pReplyBuf, "yellow", ReplyBufLen);
		return true;
	}
	return false;
}
