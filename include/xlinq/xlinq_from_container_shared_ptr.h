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
		class _StlPointerEnumerator : public _StlEnumerator<TIterator, TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end)
				: _StlEnumerator<TIterator, TElem>(begin, end), _container(container) {}
		};
		
		template<typename TContainer, typename TIterator, typename TElem>
		class _StlPointerBidirectionalEnumerator : public _StlBidirectionalEnumerator<TIterator, TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerBidirectionalEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end, bool atBegin = true)
				: _StlBidirectionalEnumerator<TIterator, TElem>(begin, end, atBegin), _container(container) {}
		};
		
		template<typename TContainer, typename TIterator, typename TElem>
		class _StlPointerRandomAccessEnumerator : public _StlRandomAccessEnumerator<TIterator, TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerRandomAccessEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end)
				: _StlRandomAccessEnumerator<TIterator, TElem>(begin, end), _container(container) {}

			_StlPointerRandomAccessEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end, TIterator current)
				: _StlRandomAccessEnumerator<TIterator, TElem>(begin, end, current), _container(container) {}
		};

		template<typename TContainer, typename TElem>
		class _StlPointerEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerEnumerable(std::shared_ptr<TContainer> container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlPointerEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end()));
			}
		};

		template<typename TContainer, typename TElem>
		class _StlPointerBidirectionalEnumerable : public IBidirectionalEnumerable<TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerBidirectionalEnumerable(std::shared_ptr<TContainer> container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlPointerBidirectionalEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end(), true));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _StlPointerBidirectionalEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end(), false));
			}
		};

		template<typename TContainer, typename TElem>
		class _StlPointerRandomAccessEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerRandomAccessEnumerable(std::shared_ptr<TContainer> container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlPointerRandomAccessEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _StlPointerRandomAccessEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end(), _container->end()));
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				auto result = this->getEnumerator();
				result->advance(elementIndex + 1);
				return result;
			}
		};

		template<typename iterator_tag, typename TContainer, typename TElem>
		struct StlPointerEnumerableSelectorHelper
		{
		};

		template<typename TContainer, typename TElem>
		struct StlPointerEnumerableSelectorHelper<std::forward_iterator_tag, TContainer, TElem>
		{
			typedef _StlPointerEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlPointerEnumerableSelectorHelper<std::bidirectional_iterator_tag, TContainer, TElem>
		{
			typedef _StlPointerBidirectionalEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlPointerEnumerableSelectorHelper<std::random_access_iterator_tag, TContainer, TElem>
		{
			typedef _StlPointerRandomAccessEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlPointerEnumerableSelector
		{
			typedef typename std::iterator_traits<typename TContainer::iterator> traits;
			typedef typename StlPointerEnumerableSelectorHelper<typename traits::iterator_category, TContainer, typename TContainer::value_type>::enumerable type;
		};
	}
	/*@endcond*/

	/**
	*	Creates enumerable from shared pointer to STL container.
	*	This function may be used to create enumerable from pointer to STL container.
	*	It is implemented using deffered execution so traversing of the collection
	*	will wait until its next element will be requested.
	*	@param container Source STL container.
	*	@return Enumerable from container.
	*/
	template<typename TContainer>
	auto from(std::shared_ptr<TContainer> container) -> std::shared_ptr<typename internal::StlPointerEnumerableSelector<TContainer, typename TContainer::value_type>::type>
	{
		return std::shared_ptr<typename internal::StlPointerEnumerableSelector<TContainer, typename TContainer::value_type>::type>(new typename internal::StlPointerEnumerableSelector<TContainer, typename TContainer::value_type>::type(container));
	}
}

#endif