#include <test/chillerbot_assert.h>
#include <test/setup.h>

static void TestBasic()
{
	InitTest("str");
	EXPECT_STREQ("hello", "hello");
}

int main()
{
	TestBasic();
}
