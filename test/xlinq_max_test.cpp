#include <gtest/gtest.h>
#include <memory>
#include <list>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_max.h>

using namespace std;
using namespace xlinq;

TEST(XlinqMaxTest, EmptyCollectionThrowsException)
{
	auto numbers = list<int>();
	try
	{
		from(numbers) >> max();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqMaxTest, MaximumFromChar)
{
	char numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(7, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromUChar)
{
	unsigned char numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(32, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromShort)
{
	short numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(7, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromUShort)
{
	unsigned short numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(32, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromInt)
{
	int numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(7, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromUInt)
{
	unsigned int numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(32, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromLong)
{
	long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(7, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromULong)
{
	unsigned long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(32, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromLongLong)
{
	long long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(7, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromULongLong)
{
	unsigned long long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(32, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromFloat)
{
	float numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(7, from(numbers) >> max());
}

TEST(XlinqMaxTest, MaximumFromDouble)
{
	double numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(7, from(numbers) >> max());
}