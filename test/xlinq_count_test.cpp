#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_count.h>
#include <list>
#include <forward_list>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XLinqCountTest, GetCountFromForwardList)
{
	forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	ASSERT_EQ(5, from(numbers) >> count());
}

TEST(XLinqCountTest, GetCountFromList)
{
	list<int> numbers = { 1, 2, 3, 4, 5 };
	ASSERT_EQ(5, from(numbers) >> count());
}

TEST(XLinqCountTest, GetCountFromVector)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };
	ASSERT_EQ(5, from(numbers) >> count());
}