#include <gtest/gtest.h>
#include <xlinq/xlinq_element_at.h>
#include <xlinq/xlinq_from.h>
#include <forward_list>
#include <list>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XLinqElementAtTest, ElementAtFromEnumerable)
{
	forward_list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	ASSERT_EQ(1, from(numbers) >> element_at(4));
}

TEST(XLinqElementAtTest, ElementAtFromBidirectionalEnumerable)
{
	list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	ASSERT_EQ(1, from(numbers) >> element_at(4));
}

TEST(XLinqElementAtTest, ElementAtFromRandomAccessEnumerable)
{
	vector<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	ASSERT_EQ(1, from(numbers) >> element_at(4));
}

TEST(XLinqElementAtTest, ElementAtEmptyEnumerable)
{
	forward_list<int> numbers;
	try
	{
		from(numbers) >> element_at(0);
		FAIL();
	}
	catch (IterationFinishedException)
	{
	}
	try
	{
		from(numbers) >> element_at(-1);
		FAIL();
	}
	catch (IterationNotStartedException)
	{
	}
}

TEST(XLinqElementAtTest, ElementAtEmptyBidirectionalEnumerable)
{
	list<int> numbers;
	try
	{
		from(numbers) >> element_at(0);
		FAIL();
	}
	catch (IterationFinishedException)
	{
	}
	try
	{
		from(numbers) >> element_at(-1);
		FAIL();
	}
	catch (IterationNotStartedException)
	{
	}
}

TEST(XLinqElementAtTest, ElementAtEmptyRandomAccessEnumerable)
{
	vector<int> numbers;
	try
	{
		from(numbers) >> element_at(0);
		FAIL();
	}
	catch (IterationFinishedException)
	{
	}
	try
	{
		from(numbers) >> element_at(-1);
		FAIL();
	}
	catch (IterationNotStartedException)
	{
	}
}

TEST(XLinqElementAtOrDefaultTest, ElementAtOrDefaultFromEnumerable)
{
	forward_list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	ASSERT_EQ(1, from(numbers) >> element_at_or_default(4, 0));
}

TEST(XLinqElementAtOrDefaultTest, ElementAtOrDefaultFromBidirectionalEnumerable)
{
	list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	ASSERT_EQ(1, from(numbers) >> element_at_or_default(4, 0));
}

TEST(XLinqElementAtOrDefaultTest, ElementAtOrDefaultFromRandomAccessEnumerable)
{
	vector<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	ASSERT_EQ(1, from(numbers) >> element_at_or_default(4, 0));
}

TEST(XLinqElementAtOrDefaultTest, ElementAtOrDefaultEmptyEnumerable)
{
	forward_list<int> numbers;
	ASSERT_EQ(0, from(numbers) >> element_at_or_default(0, 0));
	ASSERT_EQ(0, from(numbers) >> element_at_or_default(-1, 0));
}

TEST(XLinqElementAtOrDefaultTest, ElementAtOrDefaultEmptyBidirectionalEnumerable)
{
	list<int> numbers;
	ASSERT_EQ(0, from(numbers) >> element_at_or_default(0, 0));
	ASSERT_EQ(0, from(numbers) >> element_at_or_default(-1, 0));
}

TEST(XLinqElementAtOrDefaultTest, ElementAtOrDefaultEmptyRandomAccessEnumerable)
{
	vector<int> numbers;
	ASSERT_EQ(0, from(numbers) >> element_at_or_default(0, 0));
	ASSERT_EQ(0, from(numbers) >> element_at_or_default(-1, 0));
}