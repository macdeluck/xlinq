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
*	@file xlinq_distinct.h
*	Selecting unique elements from collection.
*	@author TrolleY
*/
#ifndef XLINQ_DISTINCT_H_
#define XLINQ_DISTINCT_H_

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include "xlinq_base.h"
#include "xlinq_from.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TEnumerable, typename THasher, typename TEqComp>
		std::shared_ptr<IBidirectionalEnumerable<typename TEnumerable::element_type::ElemType>> build_distinct(TEnumerable enumerable, THasher hasher, TEqComp eqComp, int buckets = 8)
		{
			typedef typename TEnumerable::element_type::ElemType TElem;
			typedef std::unordered_set<TElem, THasher, TEqComp> TSet;
			typedef std::list<TElem> TResultList;

			auto res = std::shared_ptr<TResultList>(new TResultList());
			auto set = TSet(buckets, hasher, eqComp);
			for (auto it = enumerable->getEnumerator(); it->next();)
				if (set.insert(it->current()).second)
					res->push_back(it->current());

			return from(res);
		}

		template<typename TEnumerable, typename THasher, typename TEqComp, typename TSelector>
		std::shared_ptr<IBidirectionalEnumerable<typename TEnumerable::element_type::ElemType>>
			build_distinct_by(TEnumerable enumerable, THasher hasher, TEqComp eqComp, TSelector selector, int buckets = 8)
		{
			typedef typename TEnumerable::element_type::ElemType TElem;
			typedef typename unaryreturntype<TSelector, TElem>::type TResult;
			typedef std::unordered_set<TResult, THasher, TEqComp> TSet;
			typedef std::list<TResult> TResultList;

			auto res = std::shared_ptr<TResultList>(new TResultList());
			auto set = TSet(buckets, hasher, eqComp);
			for (auto it = enumerable->getEnumerator(); it->next();)
				if (set.insert(selector(it->current())).second)
					res->push_back(it->current());

			return from(res);
		}

		class _DistinctBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, std::hash<TElem>(), std::equal_to<TElem>());
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, std::hash<TElem>(), std::equal_to<TElem>());
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, std::hash<TElem>(), std::equal_to<TElem>(), enumerable->size());
			}
		};

		template<typename TEqComp>
		class _DistinctWithEqCompBuilder
		{
		private:
			TEqComp _eqComp;
		public:
			_DistinctWithEqCompBuilder(TEqComp eqComp) : _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, std::hash<TElem>(), _eqComp);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, std::hash<TElem>(), _eqComp);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, std::hash<TElem>(), _eqComp, enumerable->size());
			}
		};

		template<typename THasher, typename TEqComp>
		class _DistinctFullBuilder
		{
		private:
			THasher _hasher;
			TEqComp _eqComp;
		public:
			_DistinctFullBuilder(THasher hasher, TEqComp eqComp) : _hasher(hasher), _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, _hasher, _eqComp);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, _hasher, _eqComp);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_distinct(enumerable, _hasher, _eqComp, enumerable->size());
			}
		};

		template<typename TSelector>
		class _DistinctByBuilder
		{
		private:
			TSelector _selector;
		public:
			_DistinctByBuilder(TSelector selector) : _selector(selector) {}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, std::hash<TElem>(), std::equal_to<TElem>(), _selector);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, std::hash<TElem>(), std::equal_to<TElem>(), _selector);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, std::hash<TElem>(), std::equal_to<TElem>(), _selector, enumerable->size());
			}
		};

		template<typename TSelector, typename TEqComp>
		class _DistinctByWithEqCompBuilder
		{
		private:
			TSelector _selector;
			TEqComp _eqComp;
		public:
			_DistinctByWithEqCompBuilder(TSelector selector, TEqComp eqComp) : _selector(selector), _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, std::hash<TElem>(), _eqComp, _selector);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, std::hash<TElem>(), _eqComp, _selector);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, std::hash<TElem>(), _eqComp, _selector, enumerable->size());
			}
		};

		template<typename TSelector, typename THasher, typename TEqComp>
		class _DistinctByFullBuilder
		{
		private:
			TSelector _selector;
			THasher _hasher;
			TEqComp _eqComp;
		public:
			_DistinctByFullBuilder(TSelector selector, THasher hasher, TEqComp eqComp) : _selector(selector), _hasher(hasher), _eqComp(eqComp) {}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, _hasher, _eqComp, _selector);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, _hasher, _eqComp, _selector);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build_distinct_by(enumerable, _hasher, _eqComp, _selector, enumerable->size());
			}
		};
	}
	/*@endcond*/

	/**
	*	Filters unique elements from collection.
	*	This function may be used to create collection with unique set of elements.
	*	Elements should be equality comparable and hashable.
	*	@return Builder of distinct expression.
	*/
	XLINQ_INLINE internal::_DistinctBuilder distinct()
	{
		return internal::_DistinctBuilder();
	}

	/**
	*	Filters unique elements from collection using specified equality comparer.
	*	This function may be used to create collection with unique set of elements.
	*	Elements should be hashable.
	*	@param comparer Functor used to compare elements.
	*	@return Builder of distinct expression.
	*/
	template<typename TComparer>
	XLINQ_INLINE internal::_DistinctWithEqCompBuilder<TComparer> distinct(TComparer comparer)
	{
		return internal::_DistinctWithEqCompBuilder<TComparer>(comparer);
	}

	/**
	*	Filters unique elements from collection using specified equality comparer and hasher.
	*	This function may be used to create collection with unique set of elements.
	*	@param hasher Functor used to hash elements.
	*	@param comparer Functor used to compare elements.
	*	@return Builder of distinct expression.
	*/
	template<typename THasher, typename TComparer>
	XLINQ_INLINE internal::_DistinctFullBuilder<THasher, TComparer> distinct(THasher hasher, TComparer comparer)
	{
		return internal::_DistinctFullBuilder<THasher, TComparer>(hasher, comparer);
	}

	/**
	*	Filters unique elements from collection by given selector.
	*	This function may be used to create collection with unique set of elements
	*	by specified selector means. Elements returned by selector 
	*	should be equality comparable and hashable.
	*	@param selector Function used to extract keys from elements.
	*	@return Builder of distinct_by expression.
	*/
	template<typename TSelector>
	XLINQ_INLINE internal::_DistinctByBuilder<TSelector> distinct_by(TSelector selector)
	{
		return internal::_DistinctByBuilder<TSelector>(selector);
	}

	/**
	*	Filters unique elements from collection by given selector.
	*	This function may be used to create collection with unique set of elements
	*	by specified selector means. Elements returned by selector 
	*	should be hashable.
	*	@param selector Function used to extract keys from elements.
	*	@param comparer Functor used to compare keys.
	*	@return Builder of distinct_by expression.
	*/
	template<typename TSelector, typename TComparer>
	XLINQ_INLINE internal::_DistinctByWithEqCompBuilder<TSelector, TComparer> distinct_by(TSelector selector, TComparer comparer)
	{
		return internal::_DistinctByWithEqCompBuilder<TSelector, TComparer>(selector, comparer);
	}

	/**
	*	Filters unique elements from collection by given selector.
	*	This function may be used to create collection with unique set of elements
	*	by specified selector means. Elements returned by selector 
	*	should be hashable.
	*	@param selector Function used to extract keys from elements.
	*	@param hasher Functor used to hash keys.
	*	@param comparer Functor used to compare keys.
	*	@return Builder of distinct_by expression.
	*/
	template<typename TSelector, typename THasher, typename TComparer>
	XLINQ_INLINE internal::_DistinctByFullBuilder<TSelector, THasher, TComparer> distinct_by(TSelector selector, THasher hasher, TComparer comparer)
	{
		return internal::_DistinctByFullBuilder<TSelector, THasher, TComparer>(selector, hasher, comparer);
	}
}

#endif