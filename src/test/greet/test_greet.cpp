#include <chillerbot_reply/chillerbot_reply.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>
#include <test/setup.h>

static void TestBasic()
{
	InitTest("hello");

	ExpectReply("hellow", "yellow");
}

int main()
{
	TestBasic();
}
