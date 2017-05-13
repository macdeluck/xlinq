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
*	@file xlinq_where.h
*	Filtering elements of collection using given predicate.
*	@author TrolleY
*/
#ifndef XLINQ_WHERE_H_
#define XLINQ_WHERE_H_

#include <memory>
#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TPredicate>
		class _WhereBuilder
		{
		private:
			TPredicate _predicate;
		public:
			_WhereBuilder(TPredicate predicate) : _predicate(predicate) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return enumerable;
			}
		};
	}
	/*@endcond*/

	/**
	*	Filters elements of collection using predicate.
	*	This function may be used to filter collection elements using given condition.
	*	It is implemented using deffered execution. The traversing of collection
	*	in looking for next element passing given criteria stops until next element
	*	will be requested.
	*	@param predicate Function used to filter elements of source collection.
	*	It is common to use lambda expression as selector.
	*	@return Builder of select expression.
	*/
	template<typename TPredicate>
	internal::_WhereBuilder<TPredicate> where(TPredicate predicate)
	{
		return internal::_WhereBuilder<TPredicate>(predicate);
	}
}

#endif