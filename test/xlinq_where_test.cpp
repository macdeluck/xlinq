#include <gtest/gtest.h>
#include <xlinq/xlinq_where.h>
#include <xlinq/xlinq_from.h>
#include "model/xlinq_test_model.h"
#include <memory>

using namespace std;
using namespace xlinq;

TEST(XLinqWhereTest, XlinqFiltersEvenNumbersTest)
{
	auto numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	auto enumerator = from(numbers)
		>> where([](int number) { return number % 2; })
		>> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_FALSE(enumerator->next());

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqWhereTest, CloneAndEqualsEnumeratorTest)
{
	auto numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	auto enumerator = from(numbers)
		>> where([](int number) { return number % 2; })
		>> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_EQ(1, second->current());

	while (enumerator->next());

	ASSERT_EQ(1, second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ(3, second->current());
}