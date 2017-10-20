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
*	@file xlinq_join.h
*	Correlating elements of two collection.
*	@author TrolleY
*/
#ifndef XLINQ_JOIN_H_
#define XLINQ_JOIN_H_

#include "xlinq_group_by.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TInner, typename TOuter, typename TKeySelector, typename TOuterKeySelector, typename TResultSelector>
		class JoinTypeSelector
		{
		public:
			typedef decltype(from(std::declval<typename std::add_lvalue_reference<TInner>::type>())) InnerCollection;
			typedef typename InnerCollection::element_type::ElemType InnerElemType;
			typedef decltype(from(std::declval<typename std::add_lvalue_reference<TOuter>::type>())) OuterCollection;
			typedef typename OuterCollection::element_type::ElemType OuterElemType;
			typedef typename unaryreturntype<TKeySelector, InnerElemType>::type InnerKeyType;
			typedef typename unaryreturntype<TOuterKeySelector, OuterElemType>::type OuterKeyType;
			typedef typename binaryreturntype<TResultSelector, InnerElemType, OuterElemType>::type TResult;
		};

		template<typename TOuter, typename TOuterKeySelector, typename TInnerKey, typename TOuterKey, typename TInnerElem, typename TOuterElem, typename TResultSelector, typename TResult>
		class JoinLookup
		{
		private:
			TOuter _outer;
			std::shared_ptr<IEnumerator<std::shared_ptr<IGrouping<TInnerKey, TInnerElem>>>> _innerGroupEnumerator;
			std::shared_ptr<IEnumerator<std::shared_ptr<IGrouping<TOuterKey, TOuterElem>>>> _outerGroupEnumerator;
			std::unordered_map<TOuterKey, std::shared_ptr<IEnumerable<TOuterElem>>> _outerSkipped;
			TResultSelector _resultSelector;
			std::list<TResult> _result;

		public:
			JoinLookup(std::shared_ptr<IEnumerator<std::shared_ptr<IGrouping<TInnerKey, TInnerElem>>>> innerGroupEnumerator,
				TOuter outer, TOuterKeySelector outerKeySelector, TResultSelector resultSelector)
				: _outer(outer), _innerGroupEnumerator(innerGroupEnumerator), 
				_outerGroupEnumerator(from(_outer) >> group_by(outerKeySelector) >> getEnumerator()), _outerSkipped(std::unordered_map<TOuterKey, std::shared_ptr<IEnumerable<TOuterElem>>>()), _resultSelector(resultSelector), _result(std::list<TResult>())
			{
			}

			bool finished()
			{
				return !_innerGroupEnumerator && !_outerGroupEnumerator;
			}

			bool next()
			{
				if (finished()) return false;
				while (_innerGroupEnumerator && _innerGroupEnumerator->next())
				{
					auto current = _innerGroupEnumerator->current();
					auto outerCurrent = _outerSkipped.find(current->getKey());
					if (outerCurrent != _outerSkipped.end())
					{
						for (auto inIt = current->getEnumerator(); inIt->next();)
						{
							for (auto it = (*outerCurrent).second->getEnumerator(); it->next();)
							{
								_result.push_back(_resultSelector(inIt->current(), it->current()));
							}
						}
						_outerSkipped.erase(current->getKey());
						return true;
					}
					else
					{
						while (_outerGroupEnumerator && _outerGroupEnumerator->next())
						{
							if (current->getKey() == _outerGroupEnumerator->current()->getKey())
							{
								for (auto inIt = current->getEnumerator(); inIt->next();)
								{
									for (auto it = _outerGroupEnumerator->current()->getEnumerator(); it->next();)
									{
										_result.push_back(_resultSelector(inIt->current(), it->current()));
									}
								}
								return true;
							}
							else
							{
								_outerSkipped.insert({ _outerGroupEnumerator->current()->getKey(), _outerGroupEnumerator->current() });
							}
						}
						_outerGroupEnumerator = nullptr;
					}
				}
				_innerGroupEnumerator = nullptr;
				_outerSkipped.clear();
				return false;
			}

			typename std::list<TResult>::const_iterator begin() { return _result.begin(); }

			typename std::list<TResult>::const_iterator end() { return _result.end(); }
		};

		template<typename TOuter, typename TKeySelector, typename TOuterKeySelector, typename TResultSelector, typename TInnerKey, typename TOuterKey, typename TInnerElem, typename TOuterElem, typename TResult>
		class _JoinEnumerator : public IEnumerator<TResult>
		{
		private:
			std::shared_ptr<JoinLookup<TOuter, TOuterKeySelector, TInnerKey, TOuterKey, TInnerElem, TOuterElem, TResultSelector, TResult>> _lookup;
			typename std::list<TResult>::const_iterator _it;
			bool _started;
			bool _finished;

		public:
			_JoinEnumerator(std::shared_ptr<JoinLookup<TOuter, TOuterKeySelector, TInnerKey, TOuterKey, TInnerElem, TOuterElem, TResultSelector, TResult>> lookup)
				: _lookup(lookup), _it(lookup->begin()), _started(false), _finished(false) {}

			bool next()
			{
				if (_finished) throw IterationFinishedException();
				auto itn = _it;
				auto ite = _lookup->end();
				if ((_it == ite) || (_started && (++itn == ite)))
				{
					if (!_lookup->finished() && _lookup->next())
					{
						if (!_started)
							_it = _lookup->begin();
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

			TResult current()
			{
				if (_finished) throw IterationFinishedException();
				if (!_started) throw IterationNotStartedException();
				return *_it;
			}
		};

		template<typename TOuter, typename TKeySelector, typename TOuterKeySelector, typename TResultSelector, typename TInnerKey, typename TOuterKey, typename TInnerElem, typename TOuterElem, typename TResult>
		class _JoinEnumerable : public IEnumerable<TResult>
		{
		private:
			std::shared_ptr<JoinLookup<TOuter, TOuterKeySelector, TInnerKey, TOuterKey, TInnerElem, TOuterElem, TResultSelector, TResult>> _lookup;

		public:
			_JoinEnumerable(std::shared_ptr<IEnumerable<TInnerElem>> inner, TOuter outer, TKeySelector keySelector, TOuterKeySelector outerKeySelector, TResultSelector resultSelector)
				: _lookup(new JoinLookup<TOuter, TOuterKeySelector, TInnerKey, TOuterKey, TInnerElem, TOuterElem, TResultSelector, TResult>(
					(inner >> group_by(keySelector))->getEnumerator(),
					outer,
					outerKeySelector,
					resultSelector))
			{}

			std::shared_ptr<IEnumerator<TResult>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TResult>>(new _JoinEnumerator<TOuter, TKeySelector, TOuterKeySelector, TResultSelector, TInnerKey, TOuterKey, TInnerElem, TOuterElem, TResult>(_lookup));
			}
		};

		template<typename TOuter, typename TKeySelector, typename TOuterKeySelector, typename TResultSelector>
		class _JoinBuilder
		{
		private:
			TOuter _outer;
			TKeySelector _keySelector;
			TOuterKeySelector _outerKeySelector;
			TResultSelector _resultSelector;
		public:
			_JoinBuilder(TOuter outer, TKeySelector keySelector, TOuterKeySelector outerKeySelector, TResultSelector resultSelector)
				: _outer(outer), _keySelector(keySelector), _outerKeySelector(outerKeySelector), _resultSelector(resultSelector)
			{}

			template<typename TElem>
			auto build(std::shared_ptr<IEnumerable<TElem>> inner) -> 
				std::shared_ptr<IEnumerable<typename JoinTypeSelector<decltype(inner), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::TResult>>
			{
				typedef typename JoinTypeSelector<decltype(inner), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::InnerKeyType TInnerKey;
				typedef typename JoinTypeSelector<decltype(inner), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::OuterKeyType TOuterKey;
				typedef typename JoinTypeSelector<decltype(inner), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::InnerElemType TInnerElem;
				typedef typename JoinTypeSelector<decltype(inner), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::OuterElemType TOuterElem;
				typedef typename JoinTypeSelector<decltype(inner), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::TResult TResult;
				return std::shared_ptr<IEnumerable<typename JoinTypeSelector<std::shared_ptr<IEnumerable<TElem>>, TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::TResult>>
					(new _JoinEnumerable<TOuter, TKeySelector, TOuterKeySelector, TResultSelector, TInnerKey, TOuterKey, TInnerElem, TOuterElem, TResult>(inner, _outer, _keySelector, _outerKeySelector, _resultSelector));
			}

			template<typename TElem>
			auto build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable) ->
				std::shared_ptr<IEnumerable<typename JoinTypeSelector<decltype(enumerable), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::TResult>>
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			auto build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable) ->
				std::shared_ptr<IEnumerable<typename JoinTypeSelector<decltype(enumerable), TOuter, TKeySelector, TOuterKeySelector, TResultSelector>::TResult>>
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};
	}
	/*@endcond*/

	/**
	*	Correlates elements of two collections by common key.
	*	This function may be used to correlate elements of two collection
	*	when theirs' keys match. Specified functions are used to extract
	*	keys from collection elements.
	*	@return Builder of join expression.
	*/
	template<typename TOuter, typename TKeySelector, typename TOuterKeySelector, typename TResultSelector>
	XLINQ_INLINE internal::_JoinBuilder<TOuter, TKeySelector, TOuterKeySelector, TResultSelector> join(TOuter outer, TKeySelector keySelector, TOuterKeySelector outerKeySelector, TResultSelector resultSelector)
	{
		return internal::_JoinBuilder<TOuter, TKeySelector, TOuterKeySelector, TResultSelector>(outer, keySelector, outerKeySelector, resultSelector);
	}
}

#endif