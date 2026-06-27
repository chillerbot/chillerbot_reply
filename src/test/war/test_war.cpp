#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>
#include <test/setup.h>

using namespace ddnet_base;

static void MockGetWarClansStr(char *pBuf, size_t BufLen, void *pUnusedUser)
{
	str_copy(pBuf, "foo, bar, baz", BufLen);
}

static void TestBasic()
{
	InitTest("war");

	CChillerBotReplyContext Context;
	Context.m_pfnGetWarClansStr = MockGetWarClansStr;
	ExpectReply("testo1 which clans do you war?", "testo1 I war those clans: foo, bar, baz", Context);
}

int main()
{
	TestBasic();
}
