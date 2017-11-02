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

TEST(XLinqConcatForwardOnlyTest, CloneAndEqualsEnumeratorTest)
{
	std::forward_list<int> vfirst = { 1, 2, 3, 4, 5 };
	std::forward_list<int> vsecond = { 6, 7, 8, 9 };

	auto enumerator = from(vfirst) >> concat(vsecond) >> getEnumerator();

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
	ASSERT_TRUE(second->next());
	ASSERT_EQ(6, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(7, second->current());
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

TEST(XLinqConcatBidirectionalOnlyTest, CloneAndEqualsEnumeratorTest)
{
	std::list<int> vfirst = { 1, 2, 3, 4, 5 };
	std::list<int> vsecond = { 6, 7, 8, 9 };

	auto enumerator = from(vfirst) >> concat(vsecond) >> getEnumerator();

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
	ASSERT_TRUE(second->next());
	ASSERT_EQ(6, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(7, second->current());

	ASSERT_TRUE(enumerator->back());
	ASSERT_TRUE(enumerator->back());
	ASSERT_TRUE(enumerator->back());
	ASSERT_TRUE(enumerator->equals(second));
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

TEST(XLinqConcatRandomAccessOnlyTest, CloneAndEqualsEnumeratorTest)
{
	std::vector<int> vfirst = { 1, 2, 3, 4, 5 };
	std::vector<int> vsecond = { 6, 7, 8, 9 };

	auto enumerator = from(vfirst) >> concat(vsecond) >> getEnumerator();

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
	ASSERT_TRUE(second->next());
	ASSERT_EQ(6, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(7, second->current());

	ASSERT_TRUE(enumerator->back());
	ASSERT_TRUE(enumerator->back());
	ASSERT_TRUE(enumerator->back());
	ASSERT_TRUE(enumerator->equals(second));
}

TEST(XLinqConcatRandomAccessOnlyTest, DistanceLtGtTest)
{
	std::vector<int> vfirst = { 1, 2, 3, 4, 5 };
	std::vector<int> vsecond = { 6, 7, 8, 9 };

	auto enumerable = from(vfirst) >> concat(vsecond);
	auto it = enumerable >> getEnumerator();
	auto end = enumerable >> getEndEnumerator();

	ASSERT_FALSE(it->equals(end));
	ASSERT_TRUE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_TRUE(end->greater_than(it));

	ASSERT_EQ(enumerable->size() + 1, it->distance_to(end));
	ASSERT_EQ(-(enumerable->size() + 1), end->distance_to(it));

	ASSERT_TRUE(it->next());

	ASSERT_FALSE(it->equals(end));
	ASSERT_TRUE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_TRUE(end->greater_than(it));

	ASSERT_EQ(enumerable->size(), it->distance_to(end));
	ASSERT_EQ(-enumerable->size(), end->distance_to(it));

	auto itc = std::dynamic_pointer_cast<IRandomAccessEnumerator<int>>(it->clone());

	ASSERT_TRUE(it->equals(itc));
	ASSERT_FALSE(it->less_than(itc));
	ASSERT_FALSE(itc->less_than(it));
	ASSERT_FALSE(it->greater_than(itc));
	ASSERT_FALSE(itc->greater_than(it));

	ASSERT_EQ(0, it->distance_to(itc));
	ASSERT_EQ(0, itc->distance_to(it));

	ASSERT_TRUE(end->back());

	ASSERT_FALSE(it->equals(end));
	ASSERT_TRUE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_TRUE(end->greater_than(it));

	ASSERT_EQ(enumerable->size() - 1, it->distance_to(end));
	ASSERT_EQ(-(enumerable->size() - 1), end->distance_to(it));

	ASSERT_TRUE(it->advance(it->distance_to(end)));

	ASSERT_TRUE(it->equals(end));
	ASSERT_FALSE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_FALSE(end->greater_than(it));

	ASSERT_EQ(0, it->distance_to(end));
	ASSERT_EQ(0, end->distance_to(it));
}

TEST(XLinqConcatArray, Enumeration)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	int second[] = { 6, 7, 8, 9 };

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

TEST(XLinqConcatVectorPtr, Enumeration)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::vector<int> second { 6, 7, 8, 9 };

	auto enumerator = from(first) >> concat(&second) >> getEnumerator();

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

TEST(XLinqConcatVectorSharedPtr, Enumeration)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::shared_ptr<std::vector<int>> second(new std::vector<int>({ 6, 7, 8, 9 }));

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

TEST(XLinqConcatEnumerable, Enumeration)
{
	std::vector<int> first = { 1, 2, 3, 4, 5 };
	std::vector<int> second = { 6, 7, 8, 9 };

	auto enumerator = from(first) >> concat(from(second)) >> getEnumerator();

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