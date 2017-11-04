#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_to_container.h>
#include <xlinq/xlinq_select_many.h>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>

using namespace std;
using namespace xlinq;

TEST(XlinqToVectorTest, Test)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };
	vector<int> doubled = from(numbers)
		>> select_many([](int n)
			{
				std::array<int, 2> arr = { n, n };
				return arr;
			})
		>> to_vector();
	ASSERT_EQ(10, doubled.size());
	auto it = doubled.begin();
	ASSERT_EQ(1, *it++);
	ASSERT_EQ(1, *it++);
	ASSERT_EQ(2, *it++);
	ASSERT_EQ(2, *it++);
	ASSERT_EQ(3, *it++);
	ASSERT_EQ(3, *it++);
	ASSERT_EQ(4, *it++);
	ASSERT_EQ(4, *it++);
	ASSERT_EQ(5, *it++);
	ASSERT_EQ(5, *it++);
	ASSERT_EQ(it, doubled.end());
}

TEST(XlinqToListTest, Test)
{
	list<int> numbers = { 1, 2, 3, 4, 5 };
	list<int> doubled = from(numbers)
		>> select_many([](int n)
		{
			std::array<int, 2> arr = { n, n };
			return arr;
		})
		>> to_list();
	ASSERT_EQ(10, doubled.size());
	auto it = doubled.begin();
	ASSERT_EQ(1, *it++);
	ASSERT_EQ(1, *it++);
	ASSERT_EQ(2, *it++);
	ASSERT_EQ(2, *it++);
	ASSERT_EQ(3, *it++);
	ASSERT_EQ(3, *it++);
	ASSERT_EQ(4, *it++);
	ASSERT_EQ(4, *it++);
	ASSERT_EQ(5, *it++);
	ASSERT_EQ(5, *it++);
	ASSERT_EQ(it, doubled.end());
}

TEST(XlinqToForwardListTest, Test)
{
	forward_list<int> numbers = { 1, 2, 3, 4, 5 };
	forward_list<int> doubled = from(numbers)
		>> select_many([](int n)
		{
			std::array<int, 2> arr = { n, n };
			return arr;
		})
		>> to_forward_list();
	auto it = doubled.begin();
	ASSERT_EQ(1, *it++);
	ASSERT_EQ(1, *it++);
	ASSERT_EQ(2, *it++);
	ASSERT_EQ(2, *it++);
	ASSERT_EQ(3, *it++);
	ASSERT_EQ(3, *it++);
	ASSERT_EQ(4, *it++);
	ASSERT_EQ(4, *it++);
	ASSERT_EQ(5, *it++);
	ASSERT_EQ(5, *it++);
	ASSERT_EQ(it, doubled.end());
}