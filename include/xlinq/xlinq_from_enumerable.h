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
*	@file xlinq_from_enumerable.h
*	Creating enumerable object from already enumerable object by abstracting its type.
*	@author TrolleY
*/
#ifndef XLINQ_FROM_ENUMERABLE_H_
#define XLINQ_FROM_ENUMERABLE_H_

#include <memory>
#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TEnumerable, typename TElemType>
		struct EnumerableFilterHelper
		{
			typedef typename EnumerableTypeSelector<TEnumerable>::type type;
		};
	}
	/*@endcond*/
	
	/**
	*	Creates enumerable from shared pointer to enumerable.
	*	This function returns cleaned type from given enumerable.
	*	@param enumerable Source enumerable
	*	@return Given pointer.
	*/
	template<typename TEnumerable>
	std::shared_ptr<typename internal::EnumerableFilterHelper<TEnumerable, typename TEnumerable::ElemType>::type> from(std::shared_ptr<TEnumerable> enumerable)
	{
		return (std::shared_ptr<typename internal::EnumerableFilterHelper<TEnumerable, typename TEnumerable::ElemType>::type>)enumerable;
	}
}

#endif