#ifndef XLINQ_JOIN_H_
#define XLINQ_JOIN_H_

#include "xlinq_base.h"
#include <map>

namespace xlinq
{
	template<typename TElem, typename TOtherElem, typename TResultElem, typename TKey, typename TElemKeySelector, typename TOtherKeySelector, typename TJoinSelector>
	class _JoinEnumerable : public IEnumerable<TResultElem>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _enumerable;
		std::shared_ptr<IEnumerable<TOtherElem>> _otherEnumerable;
		TElemKeySelector _elemKeySelector;
		TOtherKeySelector _otherKeySelector;
		TJoinSelector _joinSelector;

	public:
		_JoinEnumerable(
			std::shared_ptr<IEnumerable<TElem>> enumerable,
			std::shared_ptr<IEnumerable<TOtherElem>> otherEnumerable,
			TElemKeySelector elemKeySelector,
			TOtherKeySelector otherKeySelector,
			TJoinSelector joinSelector)
			: _enumerable(enumerable),
			_otherEnumerable(otherEnumerable),
			_elemKeySelector(elemKeySelector),
			_otherKeySelector(otherKeySelector),
			_joinSelector(joinSelector) {}

		class _JoinEnumerator : public IEnumerator<TResultElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			std::shared_ptr<IEnumerator<TOtherElem>> _otherEnumerator;
			TElemKeySelector _elemKeySelector;
			TOtherKeySelector _otherKeySelector;
			TJoinSelector _joinSelector;
			std::map<TKey, TOtherElem> _otherMap;

		public:
			_JoinEnumerator(
				std::shared_ptr<IEnumerator<TElem>> enumerator,
				std::shared_ptr<IEnumerator<TOtherElem>> otherEnumerator,
				TElemKeySelector elemKeySelector,
				TOtherKeySelector otherKeySelector,
				TJoinSelector joinSelector)
				: _enumerator(enumerator),
				_otherEnumerator(otherEnumerator),
				_elemKeySelector(elemKeySelector),
				_otherKeySelector(otherKeySelector),
				_joinSelector(joinSelector) {}

			virtual bool next()
			{
				while (_enumerator->next())
				{
					auto key = _elemKeySelector(_enumerator->current());
					if (_otherMap.find(key) == _otherMap.end())
					{
						while (_otherEnumerator->next())
						{
							auto otherElem = _otherEnumerator->current();
							auto otherKey = _otherKeySelector(otherElem);
							_otherMap[otherKey] = otherElem;
							if (key == otherKey)
								return true;
						}
						continue;
					}
					return true;
				}
				return false;
			}

			virtual TResultElem current()
			{
				return _joinSelector(_enumerator->current(), _otherMap[_elemKeySelector(_enumerator->current())]);
			}
		};

		virtual std::shared_ptr<IEnumerator<TResultElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TResultElem>>(
				new _JoinEnumerator(_enumerable->getEnumerator(),
					_otherEnumerable->getEnumerator(),
					_elemKeySelector, _otherKeySelector, _joinSelector));
		}
	};

	template<typename TOtherElem, typename TElemKeySelector, typename TOtherKeySelector, typename TJoinSelector>
	class _JoinBuilder
	{
	private:
		std::shared_ptr<IEnumerable<TOtherElem>> _otherEnumerable;
		TElemKeySelector _elemKeySelector;
		TOtherKeySelector _otherKeySelector;
		TJoinSelector _joinSelector;

	public:
		_JoinBuilder(
			std::shared_ptr<IEnumerable<TOtherElem>> otherEnumerable,
			TElemKeySelector elemKeySelector,
			TOtherKeySelector otherKeySelector,
			TJoinSelector joinSelector)
			: _otherEnumerable(otherEnumerable),
			_elemKeySelector(elemKeySelector),
			_otherKeySelector(otherKeySelector),
			_joinSelector(joinSelector) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			using TResultElem = decltype(_joinSelector(std::declval<TElem>(), std::declval<TOtherElem>()));
			using TKey = decltype(_elemKeySelector(std::declval<TElem>()));
			return std::shared_ptr<IEnumerable<TResultElem>>(new _JoinEnumerable<TElem, TOtherElem, TResultElem, TKey, TElemKeySelector, TOtherKeySelector, TJoinSelector>(enumerable, _otherEnumerable, _elemKeySelector, _otherKeySelector, _joinSelector));
		}
	};

	template<typename TOtherElem, typename TElemKeySelector, typename TOtherKeySelector, typename TJoinSelector>
	decltype(auto) join(
		std::shared_ptr<IEnumerable<TOtherElem>> otherEnumerable,
		TElemKeySelector elemKeySelector,
		TOtherKeySelector otherKeySelector,
		TJoinSelector joinSelector)
	{
		return _JoinBuilder<TOtherElem, TElemKeySelector, TOtherKeySelector, TJoinSelector>(otherEnumerable, elemKeySelector, otherKeySelector, joinSelector);
	}
}

#endif