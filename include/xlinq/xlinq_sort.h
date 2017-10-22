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
*	@file xlinq_sort.h
*	Sorting collection.
*	@author TrolleY
*/
#ifndef XLINQ_SORT_H_
#define XLINQ_SORT_H_

#include <memory>
#include <algorithm>
#include <vector>
#include <functional>
#include "xlinq_base.h"
#include "xlinq_from.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{

		template<typename TEnumerable, typename TComparer>
		std::shared_ptr<IRandomAccessEnumerable<typename TEnumerable::element_type::ElemType>> build_sort(TEnumerable enumerable, TComparer comparer, int size = -1)
		{
			typedef typename TEnumerable::element_type::ElemType TElem;
			auto vec = std::shared_ptr<std::vector<TElem>>(new std::vector<TElem>());
			if (size >= 0)
				vec->reserve(size);
			for (auto it = enumerable->getEnumerator(); it->next();)
			{
				vec->push_back(it->current());
			}
			std::stable_sort(vec->begin(), vec->end(), comparer);
			return from(vec);
		}

		template<typename TComparer>
		class _SortBuilderWithComp
		{
			TComparer _comparer;
		public:
			_SortBuilderWithComp(TComparer comparer) : _comparer(comparer) {}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_sort(enumerable, _comparer);
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_sort(enumerable, _comparer);
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_sort(enumerable, _comparer, enumerable->size());
			}
		};

		class _SortBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_sort(enumerable, std::less<TElem>());
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_sort(enumerable, std::less<TElem>());
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_sort(enumerable, std::less<TElem>(), enumerable->size());
			}
		};
	}
	/*@endcond*/

	/**
	*	Performs stable sort on collection.
	*	This function may be used to sort collection elements using standard
	*	std::stable_sort algorithm using std::less<T> comparer.
	*	@return Builder of sort expression.
	*/
	XLINQ_INLINE internal::_SortBuilder sort()
	{
		return internal::_SortBuilder();
	}

	/**
	*	Performs stable sort on collection.
	*	This function may be used to sort collection elements using standard
	*	std::stable_sort algorithm with specified elements comparer.
	*	@return Builder of sort expression.
	*/
	template<typename TComparer>
	XLINQ_INLINE internal::_SortBuilderWithComp<TComparer> sort(TComparer comparer)
	{
		return internal::_SortBuilderWithComp<TComparer>(comparer);
	}
}

#endif