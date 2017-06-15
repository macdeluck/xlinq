#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_from_container.h>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;
using namespace xlinq;

TEST(XLinqFromVectorTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersons();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromVectorTest, CallBackBeforeEnumerationWasStarted)
{
	auto persons = getPersons();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->back();
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

TEST(XLinqFromVectorTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersons();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromVectorTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersons();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromVectorTest, RandomAccessEnumerableBegin)
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

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Kamil", enumerator->current().firstName);
	ASSERT_EQ("Goryszewicz", enumerator->current().secondName);
	ASSERT_EQ(18, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);

	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(7));

	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->advance(4));
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_FALSE(enumerator->advance(-7));

	ASSERT_TRUE(enumerator->advance(6));
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);
}

TEST(XLinqFromVectorTest, RandomAccessEnumerableEnd)
{
	auto persons = getPersons();
	auto enumerator = from(persons) >> getEndEnumerator();

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Kamil", enumerator->current().firstName);
	ASSERT_EQ("Goryszewicz", enumerator->current().secondName);
	ASSERT_EQ(18, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);

	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(7));

	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->advance(4));
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_FALSE(enumerator->advance(-7));

	ASSERT_TRUE(enumerator->advance(6));
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);
}

TEST(XLinqFromVectorTest, RandomAccessEnumerableAt)
{
	auto persons = getPersons();
	auto enumerator = from(persons) >> getEnumeratorAt(2);

	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);

	ASSERT_FALSE(enumerator->back());

	ASSERT_FALSE(enumerator->advance(7));

	ASSERT_TRUE(enumerator->advance(-4));
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->advance(-2));
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->advance(4));
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_FALSE(enumerator->advance(-7));

	ASSERT_TRUE(enumerator->advance(6));
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);
}

TEST(XLinqFromVectorTest, RandomAccessEnumerableEmpty)
{
	std::vector<int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
	ASSERT_FALSE((from(container) >> getEndEnumerator())->back());
	ASSERT_FALSE((from(container) >> getEnumerator())->advance(1));
	ASSERT_FALSE((from(container) >> getEnumerator())->advance(-1));
}

TEST(XLinqFromVectorTest, RandomAccessEnumerableSize)
{
	auto persons = getPersons();
	ASSERT_EQ(6, from(persons)->size());
}

TEST(XLinqFromListTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsList();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromListTest, CallBackBeforeEnumerationWasStarted)
{
	auto persons = getPersonsList();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->back();
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

TEST(XLinqFromListTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsList();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromListTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsList();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromListTest, BidirectionalEnumerableBegin)
{
	auto persons = getPersonsList();
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

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Kamil", enumerator->current().firstName);
	ASSERT_EQ("Goryszewicz", enumerator->current().secondName);
	ASSERT_EQ(18, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);

	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqFromListTest, BidirectionalEnumerableEnd)
{
	auto persons = getPersonsList();
	auto enumerator = from(persons) >> getEndEnumerator();

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Jan", enumerator->current().firstName);
	ASSERT_EQ("Lampus", enumerator->current().secondName);
	ASSERT_EQ(71, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Kamil", enumerator->current().firstName);
	ASSERT_EQ("Goryszewicz", enumerator->current().secondName);
	ASSERT_EQ(18, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Joanna", enumerator->current().firstName);
	ASSERT_EQ("Gwizd", enumerator->current().secondName);
	ASSERT_EQ(37, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Anna", enumerator->current().firstName);
	ASSERT_EQ("Ga³¹zka", enumerator->current().secondName);
	ASSERT_EQ(54, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Kania", enumerator->current().secondName);
	ASSERT_EQ(22, enumerator->current().age);

	ASSERT_TRUE(enumerator->back());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);

	ASSERT_FALSE(enumerator->back());
}

TEST(XLinqFromListTest, BidirectionalEnumerableEmpty)
{
	std::list<int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
	ASSERT_FALSE((from(container) >> getEndEnumerator())->back());
}

TEST(XLinqFromForwardListTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsForwardList();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromForwardListTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsForwardList();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromForwardListTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsForwardList();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromForwardListTest, EnumerableBegin)
{
	auto persons = getPersonsForwardList();
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

TEST(XLinqFromForwardListTest, EnumerableEmpty)
{
	std::forward_list<int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
}

TEST(XLinqFromSetTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromSetTest, CallBackBeforeEnumerationWasStarted)
{
	auto persons = getPersonsSet();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->back();
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

TEST(XLinqFromSetTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromSetTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromSetTest, BidirectionalEnumerableBegin)
{
	auto persons = getPersonsSet();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromSetTest, BidirectionalEnumerableEnd)
{
	auto persons = getPersonsSet();
	auto enumerator = from(persons) >> getEndEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
}

TEST(XLinqFromSetTest, BidirectionalEnumerableEmpty)
{
	std::set<int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
	ASSERT_FALSE((from(container) >> getEndEnumerator())->back());
}

TEST(XLinqFromMultiSetTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromMultiSetTest, CallBackBeforeEnumerationWasStarted)
{
	auto persons = getPersonsMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->back();
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

TEST(XLinqFromMultiSetTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromMultiSetTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromMultiSetTest, BidirectionalEnumerableFromBegin)
{
	auto persons = getPersonsMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromMultiSetTest, BidirectionalEnumerableFromEnd)
{
	auto persons = getPersonsMultiSet();
	auto enumerator = from(persons) >> getEndEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
}

TEST(XLinqFromMultiSetTest, BidirectionalEnumerableEmpty)
{
	std::multiset<int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
	ASSERT_FALSE((from(container) >> getEndEnumerator())->back());
}

TEST(XLinqFromMapTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromMapTest, CallBackBeforeEnumerationWasStarted)
{
	auto persons = getPersonsMap();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->back();
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

TEST(XLinqFromMapTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromMapTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromMapTest, BidirectionalEnumerableBegin)
{
	auto persons = getPersonsMap();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromMapTest, BidirectionalEnumerableEnd)
{
	auto persons = getPersonsMap();
	auto enumerator = from(persons) >> getEndEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
}

TEST(XLinqFromMapTest, BidirectionalEnumerableEmpty)
{
	std::map<int, int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
	ASSERT_FALSE((from(container) >> getEndEnumerator())->back());
}

TEST(XLinqFromMultiMapTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->current().second;
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

TEST(XLinqFromMultiMapTest, CallBackBeforeEnumerationWasStarted)
{
	auto persons = getPersonsMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->back();
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

TEST(XLinqFromMultiMapTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
	while (enumerator->next());
	try
	{
		enumerator->current().second;
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

TEST(XLinqFromMultiMapTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromMultiMapTest, BidirectionalEnumerableBegin)
{
	auto persons = getPersonsMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromMultiMapTest, BidirectionalEnumerableEnd)
{
	auto persons = getPersonsMultiMap();
	auto enumerator = from(persons) >> getEndEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->back());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->back());
	ASSERT_EQ(6, occuredPersons.size());

	occuredPersons.clear();
}

TEST(XLinqFromMultiMapTest, BidirectionalEnumerableEmpty)
{
	std::multimap<int, int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
	ASSERT_FALSE((from(container) >> getEndEnumerator())->back());
}

TEST(XLinqFromUnorderedSetTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsUnorderedSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedSetTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedSetTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedSetTest, EnumerableBegin)
{
	auto persons = getPersonsUnorderedSet();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromUnorderedSetTest, EnumerableEmpty)
{
	std::unordered_set<int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
}

TEST(XLinqFromUnorderedMultiSetTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsUnorderedMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedMultiSetTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedMultiSetTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedMultiSetTest, EnumerableBegin)
{
	auto persons = getPersonsUnorderedMultiSet();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current());

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromUnorderedMultiSetTest, EnumerableEmpty)
{
	std::unordered_multiset<int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
}

TEST(XLinqFromUnorderedMapTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedMapTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedMapTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedMapTest, EnumerableBegin)
{
	auto persons = getPersonsUnorderedMap();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromUnorderedMapTest, EnumerableEmpty)
{
	std::unordered_map<int, int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
}

TEST(XLinqFromUnorderedMultiMapTest, CallCurrentBeforeEnumerationWasStarted)
{
	auto persons = getPersonsUnorderedMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
	try
	{
		enumerator->current().second;
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

TEST(XLinqFromUnorderedMultiMapTest, CallCurrentWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
	while (enumerator->next());
	try
	{
		enumerator->current().second;
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

TEST(XLinqFromUnorderedMultiMapTest, CallNextWhenEnumerationWasFinished)
{
	auto persons = getPersonsUnorderedMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
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

TEST(XLinqFromUnorderedMultiMapTest, EnumerableBegin)
{
	auto persons = getPersonsUnorderedMultiMap();
	auto enumerator = from(persons) >> getEnumerator();
	std::set<Person> occuredPersons;

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_TRUE(enumerator->next());
	occuredPersons.insert(enumerator->current().second);

	ASSERT_FALSE(enumerator->next());
	ASSERT_EQ(6, occuredPersons.size());
}

TEST(XLinqFromUnorderedMultiMapTest, EnumerableEmpty)
{
	std::unordered_multimap<int, int> container;
	ASSERT_FALSE((from(container) >> getEnumerator())->next());
}
