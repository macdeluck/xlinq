#include <gtest/gtest.h>
#include <memory>
#include <list>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_sum.h>

using namespace std;
using namespace xlinq;

TEST(XlinqSumTest, EmptyCollectionThrowsException)
{
	auto numbers = list<int>();
	try
	{
		from(numbers) >> sum();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqSumTest, SumFromChar)
{
	char numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromUChar)
{
	unsigned char numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(45, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromShort)
{
	short numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromUShort)
{
	unsigned short numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(45, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromInt)
{
	int numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromUInt)
{
	unsigned int numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(45, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromLong)
{
	long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromULong)
{
	unsigned long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(45, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromLongLong)
{
	long long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromULongLong)
{
	unsigned long long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(45, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromFloat)
{
	float numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> sum());
}

TEST(XlinqSumTest, SumFromDouble)
{
	double numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> sum());
}