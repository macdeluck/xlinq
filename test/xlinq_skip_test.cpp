#include <gtest/gtest.h>
#include <xlinq/xlinq_skip.h>
#include <xlinq/xlinq_from.h>
#include <forward_list>
#include <list>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XLinqSkipTest, SkipFromEnumerable)
{
	forward_list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	auto enumerator = from(numbers) >> skip(4) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSkipTest, SkipFromBidirectionalEnumerable)
{
	list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	auto enumerator = from(numbers) >> skip(4) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSkipTest, BidirectionalEnumerable_CloneAndEqualsEnumeratorTest)
{
	list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	auto enumerable = from(numbers) >> skip(4);
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

TEST(XLinqSkipTest, SkipFromRandomAccessEnumerable)
{
	vector<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	auto enumerator = from(numbers) >> skip(4) >> getEnumerator();
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

	enumerator = from(numbers) >> skip(4) >> getEndEnumerator();

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

	enumerator = from(numbers) >> skip(4) >> getEnumeratorAt(1);
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(1, enumerator->current());
}

TEST(XLinqSkipTest, RandomAccessEnumerable_CloneAndEqualsEnumeratorTest)
{
	vector<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	auto enumerable = from(numbers) >> skip(4);
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

TEST(XLinqSkipTest, RandomAccessEnumerable_DistanceLtGtTest)
{
	vector<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	auto enumerable = from(numbers) >> skip(4);
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

TEST(XLinqSkipTest, SkipMoreElementsThanCollectionContainsFromEnumerable)
{
	forward_list<int> numbers = { 1, 2, 3 };
	auto enumerator = from(numbers) >> skip(10) >> getEnumerator();
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSkipTest, SkipMoreElementsThanCollectionContainsFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 2, 3 };
	auto enumerator = from(numbers) >> skip(3) >> getEnumerator();
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSkipTest, SkipMoreElementsThanCollectionContainsFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 2, 3 };
	auto enumerator = from(numbers) >> skip(3) >> getEnumerator();
	ASSERT_FALSE(enumerator->next());
	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(4));
	ASSERT_FALSE(enumerator->advance(-4));

	enumerator = from(numbers) >> skip(3) >> getEndEnumerator();

	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(4));
	ASSERT_FALSE(enumerator->advance(-4));

	enumerator = from(numbers) >> skip(3) >> getEnumeratorAt(1);
	ASSERT_FALSE(enumerator->back());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSkipWhileTest, SkipFromEnumerable)
{
	forward_list<int> numbers = { 4, 5, 6, 7, 1, 2, 3 };
	auto enumerator = from(numbers) >> skip_while([](int a) { return a > 2; }) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}