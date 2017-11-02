#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_group_by.h>
#include <xlinq/xlinq_first.h>
#include <xlinq/xlinq_count.h>
#include <memory>
#include <vector>
#include <utility>

using namespace std;
using namespace xlinq;

TEST(XlinqGroupByTest, TestGrouping)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	auto enumerator = from(numbers) >> group_by([](int num) { return num % 2 + 1; }) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	auto grouping = enumerator->current();
	ASSERT_EQ(2, grouping->getKey());
	auto groupingEnumerator = grouping->getEnumerator();
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(1, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(3, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(5, groupingEnumerator->current());
	ASSERT_FALSE(groupingEnumerator->next());
	ASSERT_TRUE(enumerator->next());
	grouping = enumerator->current();
	ASSERT_EQ(1, grouping->getKey());
	ASSERT_FALSE(enumerator->next());
	groupingEnumerator = grouping->getEnumerator();
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(2, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(4, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(6, groupingEnumerator->current());
	ASSERT_FALSE(groupingEnumerator->next());

	groupingEnumerator = from(numbers) >> group_by([](int num) { return (num % 3) != 0; }) >> first() >> getEnumerator();
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(1, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(2, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(4, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(5, groupingEnumerator->current());
	ASSERT_FALSE(groupingEnumerator->next());
}

TEST(XlinqGroupByResultTest, TestGrouping)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	auto enumerator = from(numbers) >> group_by([](int num) { return num % 2 + 1; }, [](std::shared_ptr<IGrouping<int, int>> g) { return std::pair<int, int>(g->getKey(), g >> count());}) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	auto grouping = enumerator->current();
	ASSERT_EQ(2, grouping.first);
	ASSERT_EQ(3, grouping.second);
	ASSERT_TRUE(enumerator->next());
	grouping = enumerator->current();
	ASSERT_EQ(1, grouping.first);
	ASSERT_EQ(3, grouping.second);
	ASSERT_FALSE(enumerator->next());
}

TEST(XlinqGroupByTest, CloneAndEqualsEnumeratorTest)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	auto enumerable = from(numbers) >> group_by([](int num) { return num % 2 + 1; });
	auto enumerator = enumerable >> getEnumerator();
	auto secondEnumerator = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(secondEnumerator));
	ASSERT_TRUE(secondEnumerator->equals(enumerator));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(secondEnumerator));
	ASSERT_FALSE(secondEnumerator->equals(enumerator));

	auto grouping = enumerator->current();
	ASSERT_EQ(2, grouping->getKey());

	auto groupingEnumerator = grouping->getEnumerator();
	auto secondGroupingEnumerator = groupingEnumerator->clone();
	ASSERT_TRUE(groupingEnumerator->equals(secondGroupingEnumerator));
	ASSERT_TRUE(secondGroupingEnumerator->equals(groupingEnumerator));
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_FALSE(groupingEnumerator->equals(secondGroupingEnumerator));
	ASSERT_FALSE(secondGroupingEnumerator->equals(groupingEnumerator));

	ASSERT_EQ(1, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(3, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(5, groupingEnumerator->current());
	ASSERT_FALSE(groupingEnumerator->next());

	ASSERT_TRUE(secondGroupingEnumerator->next());
	ASSERT_EQ(1, secondGroupingEnumerator->current());
	ASSERT_TRUE(secondGroupingEnumerator->next());
	ASSERT_EQ(3, secondGroupingEnumerator->current());
	ASSERT_TRUE(secondGroupingEnumerator->next());
	ASSERT_EQ(5, secondGroupingEnumerator->current());
	ASSERT_FALSE(secondGroupingEnumerator->next());
	ASSERT_TRUE(groupingEnumerator->equals(secondGroupingEnumerator));
	ASSERT_TRUE(secondGroupingEnumerator->equals(groupingEnumerator));

	ASSERT_TRUE(secondEnumerator->next());
	ASSERT_TRUE(enumerator->equals(secondEnumerator));
	ASSERT_TRUE(secondEnumerator->equals(enumerator));
}


/**
*	Simple generally invalid hasher just for test purposes.
*/
struct MyHash
{
	int operator()(const int& val) const
	{
		return val / 3;
	}
};

/**
*	Simple generally invalid equality comparer just for test purposes.
*/
struct MyEq
{
	bool operator()(const int& first, const int& second) const
	{
		return (first % 3) == (second % 3);
	}
};

TEST(XlinqGroupByTest, TestGroupingOwnHasherAndEqComp)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	auto enumerator = from(numbers) >> group_by([](int num) { return num % 2 + 1; }, MyHash(), MyEq()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	auto grouping = enumerator->current();
	ASSERT_EQ(2, grouping->getKey());
	auto groupingEnumerator = grouping->getEnumerator();
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(1, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(3, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(5, groupingEnumerator->current());
	ASSERT_FALSE(groupingEnumerator->next());
	ASSERT_TRUE(enumerator->next());
	grouping = enumerator->current();
	ASSERT_EQ(1, grouping->getKey());
	ASSERT_FALSE(enumerator->next());
	groupingEnumerator = grouping->getEnumerator();
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(2, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(4, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(6, groupingEnumerator->current());
	ASSERT_FALSE(groupingEnumerator->next());

	groupingEnumerator = from(numbers) >> group_by([](int num) { return (num % 3) != 0; }, MyHash(), MyEq()) >> first() >> getEnumerator();
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(1, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(2, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(4, groupingEnumerator->current());
	ASSERT_TRUE(groupingEnumerator->next());
	ASSERT_EQ(5, groupingEnumerator->current());
	ASSERT_FALSE(groupingEnumerator->next());
}

TEST(XlinqGroupByResultTest, TestGroupingOwnHasherAndEqComp)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	auto enumerable = from(numbers) >> group_by([](int num) { return num % 2 + 1; }, [](std::shared_ptr<IGrouping<int, int>> g) { return std::pair<int, int>(g->getKey(), g >> count());}, MyHash(), MyEq());
	auto enumerator = enumerable >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	auto grouping = enumerator->current();
	ASSERT_EQ(2, grouping.first);
	ASSERT_EQ(3, grouping.second);
	ASSERT_TRUE(enumerator->next());
	grouping = enumerator->current();
	ASSERT_EQ(1, grouping.first);
	ASSERT_EQ(3, grouping.second);
	ASSERT_FALSE(enumerator->next());
}