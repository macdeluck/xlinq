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
*	@file xlinq_any.h
*	Checking if collection is empty.
*	@author TrolleY
*/
#ifndef XLINQ_ANY_H_
#define XLINQ_ANY_H_

#include "xlinq_base.h"
#include "xlinq_where.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _AnyBuilder
		{
		public:
			template<typename TElem>
			bool build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumerator()->next();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumerator()->next();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumerator()->next();
			}
		};

		template<typename TPredicate>
		class _AnyPredicateBuilder
		{
			TPredicate _predicate;
		public:
			_AnyPredicateBuilder(TPredicate predicate) : _predicate(predicate) {}

			template<typename TElem>
			bool build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return (enumerable >> where(_predicate))->getEnumerator()->next();
			}

			template<typename TElem>
			bool build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return (enumerable >> where(_predicate))->getEnumerator()->next();
			}

			template<typename TElem>
			bool build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return (enumerable >> where(_predicate))->getEnumerator()->next();
			}
		};
	}
	/*@endcond*/

	/**
	*	Checks if collection contains any element.
	*	This function may be used check if collection is empty.
	*	@return Builder of any expression.
	*/
	XLINQ_INLINE internal::_AnyBuilder any()
	{
		return internal::_AnyBuilder();
	}

	/**
	*	Checks if collection contains any element satysfaing predicate.
	*	This function may be used to check if collection contains elements
	*	which will satisfy provided predicate.
	*	@param predicate Predicate function to satisfy.
	*	@return Builder of any expression.
	*/
	template<typename TPredicate>
	XLINQ_INLINE internal::_AnyPredicateBuilder<TPredicate> any(TPredicate predicate)
	{
		return internal::_AnyPredicateBuilder<TPredicate>(predicate);
	}
}

#endif