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
*	@file xlinq_except.h
*	Selecting unique elements from collection.
*	@author TrolleY
*/
#ifndef XLINQ_EXCEPT_H_
#define XLINQ_EXCEPT_H_

#include <memory>
#include <unordered_set>
#include "xlinq_base.h"
#include "xlinq_from.h"
#include "xlinq_to_container.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem, typename THasher, typename TEqComp>
		class _ExceptEnumerator : public IEnumerator<TElem>
		{
			std::unordered_set<TElem, THasher, TEqComp> _set;
			std::shared_ptr<IEnumerator<TElem>> _source;
		public:
			_ExceptEnumerator(const std::unordered_set<TElem, THasher, TEqComp>& set, std::shared_ptr<IEnumerator<TElem>> source)
				: _set(set), _source(source) {}

			bool next() override
			{
				while (_source->next())
					if (_set.insert(_source->current()).second)
						return true;
				return false;
			}

			TElem current() override
			{
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_ExceptEnumerator<TElem, THasher, TEqComp>>(other);
				if (!pother)
					return false;
				return this->_set.size() == pother->_set.size() &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ExceptEnumerator<TElem, THasher, TEqComp>(this->_set, this->_source->clone()));
			}
		};

		template<typename TContainer, typename TElem, typename THasher, typename TEqComp>
		class _ExceptEnumerable : public IEnumerable<TElem>
		{
		private:
			std::unordered_set<TElem, THasher, TEqComp> _set;
			std::shared_ptr<IEnumerable<TElem>> _source;
		public:
			_ExceptEnumerable(TContainer container, THasher hasher, TEqComp eqComp, std::shared_ptr<IEnumerable<TElem>> source)
				: _set(from(container) >> to_unordered_set(hasher, eqComp)), _source(source) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ExceptEnumerator<TElem, THasher, TEqComp>(_set, _source->getEnumerator()));
			}
		};

		template<typename TContainer>
		class _ExceptBuilder
		{
			TContainer _container;

		public:
			_ExceptBuilder(TContainer container) : _container(container) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, std::hash<TElem>, std::equal_to<TElem>>(_container, std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, std::hash<TElem>, std::equal_to<TElem>>(_container, std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, std::hash<TElem>, std::equal_to<TElem>>(_container, std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}
		};

		template<typename TContainer, typename TEqComp>
		class _ExceptBuilderWithEqCompBuilder
		{
		private:
			TContainer _container;
			TEqComp _eqComp;

		public:
			_ExceptBuilderWithEqCompBuilder(TContainer container, TEqComp eqComp) : _container(container), _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, std::hash<TElem>, TEqComp>(_container, std::hash<TElem>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, std::hash<TElem>, TEqComp>(_container, std::hash<TElem>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, std::hash<TElem>, TEqComp>(_container, std::hash<TElem>(), _eqComp, enumerable));
			}
		};

		template<typename TContainer, typename THasher, typename TEqComp>
		class _ExceptBuilderFullBuilder
		{
		private:
			TContainer _container;
			THasher _hasher;
			TEqComp _eqComp;
		public:
			_ExceptBuilderFullBuilder(TContainer container, THasher hasher, TEqComp eqComp) : _container(container), _hasher(hasher), _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, THasher, TEqComp>(_container, _hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, THasher, TEqComp>(_container, _hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _ExceptEnumerable<TContainer, TElem, THasher, TEqComp>(_container, _hasher, _eqComp, enumerable));
			}
		};
	}
	/*@endcond*/

	/**
	*	Filters elements skipping those which were passed in container.
	*	This function may be used to create collection with unique set of elements.
	*	Elements should be equality comparable and hashable.
	*	@param container Container with elements to skip.
	*	@return Builder of except expression.
	*/
	template<typename TContainer>
	XLINQ_INLINE internal::_ExceptBuilder<TContainer> except(TContainer container)
	{
		return internal::_ExceptBuilder<TContainer>(container);
	}

	/**
	*	Filters elements skipping those which were passed in container using specified equality comparer.
	*	This function may be used to create collection with unique set of elements.
	*	Elements should be hashable.
	*	@param container Container with elements to skip.
	*	@param comparer Functor used to compare elements.
	*	@return Builder of except expression.
	*/
	template<typename TContainer, typename TComparer>
	XLINQ_INLINE internal::_ExceptBuilderWithEqCompBuilder<TContainer, TComparer> except(TContainer container, TComparer comparer)
	{
		return internal::_ExceptBuilderWithEqCompBuilder<TContainer, TComparer>(container, comparer);
	}

	/**
	*	Filters elements skipping those which were passed in container using specified equality comparer and hasher.
	*	This function may be used to create collection with unique set of elements.
	*	@param container Container with elements to skip.
	*	@param hasher Functor used to hash elements.
	*	@param comparer Functor used to compare elements.
	*	@return Builder of except expression.
	*/
	template<typename TContainer, typename THasher, typename TComparer>
	XLINQ_INLINE internal::_ExceptBuilderFullBuilder<TContainer, THasher, TComparer> except(TContainer container, THasher hasher, TComparer comparer)
	{
		return internal::_ExceptBuilderFullBuilder<TContainer, THasher, TComparer>(container, hasher, comparer);
	}
}

#endif