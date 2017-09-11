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
*	@file xlinq_all.h
*	Checking if collection elements satisfies condition.
*	@author TrolleY
*/
#ifndef XLINQ_ALL_H_
#define XLINQ_ALL_H_

#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TPredicate>
		class _AllBuilder
		{
			TPredicate _predicate;
		public:
			_AllBuilder(TPredicate predicate) : _predicate(predicate) {}

			template<typename TElem>
			bool build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEnumerator();
				while (enumerator->next())
				{
					if (!_predicate(enumerator->current()))
						return false;
				}
				return true;
			}

			template<typename TElem>
			bool build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEnumerator();
				while (enumerator->next())
				{
					if (!_predicate(enumerator->current()))
						return false;
				}
				return true;
			}

			template<typename TElem>
			bool build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEnumerator();
				while (enumerator->next())
				{
					if (!_predicate(enumerator->current()))
						return false;
				}
				return true;
			}
		};
	}
	/*@endcond*/

	/**
	*	Checks if all collection elements satysfy given condition.
	*	This function may be used to check if all collection elements
	*	satisfy provided condition.
	*	@param predicate Condition to satisfy.
	*	@return Builder of all expression.
	*/
	template<typename TPredicate>
	XLINQ_INLINE internal::_AllBuilder<TPredicate> all(TPredicate predicate)
	{
		return internal::_AllBuilder<TPredicate>(predicate);
	}
}

#endif