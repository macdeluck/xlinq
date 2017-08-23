#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_base.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_exception.h>
#include <memory>
#include <vector>
#include <array>
#include <iterator>

using namespace std;
using namespace xlinq;

typedef typename vector<Person>::iterator PersonIterator;
typedef internal::_StlEnumerator<PersonIterator, Person> PersonEnumerator;

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

TEST(XLinqFromTest, XLinqStlIterableFromVector)
{
	auto persons = getPersons();
	auto enumerator = from(persons) >> getEnumerator();

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

TEST(XLinqFromTest, XLinqStlIterableFromPointerToVector)
{
	auto persons = make_shared<vector<Person>>(getPersons());
	auto enumerator = from(persons) >> getEnumerator();

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

TEST(XLinqFromTest, XLinqArrayEnumerator)
{
	int values[] = { 1, 2, 3, 4, 5 };
	shared_ptr<internal::_ArrayEnumerator<int>> enumerator(new internal::_ArrayEnumerator<int>(values, 5));
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
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqFromTest, XLinqArrayEnumeratorCallCurrentBeforeEnumerationWasStarted)
{
	int values[] = { 1, 2, 3, 4, 5 };
	shared_ptr<internal::_ArrayEnumerator<int>> enumerator(new internal::_ArrayEnumerator<int>(values, 5));
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

TEST(XLinqFromTest, XLinqArrayEnumeratorCallCurrentWhenEnumerationWasFinished)
{
	int values[] = { 1, 2, 3, 4, 5 };
	shared_ptr<internal::_ArrayEnumerator<int>> enumerator(new internal::_ArrayEnumerator<int>(values, 5));
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

TEST(XLinqFromTest, XLinqArrayEnumeratorCallNextWhenEnumerationWasFinished)
{
	int values[] = { 1, 2, 3, 4, 5 };
	shared_ptr<internal::_ArrayEnumerator<int>> enumerator(new internal::_ArrayEnumerator<int>(values, 5));
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

TEST(XLinqFromTest, XLinqEnumerableFromArray)
{
	int values[] = { 1, 2, 3, 4, 5 };
	auto enumerator = from(values) >> getEnumerator();
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
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqFromTest, XLinqEnumerableFromMultiDimArray)
{
	int values[2][2][3] = {
		{
			{ 1, 2, 3 },
			{ 4, 5, 6 }
		},
		{
			{ 7, 8, 9 },
			{ 10, 11, 12 },
		}
	};
	auto enumerator = from_array(values) >> getEnumerator();
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
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(11, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(12, enumerator->current());
	ASSERT_FALSE(enumerator->next());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(12, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(11, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(10, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(8, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());
	ASSERT_TRUE(enumerator->advance(3));
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->advance(3));
	ASSERT_EQ(6, enumerator->current());
	ASSERT_FALSE(enumerator->advance(7));
	ASSERT_TRUE(enumerator->advance(-7));
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_FALSE(enumerator->advance(10));
	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ(11, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-11));
	ASSERT_TRUE(enumerator->advance(5));
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->advance(7));
	ASSERT_EQ(12, enumerator->current());
	ASSERT_FALSE(enumerator->next());
	ASSERT_TRUE(enumerator->advance(-3));
	ASSERT_EQ(10, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-3));
	ASSERT_EQ(7, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-6));
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->advance(12));
	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-3));
	ASSERT_EQ(6, enumerator->current());
	ASSERT_TRUE(enumerator->advance(3));
	ASSERT_EQ(9, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-6));
	ASSERT_EQ(3, enumerator->current());
}

TEST(XLinqFromTest, XLinqEnumerableFromStdArray)
{
	array<int, 5> values = { 1, 2, 3, 4, 5 };
	auto enumerator = from(values) >> getEnumerator();
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
	ASSERT_FALSE(enumerator->next());
}

TEST(XLinqFromTest, XlinqStlRandomAccessEnumerator)
{
	typedef typename internal::_StlRandomAccessEnumerator<typename vector<int>::iterator, int> RAE;
	vector<int> numbers = { 1, 2, 3, 4, 5 };
	auto enumerator = std::shared_ptr<IRandomAccessEnumerator<int>>(new RAE(numbers.begin(), numbers.end()));

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
	ASSERT_FALSE(enumerator->next());

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-3));
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->advance(10));
	ASSERT_TRUE(enumerator->advance(-1));
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-10));
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(1, enumerator->current());
}

TEST(XLinqFromTest, XlinqFromVectorRandomAccessEnumerator)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };
	auto enumerator = from(numbers) >> getEnumerator();

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
	ASSERT_FALSE(enumerator->next());

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(5, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->back());

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->advance(2));
	ASSERT_EQ(4, enumerator->current());
	ASSERT_TRUE(enumerator->advance(-3));
	ASSERT_EQ(1, enumerator->current());
	ASSERT_FALSE(enumerator->advance(10));
	ASSERT_TRUE(enumerator->advance(-1));
	ASSERT_EQ(5, enumerator->current());
	ASSERT_FALSE(enumerator->advance(-10));
	ASSERT_TRUE(enumerator->advance(1));
	ASSERT_EQ(1, enumerator->current());
}