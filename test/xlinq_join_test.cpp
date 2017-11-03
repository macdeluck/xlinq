#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_join.h>
#include <memory>
#include <vector>
#include <utility>

using namespace std;
using namespace xlinq;

TEST(XlinqJoinTest, TestJoin)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	vector<string> strings = { "a", "bb", "zz", "xx", "eeeee", "fffff" };
	auto enumerator = from(numbers) >> join(strings, 
		[](int i) { return i; }, 
		[](string s) { return s.size(); }, 
		[](int i, string s) { return to_string(i) + s; }) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("1a", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2bb", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2zz", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2xx", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5eeeee", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5fffff", enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XlinqJoinTest, CloneAndEqualsEnumeratorTest)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	vector<string> strings = { "a", "bb", "zz", "xx", "eeeee", "fffff" };
	auto enumerable = from(numbers) >> join(strings,
		[](int i) { return i; },
		[](string s) { return s.size(); },
		[](int i, string s) { return to_string(i) + s; });
	auto enumerator = enumerable >> getEnumerator();
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(second->equals(enumerator));

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("1a", enumerator->current());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_FALSE(second->equals(enumerator));

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2bb", enumerator->current());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_FALSE(second->equals(enumerator));

	ASSERT_TRUE(second->next());
	ASSERT_EQ("1a", second->current());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_FALSE(second->equals(enumerator));

	ASSERT_TRUE(second->next());
	ASSERT_EQ("2bb", second->current());
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(second->equals(enumerator));

	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_FALSE(second->equals(enumerator));
	ASSERT_EQ("2zz", enumerator->current());

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2xx", enumerator->current());

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5eeeee", enumerator->current());

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5fffff", enumerator->current());

	ASSERT_FALSE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_FALSE(second->equals(enumerator));

	ASSERT_TRUE(second->next());
	ASSERT_TRUE(second->next());
	ASSERT_TRUE(second->next());
	ASSERT_TRUE(second->next());
	ASSERT_EQ("5fffff", second->current());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_FALSE(second->equals(enumerator));
	ASSERT_FALSE(second->next());
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(second->equals(enumerator));
}

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
		return first == second;
	}
};

TEST(XlinqJoinFullTest, TestJoin)
{
	auto numbers = { 1, 2, 3, 4, 5, 6 };
	vector<string> strings = { "a", "bb", "zz", "xx", "eeeee", "fffff" };
	auto enumerator = from(numbers) >> join(strings,
		[](int i) { return i; },
		[](string s) { return s.size(); },
		[](int i, string s) { return to_string(i) + s; },
		Eq<int, unsigned>(),
		Hash<int>(),
		Eq<int, int>(),
		Hash<unsigned>(),
		Eq<unsigned, unsigned>()) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("1a", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2bb", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2zz", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("2xx", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5eeeee", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("5fffff", enumerator->current());
	ASSERT_FALSE(enumerator->next());
}