#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_base.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_exception.h>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;
using namespace xlinq;

typedef typename vector<Person>::iterator PersonIterator;
typedef _StlEnumerator<PersonIterator, Person> PersonEnumerator;

TEST(XLinqFromTest, XLinqStlIteratorFromVector)
{
	auto persons = getPersons();
	shared_ptr<IEnumerator<Person>> enumerator(new PersonEnumerator(persons.begin(), persons.end()));

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Kamil", enumerator->current().firstName);
	ASSERT_EQ("Goryszewicz", enumerator->current().secondName);
	ASSERT_EQ(18, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqFromTest, XLinqStlIteratorFromVectorCallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersons();
	shared_ptr<IEnumerator<Person>> enumerator(new PersonEnumerator(persons.begin(), persons.end()));
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

TEST(XLinqFromTest, XLinqStlIteratorFromVectorCallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersons();
	shared_ptr<IEnumerator<Person>> enumerator(new PersonEnumerator(persons.begin(), persons.end()));
	auto exception = new IterationFinishedException();
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

TEST(XLinqFromTest, XLinqStlIteratorFromVectorCallNextWhenEnumerationWasFinished)
{
	auto persons = getPersons();
	shared_ptr<IEnumerator<Person>> enumerator(new PersonEnumerator(persons.begin(), persons.end()));
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

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}