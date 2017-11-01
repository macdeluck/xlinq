#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_stl.h>
#include <memory>
#include <vector>
#include <list>
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

TEST(XlinqStlIteratorTest, EqualsTest)
{
	forward_list<int> numbers = { 1, 2, 3, 4, 5 };

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

TEST(XlinqStlIteratorTest, StlMinElement)
{
	forward_list<int> numbers = { 1, 2, 3, -1, 4, 5 };
	auto container = from(numbers) >> stl();
	auto minElement = std::min_element(container.begin(), container.end());
	ASSERT_EQ(-1, *minElement);
}

TEST(XlinqStlBidirectionalIteratorTest, EqualsTest)
{
	list<int> numbers = { 1, 2, 3, 4, 5 };

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
	--it;
	ASSERT_EQ(it, sec);
	++it;
	++it;
	++it;
	ASSERT_EQ(it, container.end());

	it = container.rbegin();
	sec = container.rbegin();
	ASSERT_EQ(it, container.rbegin());
	ASSERT_EQ(it, sec);
	ASSERT_NE(it, container.rend());
	++it;
	ASSERT_EQ(4, *it);
	ASSERT_NE(it, container.rbegin());
	ASSERT_NE(it, sec);
	++sec;
	ASSERT_EQ(it, sec);
	sec = it++;
	ASSERT_NE(it, sec);
	sec = it;
	++it;
	ASSERT_NE(it, sec);
	--it;
	ASSERT_EQ(it, sec);
	++it;
	++it;
	++it;
	ASSERT_EQ(it, container.rend());
}

TEST(XlinqStlBidirectionalIteratorTest, StlMinElement)
{
	list<int> numbers = { 1, 2, 3, -1, 4, 5 };
	auto container = from(numbers) >> stl();
	auto minElement = std::min_element(container.begin(), container.end());
	ASSERT_EQ(-1, *minElement);

	minElement = std::min_element(container.rbegin(), container.rend());
	ASSERT_EQ(-1, *minElement);
}

TEST(XlinqStlRandomAccessIteratorTest, EqualsTest)
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
	it += 1;
	ASSERT_NE(it, sec);
	--it;
	ASSERT_EQ(it, sec);
	ASSERT_FALSE(it < sec);
	ASSERT_TRUE(it <= sec);
	ASSERT_FALSE(it > sec);
	ASSERT_TRUE(it >= sec);
	ASSERT_FALSE(sec < it);
	ASSERT_TRUE(sec <= it);
	ASSERT_FALSE(sec > it);
	ASSERT_TRUE(sec >= it);
	it += 2;
	ASSERT_EQ(2, it - sec);
	ASSERT_EQ(-2, sec - it);
	ASSERT_FALSE(it < sec);
	ASSERT_FALSE(it <= sec);
	ASSERT_TRUE(it > sec);
	ASSERT_TRUE(it >= sec);
	ASSERT_TRUE(sec < it);
	ASSERT_TRUE(sec <= it);
	ASSERT_FALSE(sec > it);
	ASSERT_FALSE(sec >= it);
	ASSERT_EQ(sec + 2, it);
	ASSERT_EQ(sec, it - 2);
	ASSERT_EQ(sec[2], *it);
	++it;
	ASSERT_EQ(it, container.end());

	it = container.rbegin();
	sec = container.rbegin();
	ASSERT_EQ(it, container.rbegin());
	ASSERT_EQ(it, sec);
	ASSERT_NE(it, container.rend());
	++it;
	ASSERT_EQ(4, *it);
	ASSERT_NE(it, container.rbegin());
	ASSERT_NE(it, sec);
	++sec;
	ASSERT_EQ(it, sec);
	sec = it++;
	ASSERT_NE(it, sec);
	sec = it;
	it += 1;
	ASSERT_NE(it, sec);
	--it;
	ASSERT_EQ(it, sec);
	ASSERT_FALSE(it < sec);
	ASSERT_TRUE(it <= sec);
	ASSERT_FALSE(it > sec);
	ASSERT_TRUE(it >= sec);
	ASSERT_FALSE(sec < it);
	ASSERT_TRUE(sec <= it);
	ASSERT_FALSE(sec > it);
	ASSERT_TRUE(sec >= it);
	it += 2;
	ASSERT_EQ(2, it - sec);
	ASSERT_EQ(-2, sec - it);
	ASSERT_FALSE(it < sec);
	ASSERT_FALSE(it <= sec);
	ASSERT_TRUE(it > sec);
	ASSERT_TRUE(it >= sec);
	ASSERT_TRUE(sec < it);
	ASSERT_TRUE(sec <= it);
	ASSERT_FALSE(sec > it);
	ASSERT_FALSE(sec >= it);
	ASSERT_EQ(sec + 2, it);
	ASSERT_EQ(sec, it - 2);
	ASSERT_EQ(sec[2], *it);
	++it;
	ASSERT_EQ(it, container.rend());
}

TEST(XlinqStlRandomAccessIteratorTest, StlMinElement)
{
	vector<int> numbers = { 1, 2, 3, -1, 4, 5 };
	auto container = from(numbers) >> stl();
	auto minElement = std::min_element(container.begin(), container.end());
	ASSERT_EQ(-1, *minElement);

	minElement = std::min_element(container.rbegin(), container.rend());
	ASSERT_EQ(-1, *minElement);
}