#include "CppUnitTest.h"
#include <xlinq.h>

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
	}
}