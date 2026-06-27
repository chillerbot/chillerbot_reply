#include <chillerbot_reply/chillerbot_reply.h>
#include <chillerbot_reply/langparser.h>
#include <test/chillerbot_assert.h>
#include <test/chillerbot_reply_helper.h>
#include <test/setup.h>

using namespace LangParser;

static void TestBasic()
{
	InitTest("langparser");

	//
	// greetings
	//

	// proper greetings
	EXPECT_TRUE(IsGreeting("hi"));
	EXPECT_TRUE(IsGreeting("hi!"));

	// possible false positives
	EXPECT_FALSE(IsGreeting("Do you know the streamer Hallowed1986?"));
	EXPECT_FALSE(IsGreeting("Ich hoffe das wird ein guter tag"));

	//
	// bye
	//

	// proper
	EXPECT_TRUE(IsBye("bye"));
	EXPECT_TRUE(IsBye("bye!"));
	EXPECT_TRUE(IsBye("ChillerDragon: bye"));

	// possible false positives
	EXPECT_FALSE(IsBye("bye: hello!"));

	//
	// insult
	//

	// proper
	EXPECT_TRUE(IsInsult("fuck your relatives"));
	EXPECT_TRUE(IsInsult("DELETE THE GAME"));

	// possible false positives
	EXPECT_FALSE(IsInsult("fuck yeah! that was awesome"));
	EXPECT_FALSE(IsInsult("dogshit i died"));

	//
	// why
	//

	// proper
	EXPECT_TRUE(IsQuestionWhy("why did you!"));
	EXPECT_TRUE(IsQuestionWhy("warum ist das passiert?!"));

	// possible false positives
	EXPECT_FALSE(IsQuestionWhy("when did what happen?"));
}

static void TestStrFindOrder()
{
	EXPECT_TRUE(StrFindOrder("can i ask you something", 2, "can", "ask"));
	EXPECT_TRUE(StrFindOrder("foobarbaz", 3, "foo", "oobar", "barbaz"));
	EXPECT_TRUE(
		StrFindOrder(
			"foo, bar, baz, qux, quux, quuz, corge, grault, garply, waldo, fred, plugh, xyzzy, and thud",
			14,
			"foo", "bar", "baz", "qux", "quux", "quuz", "corge", "grault", "garply", "waldo", "fred", "plugh", "xyzzy", "thud"));

	EXPECT_FALSE(StrFindOrder("i ask you can something", 2, "can", "ask"));
	EXPECT_FALSE(StrFindOrder("foo baz bar", 3, "foo", "bar", "baz"));
}

static void TestStrFindIndex()
{
	EXPECT_EQ(StrFindIndex("foo bar", "bar"), 4);
	EXPECT_EQ(StrFindIndex("foo bar", "404"), -1);
	EXPECT_EQ(StrFindIndex("foo bar", "foo"), 0);
}

int main()
{
	TestBasic();
	TestStrFindOrder();
	TestStrFindIndex();
}
