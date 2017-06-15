#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_concat.h>
#include <memory>
#include <forward_list>

using namespace std;
using namespace xlinq;

TEST(XLinqConcatTest, XlinqConcatForwardOnlyTest)
{
	std::forward_list<int> first = { 1, 2, 3, 4, 5 };
	std::forward_list<int> second = { 6, 7, 8, 9 };

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

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
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqConcatTest, XlinqConcatForwardOnlyFirstEmptyTest)
{
	std::forward_list<int> first = { };
	std::forward_list<int> second = { 6, 7, 8, 9 };

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqConcatTest, XlinqConcatForwardOnlySecondEmptyTest)
{
	std::forward_list<int> first = { 1, 2, 3, 4, 5 };
	std::forward_list<int> second = { };

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

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

TEST(XLinqConcatTest, XlinqConcatForwardOnlyBothEmptyTest)
{
	std::forward_list<int> first = { };
	std::forward_list<int> second = { };

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
}