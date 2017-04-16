#ifndef XLINQ_TEST_MODEL_H_
#define XLINQ_TEST_MODEL_H_

#include <xlinq/xlinq_defs.h>
#include <xlinq/xlinq_base.h>
#include <string>
#include <vector>
#include <memory>

namespace xlinq
{
	class Person
	{
	public:
		std::string firstName;
		std::string secondName;
		int age;
	};

	inline std::vector<Person> getPersons()
	{
		std::vector<Person> result = {
			Person { "Piotr", "Kempa", 21 },
			Person { "Micha³", "Kania", 22 },
			Person { "Anna", "Ga³¹zka", 54 },
			Person { "Joanna", "Gwizd", 37 },
			Person { "Kamil", "Goryszewicz", 18 },
			Person { "Jan", "Lampus", 71 }
		};
		return result;
	}

	class PersonEnumerable : public IEnumerable<Person>
	{
	private:
		std::vector<Person> _source;

		class PersonEnumerator : public IEnumerator<Person>
		{
		private:
			PersonEnumerable* _parent;
			int _index;

		public:
			XLINQ_INLINE PersonEnumerator(PersonEnumerable* parent)
				: _parent(parent), _index(-1)
			{
			}

			XLINQ_INLINE Person current() override
			{
				if (_index < 0) throw IterationNotStartedException();
				if (_index >= (int)_parent->_source.size()) throw IterationFinishedException();
				return _parent->_source[_index];
			}

			XLINQ_INLINE bool next() override
			{
				if (_index >= (int)_parent->_source.size()) throw IterationFinishedException();
				return ++_index < (int)_parent->_source.size();
			}
		};
	public:
		XLINQ_INLINE PersonEnumerable() : _source(getPersons()) {}

		XLINQ_INLINE std::shared_ptr<IEnumerator<Person>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<Person>>(new PersonEnumerator(this));
		}
	};
}

#endif