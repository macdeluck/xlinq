#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_any.h>
#include <xlinq/xlinq_enumerable.h>
using namespace xlinq;

TEST(XlinqAnyTest, CollectionWithElements)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> any());
}

TEST(XlinqAnyTest, EmptyCollection)
{
	ASSERT_FALSE(Enumerable::empty<int>() >> any());
}

TEST(XlinqAnyWithPredicateTest, CollectionWithElements)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> any([](int n) { return n > 3; }));
}

TEST(XlinqAnyWithPredicateTest, CollectionWithElementsPredicateNotSatisfied)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	ASSERT_FALSE(from(numbers) >> any([](int n) { return n > 10; }));
}

TEST(XlinqAnyWithPredicateTest, EmptyCollection)
{
	ASSERT_FALSE(Enumerable::empty<int>() >> any([](int n) { return n > 10; }));
}