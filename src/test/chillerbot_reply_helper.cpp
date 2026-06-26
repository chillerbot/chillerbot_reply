#include <chillerbot_reply/chillerbot_reply.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>

void ExpectReply(const char *pMessage, const char *pExpectedReply, CChillerBotReplyContext Context)
{
	CChillerBotReply Bot;
	char aReply[512];
	CChillerBotReplyChatMessage Message;
	Message.m_pAuthor = "testo1";
	Message.m_pMessage = pMessage;
	Bot.m_Context = Context;
	Bot.Reply(&Message, aReply, sizeof(aReply));
	EXPECT_STREQ(aReply, pExpectedReply);
}
