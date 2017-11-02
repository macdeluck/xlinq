#include <gtest/gtest.h>
#include <xlinq/xlinq_gather.h>
#include <xlinq/xlinq_where.h>
#include "model/xlinq_test_model.h"
#include <memory>
#include <vector>

using namespace std;
using namespace xlinq;

shared_ptr<IEnumerable<int>> gatherInts()
{
	vector<int> vec = { 1, 2, 3, 4, 5 };
	return from(vec) >> gather();
}

TEST(XLinqGatherTest, XlinqGatherValuesAllocatedOnStackTest)
{
	auto enumerator = gatherInts() >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqLazyGatherTest, TestGatherReleasingPointer)
{
	int useCount = 0;
	auto ints = { 1, 2, 3, 4, 5 };
	auto enumerable = from(ints) >> where([&useCount](int a) { ++useCount; return true;}) >> lazy_gather();
	auto enumerator = enumerable >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_EQ(3, useCount);

	auto enumerator2 = enumerable >> getEnumerator();
	ASSERT_TRUE(enumerator2->next());
	ASSERT_EQ(1, enumerator2->current());
	ASSERT_TRUE(enumerator2->next());
	ASSERT_EQ(2, enumerator2->current());
	ASSERT_TRUE(enumerator2->next());
	ASSERT_EQ(3, enumerator2->current());

	// not incremented, source ignored
	ASSERT_EQ(3, useCount);
	ASSERT_TRUE(enumerator2->next());
	ASSERT_EQ(4, enumerator2->current());
	ASSERT_TRUE(enumerator2->next());
	ASSERT_EQ(5, enumerator2->current());
	ASSERT_FALSE(enumerator2->next());
	// took least elements from source
	ASSERT_EQ(5, useCount);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->next());
	// not incremented, source ignored
	ASSERT_EQ(5, useCount);
}

TEST(XLinqLazyGatherTest, CloneAndEqualsEnumeratorTest)
{
	auto ints = { 1, 2, 3, 4, 5 };
	auto enumerable = from(ints) >> lazy_gather();
	auto enumerator = enumerable >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ(2, enumerator->current());
	ASSERT_EQ(1, second->current());

	while (enumerator->next());

	ASSERT_EQ(1, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(2, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(3, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(4, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(5, second->current());
	ASSERT_FALSE(second->next());

	ASSERT_TRUE(enumerator->equals(second));
}
