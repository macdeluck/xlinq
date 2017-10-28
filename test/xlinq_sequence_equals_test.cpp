#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_sequence_equals.h>
#include <list>
#include <forward_list>
#include <vector>

using namespace std;
using namespace xlinq;

template<typename TFirst, typename TSecond>
struct Eq
{
	bool operator()(const TFirst& first, const TSecond& second) const
	{
		return first == second;
	}
};

TEST(XLinqSequenceEqualsTest, SequenceEquals)
{
	forward_list<int> emptynumbers = {};
	forward_list<int> numbers = { 1, 2, 3, 4, 5 };

	forward_list<int> test1 = {};
	ASSERT_FALSE(from(numbers) >> sequence_equals(test1));
	ASSERT_TRUE(from(emptynumbers) >> sequence_equals(test1));

	test1 = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> sequence_equals(test1));
	ASSERT_FALSE(from(emptynumbers) >> sequence_equals(test1));

	list<int> test2 = {};
	ASSERT_FALSE(from(numbers) >> sequence_equals(test2));
	ASSERT_TRUE(from(emptynumbers) >> sequence_equals(test2));

	test2 = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> sequence_equals(test2));
	ASSERT_FALSE(from(emptynumbers) >> sequence_equals(test2));

	vector<int> test3 = {};
	ASSERT_FALSE(from(numbers) >> sequence_equals(test3));
	ASSERT_TRUE(from(emptynumbers) >> sequence_equals(test3));
	
	test3 = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> sequence_equals(test3));
	ASSERT_FALSE(from(emptynumbers) >> sequence_equals(test3));
}

TEST(XLinqSequenceEqualsTest, SequenceWithEqCompEquals)
{
	forward_list<int> emptynumbers = {};
	forward_list<int> numbers = { 1, 2, 3, 4, 5 };

	forward_list<int> test1 = {};
	ASSERT_FALSE(from(numbers) >> sequence_equals(test1, Eq<int, int>()));
	ASSERT_TRUE(from(emptynumbers) >> sequence_equals(test1, Eq<int, int>()));

	test1 = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> sequence_equals(test1, Eq<int, int>()));
	ASSERT_FALSE(from(emptynumbers) >> sequence_equals(test1, Eq<int, int>()));

	list<int> test2 = {};
	ASSERT_FALSE(from(numbers) >> sequence_equals(test2, Eq<int, int>()));
	ASSERT_TRUE(from(emptynumbers) >> sequence_equals(test2, Eq<int, int>()));

	test2 = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> sequence_equals(test2, Eq<int, int>()));
	ASSERT_FALSE(from(emptynumbers) >> sequence_equals(test2, Eq<int, int>()));

	vector<int> test3 = {};
	ASSERT_FALSE(from(numbers) >> sequence_equals(test3, Eq<int, int>()));
	ASSERT_TRUE(from(emptynumbers) >> sequence_equals(test3, Eq<int, int>()));
	
	test3 = { 1, 2, 3, 4, 5 };
	ASSERT_TRUE(from(numbers) >> sequence_equals(test3, Eq<int, int>()));
	ASSERT_FALSE(from(emptynumbers) >> sequence_equals(test3, Eq<int, int>()));
}