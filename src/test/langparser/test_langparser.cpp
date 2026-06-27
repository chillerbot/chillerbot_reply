#include <chillerbot_reply/chillerbot_reply.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>
#include <test/setup.h>

static void TestBasic()
{
	InitTest("clan");

	CChillerBotReplyContext Context;
	Context.m_aOwnTees[0].m_pClan = "Chilli.*";
	ExpectReply("can I join your clan?", "testo1 Chilli.* is a fun clan everybody that uses the skin greensward can join", Context);
}

int main()
{
	TestBasic();
}
