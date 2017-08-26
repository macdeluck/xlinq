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
*	@file xlinq_groupby.h
*	Grouping collection ellements.
*	@author TrolleY
*/
#ifndef XLINQ_GROUPBY_H_
#define XLINQ_GROUPBY_H_

#include "xlinq_base.h"
#include "xlinq_lookup.h"
#include "xlinq_exception.h"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <list>
#include <utility>

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TKeySelector, typename TKey, typename TElem, typename THasher, typename TEqComp>
		class GroupingEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>> _lookup;
			TKey _key;
			typename std::list<TElem>::const_iterator _it;
			bool _started;
			bool _finished;
		public:
			GroupingEnumerator(std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>> lookup, TKey key, typename std::list<TElem>::const_iterator it)
				: _lookup(lookup), _key(key), _it(it), _started(false), _finished(false) {}

			bool next() override
			{
				if (_finished) throw IterationFinishedException();

				TEqComp comparer = _lookup->getComparer();
				TKey key;
				auto itn = _it;
				auto ite = _lookup->end(_key);
				if ((_it == ite) || (_started && (++itn == ite)))
				{
					while (!_lookup->finished() && _lookup->next(false, &key))
					{
						if (comparer(_key, key))
						{
							if (!_started)
								_it == _lookup->begin(_key);
							else ++_it;
							_started = true;
							return true;
						}
					}
					_it = ite;
					_started = true;
					_finished = true;
					return false;
				}
				else if (_started)
					++_it;
				_started = true;
				return true;
			}

			TElem current() override
			{
				if (_finished) throw IterationFinishedException();
				if (!_started) throw IterationNotStartedException();
				return *_it;
			}
		};

		template<typename TKeySelector, typename TKey, typename TElem, typename THasher, typename TEqComp>
		class Grouping : public IGrouping<TKey, TElem>
		{
		private:
			std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>> _lookup;
			TKey _key;
		public:
			Grouping(std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>> lookup, TKey key)
				: _lookup(lookup), _key(key) {}

			virtual TKey getKey() const override { return _key; }

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new GroupingEnumerator<TKeySelector, TKey, TElem, THasher, TEqComp>(_lookup, _key, _lookup->begin(_key)));
			}
		};

		template<typename TKeySelector, typename TKey, typename TElem, typename THasher, typename TEqComp>
		class GroupsEnumerator : public IEnumerator<std::shared_ptr<IGrouping<TKey, TElem>>>
		{
			std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>> _lookup;
			typename std::list<TKey>::const_iterator _it;
			bool _started;
			bool _finished;

		public:
			GroupsEnumerator(std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>> lookup)
				: _lookup(lookup), _it(lookup->beginKeyIterator()), _started(false), _finished(false) {}

			bool next() override
			{
				if (_finished) throw IterationFinishedException();
				auto itn = _it;
				auto ite = _lookup->endKeyIterator();
				if ((_it == ite) || (_started && (++itn == ite)))
				{
					if (!_lookup->finished() && _lookup->next())
					{
						if (!_started)
							_it = _lookup->beginKeyIterator();
						else ++_it;
						_started = true;
						return true;
					}
					_finished = true;
					_started = true;
					return false;
				}
				else if (_started)
					++_it;
				_started = true;
				return true;
			}

			std::shared_ptr<IGrouping<TKey, TElem>> current() override
			{
				if (_finished) throw IterationFinishedException();
				if (!_started) throw IterationNotStartedException();
				return std::shared_ptr<IGrouping<TKey, TElem>>(new Grouping<TKeySelector, TKey, TElem, THasher, TEqComp>(_lookup, *_it));
			}
		};

		template<typename TKeySelector, typename TKey, typename TElem, typename THasher, typename TEqComp>
		class _GroupByEnumerable : public IEnumerable<std::shared_ptr<IGrouping<TKey, TElem>>>
		{
		private:
			std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>> _lookup;
		public:
			_GroupByEnumerable(std::shared_ptr<IEnumerable<TElem>> sourceEnumerable, TKeySelector keySelector, THasher hasher, TEqComp eqComp)
				: _lookup(std::shared_ptr<Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>>(new Lookup<TKeySelector, TKey, TElem, THasher, TEqComp>(sourceEnumerable->getEnumerator(), keySelector, hasher, eqComp))) {}

			std::shared_ptr<IEnumerator<std::shared_ptr<IGrouping<TKey, TElem>>>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<std::shared_ptr<IGrouping<TKey, TElem>>>>(new GroupsEnumerator<TKeySelector, TKey, TElem, THasher, TEqComp>(_lookup));
			}
		};

		template<typename TKeySelector>
		class _GroupByBuilder
		{
			TKeySelector _keySelector;
		public:
			_GroupByBuilder(TKeySelector keySelector)
				: _keySelector(keySelector) {}

			template<typename TElem>
			auto build(std::shared_ptr<IEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>
			{
				typedef typename unaryreturntype<TKeySelector, TElem>::type TKey;
				return std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>(
					new _GroupByEnumerable<TKeySelector, TKey, TElem, std::hash<TKey>, std::equal_to<TKey>>(enumerable, _keySelector, std::hash<TKey>(), std::equal_to<TKey>()));
			}

			template<typename TElem>
			auto build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			auto build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};

		template<typename TKeySelector, typename THasher, typename TEqComp>
		class _GroupByBuilderWithHashAndEqComp
		{
			TKeySelector _keySelector;
			THasher _hasher;
			TEqComp _eqComp;
		public:
			_GroupByBuilderWithHashAndEqComp(TKeySelector keySelector, THasher hasher, TEqComp eqComp)
				: _keySelector(keySelector), _hasher(hasher), _eqComp(eqComp) {}

			template<typename TElem>
			auto build(std::shared_ptr<IEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>
			{
				typedef typename unaryreturntype<TKeySelector, TElem>::type TKey;
				return std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>(
					new _GroupByEnumerable<TKeySelector, TKey, TElem, THasher, TEqComp>(enumerable, _keySelector, _hasher, _eqComp));
			}

			template<typename TElem>
			auto build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			auto build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable) -> std::shared_ptr<IEnumerable<std::shared_ptr<IGrouping<typename unaryreturntype<TKeySelector, TElem>::type, TElem>>>>
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};
	}
	/*@endcond*/

	/**
	*	Groups elements by common key.
	*	This function may be used to group elements by common key which
	*	is extracted from them by keySelector provided by user.
	*	Key must have defined std::hash and std::equal_to type spetialization.
	*	@return Builder of groupby expression.
	*/
	template<typename TKeySelector>
	XLINQ_INLINE internal::_GroupByBuilder<TKeySelector> groupby(TKeySelector keySelector)
	{
		return internal::_GroupByBuilder<TKeySelector>(keySelector);
	}

	/**
	*	Groups elements by common key with specified key hasher and equality comparer.
	*	This function may be used to group elements by common key which
	*	is extracted from them by keySelector provided by user.
	*	Key must be hashable by specified hash function and comparable with specified
	*	equality comparer. See std::hash and std::equal_to for details.
	*	@return Builder of groupby expression.
	*/
	template<typename TKeySelector, typename THasher, typename TEqComp>
	XLINQ_INLINE internal::_GroupByBuilderWithHashAndEqComp<TKeySelector, THasher, TEqComp> groupby(TKeySelector keySelector, THasher hasher, TEqComp eqComp)
	{
		return internal::_GroupByBuilderWithHashAndEqComp<TKeySelector, THasher, TEqComp>(keySelector, hasher, eqComp);
	}
}

#endif