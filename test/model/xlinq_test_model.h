#ifndef XLINQ_TEST_MODEL_H_
#define XLINQ_TEST_MODEL_H_

#include <string>
#include <vector>

namespace xlinq
{
	class Person
	{
	public:
		std::string firstName;
		std::string secondName;
		int age;
	};

	std::vector<Person> getPersons()
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
}

#endif