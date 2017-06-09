#ifndef XLINQ_TEST_MODEL_H_
#define XLINQ_TEST_MODEL_H_

#include <xlinq/xlinq_defs.h>
#include <xlinq/xlinq_base.h>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <memory>

namespace xlinq
{
	class Person
	{
	public:
		std::string firstName;
		std::string secondName;
		int age;

		XLINQ_INLINE bool operator<(const Person& other) const { return age < other.age; }

		XLINQ_INLINE bool operator>(const Person& other) const { return age > other.age; }
	};

	struct PersonHasher
	{
		std::hash<int> hasher;

	public:
		XLINQ_INLINE size_t operator()(const Person& val) const
		{
			return hasher(val.age);
		}
	};

	struct PersonEqualityComparer
	{
	public:
		XLINQ_INLINE size_t operator()(const Person& first, const Person& second) const
		{
			return first.age == second.age && first.firstName == second.firstName && first.secondName == second.secondName;
		}
	};

	XLINQ_INLINE std::vector<Person> getPersons()
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

	XLINQ_INLINE std::list<Person> getPersonsList()
	{
		auto result = getPersons();
		return std::list<Person>(result.begin(), result.end());
	}

	XLINQ_INLINE std::forward_list<Person> getPersonsForwardList()
	{
		auto result = getPersons();
		return std::forward_list<Person>(result.begin(), result.end());
	}

	XLINQ_INLINE std::set<Person> getPersonsSet()
	{
		auto result = getPersons();
		return std::set<Person>(result.begin(), result.end());
	}

	XLINQ_INLINE std::multiset<Person> getPersonsMultiSet()
	{
		auto result = getPersons();
		return std::multiset<Person>(result.begin(), result.end());
	}

	XLINQ_INLINE std::map<int, Person> getPersonsMap()
	{
		auto prsns = getPersons();
		auto result = std::map<int, Person>();
		for (auto p : prsns)
			result.insert(std::pair<int, Person>(p.age, p));
		return result;
	}

	XLINQ_INLINE std::multimap<int, Person> getPersonsMultiMap()
	{
		auto prsns = getPersons();
		auto result = std::multimap<int, Person>();
		for (auto p : prsns)
			result.insert(std::pair<int, Person>(p.age, p));
		return result;
	}

	XLINQ_INLINE std::unordered_set<Person, PersonHasher, PersonEqualityComparer> getPersonsUnorderedSet()
	{
		auto result = getPersons();
		return std::unordered_set<Person, PersonHasher, PersonEqualityComparer>(result.begin(), result.end());
	}

	XLINQ_INLINE std::unordered_multiset<Person, PersonHasher, PersonEqualityComparer> getPersonsUnorderedMultiSet()
	{
		auto result = getPersons();
		return std::unordered_multiset<Person, PersonHasher, PersonEqualityComparer>(result.begin(), result.end());
	}

	XLINQ_INLINE std::unordered_map<int, Person> getPersonsUnorderedMap()
	{
		auto prsns = getPersons();
		auto result = std::unordered_map<int, Person>();
		for (auto p : prsns)
			result.insert(std::pair<int, Person>(p.age, p));
		return result;
	}

	XLINQ_INLINE std::unordered_multimap<int, Person, std::hash<int>> getPersonsUnorderedMultiMap()
	{
		auto prsns = getPersons();
		auto result = std::unordered_multimap<int, Person>();
		for (auto p : prsns)
			result.insert(std::pair<int, Person>(p.age, p));
		return result;
	}

	XLINQ_INLINE std::shared_ptr<std::vector<Person>> getPersonsSharedPtr()
	{
		auto result = getPersons();
		return std::shared_ptr<std::vector<Person>>(new std::vector<Person>(result.begin(), result.end()));
	}

	XLINQ_INLINE std::shared_ptr<std::list<Person>> getPersonsListSharedPtr()
	{
		auto result = getPersons();
		return std::shared_ptr<std::list<Person>>(new std::list<Person>(result.begin(), result.end()));
	}

	XLINQ_INLINE std::shared_ptr<std::forward_list<Person>> getPersonsForwardListSharedPtr()
	{
		auto result = getPersons();
		return std::shared_ptr<std::forward_list<Person>>(new std::forward_list<Person>(result.begin(), result.end()));
	}

	XLINQ_INLINE std::shared_ptr<std::set<Person>> getPersonsSetSharedPtr()
	{
		auto result = getPersons();
		return std::shared_ptr<std::set<Person>>(new std::set<Person>(result.begin(), result.end()));
	}

	XLINQ_INLINE std::shared_ptr<std::multiset<Person>> getPersonsMultiSetSharedPtr()
	{
		auto result = getPersons();
		return std::shared_ptr<std::multiset<Person>>(new std::multiset<Person>(result.begin(), result.end()));
	}

	XLINQ_INLINE std::shared_ptr<std::map<int, Person>> getPersonsMapSharedPtr()
	{
		auto prsns = getPersons();
		auto result = std::shared_ptr<std::map<int, Person>>(new std::map<int, Person>());
		for (auto p : prsns)
			result->insert(std::pair<int, Person>(p.age, p));
		return result;
	}

	XLINQ_INLINE std::shared_ptr<std::multimap<int, Person>> getPersonsMultiMapSharedPtr()
	{
		auto prsns = getPersons();
		auto result = std::shared_ptr<std::multimap<int, Person>>(new std::multimap<int, Person>());
		for (auto p : prsns)
			result->insert(std::pair<int, Person>(p.age, p));
		return result;
	}

	XLINQ_INLINE std::shared_ptr<std::unordered_set<Person, PersonHasher, PersonEqualityComparer>> getPersonsUnorderedSetSharedPtr()
	{
		auto result = getPersons();
		return std::shared_ptr<std::unordered_set<Person, PersonHasher, PersonEqualityComparer>>(new std::unordered_set<Person, PersonHasher, PersonEqualityComparer>(result.begin(), result.end()));
	}

	XLINQ_INLINE std::shared_ptr<std::unordered_multiset<Person, PersonHasher, PersonEqualityComparer>> getPersonsUnorderedMultiSetSharedPtr()
	{
		auto result = getPersons();
		return std::shared_ptr<std::unordered_multiset<Person, PersonHasher, PersonEqualityComparer>>(new std::unordered_multiset<Person, PersonHasher, PersonEqualityComparer>(result.begin(), result.end()));
	}

	XLINQ_INLINE std::shared_ptr<std::unordered_map<int, Person>> getPersonsUnorderedMapSharedPtr()
	{
		auto prsns = getPersons();
		auto result = std::shared_ptr<std::unordered_map<int, Person>>(new std::unordered_map<int, Person>());
		for (auto p : prsns)
			result->insert(std::pair<int, Person>(p.age, p));
		return result;
	}

	XLINQ_INLINE std::shared_ptr<std::unordered_multimap<int, Person, std::hash<int>>> getPersonsUnorderedMultiMapSharedPtr()
	{
		auto prsns = getPersons();
		auto result = std::shared_ptr<std::unordered_multimap<int, Person>>(new std::unordered_multimap<int, Person>());
		for (auto p : prsns)
			result->insert(std::pair<int, Person>(p.age, p));
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

		XLINQ_INLINE std::shared_ptr<IEnumerator<Person>> createEnumerator() override
		{
			return std::shared_ptr<IEnumerator<Person>>(new PersonEnumerator(this));
		}
	};
}

#endif