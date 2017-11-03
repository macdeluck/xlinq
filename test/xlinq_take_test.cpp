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

TEST(XLinqTakeTest, BidirectionalEnumerable_CloneAndEqualsEnumeratorTest)
{
	list<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
	auto enumerable = from(numbers) >> take(3);
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

TEST(XLinqTakeTest, RandomAccessEnumerable_CloneAndEqualsEnumeratorTest)
{
	vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
	auto enumerable = from(numbers) >> take(3);
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
}

TEST(XLinqTakeTest, RandomAccessEnumerable_DistanceLtGtTest)
{
	vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
	auto enumerable = from(numbers) >> take(3);
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