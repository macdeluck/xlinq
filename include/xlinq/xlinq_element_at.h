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
*	@file xlinq_element_at.h
*	Extracting element at given index.
*	@author TrolleY
*/
#ifndef XLINQ_ELEMENT_AT_H_
#define XLINQ_ELEMENT_AT_H_

#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _ElementAtBuilder
		{
			int _index;
		public:
			_ElementAtBuilder(int index) : _index(index) {}

			template<typename TElem>
			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEnumerator();
				for (int i = 0; i <= _index; i++)
					enumerator->next();
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto enumerator = enumerable->getEnumerator();
				for (int i = 0; i <= _index; i++)
					enumerator->next();
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumeratorAt(_index)->current();
			}
		};

		template<typename TDefaultElem>
		class _ElementAtOrDefaultBuilder
		{
			int _index;
			TDefaultElem _default;
		public:
			_ElementAtOrDefaultBuilder(int index, TDefaultElem def) : _index(index), _default(def) {}

			template<typename TElem>
			TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				if (_index < 0)
					return (TElem)_default;
				auto enumerator = enumerable->getEnumerator();
				for (int i = 0; i <= _index; i++)
					if (!enumerator->next()) return (TElem)_default;
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				if (_index < 0)
					return (TElem)_default;
				auto enumerator = enumerable->getEnumerator();
				for (int i = 0; i <= _index; i++)
					if (!enumerator->next()) return (TElem)_default;
				return enumerator->current();
			}

			template<typename TElem>
			TElem build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				if ((_index < 0) || (_index >= enumerable->size())) return (TElem)_default;
				return enumerable->getEnumeratorAt(_index)->current();
			}
		};
	}
	/*@endcond*/

	/**
	*	Extracts element at given index.
	*	This function may be used to extract element at given index.
	*	It will throw IterationFinishedException if collection has less elements than specified
	*	index or IterationNotStartedException if index is negative number.
	*	@throws IterationNotStartedException
	*	@throws IterationFinishedException
	*	@return Builder of element_at expression.
	*/
	XLINQ_INLINE internal::_ElementAtBuilder element_at(int index)
	{
		return internal::_ElementAtBuilder(index);
	}

	/**
	*	Extracts element at given index or returns default value if it is out of range.
	*	This function may be used to extract element at given index.
	*	It will return defaultElem if specified index is out of range.
	*	@return Builder of element_at_or_default expression.
	*/
	template<typename TElem>
	XLINQ_INLINE internal::_ElementAtOrDefaultBuilder<TElem> element_at_or_default(int index, TElem defaultElem)
	{
		return internal::_ElementAtOrDefaultBuilder<TElem>(index, defaultElem);
	}
}

#endif