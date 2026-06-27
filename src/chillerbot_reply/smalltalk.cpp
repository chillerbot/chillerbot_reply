#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>

using namespace ddnet_base;

bool CChillerBotReply::SmallTalk(int NameLen, int MsgLen)
{
	// small talk
	if(str_find_nocase(m_pMessage, "how are you") ||
		str_find_nocase(m_pMessage, "how r u") ||
		str_find_nocase(m_pMessage, "how ru ") ||
		str_find_nocase(m_pMessage, "how ru?") ||
		str_find_nocase(m_pMessage, "how r you") ||
		str_find_nocase(m_pMessage, "how are u") ||
		str_find_nocase(m_pMessage, "how is it going") ||
		str_find_nocase(m_pMessage, "ca va") ||
		(str_find_nocase(m_pMessage, "как") && str_find_nocase(m_pMessage, "дела")))
	{
		WriteReplyBufFormat("%s good, and you? :)", m_pMessageAuthor);
		return true;
	}
	if(str_find_nocase(m_pMessage, "wie gehts") || str_find_nocase(m_pMessage, "wie geht es") || str_find_nocase(m_pMessage, "was geht"))
	{
		WriteReplyBufFormat("%s gut, und dir? :)", m_pMessageAuthor);
		return true;
	}
	if(str_find_nocase(m_pMessage, "ca va") ||
		str_find_nocase(m_pMessage, "Ça va") ||
		str_find_nocase(m_pMessage, "ça va") ||
		(str_find_nocase(m_pMessage, "cv") && ((MsgLen - NameLen) < 6)) ||
		(str_find_nocase(m_pMessage, "cv ") && ((MsgLen - NameLen) < 14)) ||
		(str_find_nocase(m_pMessage, "cv?") && ((MsgLen - NameLen) < 14)) ||
		(str_find_nocase(m_pMessage, "çv ") && ((MsgLen - NameLen) < 14)) ||
		(str_find_nocase(m_pMessage, "çv?") && ((MsgLen - NameLen) < 14)))
	{
		WriteReplyBufFormat("je vais bien, et toi %s ?", m_pMessageAuthor);
		return true;
	}
	if(str_find_nocase(m_pMessage, "et toi"))
	{
		WriteReplyBufFormat("%s moi aussi, merci", m_pMessageAuthor);
		return true;
	}
	if(str_find_nocase(m_pMessage, "about you") ||
		str_find_nocase(m_pMessage, " and you") ||
		str_startswith(m_pMessage, "and you") ||
		str_find_nocase(m_pMessage, " and u") ||
		str_startswith(m_pMessage, "and u") ||
		(str_find_nocase(m_pMessage, "u?") && MsgLen < NameLen + 5) ||
		(str_find_nocase(m_pMessage, "wbu") && MsgLen < NameLen + 8) ||
		(str_find_nocase(m_pMessage, "hbu") && MsgLen < NameLen + 8))
	{
		WriteReplyBufFormat("%s good", m_pMessageAuthor);
		return true;
	}
	return false;
}
