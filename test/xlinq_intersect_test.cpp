#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_intersect.h>
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

TEST(XLinqIntersectTest, EnumerableTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> toleave = { 2, 4, 5 };
	auto enumerator = from(numbers) >> intersect(toleave) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqIntersectTest, Enumerable_CloneAndEqualsEnumeratorTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> toleave = { 2, 4, 5 };
	auto enumerable = from(numbers) >> intersect(toleave);
	auto enumerator = enumerable >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_EQ(2, second->current());

	while (enumerator->next());

	ASSERT_EQ(2, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(4, second->current());
}


TEST(XLinqIntersectTest, EnumerableWithEqCompTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> toleave = { 2, 4, 5 };
	auto enumerator = from(numbers) >> intersect(toleave) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqIntersectTest, EnumerableWithEqComp_CloneAndEqualsEnumeratorTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> toleave = { 2, 4, 5 };
	auto enumerable = from(numbers) >> intersect(toleave, Eq<int, int>());
	auto enumerator = enumerable >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_EQ(2, second->current());

	while (enumerator->next());

	ASSERT_EQ(2, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(4, second->current());
}


TEST(XLinqIntersectTest, EnumerableFullTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> toleave = { 2, 4, 5 };
	auto enumerator = from(numbers) >> intersect(toleave) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqIntersectTest, EnumerableFull_CloneAndEqualsEnumeratorTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> toleave = { 2, 4, 5 };
	auto enumerable = from(numbers) >> intersect(toleave, Hash<int>(), Eq<int, int>());
	auto enumerator = enumerable >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_EQ(2, second->current());

	while (enumerator->next());

	ASSERT_EQ(2, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(4, second->current());
}