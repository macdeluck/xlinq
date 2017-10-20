#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_join.h>
#include <memory>
#include <vector>
#include <utility>

using namespace std;
using namespace xlinq;

TEST(XlinqJoinTest, TestJoin)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	vector<string> strings = { "a", "bb", "zz", "xx", "eeeee", "fffff" };
	auto enumerator = from(numbers) >> join(strings, 
		[](int i) { return i; }, 
		[](string s) { return s.size(); }, 
		[](int i, string s) { return to_string(i) + s; }) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("1a", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2bb", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2zz", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2xx", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5eeeee", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5fffff", enumerator->current());
	ASSERT_FALSE(enumerator->next());
}