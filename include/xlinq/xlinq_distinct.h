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
		template<typename TElem, typename THasher, typename TEqComp>
		class _DistinctEnumerator : public IEnumerator<TElem>
		{
			std::unordered_set<TElem, THasher, TEqComp> _set;
			std::shared_ptr<IEnumerator<TElem>> _source;
		public:
			_DistinctEnumerator(const std::unordered_set<TElem, THasher, TEqComp>& set, std::shared_ptr<IEnumerator<TElem>> source)
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
				auto pother = std::dynamic_pointer_cast<_DistinctEnumerator<TElem, THasher, TEqComp>>(other);
				if (!pother)
					return false;
				return this->_set.size() == pother->_set.size() &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _DistinctEnumerator<TElem, THasher, TEqComp>(this->_set, this->_source->clone()));
			}
		};

		template<typename TElem, typename THasher, typename TEqComp>
		class _DistinctEnumerable : public IEnumerable<TElem>
		{
			const int default_buckets = 8;
			THasher _hasher;
			TEqComp _eqComp;
			std::shared_ptr<IEnumerable<TElem>> _source;
		public:
			_DistinctEnumerable(THasher hasher, TEqComp eqComp, std::shared_ptr<IEnumerable<TElem>> source)
				: _hasher(hasher), _eqComp(eqComp), _source(source) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _DistinctEnumerator<TElem, THasher, TEqComp>(std::unordered_set<TElem, THasher, TEqComp>(default_buckets, _hasher, _eqComp), _source->getEnumerator()));
			}
		};

		class _DistinctBuilder
		{
		public:

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, std::hash<TElem>, std::equal_to<TElem>>(std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, std::hash<TElem>, std::equal_to<TElem>>(std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, std::hash<TElem>, std::equal_to<TElem>>(std::hash<TElem>(), std::equal_to<TElem>(), enumerable));
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
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, std::hash<TElem>, TEqComp>(std::hash<TElem>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, std::hash<TElem>, TEqComp>(std::hash<TElem>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, std::hash<TElem>, TEqComp>(std::hash<TElem>(), _eqComp, enumerable));
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
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, THasher, TEqComp>(_hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, THasher, TEqComp>(_hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctEnumerable<TElem, THasher, TEqComp>(_hasher, _eqComp, enumerable));
			}
		};

		template<typename TSelector, typename TSelect, typename TElem, typename THasher, typename TEqComp>
		class _DistinctByEnumerator : public IEnumerator<TElem>
		{
			TSelector _selector;
			std::unordered_set<TSelect, THasher, TEqComp> _set;
			std::shared_ptr<IEnumerator<TElem>> _source;
		public:
			_DistinctByEnumerator(TSelector selector, const std::unordered_set<TSelect, THasher, TEqComp>& set, std::shared_ptr<IEnumerator<TElem>> source)
				: _selector(selector), _set(set), _source(source) {}

			bool next() override
			{
				while (_source->next())
					if (_set.insert(_selector(_source->current())).second)
						return true;
				return false;
			}

			TElem current() override
			{
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_DistinctByEnumerator<TSelector, TSelect, TElem, THasher, TEqComp>>(other);
				if (!pother)
					return false;
				return this->_set.size() == pother->_set.size() &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _DistinctByEnumerator<TSelector, TSelect, TElem, THasher, TEqComp>(this->_selector, this->_set, this->_source->clone()));
			}
		};

		template<typename TSelector, typename TSelect, typename TElem, typename THasher, typename TEqComp>
		class _DistinctByEnumerable : public IEnumerable<TElem>
		{
			const int default_buckets = 8;
			TSelector _selector;
			THasher _hasher;
			TEqComp _eqComp;
			std::shared_ptr<IEnumerable<TElem>> _source;
		public:
			_DistinctByEnumerable(TSelector selector, THasher hasher, TEqComp eqComp, std::shared_ptr<IEnumerable<TElem>> source)
				: _selector(selector), _hasher(hasher), _eqComp(eqComp), _source(source) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _DistinctByEnumerator<TSelector, TSelect, TElem, THasher, TEqComp>(_selector, std::unordered_set<TSelect, THasher, TEqComp>(default_buckets, _hasher, _eqComp), _source->getEnumerator()));
			}
		};

		template<typename TSelector>
		class _DistinctByBuilder
		{
			TSelector _selector;
		public:
			_DistinctByBuilder(TSelector selector) : _selector(selector) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, std::hash<TSelect>, std::equal_to<TSelect>>(_selector, std::hash<TSelect>(), std::equal_to<TSelect>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, std::hash<TSelect>, std::equal_to<TSelect>>(_selector, std::hash<TSelect>(), std::equal_to<TSelect>(), enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, std::hash<TSelect>, std::equal_to<TSelect>>(_selector, std::hash<TSelect>(), std::equal_to<TSelect>(), enumerable));
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
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, std::hash<TSelect>, TEqComp>(_selector, std::hash<TSelect>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, std::hash<TSelect>, TEqComp>(_selector, std::hash<TSelect>(), _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, std::hash<TSelect>, TEqComp>(_selector, std::hash<TSelect>(), _eqComp, enumerable));
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
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, THasher, TEqComp>(_selector, _hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, THasher, TEqComp>(_selector, _hasher, _eqComp, enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				typedef typename unaryreturntype<TSelector, TElem>::type TSelect;
				return std::shared_ptr<IEnumerable<TElem>>(new _DistinctByEnumerable<TSelector, TSelect, TElem, THasher, TEqComp>(_selector, _hasher, _eqComp, enumerable));
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