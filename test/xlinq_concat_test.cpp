#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_concat.h>
#include <memory>
#include <forward_list>
#include <list>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XLinqConcatForwardOnlyTest, Enumeration)
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

TEST(XLinqConcatForwardOnlyTest, FirstEmpty)
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

TEST(XLinqConcatForwardOnlyTest, SecondEmpty)
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

TEST(XLinqConcatForwardOnlyTest, BothEmpty)
{
	std::forward_list<int> first = { };
	std::forward_list<int> second = { };

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqConcatForwardAndBidirectionalTest, Enumeration)
{
	std::forward_list<int> first = { 1, 2, 3, 4, 5 };
	std::list<int> second = { 6, 7, 8, 9 };

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

TEST(XLinqConcatForwardAndBidirectionalTest, FirstEmpty)
{
	std::forward_list<int> first = {};
	std::list<int> second = { 6, 7, 8, 9 };

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

TEST(XLinqConcatForwardAndBidirectionalTest, SecondEmpty)
{
	std::forward_list<int> first = { 1, 2, 3, 4, 5 };
	std::list<int> second = {};

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

TEST(XLinqConcatForwardAndBidirectionalTest, BothEmpty)
{
	std::forward_list<int> first = {};
	std::list<int> second = {};

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqConcatBidirectionalAndForwardTest, Enumeration)
{
	std::list<int> first = { 1, 2, 3, 4, 5 };
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

TEST(XLinqConcatBidirectionalAndForwardTest, FirstEmpty)
{
	std::list<int> first = {};
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

TEST(XLinqConcatBidirectionalAndForwardTest, SecondEmpty)
{
	std::list<int> first = { 1, 2, 3, 4, 5 };
	std::forward_list<int> second = {};

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

TEST(XLinqConcatBidirectionalAndForwardTest, BothEmpty)
{
	std::list<int> first = {};
	std::forward_list<int> second = {};

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqConcatBidirectionalOnlyTest, Enumeration)
{
	std::list<int> first = { 1, 2, 3, 4, 5 };
	std::list<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
	
	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatBidirectionalOnlyTest, FirstEmpty)
{
	std::list<int> first = { };
	std::list<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatBidirectionalOnlyTest, SecondEmpty)
{
	std::list<int> first = { 1, 2, 3, 4, 5 };
	std::list<int> second = { };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatBidirectionalOnlyTest, BothEmpty)
{
	std::list<int> first = {};
	std::list<int> second = {};

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatBidirectionalAndRandomAccessTest, Enumeration)
{
	std::list<int> first = { 1, 2, 3, 4, 5 };
	std::vector<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatBidirectionalAndRandomAccessTest, FirstEmpty)
{
	std::list<int> first = {};
	std::vector<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatBidirectionalAndRandomAccessTest, SecondEmpty)
{
	std::list<int> first = { 1, 2, 3, 4, 5 };
	std::vector<int> second = {};

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatBidirectionalAndRandomAccessTest, BothEmpty)
{
	std::list<int> first = {};
	std::vector<int> second = {};

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();

	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatRandomAccessAndBidirectionalTest, Enumeration)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::list<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatRandomAccessAndBidirectionalTest, FirstEmpty)
{
	std::vector<int> first = {};
	std::list<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatRandomAccessAndBidirectionalTest, SecondEmpty)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::list<int> second = {};

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatRandomAccessAndBidirectionalTest, BothEmpty)
{
	std::vector<int> first = {};
	std::list<int> second = {};

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEndEnumerator();

	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqConcatRandomAccessOnlyTest, Enumeration)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::vector<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
	ASSERT_TRUE(enumerator->advance(6));
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-10));
	ASSERT_FALSE(enumerator->advance(10));

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEnumeratorAt(2);
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(4));
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-8));
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->advance(8));
	ASSERT_EQ(9, enumerator->current());
	ASSERT_FALSE(enumerator->advance(1));
	ASSERT_TRUE(enumerator->advance(-1));
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-8));
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-1));
	ASSERT_TRUE(enumerator->advance(5));
	ASSERT_EQ(5, enumerator->current());
}

TEST(XLinqConcatRandomAccessOnlyTest, FirstEmpty)
{
	std::vector<int> first = { };
	std::vector<int> second = { 6, 7, 8, 9 };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());
	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(7, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-2));
	ASSERT_FALSE(enumerator->advance(5));

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEnumeratorAt(2);
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(9, enumerator->current());
	ASSERT_FALSE(enumerator->advance(1));
	ASSERT_FALSE(enumerator->advance(-5));
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-1));
	ASSERT_FALSE(enumerator->advance(5));
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(8, enumerator->current());
}

TEST(XLinqConcatRandomAccessOnlyTest, SecondEmpty)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::vector<int> second = { };

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
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_TRUE(enumerator->advance(4));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_FALSE(enumerator->advance(2));
	ASSERT_TRUE(enumerator->advance(-3));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->advance(3));
	ASSERT_FALSE(enumerator->advance(-6));

	enumerator = from(first) >> concat(second) >> getEndEnumerator();
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(first) >> concat(second) >> getEnumeratorAt(2);
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->advance(1));
	ASSERT_TRUE(enumerator->advance(-1));
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-1));
	ASSERT_FALSE(enumerator->advance(6));
	ASSERT_FALSE(enumerator->advance(-6));
	ASSERT_TRUE(enumerator->advance(5));
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ(1, enumerator->current());
}

TEST(XLinqConcatRandomAccessOnlyTest, BothEmpty)
{
	std::vector<int> first = {};
	std::vector<int> second = {};

	auto enumerator = from(first) >> concat(second) >> getEnumerator();

	ASSERT_FALSE(enumerator->next());
	ASSERT_FALSE(enumerator->back());
	ASSERT_FALSE(enumerator->advance(1));
	ASSERT_FALSE(enumerator->advance(-1));

	enumerator = from(first) >> concat(second) >> getEndEnumerator();

	ASSERT_FALSE(enumerator->back());
	ASSERT_FALSE(enumerator->advance(1));
	ASSERT_FALSE(enumerator->advance(-1));

	enumerator = from(first) >> concat(second) >> getEnumeratorAt(1);

	ASSERT_FALSE(enumerator->back());
	ASSERT_FALSE(enumerator->advance(1));
	ASSERT_FALSE(enumerator->advance(-1));
}

TEST(XLinqConcatRandomAccessOnlyTest, Size)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::vector<int> second = { 6, 7, 8, 9 };

	ASSERT_EQ(9, (from(first) >> concat(second))->size());
}