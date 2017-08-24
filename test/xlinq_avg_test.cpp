#include <gtest/gtest.h>
#include <memory>
#include <list>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_avg.h>

using namespace std;
using namespace xlinq;

TEST(XlinqAvgTest, EmptyCollectionThrowsException)
{
	auto numbers = list<int>();
	try
	{
		from(numbers) >> avg();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqAvgTest, AvgFromChar)
{
	char numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromUChar)
{
	unsigned char numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromShort)
{
	short numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromUShort)
{
	unsigned short numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromInt)
{
	int numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromUInt)
{
	unsigned int numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromLong)
{
	long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromULong)
{
	unsigned long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromLongLong)
{
	long long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromULongLong)
{
	unsigned long long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromFloat)
{
	float numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5, from(numbers) >> avg());
}

TEST(XlinqAvgTest, AvgFromDouble)
{
	double numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5, from(numbers) >> avg());
}

TEST(XlinqAvgTest, FloatAvgFromChar)
{
	char numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromUChar)
{
	unsigned char numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9.0f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromShort)
{
	short numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromUShort)
{
	unsigned short numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9.0f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromInt)
{
	int numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromUInt)
{
	unsigned int numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9.0f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromLong)
{
	long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromULong)
{
	unsigned long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9.0f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromLongLong)
{
	long long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromULongLong)
{
	unsigned long long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9.0f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromFloat)
{
	float numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, FloatAvgFromDouble)
{
	double numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3.5f, from(numbers) >> avg<float>());
}

TEST(XlinqAvgTest, IntAvgFromChar)
{
	char numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromUChar)
{
	unsigned char numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromShort)
{
	short numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromUShort)
{
	unsigned short numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromInt)
{
	int numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromUInt)
{
	unsigned int numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromLong)
{
	long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromULong)
{
	unsigned long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromLongLong)
{
	long long numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromULongLong)
{
	unsigned long long numbers[] = { 3, 7, 32, 1, 2 };
	ASSERT_EQ(9, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromFloat)
{
	float numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3, from(numbers) >> avg<int>());
}

TEST(XlinqAvgTest, IntAvgFromDouble)
{
	double numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-3, from(numbers) >> avg<int>());
}