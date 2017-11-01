#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_stl.h>
#include <memory>
#include <vector>
#include <forward_list>
#include <algorithm>

using namespace std;
using namespace xlinq;

TEST(XlinqStlTest, ModernForEach)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };

	int i = 0;
	for (auto val : from(numbers) >> stl())
		ASSERT_EQ(numbers[i++], val);
}

TEST(XlinqStlTest, EqualsTest)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };

	auto container = from(numbers) >> stl();
	auto it = container.begin();
	auto sec = container.begin();
	ASSERT_EQ(it, container.begin());
	ASSERT_EQ(it, sec);
	ASSERT_NE(it, container.end());
	++it;
	ASSERT_EQ(2, *it);
	ASSERT_NE(it, container.begin());
	ASSERT_NE(it, sec);
	++sec;
	ASSERT_EQ(it, sec);
	sec = it++;
	ASSERT_NE(it, sec);
	sec = it;
	++it;
	ASSERT_NE(it, sec);
	++it;
	++it;
	ASSERT_EQ(it, container.end());
}

TEST(XlinqStlTest, StlMinElement)
{
	vector<int> numbers = { 1, 2, 3, -1, 4, 5 };
	XlinqContainer<int> container = from(numbers) >> stl();
	XlinqIterator<int> minElement = std::min_element(container.begin(), container.end());
	ASSERT_EQ(-1, *minElement);
}