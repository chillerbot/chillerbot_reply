#pragma once

#include <cstddef>

class CChillerBotReply {
public:
	// returns true when it found a reply to `pMsg`
	// and fills the reply into `pReplyBuf`
	bool Reply(const char *pMsg, char *pReplyBuf, size_t ReplyBufLen);
};

