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
*	@file xlinq_from_container_shared_ptr.h
*	Creating enumerable object from containers passed by shared ptr.
*	@author TrolleY
*/
#ifndef XLINQ_FROM_CONTAINER_SHARED_PTR_H_
#define XLINQ_FROM_CONTAINER_SHARED_PTR_H_

#include <cstdlib>
#include <memory>
#include <iterator>
#include "xlinq_base.h"
#include "xlinq_exception.h"
#include "xlinq_from_container.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TContainer, typename TIterator, typename TElem>
		class _StlSharedPointerEnumerator : public _StlEnumerator<TIterator, TElem>
		{
		protected:
			std::shared_ptr<TContainer> _container;
		public:
			_StlSharedPointerEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end)
				: _StlEnumerator<TIterator, TElem>(begin, end), _container(container) {}
			
			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_StlSharedPointerEnumerator<TContainer, TIterator, TElem>>(other);
				if (!pother) return false;
				return pother->_container == this->_container &&
					pother->_begin == this->_begin &&
					pother->_end == this->_end &&
					pother->_started == this->_started;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _StlSharedPointerEnumerator<TContainer, TIterator, TElem>(this->_container, this->_begin, this->_end);
				ptr->_started = this->_started;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};
		
		template<typename TContainer, typename TIterator, typename TElem>
		class _StlSharedPointerBidirectionalEnumerator : public _StlBidirectionalEnumerator<TIterator, TElem>
		{
		protected:
			std::shared_ptr<TContainer> _container;
		public:
			_StlSharedPointerBidirectionalEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end, bool atBegin = true)
				: _StlBidirectionalEnumerator<TIterator, TElem>(begin, end, atBegin), _container(container) {}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_StlSharedPointerBidirectionalEnumerator<TContainer, TIterator, TElem>>(other);
				if (!pother) return false;
				return pother->_container == this->_container &&
					pother->_begin == this->_begin &&
					pother->_current == this->_current &&
					pother->_end == this->_end &&
					pother->_started == this->_started;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _StlSharedPointerBidirectionalEnumerator<TContainer, TIterator, TElem>(this->_container, this->_begin, this->_end);
				ptr->_current = this->_current;
				ptr->_started = this->_started;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};
		
		template<typename TContainer, typename TIterator, typename TElem>
		class _StlSharedPointerRandomAccessEnumerator : public _StlRandomAccessEnumerator<TIterator, TElem>
		{
		protected:
			std::shared_ptr<TContainer> _container;
		public:
			_StlSharedPointerRandomAccessEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end)
				: _StlRandomAccessEnumerator<TIterator, TElem>(begin, end), _container(container) {}

			_StlSharedPointerRandomAccessEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end, TIterator current)
				: _StlRandomAccessEnumerator<TIterator, TElem>(begin, end, current), _container(container) {}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_StlSharedPointerRandomAccessEnumerator<TContainer, TIterator, TElem>>(other);
				if (!pother) return false;
				return pother->_container == this->_container &&
					pother->_begin == this->_begin &&
					pother->_current == this->_current &&
					pother->_end == this->_end &&
					pother->_started == this->_started;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _StlSharedPointerRandomAccessEnumerator<TContainer, TIterator, TElem>(this->_container, this->_begin, this->_end, this->_current);
				ptr->_started = this->_started;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};

		template<typename TContainer, typename TElem>
		class _StlSharedPointerEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlSharedPointerEnumerable(std::shared_ptr<TContainer> container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlSharedPointerEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end()));
			}
		};

		template<typename TContainer, typename TElem>
		class _StlSharedPointerBidirectionalEnumerable : public IBidirectionalEnumerable<TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlSharedPointerBidirectionalEnumerable(std::shared_ptr<TContainer> container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlSharedPointerBidirectionalEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end(), true));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _StlSharedPointerBidirectionalEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end(), false));
			}
		};

		template<typename TContainer, typename TElem>
		class _StlSharedPointerRandomAccessEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlSharedPointerRandomAccessEnumerable(std::shared_ptr<TContainer> container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlSharedPointerRandomAccessEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _StlSharedPointerRandomAccessEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end(), _container->end()));
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				auto result = this->getEnumerator();
				result->advance(elementIndex + 1);
				return result;
			}

			int size() override
			{
				return (int)_container->size();
			}
		};

		template<typename iterator_tag, typename TContainer, typename TElem>
		struct StlSharedPointerEnumerableSelectorHelper
		{
		};

		template<typename TContainer, typename TElem>
		struct StlSharedPointerEnumerableSelectorHelper<std::forward_iterator_tag, TContainer, TElem>
		{
			typedef _StlSharedPointerEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlSharedPointerEnumerableSelectorHelper<std::bidirectional_iterator_tag, TContainer, TElem>
		{
			typedef _StlSharedPointerBidirectionalEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlSharedPointerEnumerableSelectorHelper<std::random_access_iterator_tag, TContainer, TElem>
		{
			typedef _StlSharedPointerRandomAccessEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlSharedPointerEnumerableSelector
		{
			typedef typename std::iterator_traits<typename TContainer::iterator> traits;
			typedef typename StlSharedPointerEnumerableSelectorHelper<typename traits::iterator_category, TContainer, typename TContainer::value_type>::enumerable type;
		};
	}
	/*@endcond*/

	/**
	*	Creates enumerable from shared pointer to STL container.
	*	This function may be used to create enumerable from shared pointer to STL container.
	*	It is implemented using deffered execution so traversing of the collection
	*	will wait until its next element will be requested.
	*	@param container Source STL container.
	*	@return Enumerable from container.
	*/
	template<typename TContainer>
	auto from(std::shared_ptr<TContainer> container) -> std::shared_ptr<typename internal::EnumerableTypeSelector<typename internal::StlSharedPointerEnumerableSelector<TContainer, typename TContainer::value_type>::type>::type>
	{
		return std::shared_ptr<typename internal::EnumerableTypeSelector<typename internal::StlSharedPointerEnumerableSelector<TContainer, typename TContainer::value_type>::type>::type>(new typename internal::StlSharedPointerEnumerableSelector<TContainer, typename TContainer::value_type>::type(container));
	}
}

#endif