#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_first.h>
#include <memory>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XlinqFirstTest, GetFirstElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> first();
	ASSERT_EQ(1, val);
}

TEST(XlinqFirstTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	try
	{
		from(numbers) >> first();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{

	}
}

TEST(XlinqFirstPredicateTest, GetFirstElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> first([](int a) { return a > 2; });
	ASSERT_EQ(3, val);
}

TEST(XlinqFirstPredicateTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	try
	{
		from(numbers) >> first([](int a) { return a > 2; });
		FAIL();
	}
	catch (IterationFinishedException exc)
	{

	}
}

TEST(XlinqFirstPredicateTest, NoElementSatisfiesCondition)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	try
	{
		from(numbers) >> first([](int a) { return a > 10; });
		FAIL();
	}
	catch (IterationFinishedException exc)
	{

	}
}

TEST(XlinqFirstOrDefaultTest, GetFirstElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> first_or_default(0);
	ASSERT_EQ(1, val);
}

TEST(XlinqFirstOrDefaultTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	auto val = from(numbers) >> first_or_default(0);
	ASSERT_EQ(0, val);
}

TEST(XlinqFirstOrDefaultPredicateTest, GetFirstElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> first_or_default(0, [](int a) { return a > 2; });
	ASSERT_EQ(3, val);
}

TEST(XlinqFirstOrDefaultPredicateTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	auto val = from(numbers) >> first_or_default(0, [](int a) { return a > 2; });
	ASSERT_EQ(0, val);
}

TEST(XlinqFirstOrDefaultPredicateTest, NoElementSatisfiesCondition)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> first_or_default(0, [](int a) { return a > 10; });
	ASSERT_EQ(0, val);
}