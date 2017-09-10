#include <gtest/gtest.h>
#include <xlinq/xlinq_enumerable.h>

using namespace std;
using namespace xlinq;

TEST(XlinqEmptyEnumerableTest, Iteration)
{
	auto enumerator = Enumerable::empty<int>()->getEnumerator();
	ASSERT_FALSE(enumerator->next());
	ASSERT_FALSE(enumerator->back());

	enumerator = Enumerable::empty<int>()->getEndEnumerator();
	ASSERT_FALSE(enumerator->back());
	ASSERT_FALSE(enumerator->next());
}

TEST(XlinqEmptyEnumerableTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto enumerator = Enumerable::empty<int>()->getEnumerator();
	try
	{
		enumerator->current();
	}
	catch (IterationNotStartedException)
	{
		return;
	}
	catch (...)
	{
		FAIL();
	}
	FAIL();
}

TEST(XlinqEmptyEnumerableTest, CallBackBeforeEnumerationWasStarted)
{
	auto enumerator = Enumerable::empty<int>()->getEnumerator();
	try
	{
		enumerator->back();
	}
	catch (IterationNotStartedException)
	{
		return;
	}
	catch (...)
	{
		FAIL();
	}
	FAIL();
}

TEST(XlinqEmptyEnumerableTest, CallCurrentWhenEnumerationWasFinished)
{
	auto enumerator = Enumerable::empty<int>()->getEnumerator();
	while (enumerator->next());
	try
	{
		enumerator->current();
	}
	catch (IterationFinishedException)
	{
		return;
	}
	catch (...)
	{
		FAIL();
	}
	FAIL();
}

TEST(XlinqEmptyEnumerableTest, CallNextWhenEnumerationWasFinished)
{
	auto enumerator = Enumerable::empty<int>()->getEnumerator();
	while (enumerator->next());
	try
	{
		enumerator->next();
	}
	catch (IterationFinishedException)
	{
		return;
	}
	catch (...)
	{
		FAIL();
	}
	FAIL();
}

TEST(XlinqRepeatEnumerableTest, IterateInfiniteStream)
{
	auto enumerator = Enumerable::repeat(1)->getEnumerator();
	for (size_t i = 0; i < 100; i++)
	{
		ASSERT_TRUE(enumerator->next());
		ASSERT_EQ(1, enumerator->current());
	}
	ASSERT_TRUE(enumerator->next());
}

TEST(XlinqRepeatEnumerableTest, IterateFiniteStream)
{
	auto enumerator = Enumerable::repeat(1, 100)->getEnumerator();
	for (size_t i = 0; i < 100; i++)
	{
		ASSERT_TRUE(enumerator->next());
		ASSERT_EQ(1, enumerator->current());
	}
	ASSERT_FALSE(enumerator->next());
}

TEST(XlinqRangeEnumerableTest, IterateRange)
{
	auto enumerator = Enumerable::range(1, 101)->getEnumerator();
	for (int i = 1; i < 101; i++)
	{
		ASSERT_TRUE(enumerator->next());
		ASSERT_EQ(i, enumerator->current());
	}
	ASSERT_FALSE(enumerator->next());
}