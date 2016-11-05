#include "CppUnitTest.h"
#include <xlinq.h>

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace xlinq
{
	namespace test
	{
		TEST_CLASS(testStd)
		{
		public:

			TEST_METHOD(Std_TestFromCArray)
			{
				int five[] = { 1, 2, 5, 3, 4 };
				IEnumerable<int>::Ptr en_five = from(five);

				int i = 0;
				for (auto it = en_five->getEnumerator(); it->next();)
					Assert::AreEqual(five[i++], it->current());
				Assert::AreEqual(5, i);
			}

			TEST_METHOD(Std_TestFromArray)
			{
				array<int, 5> five = { 1, 2, 5, 3, 4 };
				auto en_five = from(five);

				int i = 0;
				for (auto it = en_five->getEnumerator(); it->next();)
					Assert::AreEqual(five[i++], it->current());
				Assert::AreEqual(5, i);
			}

			TEST_METHOD(Std_TestFromVector)
			{
				vector<int> v = { 1, 2, 5, 3, 4 };
				auto en_v = from(v);

				int i = 0;
				for (auto it = en_v->getEnumerator(); it->next();)
					Assert::AreEqual(v[i++], it->current());
				Assert::AreEqual(5, i);
			}

			TEST_METHOD(Std_TestFromList)
			{
				int five[] = { 1, 2, 5, 3, 4 };
				list<int> lst;
				for (int i : five)
					lst.push_back(i);

				auto en_lst = from(lst);
				int i = 0;
				for (auto it = en_lst->getEnumerator(); it->next();)
				{
					Assert::AreEqual(five[i++], it->current());
				}
				Assert::AreEqual(5, i);
			}

			TEST_METHOD(Std_TestFromMap)
			{
				int five[] = { 1, 2, 5, 3, 4 };
				bool five_visited[] = { false, false, false, false, false };
				map<int, int> mp;
				for (int i : five)
					mp[i] = 5 * i;

				auto en_map = from(mp);
				for (auto it = en_map->getEnumerator(); it->next();)
				{
					pair<const int, int> elem = it->current();
					int i = 0;
					for (; i < 5; i++)
						if (elem.first == five[i])
							break;

					Assert::IsFalse(five_visited[i]);
					five_visited[i] = true;
					Assert::AreEqual(five[i], elem.first);
					Assert::AreEqual(5 * five[i++], elem.second);
				}

				for (bool b : five_visited)
					Assert::IsTrue(b);
			}

			TEST_METHOD(Std_TestFromSet)
			{
				int five[] = { 1, 2, 5, 3, 4 };
				bool five_visited[] = { false, false, false, false, false };
				set<int> st;
				for (int i : five)
					st.insert(i);

				auto en_st = from(st);
				for (auto it = en_st->getEnumerator(); it->next();)
				{
					auto elem = it->current();
					int i = 0;
					for (; i < 5; i++)
						if (elem == five[i])
							break;

					Assert::IsFalse(five_visited[i]);
					five_visited[i] = true;
					Assert::AreEqual(five[i], elem);
				}

				for (bool b : five_visited)
					Assert::IsTrue(b);
			}

			TEST_METHOD(Std_Iter)
			{
				vector<int> v = { 1, 2, 5, 3, 4 };

				int i = 0;
				for (auto elem : from(v) ^ iter())
					Assert::AreEqual(v[i++], elem);
				Assert::AreEqual(5, i);
			}
		};

		TEST_CLASS(testFirst)
		{
		public:

			TEST_METHOD(First_Test)
			{
				int elems[] = { 1, 3, 5, 4, 2 };
				Assert::AreEqual(elems[0], from(elems) ^ first());
			}
		};

		TEST_CLASS(testCount)
		{
		public:
			TEST_METHOD(Count_Test)
			{
				int elems[] = { 1, 3, 5, 4, 2 };
				Assert::AreEqual(5u, from(elems) ^ count());
			}
		};


		TEST_CLASS(testSum)
		{
		public:
			TEST_METHOD(Sum_Test)
			{
				int elems[] = { 1, 3, 5, 4, 2 };
				Assert::AreEqual(15, from(elems) ^ sum());
			}
		};

		TEST_CLASS(testAggregate)
		{
		public:
			TEST_METHOD(Aggregate_Test)
			{
				int elems[] = { 1, 3, 5, 4, 2 };
				Assert::AreEqual(20, from(elems) ^ aggregate(5, [](int s, int n) { return s + n; }));
			}
		};

		TEST_CLASS(testSelect)
		{
		public:
			TEST_METHOD(Select_Test)
			{
				int elems[] = { 1, 3, 5, 4, 2 };
				bool qElems[5];
				for (size_t i = 0; i < 5; i++)
				{
					qElems[i] = elems[i] * 3 < 12;
				}

				int i = 0;
				for (auto it = from(elems) ^ select([](auto i) { return i * 3 < 12; }) ^ getEnumerator(); it->next();)
				{
					Assert::AreEqual(qElems[i++], it->current());
				}
				Assert::AreEqual(5, i);
			}
		};

		TEST_CLASS(testCast)
		{
		public:
			TEST_METHOD(Cast_Test)
			{
				int elems[] = { 1, 3, 5, 4, 2 };
				double delems[] = { 1, 3, 5, 4, 2 };

				int i = 0;
				for (auto it = from(elems) ^ cast<double>() ^ getEnumerator(); it->next();)
				{
					Assert::AreEqual(delems[i++], it->current());
				}
				Assert::AreEqual(5, i);
			}
		};

		TEST_CLASS(testSelectMany)
		{
		public:
			struct VectorWrapper
			{
				vector<int> elements;

				VectorWrapper(initializer_list<int> elems) : elements(elems) {}

				IEnumerable<int>::Ptr getElements() const { return from(elements); }
			};

			
			TEST_METHOD(SelectMany_Test)
			{
				vector<std::shared_ptr<VectorWrapper>> elems = {
					std::shared_ptr<VectorWrapper>(new VectorWrapper({1, 2, 3})),
					std::shared_ptr<VectorWrapper>(new VectorWrapper({5, 6})),
					std::shared_ptr<VectorWrapper>(new VectorWrapper({8, 9, 10, 11, 12}))
				};
				int flatElems[] = { 1, 2, 3, 5, 6, 8, 9, 10, 11, 12 };
				
				int i = 0;
				for (auto it = 
					from(elems) ^
					selectMany([](std::shared_ptr<VectorWrapper> v) { 
						return v->getElements(); 
					}, [](int e) { 
						return e;
					}) ^ getEnumerator(); it->next();)
				{
					Assert::AreEqual(flatElems[i++], it->current());
				}
				Assert::AreEqual(10, i);
			}
		};

		TEST_CLASS(testWhere)
		{
		public:
			TEST_METHOD(Where_Test)
			{
				int elems[] = { 1, -2, 3, -4, 5 };
				int positive[] = { 1, 3, 5 };

				int i = 0;
				for (auto it = 
					from(elems) ^ 
					where([](auto i) { return i > 0; }) ^ 
					getEnumerator(); it->next();)
				{
					Assert::IsTrue(i < 3);
					Assert::AreEqual(positive[i++], it->current());
				}
				Assert::IsFalse(i == 2);
			}
		};

		TEST_CLASS(testMin)
		{
		public:
			TEST_METHOD(Min_Test)
			{
				int elems[] = { 1, -2, 5, -4, 3 };
				Assert::AreEqual(-4, from(elems) ^ min());
			}
		};

		TEST_CLASS(testMax)
		{
		public:
			TEST_METHOD(Max_Test)
			{
				int elems[] = { 1, -2, 5, -4, 3 };
				Assert::AreEqual(5, from(elems) ^ max());
			}
		};

		TEST_CLASS(testAverage)
		{
		public:
			TEST_METHOD(Average_Test)
			{
				double eps = 1e-06;
				int elems[] = { 1, -2, 5, -4, 3, 9 };
				Assert::IsTrue((2.0 - (from(elems) ^ average())) < eps);
			}

			struct FloatWrapper
			{
				float value;
			};

			TEST_METHOD(Average_CustomTest)
			{
				double eps = 1e-06;
				FloatWrapper elems[] = { 1, -2, 5, -4, 3, 9 };
				Assert::IsTrue((2.0 - (from(elems) ^ average([](FloatWrapper n) { return n.value;}))) < eps);
			}
		};

		TEST_CLASS(testJoin)
		{
			struct First
			{
				int first_id;
				int second_id;
			};

			struct Second
			{
				int second_id;
				char code;
			};

			struct Third
			{
				int first_id;
				int second_id;
				char code;

				Third() {}
				Third(int first_id, int second_id, char code)
					: first_id(first_id), second_id(second_id), code(code) {}
			};

		public:
			TEST_METHOD(Join_Test)
			{
				First firsts[] = { { 1, 2 }, { 2, 3 }, { 3, 1 }, { 4, 12 } };
				Second seconds[] = { { 1, 'A' }, { 2, 'B' }, { 3, 'C' }, { 4, 'D' } };
				Third results[] = { { 1, 2, 'B' }, { 2, 3, 'C' }, { 3, 1, 'A' } };
				bool visitedResults[] = { false, false, false };

				for (auto it =
					from(firsts) ^
					join(from(seconds),
						[](First f) { return f.second_id; },
						[](Second s) { return s.second_id; },
						[](First f, Second s) { return Third(f.first_id, s.second_id, s.code); }) ^
					getEnumerator(); it->next();)
				{
					Third c = it->current();
					for (int i = 0; i < 3; i++)
						if (results[i].first_id == c.first_id &&
							results[i].second_id == c.second_id &&
							results[i].code == c.code)
						{
							Assert::IsFalse(visitedResults[i]);
							visitedResults[i] = true;
							break;
						}
				}
				for (int i = 0; i < 3; i++)
					Assert::IsTrue(visitedResults[i]);
			}
		};

		TEST_CLASS(testGroupJoin)
		{
			struct Person
			{
				using Ptr = shared_ptr<Person>;
				string name;

				Person() {}
				Person(string name) : name(name) {}
			};

			struct Pet
			{
				using Ptr = shared_ptr<Pet>;
				string name;
				Person::Ptr owner;

				Pet() {}
				Pet(string name) : name(name) {}
				Pet(string name, Person::Ptr owner) : name(name), owner(owner) {}
			};

			struct PersonWithPets
			{
				using Ptr = shared_ptr<PersonWithPets>;
				string name;
				IEnumerable<Pet::Ptr>::Ptr pets;

				PersonWithPets() {}
				PersonWithPets(string name) : name(name) {}
				PersonWithPets(string name, IEnumerable<Pet::Ptr>::Ptr pets) : name(name), pets(pets) {}
			};

		public:
			TEST_METHOD(GroupJoin_Test)
			{
				Person::Ptr magnus(new Person("Magnus"));
				Person::Ptr terry(new Person("Terry"));
				Person::Ptr charlotte(new Person("Charlotte"));

				Pet::Ptr barley(new Pet("Barley", terry));
				Pet::Ptr boots(new Pet("Boots", terry));
				Pet::Ptr whiskey(new Pet("Whiskey", charlotte));
				Pet::Ptr daisy(new Pet("Daisy", magnus));

				list<Person::Ptr> people = { magnus, terry, charlotte };
				list<Pet::Ptr> pets = { barley, boots, whiskey, daisy };
				
				map<string, vector<string>> personsWithPets = {
					{ magnus->name, { daisy->name } },
					{ terry->name, { barley->name, boots->name } },
					{ charlotte->name, { whiskey->name } }
				};
				map<string, vector<bool>> personWithPetsValid = {
					{ magnus->name, { false } },
					{ terry->name, { false, false } },
					{ charlotte->name, { false } }
				};

				for (auto it =
					from(people) ^
					groupJoin(from(pets),
						[](Person::Ptr person) { return person->name; },
						[](Pet::Ptr pet) { return pet->owner->name; },
						[](Person::Ptr person, IEnumerable<Pet::Ptr>::Ptr pets)
						{
							return PersonWithPets::Ptr(new PersonWithPets(person->name, pets));
						}) ^
					getEnumerator(); it->next();)
				{
					PersonWithPets::Ptr pwp = it->current();
					auto pwpdest = personsWithPets[pwp->name];
					for (auto pt : pwp->pets ^ iter())
					{
						size_t i;
						for (i = 0; i < pwpdest.size(); i++)
						{
							if (pwpdest[i] == pt->name)
							{
								Assert::IsFalse(personWithPetsValid[pwp->name][i]);
								personWithPetsValid[pwp->name][i] = true;
								break;
							}
						}
						Assert::AreNotEqual(pwpdest.size(), i);
					}
				}

				for (auto kv : personWithPetsValid)
					for (auto val : kv.second)
						Assert::IsTrue(val);
			}
		};

		TEST_CLASS(testSkipWhile)
		{
		public:
			TEST_METHOD(SkipWhile_Test)
			{
				int numbers[] = { 1, 3, 2, 5, 4 };
				int targetNumbers[] = { 2, 5, 4 };

				int i = 0;
				for (auto it =
					from(numbers) ^
					skipWhile([](int i) { return (i % 2) == 1; }) ^
					getEnumerator(); it->next();)
				{
					Assert::AreEqual(targetNumbers[i++], it->current());
				}
				Assert::AreEqual(3, i);
			}
		};

		TEST_CLASS(testSkip)
		{
		public:
			TEST_METHOD(Skip_Test)
			{
				int numbers[] = { 1, 3, 2, 5, 4 };
				int targetNumbers[] = { 2, 5, 4 };

				int i = 0;
				for (auto it =
					from(numbers) ^
					skip(2) ^
					getEnumerator(); it->next();)
				{
					Assert::AreEqual(targetNumbers[i++], it->current());
				}
				Assert::AreEqual(3, i);
			}
		};

		TEST_CLASS(testTake)
		{
		public:
			TEST_METHOD(Take_Test)
			{
				int numbers[] = { 1, 3, 2, 5, 4 };
				int targetNumbers[] = { 1, 3 };

				int i = 0;
				for (auto it =
					from(numbers) ^
					take(2) ^
					getEnumerator(); it->next();)
				{
					Assert::AreEqual(targetNumbers[i++], it->current());
				}
				Assert::AreEqual(2, i);
			}
		};

		TEST_CLASS(testTakeWhile)
		{
		public:
			TEST_METHOD(TakeWhile_Test)
			{
				int numbers[] = { 1, 3, 2, 5, 4 };
				int targetNumbers[] = { 1, 3 };

				int i = 0;
				for (auto it =
					from(numbers) ^
					takeWhile([](int i) { return (i % 2) == 1; }) ^
					getEnumerator(); it->next();)
				{
					Assert::AreEqual(targetNumbers[i++], it->current());
				}
				Assert::AreEqual(2, i);
			}
		};

		TEST_CLASS(testOfType)
		{
			class TestBase
			{
			public:
				using Ptr = std::shared_ptr<TestBase>;
				int member;

				virtual ~TestBase() {}
				TestBase(int member) : member(member) {}
			};

			class Test1 : public TestBase
			{
			public:
				using Ptr = std::shared_ptr<Test1>;

				Test1(int member) : TestBase(member) {}
			};

			class Test2 : public TestBase
			{
			public:
				using Ptr = std::shared_ptr<Test2>;

				Test2(int member) : TestBase(member) {}
			};

		public:
			TEST_METHOD(OfType_TestSharedPtr)
			{
				vector<TestBase::Ptr> vec = {
					TestBase::Ptr(new Test1(1)),
					TestBase::Ptr(new Test2(-1)),
					TestBase::Ptr(new Test1(2)),
					TestBase::Ptr(new Test2(-2)),
					TestBase::Ptr(new Test1(3))
				};
				int test1m[] = { 1, 2, 3 };

				int i = 0;
				for (auto v : from(vec) ^ oftype<Test1>() ^ iter())
				{
					Assert::AreEqual(test1m[i++], v->member);
				}
				Assert::AreEqual(3, i);
			}

			TEST_METHOD(OfType_TestPtr)
			{
				vector<TestBase*> vec = {
					new Test1(1),
					new Test2(-1),
					new Test1(2),
					new Test2(-2),
					new Test1(3)
				};
				int test1m[] = { 1, 2, 3 };

				int i = 0;
				for (auto v : from(vec) ^ oftype<Test1>() ^ iter())
				{
					Assert::AreEqual(test1m[i++], v->member);
				}
				Assert::AreEqual(3, i);

				for (auto v : vec)
					delete v;
			}
		};
	}
}