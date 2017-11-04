#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_union.h>
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

TEST(XLinqUnionTest, EnumerableTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> tounion = { 1, 2, 6, 6, 7, };
	auto enumerator = from(numbers) >> union_with(tounion) >> getEnumerator();
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
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqUnionTest, Enumerable_CloneAndEqualsEnumeratorTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> tounion = { 1, 2, 6, 6, 7, };
	auto enumerable = from(numbers) >> union_with(tounion);
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


TEST(XLinqUnionTest, EnumerableWithEqCompTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> tounion = { 1, 2, 6, 6, 7, };
	auto enumerator = from(numbers) >> union_with(tounion) >> getEnumerator();
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
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqUnionTest, EnumerableWithEqComp_CloneAndEqualsEnumeratorTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> tounion = { 1, 2, 6, 6, 7, };
	auto enumerable = from(numbers) >> union_with(tounion, Eq<int, int>());
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


TEST(XLinqUnionTest, EnumerableFullTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> tounion = { 1, 2, 6, 6, 7, };
	auto enumerator = from(numbers) >> union_with(tounion) >> getEnumerator();
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
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqUnionTest, EnumerableFull_CloneAndEqualsEnumeratorTest)
{
	forward_list<int> numbers = { 1, 1, 2, 2, 2, 3, 4, 4, 5 };
	vector<int> tounion = { 1, 2, 6, 6, 7, };
	auto enumerable = from(numbers) >> union_with(tounion, Hash<int>(), Eq<int, int>());
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