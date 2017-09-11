#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_all.h>
#include <xlinq/xlinq_enumerable.h>
using namespace xlinq;

TEST(XlinqAllTest, CollectionWithElements)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	ASSERT_FALSE(from(numbers) >> all([](int n) { return n > 3; }));
}

TEST(XlinqAllTest, CollectionWithElementsPredicateNotSatisfied)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> all([](int n) { return n > 0; }));
}

TEST(XlinqAllTest, EmptyCollection)
{
	ASSERT_TRUE(Enumerable::empty<int>() >> all([](int n) { return n > 0; }));
}