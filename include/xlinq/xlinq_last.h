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
*	@file xlinq_last.h
*	Extracting last element from collection.
*	@author TrolleY
*/
#ifndef XLINQ_LAST_H_
#define XLINQ_LAST_H_

#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _LastBuilder
		{
		public:
			template<typename TElem>
			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable >> getEnumerator();
				enumerator->next();
				while (true)
				{
					// Workaround to not create empty object, default constructor may not be accessible.
					auto result = enumerator->current();
					if (!enumerator->next())
						return result;
				}
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable >> getEnumerator();
				enumerator->next();
				while (true)
				{
					// Workaround to not create empty object, default constructor may not be accessible.
					auto result = enumerator->current();
					if (!enumerator->next())
						return result;
				}
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable >> getEnumerator();
				enumerator->next();
				while (true)
				{
					// Workaround to not create empty object, default constructor may not be accessible.
					auto result = enumerator->current();
					if (!enumerator->next())
						return result;
				}
				return enumerator->current();
			}
		};
	}
	/*@endcond*/


	/**
	*	Extracts last element of collection.
	*	This function may be used to extract last element of collection.
	*	It will throw IterationFinishedException if collection contains no elements.
	*	@return Builder of last expression.
	*/
	XLINQ_INLINE internal::_LastBuilder last()
	{
		return internal::_LastBuilder();
	}
}

#endif