#include <gtest/gtest.h>
#include <memory>
#include <list>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_aggregate.h>
#include <functional>

using namespace std;
using namespace xlinq;

template<typename T>
class ModuloSummator
{
private:
	T _moduloRoot;
public:
	ModuloSummator(T moduloRoot) : _moduloRoot(moduloRoot) {}

	T operator()(T first, T second)
	{
		return (first % _moduloRoot + second % _moduloRoot) % _moduloRoot;
	}
};

TEST(XlinqAggregateTest, EmptyCollectionThrowsException)
{
	auto numbers = list<int>();
	try
	{
		from(numbers) >> aggregate(std::plus<int>());
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqAggregateTest, AggregateAsSum)
{
	int numbers[] = { 3, 7, -32, 0, -1, 2 };
	ASSERT_EQ(-21, from(numbers) >> aggregate(std::plus<int>()));
}

TEST(XlinqAggregateTest, AggregateAsModuloSum)
{
	int numbers[] = { 3, 7, 32, 0, 1, 2 };
	ASSERT_EQ(3, from(numbers) >> aggregate(ModuloSummator<int>(7)));
}

TEST(XlinqAggregateResultTest, EmptyCollectionThrowsException)
{
	auto numbers = list<int>();
	try
	{
		from(numbers) >> aggregate(std::plus<int>());
		FAIL();
	}
	catch (IterationFinishedException exc)
	{
	}
}

TEST(XlinqAggregateResultTest, AggregateUsingDelegate)
{
	int numbers[] = { 3, 7, -32, 0, -1, 3 };
	auto result = (from(numbers) >> aggregate(0.1f, [](float first, int second) { return first + second * 0.1f; }));
	ASSERT_TRUE(0.001f > (-1.9f - result));
}