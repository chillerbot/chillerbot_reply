#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>
#include <test/setup.h>

static void TestBasic()
{
	InitTest("operating_system");

	// portable test that basically only checks if the detect.h macros work at all
	// all known OS say "I currently use XXX" and only on unknown it says "No idea"
	// because this test could be ran on any os we can not check a full match here
	ExpectReplyContains("are you on mac?", "I currently use");
}

int main()
{
	TestBasic();
}
