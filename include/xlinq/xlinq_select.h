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
*	@file xlinq_select.h
*	Converting collection of one type to another.
*	@author TrolleY
*/
#ifndef XLINQ_SELECT_H_
#define XLINQ_SELECT_H_

#include <memory>
#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectEnumerator : public IEnumerator<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IEnumerator<TElem>> _source;

		public:
			_SelectEnumerator(TSelector selector, std::shared_ptr<IEnumerator<TElem>> source)
				: _selector(selector), _source(source) {}

			bool next() override
			{
				return _source->next();
			}

			TSelect current() override
			{
				return _selector(_source->current());
			}
		};

		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectEnumerable : public IEnumerable<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IEnumerable<TElem>> _source;

		public:
			_SelectEnumerable(TSelector selector, std::shared_ptr<IEnumerable<TElem>> source)
				: _selector(selector), _source(source) {}

			std::shared_ptr<IEnumerator<TSelect>> getEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEnumerator()));
			}
		};

		template<typename TSelector, typename TElem>
		struct selecttypeinfo
		{
			typedef decltype(std::declval<TSelector>()((std::declval<TElem>()))) TSelect;
			typedef std::shared_ptr<IEnumerable<TSelect>> TSelectEnumerable;
		};

		template<typename TSelector>
		class _SelectBuilder
		{
		private:
			TSelector _selector;
		
		public:
			_SelectBuilder(TSelector selector) : _selector(selector) {}

			template<typename TElem>
			auto build(std::shared_ptr<IEnumerable<TElem>> enumerable) -> typename selecttypeinfo<TSelector, TElem>::TSelectEnumerable
			{
				typedef typename selecttypeinfo<TSelector, TElem>::TSelect TSelect;
				typedef typename selecttypeinfo<TSelector, TElem>::TSelectEnumerable TSelectEnumerable;
				return TSelectEnumerable(new _SelectEnumerable<TSelector, TElem, TSelect>(_selector, enumerable));
			}
		};
	}
	/*@endcond*/

	/**
	*	Converts collection elements from one form to another.
	*	This function may be used to create collection of elements of one type
	*	from collection of emelents of another type.
	*	@param selector Function used to convert elements of source collection
	*	to elements of result collection. It is common to used lambda expression
	*	as selector.
	*	@return Builder of select expression.
	*/
	template<typename TSelector>
	internal::_SelectBuilder<TSelector> select(TSelector selector)
	{
		return internal::_SelectBuilder<TSelector>(selector);
	}
}

#endif