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

TEST(XLinqSelectRandomAccessTest, CloneAndEqualsEnumeratorTest)
{
	auto persons = getPersons();
	auto enumerator = from(persons)
		>> select([](Person person) { return person.firstName; })
		>> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_EQ("Piotr", second->current());

	while (enumerator->next());

	ASSERT_EQ("Piotr", second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ("Micha³", second->current());
}

TEST(XLinqSelectRandomAccessTest, DistanceLtGtTest)
{
	auto persons = getPersons();
	auto enumerable = from(persons)
		>> select([](Person person) { return person.firstName; });
	auto it = enumerable >> getEnumerator();
	auto end = enumerable >> getEndEnumerator();

	ASSERT_FALSE(it->equals(end));
	ASSERT_TRUE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_TRUE(end->greater_than(it));

	ASSERT_EQ(enumerable->size() + 1, it->distance_to(end));
	ASSERT_EQ(-(enumerable->size() + 1), end->distance_to(it));

	ASSERT_TRUE(it->next());

	ASSERT_FALSE(it->equals(end));
	ASSERT_TRUE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_TRUE(end->greater_than(it));

	ASSERT_EQ(enumerable->size(), it->distance_to(end));
	ASSERT_EQ(-enumerable->size(), end->distance_to(it));

	auto itc = std::dynamic_pointer_cast<IRandomAccessEnumerator<string>>(it->clone());

	ASSERT_TRUE(it->equals(itc));
	ASSERT_FALSE(it->less_than(itc));
	ASSERT_FALSE(itc->less_than(it));
	ASSERT_FALSE(it->greater_than(itc));
	ASSERT_FALSE(itc->greater_than(it));

	ASSERT_EQ(0, it->distance_to(itc));
	ASSERT_EQ(0, itc->distance_to(it));

	ASSERT_TRUE(end->back());

	ASSERT_FALSE(it->equals(end));
	ASSERT_TRUE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_TRUE(end->greater_than(it));

	ASSERT_EQ(enumerable->size() - 1, it->distance_to(end));
	ASSERT_EQ(-(enumerable->size() - 1), end->distance_to(it));

	ASSERT_TRUE(it->advance(it->distance_to(end)));

	ASSERT_TRUE(it->equals(end));
	ASSERT_FALSE(it->less_than(end));
	ASSERT_FALSE(end->less_than(it));
	ASSERT_FALSE(it->greater_than(end));
	ASSERT_FALSE(end->greater_than(it));

	ASSERT_EQ(0, it->distance_to(end));
	ASSERT_EQ(0, end->distance_to(it));
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

TEST(XLinqSelectBidirectionalTest, CloneAndEqualsEnumeratorTest)
{
	auto persons = getPersonsList();
	auto enumerator = from(persons)
		>> select([](Person person) { return person.firstName; })
		>> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_EQ("Piotr", second->current());

	while (enumerator->next());

	ASSERT_EQ("Piotr", second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ("Micha³", second->current());
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

TEST(XLinqSelectTest, CloneAndEqualsEnumeratorTest)
{
	auto persons = getPersonsForwardList();
	auto enumerator = from(persons)
		>> select([](Person person) { return person.firstName; })
		>> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ("Micha³", enumerator->current());
	ASSERT_EQ("Piotr", second->current());

	while (enumerator->next());

	ASSERT_EQ("Piotr", second->current());
	ASSERT_TRUE(second->next());
	ASSERT_EQ("Micha³", second->current());
}

TEST(XLinqSelectTest, EqualsDifferentSelectorsFromTheSameSource)
{
	auto persons = getPersonsForwardList();
	auto enumerator = from(persons)
		>> select([](Person person) { return person.firstName; })
		>> getEnumerator();
	auto second = from(persons)
		>> select([](Person person) { return person.secondName; })
		>> getEnumerator();
	ASSERT_FALSE(enumerator->equals(second));
}