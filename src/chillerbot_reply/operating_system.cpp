#include <chillerbot_reply/chillerbot_reply.h>
#include <chillerbot_reply/langparser.h>
#include <ddnet_base/base/detect.h>
#include <ddnet_base/base/str.h>

using namespace ddnet_base;

bool CChillerBotReply::WhatOs()
{
	if((str_find_nocase(m_pMessage, "?") || LangParser::IsQuestionWhoWhichWhat(m_pMessage) || str_find_nocase(m_pMessage, "do you") || str_find_nocase(m_pMessage, "u use") || str_find_nocase(m_pMessage, "u on")) &&
		(str_find_nocase(m_pMessage, "linux") || str_find_nocase(m_pMessage, "linus") || str_find_nocase(m_pMessage, "unix") || str_find_nocase(m_pMessage, "solaris") ||
			str_find_nocase(m_pMessage, "freebsd") || str_find_nocase(m_pMessage, "openbsd") || str_find_nocase(m_pMessage, "netbsd") || str_find_nocase(m_pMessage, "templeos") ||
			str_find_nocase(m_pMessage, "serenity") || str_find_nocase(m_pMessage, "haiku") ||
			str_find_nocase(m_pMessage, "windows") || str_find_nocase(m_pMessage, "mac") || str_find_nocase(m_pMessage, "apple") ||
			str_find_nocase(m_pMessage, "microsoft") || str_find_nocase(m_pMessage, "android") || str_find_nocase(m_pMessage, "ios") ||
			str_find_nocase(m_pMessage, "ubuntu") || str_find_nocase(m_pMessage, "debian") || str_find_nocase(m_pMessage, "kali") || str_find_nocase(m_pMessage, "popos") ||
			str_find_nocase(m_pMessage, "manjaro") || str_find_nocase(m_pMessage, "arch") || str_find_nocase(m_pMessage, "gentoo") || str_find_nocase(m_pMessage, "opensuse")))
	{
#if defined(CONF_FAMILY_WINDOWS)
		WriteReplyBufFormat("%s I currently use windows.", m_pMessageAuthor);
#elif defined(CONF_PLATFORM_LINUX)
		WriteReplyBufFormat("%s I currently use linux.", m_pMessageAuthor);
#elif defined(CONF_PLATFORM_MACOS)
		WriteReplyBufFormat("%s I currently use macOS.", m_pMessageAuthor);
#elif defined(CONF_PLATFORM_ANDROID)
		WriteReplyBufFormat("%s I currently use android.", m_pMessageAuthor);
#elif defined(CONF_PLATFORM_OPENBSD)
		WriteReplyBufFormat("%s I currently use openBSD.", m_pMessageAuthor);
#elif defined(CONF_PLATFORM_FREEBSD)
		WriteReplyBufFormat("%s I currently use freeBSD.", m_pMessageAuthor);
#elif defined(CONF_PLATFORM_NETBSD)
		WriteReplyBufFormat("%s I currently use netBSD.", m_pMessageAuthor);
#elif defined(CONF_PLATFORM_HAIKU)
		WriteReplyBufFormat("%s I currently use Haiku.", m_pMessageAuthor);
#else
		WriteReplyBufFormat("%s No idea which operating system I use.", m_pMessageAuthor);
#endif
		return true;
	}
	return false;
}
