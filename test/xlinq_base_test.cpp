#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_base.h>
#include <memory>

using namespace std;
using namespace xlinq;

TEST (XLinqBaseTest, BasicEnumerationTest)
{
	shared_ptr<PersonEnumerable> enumerable(new PersonEnumerable());
	shared_ptr<IEnumerator<Person>> enumerator = enumerable->getEnumerator();
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

TEST(XLinqBaseTest, CallCurrentBeforeEnumerationWasStarted)
{
	shared_ptr<PersonEnumerable> enumerable(new PersonEnumerable());
	shared_ptr<IEnumerator<Person>> enumerator = enumerable->getEnumerator();
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

TEST(XLinqBaseTest, CallCurrentWhenEnumerationWasFinished)
{
	shared_ptr<PersonEnumerable> enumerable(new PersonEnumerable());
	shared_ptr<IEnumerator<Person>> enumerator = enumerable->getEnumerator();
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

TEST(XLinqBaseTest, CallNextWhenEnumerationWasFinished)
{
	shared_ptr<PersonEnumerable> enumerable(new PersonEnumerable());
	shared_ptr<IEnumerator<Person>> enumerator = enumerable->getEnumerator();
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

TEST(XLinqBaseTest, XlinqGetEnumeratorTest)
{
	shared_ptr<PersonEnumerable> enumerable(new PersonEnumerable());
	auto enumerator = enumerable >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);
}

TEST(XLinqBaseTest, CloneAndEqualsEnumeratorTest)
{
	shared_ptr<PersonEnumerable> enumerable(new PersonEnumerable());
	auto enumerator = enumerable >> getEnumerator();

	ASSERT_TRUE(enumerator->next());
	auto second = enumerator->clone();
	ASSERT_TRUE(enumerator->equals(second));
	ASSERT_TRUE(enumerator->next());
	ASSERT_FALSE(enumerator->equals(second));
	ASSERT_EQ("Micha³", enumerator->current().firstName);
	ASSERT_EQ("Piotr", second->current().firstName);

	while (enumerator->next());

	ASSERT_EQ("Piotr", second->current().firstName);
	ASSERT_TRUE(second->next());
	ASSERT_EQ("Micha³", second->current().firstName);
}