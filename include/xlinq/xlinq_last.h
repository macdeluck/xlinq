/*
MIT License

Copyright (c) 2017 TrolleY

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/**
*	@file xlinq_last.h
*	Extracting last element from collection.
*	@author TrolleY
*/
#ifndef XLINQ_LAST_H_
#define XLINQ_LAST_H_

#include "xlinq_base.h"
#include "xlinq_where.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _LastBuilder
		{
		public:
			template<typename TElem>
			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEnumerator();
				enumerator->next();
				while (true)
				{
					// Workaround to not create empty object, default constructor may not be accessible.
					auto result = enumerator->current();
					if (!enumerator->next())
						return result;
				}
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEndEnumerator();
				if (!enumerator->back()) throw IterationFinishedException();
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEndEnumerator();
				if (!enumerator->back()) throw IterationFinishedException();
				return enumerator->current();
			}
		};

		template<typename TPredicate>
		class _LastPredicateBuilder
		{
			TPredicate _predicate;

		public:
			_LastPredicateBuilder(TPredicate predicate) : _predicate(predicate) {}

			template<typename TElem>
			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto enumerator = (enumerable >> where(_predicate))->getEnumerator();
				enumerator->next();
				while (true)
				{
					// Workaround to not create empty object, default constructor may not be accessible.
					auto result = enumerator->current();
					if (!enumerator->next())
						return result;
				}
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto enumerator = (enumerable >> where(_predicate))->getEndEnumerator();
				if (!enumerator->back()) throw IterationFinishedException();
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto enumerator = (enumerable >> where(_predicate))->getEndEnumerator();
				if (!enumerator->back()) throw IterationFinishedException();
				return enumerator->current();
			}
		};

		template<typename TElem>
		class _LastOrDefaultBuilder
		{
			TElem _default;
		public:
			_LastOrDefaultBuilder(TElem defaultElem) : _default(defaultElem) {}

			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEnumerator();
				if (enumerator->next())
				{
					while (true)
					{
						// Workaround to not create empty object, default constructor may not be accessible.
						auto result = enumerator->current();
						if (!enumerator->next())
							return result;
					}
				}
				return _default;
			}

			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEndEnumerator();
				return enumerator->back() ? enumerator->current() : _default;
			}

			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEndEnumerator();
				return enumerator->back() ? enumerator->current() : _default;
			}
		};

		template<typename TElem, typename TPredicate>
		class _LastOrDefaultPredicateBuilder
		{
			TElem _default;
			TPredicate _predicate;

		public:
			_LastOrDefaultPredicateBuilder(TElem defaultElem, TPredicate predicate) : _default(defaultElem), _predicate(predicate) {}

			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto enumerator = (enumerable >> where(_predicate))->getEnumerator();
				if (enumerator->next())
				{
					while (true)
					{
						// Workaround to not create empty object, default constructor may not be accessible.
						auto result = enumerator->current();
						if (!enumerator->next())
							return result;
					}
				}
				return _default;
			}

			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto enumerator = (enumerable >> where(_predicate))->getEndEnumerator();
				return enumerator->back() ? enumerator->current() : _default;
			}

			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto enumerator = (enumerable >> where(_predicate))->getEndEnumerator();
				return enumerator->back() ? enumerator->current() : _default;
			}
		};
	}
	/*@endcond*/

	/**
	*	Extracts last element of collection.
	*	This function may be used to extract last element of collection.
	*	It will throw IterationFinishedException if collection contains no elements.
	*	@return Builder of last expression.
	*/
	XLINQ_INLINE internal::_LastBuilder last()
	{
		return internal::_LastBuilder();
	}

	/**
	*	Extracts last element of collection, which satisfies given condition.
	*	This function may be used to find last element in collection
	*	satisfying given condition. It will throw IterationFinishedException
	*	if element satisfying such condition not exists.
	*	@param predicate Predicate used to find certain element.
	*	@return Builder of last expression.
	*/
	template<typename TPredicate>
	XLINQ_INLINE internal::_LastPredicateBuilder<TPredicate> last(TPredicate predicate)
	{
		return internal::_LastPredicateBuilder<TPredicate>(predicate);
	}

	/**
	*	Extracts last element of collection or returns provided default element.
	*	This function may be used to extract last element of collection or return
	*	provided default element if collection is empty.
	*	@param defaultElem Default element returned if collection is empty.
	*	@return Builder of last_or_default expression.
	*/
	template<typename TElem>
	XLINQ_INLINE internal::_LastOrDefaultBuilder<TElem> last_or_default(TElem defaultElem)
	{
		return internal::_LastOrDefaultBuilder<TElem>(defaultElem);
	}

	/**
	*	Extracts last element of collection, which satisfies given condition or returns provided default element.
	*	This function may be used to find last element in collection satisfying 
	*	given condition or return provided default element if no such element exists.
	*	@param defaultElem Default element returned if collection is empty.
	*	@param predicate Predicate used to find certain element.
	*	@return Builder of last_or_default expression.
	*/
	template<typename TElem, typename TPredicate>
	XLINQ_INLINE internal::_LastOrDefaultPredicateBuilder<TElem, TPredicate> last_or_default(TElem defaultElem, TPredicate predicate)
	{
		return internal::_LastOrDefaultPredicateBuilder<TElem, TPredicate>(defaultElem, predicate);
	}
}

#endif