#include <chillerbot_reply/chillerbot_reply.h>
#include <chillerbot_reply/langparser.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>
#include <test/setup.h>

static void TestBasic()
{
	InitTest("langparser");

	// proper greetings
	EXPECT_TRUE(LangParser::IsGreeting("hi"));
	EXPECT_TRUE(LangParser::IsGreeting("hi!"));

	// possible false positives
	EXPECT_FALSE(LangParser::IsGreeting("Do you know the streamer Hallowed1986?"));
	EXPECT_FALSE(LangParser::IsGreeting("Ich hoffe das wird ein guter tag"));
}

int main()
{
	TestBasic();
}
