#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_first.h>
#include <memory>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XlinqFirstTest, GetFirstElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> first();
	ASSERT_EQ(1, val);
}

TEST(XlinqFirstTest, CollectionContainsNoLements)
{
	std::vector<int> numbers;
	try
	{
		from(numbers) >> first();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{

	}
}