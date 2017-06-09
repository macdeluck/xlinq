#include <gtest/gtest.h>
#include <xlinq/xlinq_select.h>
#include <xlinq/xlinq_from.h>
#include "model/xlinq_test_model.h"
#include <memory>

using namespace std;
using namespace xlinq;

TEST(XLinqSelectRandomAccessTest, XlinqSelectFieldFromObjectTest)
{
	auto persons = getPersons();
	auto enumerator = from(persons)
		>> select([](Person person) { return person.firstName; })
		>> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Piotr", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Anna", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Kamil", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Jan", enumerator->current());
	ASSERT_FALSE(enumerator->next());

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Jan", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Kamil", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Joanna", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Anna", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Piotr", enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(7));

	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ("Anna", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_TRUE(enumerator->advance(4));
	ASSERT_EQ("Jan", enumerator->current());
	ASSERT_FALSE(enumerator->advance(-7));

	ASSERT_TRUE(enumerator->advance(6));
	ASSERT_EQ("Jan", enumerator->current());
}

TEST(XLinqSelectBidirectionalTest, XlinqSelectFieldFromObjectTest)
{
	auto persons = getPersonsList();
	auto enumerator = from(persons)
		>> select([](Person person) { return person.firstName; })
		>> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Piotr", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Anna", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Kamil", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Jan", enumerator->current());
	ASSERT_FALSE(enumerator->next());

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Jan", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Kamil", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Joanna", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Anna", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Piotr", enumerator->current());
	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqSelectTest, XlinqSelectFieldFromObjectTest)
{
	auto persons = getPersonsForwardList();
	auto enumerator = from(persons)
		>> select([](Person person) { return person.firstName; })
		>> getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Piotr", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Anna", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Kamil", enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Jan", enumerator->current());
	ASSERT_FALSE(enumerator->next());
}