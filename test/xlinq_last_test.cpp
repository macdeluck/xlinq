#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_last.h>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XlinqLastTest, GetLastElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last();
	ASSERT_EQ(5, val);
}

TEST(XlinqLastTest, CollectionContainsNoLements)
{
	std::vector<int> numbers;
	try
	{
		from(numbers) >> last();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}