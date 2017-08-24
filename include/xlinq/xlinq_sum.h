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
*	@file xlinq_sum.h
*	Calculating sum of collection elements.
*	@author TrolleY
*/
#ifndef XLINQ_SUM_H_
#define XLINQ_SUM_H_

#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _SumBuilder
		{
			template<typename TElem>
			TElem int_sum(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto en = enumerable->getEnumerator();
				en->next();
				auto sumVal = en->current();
				while(en->next())
				{
					sumVal += en->current();
				}
				return sumVal;
			}
		public:
			template<typename TElem>
			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return int_sum(enumerable);
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return int_sum((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return int_sum((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};
	}
	/*@endcond*/


	/**
	*	Calculates sum of collection elements.
	*	This function may be used to calculate sum of collection elements.
	*	It will throw IterationFinishedException if collection contains no elements.
	*	@return Builder of sum expression.
	*/
	XLINQ_INLINE internal::_SumBuilder sum()
	{
		return internal::_SumBuilder();
	}
}

#endif