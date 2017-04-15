#include <gtest/gtest.h>
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_base.h>
#include <xlinq/xlinq_from.h>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;
using namespace xlinq;

TEST(XLinqFromTest, XLinqStlIteratorFromVector)
{
	auto persons = getPersons();
	typedef typename vector<Person>::iterator PersonIterator;
	typedef _StlEnumerator<PersonIterator, Person> PersonEnumerator;
	shared_ptr<IEnumerator<Person>> enumerator(new PersonEnumerator(persons.begin(), persons.end()));
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}