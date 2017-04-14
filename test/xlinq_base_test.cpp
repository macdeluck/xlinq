#include "gtest/gtest.h"
#include "model/xlinq_test_model.h"
#include <xlinq/xlinq_base.h>

class PersonEnumerable : IEnumerable<Person>
{
private:
	std::vector<Person> _source;

	PersonEnumerable() : _source(getPersons()) {}

	class PersonEnumerator : IEnumerator<Person>
	{
	private:
		int _index;
		PersonEnumerable::Ptr _parent;

	public:
		PersonEnumerator(PersonEnumerable::Ptr parent)
			: _parent(parent), _index(-1) 
		{
		}

		Person current() override
		{
			if (_index < 0) throw IterationNotStartedException();
			if (_index >= _source.size()) throw IterationFinishedException();
			return _source[_index];
		}

		bool next() override
		{
			if (_index >= _source.size()) throw IterationFinishedException();
			return ++_index < _source.size();
		}
	};
public:
};

TEST (XLinqBaseTest, BasicEnumerationTest)
{
	PersonEnumerable enumerable;
	IEnumerator<Person>::Ptr enumerator = enumerable->getEnumerator();
	ASSERT_TRUE(enumerator->next());
	ASSERT_EQ("Piotr", enumerator->current().firstName);
	ASSERT_EQ("Kempa", enumerator->current().secondName);
	ASSERT_EQ(21, enumerator->current().age);
}
 
int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}