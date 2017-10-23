#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_distinct.h>
#include <list>
#include <forward_list>
#include <vector>

using namespace std;
using namespace xlinq;

template<typename TVal>
struct Hash
{
	int operator()(const TVal& val) const
	{
		return (int)val;
	}
};

template<typename TFirst, typename TSecond>
struct Eq
{
	bool operator()(const TFirst& first, const TSecond& second) const
	{
		return (first + first) == (second << 1);
	}
};

TEST(XLinqDistinctTest, DistinctFromEnumerable)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct() >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct() >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct() >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctWithEqCompFromEnumerable)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct(Eq<int, int>()) >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctWithEqCompFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct(Eq<int, int>()) >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctWithEqCompFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct(Eq<int, int>()) >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctFullFromEnumerable)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct(Hash<int>(), Eq<int, int>()) >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctFullFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct(Hash<int>(), Eq<int, int>()) >> getEnumerator();
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

TEST(XLinqDistinctTest, DistinctFullFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct(Hash<int>(), Eq<int, int>()) >> getEnumerator();
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

TEST(XLinqDistinctByTest, DistinctByFromEnumerable)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; } ) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; } ) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; } ) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByWithEqCompFromEnumerable)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; }, Eq<int, int>()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByWithEqCompFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; }, Eq<int, int>()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByWithEqCompFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; }, Eq<int, int>()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByFullFromEnumerable)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; }, Hash<int>(), Eq<int, int>()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByFullFromBidirectionalEnumerable)
{
	list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; }, Hash<int>(), Eq<int, int>()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqDistinctByTest, DistinctByFullFromRandomAccessEnumerable)
{
	vector<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	auto enumerator = from(numbers) >> distinct_by([](int a) { return a % 2; }, Hash<int>(), Eq<int, int>()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}