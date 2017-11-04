#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_to_container.h>
#include <xlinq/xlinq_select_many.h>
#include <xlinq/xlinq_select.h>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>

using namespace std;
using namespace xlinq;

template<typename TElem>
struct MyLess
{
	bool operator()(const TElem& lhs, const TElem& rhs) const
	{
		return lhs > rhs;
	}
};

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

TEST(XlinqToSetTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	set<int> single = from(numbers) >> to_set();
	ASSERT_EQ(5, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
}

TEST(XlinqToSetWithLessTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	set<int, MyLess<int>> single = from(numbers) >> to_set(MyLess<int>());
	ASSERT_EQ(5, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
}

TEST(XlinqToMultiSetTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	multiset<int> single = from(numbers) >> to_multiset();
	ASSERT_EQ(10, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
	ASSERT_EQ(2, single.count(1));
	ASSERT_EQ(2, single.count(2));
	ASSERT_EQ(2, single.count(3));
	ASSERT_EQ(2, single.count(4));
	ASSERT_EQ(2, single.count(5));
}

TEST(XlinqToMultiSetWithLessTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	multiset<int, MyLess<int>> single = from(numbers) >> to_multiset(MyLess<int>());
	ASSERT_EQ(10, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
	ASSERT_EQ(2, single.count(1));
	ASSERT_EQ(2, single.count(2));
	ASSERT_EQ(2, single.count(3));
	ASSERT_EQ(2, single.count(4));
	ASSERT_EQ(2, single.count(5));
}

TEST(XlinqToMapTest, Test)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };
	map<int, int> single = from(numbers)
		>> to_map([](int n) 
		{ 
			return n; 
		}, [](int n) 
		{ 
			return n * 10;
		});
	ASSERT_EQ(5, single.size());
	ASSERT_EQ(10, single.find(1)->second);
	ASSERT_EQ(20, single.find(2)->second);
	ASSERT_EQ(30, single.find(3)->second);
	ASSERT_EQ(40, single.find(4)->second);
	ASSERT_EQ(50, single.find(5)->second);
}

TEST(XlinqToMapWithLessTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	map<int, int, MyLess<int>> single = from(numbers) 
		>> to_map([](int n)
		{
			return n;
		}, [](int n)
		{
			return n * 10;
		}, MyLess<int>());
	ASSERT_EQ(5, single.size());
	ASSERT_EQ(10, single.find(1)->second);
	ASSERT_EQ(20, single.find(2)->second);
	ASSERT_EQ(30, single.find(3)->second);
	ASSERT_EQ(40, single.find(4)->second);
	ASSERT_EQ(50, single.find(5)->second);
}

template<typename TElem>
struct MyHasher
{
	int operator()(const TElem& val) const { return val * 13 + 5; }
};

template<typename TElem>
struct MyComparer
{
	bool operator()(const TElem& lhs, const TElem& rhs) const { return lhs == rhs; }
};

TEST(XlinqToUnorderedSetTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	unordered_set<int> single = from(numbers) >> to_unordered_set();
	ASSERT_EQ(5, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
}

TEST(XlinqToUnorderedSetFullTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	unordered_set<int, MyHasher<int>, MyComparer<int>> single = from(numbers) >> to_unordered_set(MyHasher<int>(), MyComparer<int>());
	ASSERT_EQ(5, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
}

TEST(XlinqToUnorderedMultiSetTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	unordered_multiset<int> single = from(numbers) >> to_unordered_multiset();
	ASSERT_EQ(10, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
	ASSERT_EQ(2, single.count(1));
	ASSERT_EQ(2, single.count(2));
	ASSERT_EQ(2, single.count(3));
	ASSERT_EQ(2, single.count(4));
	ASSERT_EQ(2, single.count(5));
}

TEST(XlinqToUnorderedMultiSetFullTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	unordered_multiset<int, MyHasher<int>, MyComparer<int>> single = from(numbers) >> to_unordered_multiset(MyHasher<int>(), MyComparer<int>());
	ASSERT_EQ(10, single.size());
	ASSERT_NE(single.end(), single.find(1));
	ASSERT_NE(single.end(), single.find(2));
	ASSERT_NE(single.end(), single.find(3));
	ASSERT_NE(single.end(), single.find(4));
	ASSERT_NE(single.end(), single.find(5));
	ASSERT_EQ(2, single.count(1));
	ASSERT_EQ(2, single.count(2));
	ASSERT_EQ(2, single.count(3));
	ASSERT_EQ(2, single.count(4));
	ASSERT_EQ(2, single.count(5));
}

TEST(XlinqToUnorderedMapTest, Test)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };
	unordered_map<int, int> single = from(numbers)
		>> to_unordered_map([](int n)
		{
			return n;
		}, [](int n)
		{
			return n * 10;
		});
	ASSERT_EQ(5, single.size());
	ASSERT_EQ(10, single.find(1)->second);
	ASSERT_EQ(20, single.find(2)->second);
	ASSERT_EQ(30, single.find(3)->second);
	ASSERT_EQ(40, single.find(4)->second);
	ASSERT_EQ(50, single.find(5)->second);
}

TEST(XlinqToUnorderedMapFullTest, Test)
{
	vector<int> numbers = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	unordered_map<int, int, MyHasher<int>, MyComparer<int>> single = from(numbers)
		>> to_unordered_map([](int n)
		{
			return n;
		}, [](int n)
		{
			return n * 10;
		}, MyHasher<int>(), MyComparer<int>());
	ASSERT_EQ(5, single.size());
	ASSERT_EQ(10, single.find(1)->second);
	ASSERT_EQ(20, single.find(2)->second);
	ASSERT_EQ(30, single.find(3)->second);
	ASSERT_EQ(40, single.find(4)->second);
	ASSERT_EQ(50, single.find(5)->second);
}