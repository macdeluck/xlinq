#include <gtest/gtest.h>
#include <memory>
#include <list>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_min.h>

using namespace std;
using namespace xlinq;

TEST(XlinqMinTest, EmptyCollectionThrowsException)
{
	auto numbers = list<int>();
	try
	{
		from(numbers) >> min();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqMinTest, MinimumFromChar)
{
	char numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-32, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromUChar)
{
	unsigned char numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(1, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromShort)
{
	short numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-32, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromUShort)
{
	unsigned short numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(1, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromInt)
{
	int numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-32, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromUInt)
{
	unsigned int numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(1, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromLong)
{
	long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-32, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromULong)
{
	unsigned long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(1, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromLongLong)
{
	long long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-32, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromULongLong)
{
	unsigned long long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(1, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromFloat)
{
	float numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-32, from(numbers) >> min());
}

TEST(XlinqMinTest, MinimumFromDouble)
{
	double numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-32, from(numbers) >> min());
}