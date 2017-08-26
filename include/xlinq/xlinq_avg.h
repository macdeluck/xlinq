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
*	@file xlinq_avg.h
*	Calculating average value of collection elements.
*	@author TrolleY
*/
#ifndef XLINQ_AVG_H_
#define XLINQ_AVG_H_

#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TAvgElem>
		class _AvgBuilder
		{
			template<typename TElem>
			TAvgElem int_avg(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto en = enumerable->getEnumerator();
				en->next();
				TAvgElem avgVal = (TAvgElem)en->current();
				TAvgElem items = 1;
				while(en->next())
				{
					avgVal += (TAvgElem)en->current();
					items++;
				}
				return avgVal / items;
			}
		public:
			template<typename TElem>
			TAvgElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return int_avg(enumerable);
			}

			template<typename TElem>
			TAvgElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return int_avg((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			TAvgElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return int_avg((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};
	}
	/*@endcond*/

	/**
	*	Calculates average value of collection elements.
	*	This function may be used to calculate average of collection elements.
	*	It will throw IterationFinishedException if collection contains no elements.
	*	@return Builder of average expression.
	*/
	template<typename TAvgElem>
	XLINQ_INLINE internal::_AvgBuilder<TAvgElem> avg()
	{
		return internal::_AvgBuilder<TAvgElem>();
	}

	/**
	*	Calculates average value of collection elements.
	*	This function may be used to calculate average of collection elements.
	*	It will throw IterationFinishedException if collection contains no elements.
	*	@return Builder of average expression.
	*/
	XLINQ_INLINE internal::_AvgBuilder<double> avg()
	{
		return internal::_AvgBuilder<double>();
	}
}

#endif