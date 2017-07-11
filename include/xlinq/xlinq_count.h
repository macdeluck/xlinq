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
*	@file xlinq_count.h
*	Returning number of elements in collection.
*	@author TrolleY
*/
#ifndef XLINQ_COUNT_H_
#define XLINQ_COUNT_H_

#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _CountBuilder
		{
		public:
			template<typename TElem>
			int build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				int count = 0;
				for (auto it = enumerable->getEnumerator(); it->next();)
					++count;
				return count;
			}

			template<typename TElem>
			int build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				int count = 0;
				for (auto it = enumerable->getEnumerator(); it->next();)
					++count;
				return count;
			}

			template<typename TElem>
			int build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return enumerable->size();
			}
		};
	}
	/*@endcond*/


	/**
	*	Returns number of elements in collection.
	*	This function may be used to count number of elements in collection.
	*	It does not count elements if the collection implements IRandomAccessEnumerable interface.
	*	@return Builder of first expression.
	*/
	XLINQ_INLINE internal::_CountBuilder count()
	{
		return internal::_CountBuilder();
	}
}

#endif