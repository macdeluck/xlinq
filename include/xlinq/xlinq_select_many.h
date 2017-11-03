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
*	@file xlinq_select_many.h
*	Flattening collections.
*	@author TrolleY
*/
#ifndef XLINQ_SELECT_MANY_H_
#define XLINQ_SELECT_MANY_H_

#include <memory>
#include "xlinq_base.h"
#include "xlinq_from.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/

	namespace internal
	{
		template<typename TSelector, typename TElem, typename TSelectCollection, typename TSelect>
		class _SelectManyEnumerator : public IEnumerator<TSelect>
		{
		private:
			struct CollectionContainer
			{
				TSelectCollection collection;
			};

			TSelector _selector;
			std::shared_ptr<IEnumerator<TElem>> _source;
			std::shared_ptr<CollectionContainer> _container;
			std::shared_ptr<IEnumerator<TSelect>> _current;

		public:
			_SelectManyEnumerator(TSelector selector, std::shared_ptr<IEnumerator<TElem>> source)
				: _selector(selector), _source(source), _container(nullptr), _current(nullptr) {}

			bool next() override
			{
				while (!_current || !_current->next())
				{
					if (!_source->next())
						return false;
					_container.reset(new CollectionContainer());
					_container->collection = _selector(_source->current());
					_current = from(_container->collection)->getEnumerator();
				}
				return true;
			}

			TSelect current() override
			{
				return _current->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TSelect>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SelectManyEnumerator<TSelector, TElem, TSelectCollection, TSelect>>(other);
				if (!pother)
					return false;
				return this->_source->equals(pother->_source) &&
					((bool)this->_current) == ((bool)pother->_current) &&
					((!this->_current) || (this->_current->equals(pother->_current)));
			}

			std::shared_ptr<IEnumerator<TSelect>> clone() const override
			{
				auto ptr = new _SelectManyEnumerator<TSelector, TElem, TSelectCollection, TSelect>(this->_selector, this->_source->clone());
				if (this->_container)
					ptr->_container = this->_container;
				if (this->_current)
					ptr->_current = this->_current->clone();
				return std::shared_ptr<IEnumerator<TSelect>>(ptr);
			}
		};

		template<typename TSelector, typename TElem, typename TSelectCollection, typename TSelect>
		class _SelectManyEnumerable : public IEnumerable<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IEnumerable<TElem>> _source;

		public:
			_SelectManyEnumerable(TSelector selector, std::shared_ptr<IEnumerable<TElem>> source)
				: _selector(selector), _source(source) {}

			std::shared_ptr<IEnumerator<TSelect>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectManyEnumerator<TSelector, TElem, TSelectCollection, TSelect>(_selector, _source->getEnumerator()));
			}
		};

		template<typename TSelector, typename TElem>
		struct selectmanytypeinfo
		{
			typedef decltype(std::declval<TSelector>()((std::declval<typename std::add_lvalue_reference<TElem>::type>()))) TSelectCollection;
			typedef decltype(from(std::declval<typename std::add_lvalue_reference<TSelectCollection>::type>())) TSelectEnumerable;
			typedef typename TSelectEnumerable::element_type::ElemType TSelect;
			typedef _SelectManyEnumerable<TSelector, TElem, TSelectCollection, TSelect> TEnumerable;
		};

		template<typename TSelector>
		class _SelectManyBuilder
		{
		private:
			TSelector _selector;
		
		public:
			_SelectManyBuilder(TSelector selector) : _selector(selector) {}

			template<typename TElem>
			auto build(std::shared_ptr<IEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<typename selectmanytypeinfo<TSelector, TElem>::TSelect>>
			{
				return std::shared_ptr<IEnumerable<typename selectmanytypeinfo<TSelector, TElem>::TSelect>>(new typename selectmanytypeinfo<TSelector, TElem>::TEnumerable(_selector, enumerable));
			}

			template<typename TElem>
			auto build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<typename selectmanytypeinfo<TSelector, TElem>::TSelect>>
			{
				return std::shared_ptr<IEnumerable<typename selectmanytypeinfo<TSelector, TElem>::TSelect>>(new typename selectmanytypeinfo<TSelector, TElem>::TEnumerable(_selector, enumerable));
			}

			template<typename TElem>
			auto build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<typename selectmanytypeinfo<TSelector, TElem>::TSelect>>
			{
				return std::shared_ptr<IEnumerable<typename selectmanytypeinfo<TSelector, TElem>::TSelect>>(new typename selectmanytypeinfo<TSelector, TElem>::TEnumerable(_selector, enumerable));
			}
		};
	}
	/*@endcond*/

	/**
	*	Flattens collection by concatenating subelements of original collection.
	*	This function may be used to create collection from collection of collections
	*	by concatenating results of each original collection subelements.
	*	@param selector Function used to convert each element of source collection
	*	to collection which will be selected into result collection.
	*	@return Builder of select_many expression.
	*/
	template<typename TSelector>
	internal::_SelectManyBuilder<TSelector> select_many(TSelector selector)
	{
		return internal::_SelectManyBuilder<TSelector>(selector);
	}
}

#endif