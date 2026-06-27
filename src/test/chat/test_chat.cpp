#include <chillerbot_reply/chillerbot_reply.h>
#include <chillerbot_reply/langparser.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>
#include <test/setup.h>

using namespace LangParser;

static void TestBasic()
{
	InitTest("chat");

	ExpectReply("Русский", "testo1 english please - я не говорю по-русски");
	ExpectReply("ты рыбья голова", "testo1 english please - я не говорю по-русски");
}

int main()
{
	TestBasic();
}
