#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_last.h>
#include <vector>
#include <forward_list>

using namespace std;
using namespace xlinq;

TEST(XlinqLastTest, GetLastElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last();
	ASSERT_EQ(5, val);
}

TEST(XlinqLastTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	try
	{
		from(numbers) >> last();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqLastTest, GetLastElementOfCollectionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last();
	ASSERT_EQ(5, val);
}

TEST(XlinqLastTest, CollectionContainsNoElementsForwardList)
{
	std::forward_list<int> numbers;
	try
	{
		from(numbers) >> last();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqLastPredicateTest, GetLastElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last([](int a) { return a < 3; });
	ASSERT_EQ(2, val);
}

TEST(XlinqLastPredicateTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	try
	{
		from(numbers) >> last([](int a) { return a < 3; });
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqLastPredicateTest, NoElementSatisfiesCondition)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	try
	{
		from(numbers) >> last([](int a) { return a < 0; });
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqLastPredicateTest, GetLastElementOfCollectionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last([](int a) { return a < 3; });
	ASSERT_EQ(2, val);
}

TEST(XlinqLastPredicateTest, CollectionContainsNoElementsForwardList)
{
	std::forward_list<int> numbers;
	try
	{
		from(numbers) >> last([](int a) { return a < 3; });
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqLastPredicateTest, NoElementSatisfiesConditionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	try
	{
		from(numbers) >> last([](int a) { return a < 0; });
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqLastOrDefaultTest, GetLastElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(5, val);
}

TEST(XlinqLastOrDefaultTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(0, val);
}

TEST(XlinqLastOrDefaultTest, GetLastElementOfCollectionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(5, val);
}

TEST(XlinqLastOrDefaultTest, CollectionContainsNoElementsForwardList)
{
	std::forward_list<int> numbers;
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(0, val);
}

TEST(XlinqLastOrDefaultPredicateTest, GetLastElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0, [](int a) { return a < 3; });
	ASSERT_EQ(2, val);
}

TEST(XlinqLastOrDefaultPredicateTest, CollectionContainsNoElements)
{
	std::vector<int> numbers;
	auto val = from(numbers) >> last_or_default(0, [](int a) { return a < 0; });
	ASSERT_EQ(0, val);
}

TEST(XlinqLastOrDefaultPredicateTest, NoElementSatisfiesCondition)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0, [](int a) { return a < 0; });
	ASSERT_EQ(0, val);
}

TEST(XlinqLastOrDefaultPredicateTest, GetLastElementOfCollectionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0, [](int a) { return a < 3; });
	ASSERT_EQ(2, val);
}

TEST(XlinqLastOrDefaultPredicateTest, CollectionContainsNoElementsForwardList)
{
	std::forward_list<int> numbers;
	auto val = from(numbers) >> last_or_default(0, [](int a) { return a < 3; });
	ASSERT_EQ(0, val);
}

TEST(XlinqLastOrDefaultPredicateTest, NoElementSatisfiesConditionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0, [](int a) { return a < 0; });
	ASSERT_EQ(0, val);
}
