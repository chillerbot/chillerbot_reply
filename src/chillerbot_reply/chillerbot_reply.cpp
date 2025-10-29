#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>

bool CChillerBotReply::Reply(const char *pMsg, char *pReplyBuf, size_t ReplyBufLen)
{
	if(!str_comp_nocase(pMsg, "hellow"))
	{
		str_copy(pReplyBuf, "yellow", ReplyBufLen);
		return true;
	}
	return false;
}
