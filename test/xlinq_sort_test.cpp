#include <gtest/gtest.h>
#include <xlinq/xlinq_sort.h>
#include <xlinq/xlinq_from.h>
#include <vector>
#include <string>

using namespace std;
using namespace xlinq;

bool word_length_comparer(string a, string b)
{
	return a.size() < b.size();
}

TEST(XLinqSortTest, SortUnordered)
{
	vector<string> words = { "hh", "gggggg", "ff", "d", "eee", "ccc", "bbb", "aa" };
	auto enumerator = from(words) >> sort() >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("aa", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("bbb", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("ccc", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("d", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("eee", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("ff", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("gggggg", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("hh", enumerator->current());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSortTest, SortUnorderedWithComparer)
{
	vector<string> words = { "hh", "gggggg", "ff", "d", "eee", "ccc", "bbb", "aa" };
	auto enumerator = from(words) >> sort(word_length_comparer) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("d", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("hh", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("ff", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("aa", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("eee", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("ccc", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("bbb", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("gggggg", enumerator->current());
	ASSERT_FALSE(enumerator->next());
}