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
*	@file xlinq_aggregate.h
*	Aggregating collection elements using given function.
*	@author TrolleY
*/
#ifndef XLINQ_AGGREGATE_H_
#define XLINQ_AGGREGATE_H_

#include "xlinq_base.h"
#include <type_traits>

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TAggregator>
		class _AggregateBuilder
		{
			TAggregator _aggregator;

			template<typename TElem>
			TElem int_aggregate(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto en = enumerable->getEnumerator();
				en->next();
				auto aggVal = en->current();
				while(en->next())
				{
					aggVal = _aggregator(aggVal, en->current());
				}
				return aggVal;
			}
		public:
			_AggregateBuilder(TAggregator aggregator) : _aggregator(aggregator) {}

			template<typename TElem>
			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return int_aggregate(enumerable);
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return int_aggregate((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return int_aggregate((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};

		template<typename TResult, typename TAggregator>
		class _AggregateResultBuilder
		{
			TResult _seed;
			TAggregator _aggregator;

			template<typename TElem>
			TResult int_aggregate(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto en = enumerable->getEnumerator();
				TResult aggVal = _seed;
				while (en->next())
				{
					aggVal = _aggregator(aggVal, en->current());
				}
				return aggVal;
			}
		public:
			_AggregateResultBuilder(TResult seed, TAggregator aggregator) : _seed(seed), _aggregator(aggregator) {}

			template<typename TElem>
			TResult build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return int_aggregate(enumerable);
			}

			template<typename TElem>
			TResult build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return int_aggregate((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			TResult build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return int_aggregate((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};
	}
	/*@endcond*/

	/**
	*	Aggregates collection elements using given function.
	*	This function may be used to aggregate collection elements into one result element.
	*	It will throw IterationFinishedException if collection contains no elements.
	*	@return Builder of average expression.
	*/
	template<typename TAggregator>
	XLINQ_INLINE internal::_AggregateBuilder<TAggregator> aggregate(TAggregator aggregator)
	{
		return internal::_AggregateBuilder<TAggregator>(aggregator);
	}

	/**
	*	Aggregates collection elements to any element type using given function and seed.
	*	This function may be used to aggregate collection elements into one result element of specified type.
	*	Aggregation is started with given seed.
	*	@return Builder of average expression.
	*/
	template<typename TResult, typename TAggregator>
	XLINQ_INLINE internal::_AggregateResultBuilder<TResult, TAggregator> aggregate(TResult seed, TAggregator aggregator)
	{
		return internal::_AggregateResultBuilder<TResult, TAggregator>(seed, aggregator);
	}
}

#endif