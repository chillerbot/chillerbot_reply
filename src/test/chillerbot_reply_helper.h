#pragma once

#include <chillerbot_reply/chillerbot_reply.h>

void ExpectReply(const char *pMessage, const char *pExpectedReply, CChillerBotReplyContext Context = {});
