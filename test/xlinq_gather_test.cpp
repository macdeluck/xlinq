#include <gtest/gtest.h>
#include <xlinq/xlinq_gather.h>
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