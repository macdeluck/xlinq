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
*	@file xlinq_union.h
*	Selecting unique elements that are in both of given collection.
*	@author TrolleY
*/
#ifndef XLINQ_UNION_H_
#define XLINQ_UNION_H_

#include <memory>
#include <unordered_set>
#include "xlinq_base.h"
#include "xlinq_from.h"
#include "xlinq_concat.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TContainer>
		struct UnionContainerHolder
		{
			TContainer container;
		};

		template<typename TContainer, typename TElem, typename THasher, typename TEqComp>
		class _UnionEnumerator : public IEnumerator<TElem>
		{
			const int default_buckets = 8;
			std::shared_ptr<UnionContainerHolder<TContainer>> _holder; // just to prevent container from dealloc
			std::unordered_set<TElem, THasher, TEqComp> _set;
			std::shared_ptr<IEnumerator<TElem>> _source;

			_UnionEnumerator(std::shared_ptr<UnionContainerHolder<TContainer>> holder, const std::unordered_set<TElem, THasher, TEqComp>& set, std::shared_ptr<IEnumerator<TElem>> source)
				: _holder(holder), _set(set), _source(source) {}
		public:
			_UnionEnumerator(std::shared_ptr<UnionContainerHolder<TContainer>> holder, THasher hasher, TEqComp eqComp, std::shared_ptr<IEnumerator<TElem>> source)
				: _holder(holder), _set(default_buckets, hasher, eqComp), _source(source) {}

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
				auto pother = std::dynamic_pointer_cast<_UnionEnumerator<TContainer, TElem, THasher, TEqComp>>(other);
				if (!pother)
					return false;
				return this->_set.size() == pother->_set.size() &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _UnionEnumerator<TContainer, TElem, THasher, TEqComp>(this->_holder, this->_set, this->_source->clone()));
			}
		};

		template<typename TContainer, typename TElem, typename THasher, typename TEqComp>
		class _UnionEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<UnionContainerHolder<TContainer>> _holder;
			THasher _hasher;
			TEqComp _eqComp;
			std::shared_ptr<IEnumerable<TElem>> _source;
		public:
			_UnionEnumerable(TContainer container, THasher hasher, TEqComp eqComp, std::shared_ptr<IEnumerable<TElem>> source)
				: _holder(new UnionContainerHolder<TContainer>{ container }), _hasher(hasher), _eqComp(eqComp), _source(source) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _UnionEnumerator<TContainer, TElem, THasher, TEqComp>(_holder, _hasher, _eqComp, (_source >> concat(_holder->container))->getEnumerator()));
			}
		};

		template<typename TContainer>
		class _UnionBuilder
		{
			TContainer _container;

		public:
			_UnionBuilder(TContainer container) : _container(container) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, std::hash<TElem>, std::equal_to<TElem>>(_container, std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, std::hash<TElem>, std::equal_to<TElem>>(_container, std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, std::hash<TElem>, std::equal_to<TElem>>(_container, std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}
		};

		template<typename TContainer, typename TEqComp>
		class _UnionBuilderWithEqCompBuilder
		{
		private:
			TContainer _container;
			TEqComp _eqComp;

		public:
			_UnionBuilderWithEqCompBuilder(TContainer container, TEqComp eqComp) : _container(container), _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, std::hash<TElem>, TEqComp>(_container, std::hash<TElem>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, std::hash<TElem>, TEqComp>(_container, std::hash<TElem>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, std::hash<TElem>, TEqComp>(_container, std::hash<TElem>(), _eqComp, enumerable));
			}
		};

		template<typename TContainer, typename THasher, typename TEqComp>
		class _UnionFullBuilder
		{
		private:
			TContainer _container;
			THasher _hasher;
			TEqComp _eqComp;
		public:
			_UnionFullBuilder(TContainer container, THasher hasher, TEqComp eqComp) : _container(container), _hasher(hasher), _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, THasher, TEqComp>(_container, _hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, THasher, TEqComp>(_container, _hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _UnionEnumerable<TContainer, TElem, THasher, TEqComp>(_container, _hasher, _eqComp, enumerable));
			}
		};
	}
	/*@endcond*/

	/**
	*	Filters elements skipping those which were not passed in container.
	*	This function may be used to create collection with unique set of elements.
	*	Elements should be equality comparable and hashable.
	*	@param container Container with elements to skip.
	*	@return Builder of union_with expression.
	*/
	template<typename TContainer>
	XLINQ_INLINE internal::_UnionBuilder<TContainer> union_with(TContainer container)
	{
		return internal::_UnionBuilder<TContainer>(container);
	}

	/**
	*	Filters elements skipping those which were not passed in container using specified equality comparer.
	*	This function may be used to create collection with unique set of elements.
	*	Elements should be hashable.
	*	@param container Container with elements to skip.
	*	@param comparer Functor used to compare elements.
	*	@return Builder of union_with expression.
	*/
	template<typename TContainer, typename TComparer>
	XLINQ_INLINE internal::_UnionBuilderWithEqCompBuilder<TContainer, TComparer> union_with(TContainer container, TComparer comparer)
	{
		return internal::_UnionBuilderWithEqCompBuilder<TContainer, TComparer>(container, comparer);
	}

	/**
	*	Filters elements skipping those which were not passed in container using specified equality comparer and hasher.
	*	This function may be used to create collection with unique set of elements.
	*	@param container Container with elements to skip.
	*	@param hasher Functor used to hash elements.
	*	@param comparer Functor used to compare elements.
	*	@return Builder of union_with expression.
	*/
	template<typename TContainer, typename THasher, typename TComparer>
	XLINQ_INLINE internal::_UnionFullBuilder<TContainer, THasher, TComparer> union_with(TContainer container, THasher hasher, TComparer comparer)
	{
		return internal::_UnionFullBuilder<TContainer, THasher, TComparer>(container, hasher, comparer);
	}
}

#endif