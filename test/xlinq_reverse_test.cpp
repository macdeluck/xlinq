#include <gtest/gtest.h>
#include <xlinq/xlinq_reverse.h>
#include <xlinq/xlinq_from.h>
#include <vector>
#include <list>
#include <forward_list>

using namespace std;
using namespace xlinq;

TEST(XLinqReverseTest, ReverseFromEnumerable)
{
	forward_list<int> numbers = { 7, 6, 5, 4, 3, 2, 1 };
	auto enumerator = from(numbers) >> reverse() >> getEnumerator();
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

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(numbers) >> reverse() >> getEndEnumerator();

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqReverseTest, ReverseFromBidirectionalEnumerable)
{
	list<int> numbers = { 7, 6, 5, 4, 3, 2, 1 };
	auto enumerator = from(numbers) >> reverse() >> getEnumerator();
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

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(numbers) >> reverse() >> getEndEnumerator();

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqReverseTest, ReverseFromRandomAccessEnumerable)
{
	vector<int> numbers = { 7, 6, 5, 4, 3, 2, 1 };
	auto enumerator = from(numbers) >> reverse() >> getEnumerator();
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

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(numbers) >> reverse() >> getEndEnumerator();

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	enumerator = from(numbers) >> reverse() >> getEnumeratorAt(1);
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(3));
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-4));
	ASSERT_FALSE(enumerator->advance(8));
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(6, enumerator->current());
}