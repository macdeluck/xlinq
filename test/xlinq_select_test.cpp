#include <gtest/gtest.h>
#include <xlinq/xlinq_select.h>
#include <xlinq/xlinq_from.h>
#include "model/xlinq_test_model.h"
#include <memory>

using namespace std;
using namespace xlinq;

TEST(XLinqSelectTest, XlinqSelectFieldFromObjectTest)
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
}