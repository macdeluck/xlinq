#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_last.h>
#include <vector>
#include <forward_list>

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

TEST(XlinqLastTest, GetLastElementOfCollectionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last();
	ASSERT_EQ(5, val);
}

TEST(XlinqLastTest, CollectionContainsNoLementsForwardList)
{
	std::forward_list<int> numbers;
	try
	{
		from(numbers) >> last();
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqLastOrDefaultTest, GetLastElementOfCollection)
{
	auto numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(5, val);
}

TEST(XlinqLastOrDefaultTest, CollectionContainsNoLements)
{
	std::vector<int> numbers;
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(0, val);
}

TEST(XlinqLastOrDefaultTest, GetLastElementOfCollectionForwardList)
{
	std::forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(5, val);
}

TEST(XlinqLastOrDefaultTest, CollectionContainsNoLementsForwardList)
{
	std::forward_list<int> numbers;
	auto val = from(numbers) >> last_or_default(0);
	ASSERT_EQ(0, val);
}