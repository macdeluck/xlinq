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
*	@file xlinq_gather.h
*	Gathering the collection elements.
*	@author TrolleY
*/
#ifndef XLINQ_GATHER_H_
#define XLINQ_GATHER_H_

#include "xlinq_base.h"
#include "xlinq_from.h"
#include <memory>
#include <vector>

namespace xlinq
{
	/*@cond INTERNAL*/
	namespace internal
	{
		class _GatherBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto vec = std::shared_ptr<std::vector<TElem>>(new std::vector<TElem>());
				for (auto it = enumerable->getEnumerator(); it->next();)
				{
					vec->push_back(it->current());
				}
				return from(vec);
			}
		};
	}
	/*@endcond*/

	/**
	*	Gathers elements of collection.
	*	This function allows to gather elements of collection to improve query performance,
	*	or to copy collection elements preventing from deallocating enumeration source collection.
	*	It is used to intentionally omit effects, advantages and disadvantages of deffered execution.
	*	@return Builder of gather expression.
	*/
	XLINQ_INLINE internal::_GatherBuilder gather()
	{
		return internal::_GatherBuilder();
	}
}

#endif