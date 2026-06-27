#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>

#include <cstdio>

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

void ExpectReplyContains(const char *pMessage, const char *pExpectedNeedle, CChillerBotReplyContext Context)
{
	CChillerBotReply Bot;
	char aReply[512];
	CChillerBotReplyChatMessage Message;
	Message.m_pAuthor = "testo1";
	Message.m_pMessage = pMessage;
	Bot.m_Context = Context;
	Bot.Reply(&Message, aReply, sizeof(aReply));

	bool Ok = ddnet_base::str_find_nocase(aReply, pExpectedNeedle) != nullptr;
	if(!Ok)
	{
		fprintf(stderr, "Expected reply:\n");
		fprintf(stderr, " '%s'\n", aReply);
		fprintf(stderr, "To contain the string:\n");
		fprintf(stderr, " '%s'\n", pExpectedNeedle);
		fprintf(stderr, "but it was not found\n");
	}
	EXPECT_TRUE(Ok);
}
