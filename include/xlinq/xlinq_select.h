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
#include <cassert>
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

			bool equals(std::shared_ptr<IEnumerator<TSelect>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SelectEnumerator<TSelector, TElem, TSelect>>(other);
				if (!pother)
					return false;
				return this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TSelect>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectEnumerator<TSelector, TElem, TSelect>(this->_selector, this->_source->clone()));
			}
		};

		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectBidirectionalEnumerator : public IBidirectionalEnumerator<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IBidirectionalEnumerator<TElem>> _source;

		public:
			_SelectBidirectionalEnumerator(TSelector selector, std::shared_ptr<IBidirectionalEnumerator<TElem>> source)
				: _selector(selector), _source(source) {}

			bool next() override
			{
				return _source->next();
			}

			bool back() override
			{
				return _source->back();
			}

			TSelect current() override
			{
				return _selector(_source->current());
			}

			bool equals(std::shared_ptr<IEnumerator<TSelect>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SelectBidirectionalEnumerator<TSelector, TElem, TSelect>>(other);
				if (!pother)
					return false;
				return this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TSelect>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectBidirectionalEnumerator<TSelector, TElem, TSelect>(
					this->_selector,
					std::dynamic_pointer_cast<IBidirectionalEnumerator<TElem>>(this->_source->clone())));
			}
		};

		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectRandomAccessEnumerator : public IRandomAccessEnumerator<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IRandomAccessEnumerator<TElem>> _source;

		public:
			_SelectRandomAccessEnumerator(TSelector selector, std::shared_ptr<IRandomAccessEnumerator<TElem>> source)
				: _selector(selector), _source(source) {}

			bool next() override
			{
				return _source->next();
			}

			bool back() override
			{
				return _source->back();
			}

			bool advance(int step) override
			{
				return _source->advance(step);
			}

			TSelect current() override
			{
				return _selector(_source->current());
			}

			bool equals(std::shared_ptr<IEnumerator<TSelect>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SelectRandomAccessEnumerator<TSelector, TElem, TSelect>>(other);
				if (!pother)
					return false;
				return this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TSelect>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectRandomAccessEnumerator<TSelector, TElem, TSelect>(
					this->_selector,
					std::dynamic_pointer_cast<IRandomAccessEnumerator<TElem>>(this->_source->clone())));
			}

			int distance_to(std::shared_ptr<IRandomAccessEnumerator<TSelect>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SelectRandomAccessEnumerator<TSelector, TElem, TSelect>>(other);
				assert(pother);
				return this->_source->distance_to(pother->_source);
			}

			bool less_than(std::shared_ptr<IRandomAccessEnumerator<TSelect>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SelectRandomAccessEnumerator<TSelector, TElem, TSelect>>(other);
				assert(pother);
				return this->_source->less_than(pother->_source);
			}

			bool greater_than(std::shared_ptr<IRandomAccessEnumerator<TSelect>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SelectRandomAccessEnumerator<TSelector, TElem, TSelect>>(other);
				assert(pother);
				return this->_source->greater_than(pother->_source);
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

			std::shared_ptr<IEnumerator<TSelect>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEnumerator()));
			}
		};

		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectBidirectionalEnumerable : public IBidirectionalEnumerable<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IBidirectionalEnumerable<TElem>> _source;

		public:
			_SelectBidirectionalEnumerable(TSelector selector, std::shared_ptr<IBidirectionalEnumerable<TElem>> source)
				: _selector(selector), _source(source) {}

			std::shared_ptr<IEnumerator<TSelect>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectBidirectionalEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEnumerator()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TSelect>> createEndEnumerator() override
			{
				return std::shared_ptr<IBidirectionalEnumerator<TSelect>>(new _SelectBidirectionalEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEndEnumerator()));
			}
		};

		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectRandomAccessEnumerable : public IRandomAccessEnumerable<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IRandomAccessEnumerable<TElem>> _source;

		public:
			_SelectRandomAccessEnumerable(TSelector selector, std::shared_ptr<IRandomAccessEnumerable<TElem>> source)
				: _selector(selector), _source(source) {}

			std::shared_ptr<IEnumerator<TSelect>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectRandomAccessEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEnumerator()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TSelect>> createEndEnumerator() override
			{
				return std::shared_ptr<IBidirectionalEnumerator<TSelect>>(new _SelectRandomAccessEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEndEnumerator()));
			}

			std::shared_ptr<IRandomAccessEnumerator<TSelect>> createEnumeratorAt(int elementIndex) override
			{
				return std::shared_ptr<IRandomAccessEnumerator<TSelect>>(new _SelectRandomAccessEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEnumeratorAt(elementIndex)));
			}

			int size() override
			{
				return _source->size();
			}
		};

		template<typename TSelector>
		class _SelectBuilder
		{
		private:
			TSelector _selector;
		
		public:
			_SelectBuilder(TSelector selector) : _selector(selector) {}

			template<typename TElem>
			auto build(std::shared_ptr<IEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<typename unaryreturntype<TSelector, TElem>::type>>
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TSelect>>(new _SelectEnumerable<TSelector, TElem, TSelect>(_selector, enumerable));
			}

			template<typename TElem>
			auto build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable) -> std::shared_ptr<IBidirectionalEnumerable<typename unaryreturntype<TSelector, TElem>::type>>
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IBidirectionalEnumerable<TSelect>>(new _SelectBidirectionalEnumerable<TSelector, TElem, TSelect>(_selector, enumerable));
			}

			template<typename TElem>
			auto build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable) -> std::shared_ptr<IRandomAccessEnumerable<typename unaryreturntype<TSelector, TElem>::type>>
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IRandomAccessEnumerable<TSelect>>(new _SelectRandomAccessEnumerable<TSelector, TElem, TSelect>(_selector, enumerable));
			}
		};
	}
	/*@endcond*/

	/**
	*	Converts collection elements from one form to another.
	*	This function may be used to create collection of elements of one type
	*	from collection of emelents of another type. It is implemented using deffered execution so 
	*	traversing of the source enumerable will wait until its next element will be requested.
	*	@param selector Function used to convert elements of source collection
	*	to elements of result collection. It is common to use lambda expression
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