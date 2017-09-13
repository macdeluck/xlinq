#include <gtest/gtest.h>
#include <xlinq/xlinq_take.h>
#include <xlinq/xlinq_from.h>
#include <forward_list>
#include <list>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XLinqTakeTest, TakeFromEnumerable)
{
	forward_list<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
	auto enumerator = from(numbers) >> take(3) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqTakeTest, TakeFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
	auto enumerator = from(numbers) >> take(3) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqTakeTest, TakeFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
	auto enumerator = from(numbers) >> take(3) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(4));
	ASSERT_FALSE(enumerator->advance(-4));

	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(1, enumerator->current());

	enumerator = from(numbers) >> take(3) >> getEndEnumerator();

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(4));
	ASSERT_FALSE(enumerator->advance(-4));

	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(1, enumerator->current());

	enumerator = from(numbers) >> take(3) >> getEnumeratorAt(1);
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(1, enumerator->current());
}

TEST(XLinqTakeTest, TakeMoreElementsThanCollectionContainsFromEnumerable)
{
	forward_list<int> numbers = { 1, 2, 3 };
	auto enumerator = from(numbers) >> take(10) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqTakeTest, TakeMoreElementsThanCollectionContainsFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 2, 3 };
	auto enumerator = from(numbers) >> take(3) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqTakeTest, TakeMoreElementsThanCollectionContainsFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 2, 3 };
	auto enumerator = from(numbers) >> take(3) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(4));
	ASSERT_FALSE(enumerator->advance(-4));

	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(1, enumerator->current());

	enumerator = from(numbers) >> take(3) >> getEndEnumerator();

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(4));
	ASSERT_FALSE(enumerator->advance(-4));

	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(1, enumerator->current());

	enumerator = from(numbers) >> take(3) >> getEnumeratorAt(1);
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(1, enumerator->current());
}

TEST(XLinqTakeWhileTest, TakeFromEnumerable)
{
	forward_list<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
	auto enumerator = from(numbers) >> take_while([](int a) { return a < 4; }) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}