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