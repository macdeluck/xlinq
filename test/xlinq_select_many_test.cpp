#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_select_many.h>
#include <array>

using namespace std;
using namespace xlinq;

TEST(XLinqSelectManyTest, FlatteningCollectionContainingVectors)
{
	auto companies = getCompanies();
	auto allEmployees = from(companies) >> select_many([](Company c) { return c.employees; });
	auto enumerator = allEmployees >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Walczak", enumerator->current().secondName);
	ASSERT_EQ(19, enumerator->current().age);
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Lend", enumerator->current().secondName);
	ASSERT_EQ(33, enumerator->current().age);
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Janusz", enumerator->current().firstName);
	ASSERT_EQ("Grzyb", enumerator->current().secondName);
	ASSERT_EQ(27, enumerator->current().age);
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Marian", enumerator->current().firstName);
	ASSERT_EQ("Halbut", enumerator->current().secondName);
	ASSERT_EQ(31, enumerator->current().age);
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSelectManyTest, CloneAndEqualsEnumeratorTest)
{
	auto companies = getCompanies();
	auto enumerable = from(companies) >> select_many([](Company c) { return c.employees; });
	auto enumerator = enumerable >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Anna", second->current().firstName);

	while (enumerator->next());

	ASSERT_EQ("Anna", second->current().firstName);
	ASSERT_TRUE(second->next());
	ASSERT_EQ("Micha³", second->current().firstName);
}

TEST(XLinqSelectManyTest, SelectingFromDictionary)
{
	auto companies = getCompaniesUnorderedMap();
	auto allEmployees = from(companies) >> select_many([](std::pair<std::string, Company> c) { return c.second.employees; });
	auto enumerator = allEmployees >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_TRUE(enumerator->next());
	ASSERT_TRUE(enumerator->next());
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqSelectManyTest, SelectingFrom2DArray)
{
	std::array<std::array<int, 5>, 2> array = {
		std::array<int, 5>({ 1, 2, 3, 4, 5}),
		std::array<int, 5>({ 6, 7, 8, 9, 10})
	};
	auto enumerator = from(array) >> select_many([](std::array<int, 5> a) { return a; }) >> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(10, enumerator->current());
	ASSERT_FALSE(enumerator->next());
}