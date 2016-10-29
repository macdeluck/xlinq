#ifndef XLINQ_GROUPJOIN_H_
#define XLINQ_GROUPJOIN_H_

#include "xlinq_base.h"
#include <map>
#include <vector>

namespace xlinq
{
	template<typename TElem, typename TOtherElem, typename TResultElem, typename TElemKeySelector, typename TOtherKeySelector, typename TGroupJoinSelector>
	class _GroupJoinEnumerable : public IEnumerable<TResultElem>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _enumerable;
		std::shared_ptr<IEnumerable<TOtherElem>> _otherEnumerable;
		TElemKeySelector _elemKeySelector;
		TOtherKeySelector _otherKeySelector;
		TGroupJoinSelector _groupJoinSelector;
	public:
		_GroupJoinEnumerable(
			std::shared_ptr<IEnumerable<TElem>> enumerable,
			std::shared_ptr<IEnumerable<TOtherElem>> otherEnumerable,
			TElemKeySelector elemKeySelector,
			TOtherKeySelector otherKeySelector,
			TGroupJoinSelector groupJoinSelector)
			: _enumerable(enumerable),
			_otherEnumerable(otherEnumerable),
			_elemKeySelector(elemKeySelector),
			_otherKeySelector(otherKeySelector),
			_groupJoinSelector(groupJoinSelector) {}

		class _GroupJoinEnumerator : public IEnumerator<TResultElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			std::shared_ptr<IEnumerator<TOtherElem>> _otherEnumerator;
			bool _otherEnumerationDone;
			TElemKeySelector _elemKeySelector;
			TOtherKeySelector _otherKeySelector;
			TGroupJoinSelector _groupJoinSelector;

			using TKey = decltype(_elemKeySelector(std::declval<TElem>()));

			class _Grouping : public IEnumerable<TOtherElem>
			{
			private:
				_GroupJoinEnumerator* _parent;
				TKey _key;
			public:
				_Grouping(_GroupJoinEnumerator* parent, TKey key) : _parent(parent), _key(key) {}

				class _GroupingEnumerator : public IEnumerator<TOtherElem>
				{
				private:
					_GroupJoinEnumerator* _parent;
					TKey _key;
					size_t _currentElement;
				public:
					_GroupingEnumerator(_GroupJoinEnumerator* parent, TKey key) : _parent(parent), _key(key), _currentElement(0) {}

					virtual bool next() override
					{
						while (_currentElement == _parent->_groupingData[_key].size())
						{
							if (!_parent->_otherEnumerationDone && _parent->_otherEnumerator->next())
							{
								auto current = _parent->_otherEnumerator->current();
								TKey otherKey = _parent->_otherKeySelector(current);
								if (_parent->_groupingMap.find(otherKey) == _parent->_groupingMap.end())
								{
									_parent->_groupingMap[otherKey] = std::shared_ptr<_Grouping>(new _Grouping(_parent, otherKey));
									_parent->_groupingData[otherKey] = std::vector<TOtherElem>();
								}
								_parent->_groupingData[otherKey].push_back(current);
							}
							else
							{
								_parent->_otherEnumerationDone = true;
								return false;
							}
						}
						_currentElement++;
						return true;
					}

					virtual TOtherElem current() override
					{
						return _parent->_groupingData[_key][_currentElement - 1];
					}
				};

				virtual std::shared_ptr<IEnumerator<TOtherElem>> getEnumerator() override
				{
					return std::shared_ptr<IEnumerator<TOtherElem>>(new _GroupingEnumerator(_parent, _key));
				}
			};

			std::map<TKey, std::shared_ptr<_Grouping>> _groupingMap;
			std::map<TKey, std::vector<TOtherElem>> _groupingData;
		public:
			_GroupJoinEnumerator(
				std::shared_ptr<IEnumerator<TElem>> enumerator,
				std::shared_ptr<IEnumerator<TOtherElem>> otherEnumerator,
				TElemKeySelector elemKeySelector,
				TOtherKeySelector otherKeySelector,
				TGroupJoinSelector groupJoinSelector)
				: _enumerator(enumerator),
				_otherEnumerator(otherEnumerator),
				_elemKeySelector(elemKeySelector),
				_otherKeySelector(otherKeySelector),
				_groupJoinSelector(groupJoinSelector),
				_otherEnumerationDone(false)
			{
			}

			virtual bool next() override
			{
				while (_enumerator->next())
				{
					auto key = _elemKeySelector(_enumerator->current());
					if (_groupingData.find(key) == _groupingData.end())
					{
						_groupingMap[key] = std::shared_ptr<_Grouping>(new _Grouping(this, key));
						_groupingData[key] = std::vector<TOtherElem>();
					}
					return true;
				}
				return false;
			}

			virtual TResultElem current() override
			{
				auto current = _enumerator->current();
				return _groupJoinSelector(current, _groupingMap[_elemKeySelector(current)]);
			}
		};

		virtual std::shared_ptr<IEnumerator<TResultElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TResultElem>>(new _GroupJoinEnumerable::_GroupJoinEnumerator(_enumerable->getEnumerator(), _otherEnumerable->getEnumerator(), _elemKeySelector, _otherKeySelector, _groupJoinSelector));
		}
	};


	template<typename TOtherElem, typename TElemKeySelector, typename TOtherKeySelector, typename TGroupJoinSelector>
	class _GroupJoinBuilder
	{
	private:
		std::shared_ptr<IEnumerable<TOtherElem>> _otherEnumerable;
		TElemKeySelector _elemKeySelector;
		TOtherKeySelector _otherKeySelector;
		TGroupJoinSelector _groupJoinSelector;

	public:
		_GroupJoinBuilder(
			std::shared_ptr<IEnumerable<TOtherElem>> otherEnumerable,
			TElemKeySelector elemKeySelector,
			TOtherKeySelector otherKeySelector,
			TGroupJoinSelector groupJoinSelector)
			: _otherEnumerable(otherEnumerable),
			_elemKeySelector(elemKeySelector),
			_otherKeySelector(otherKeySelector),
			_groupJoinSelector(groupJoinSelector) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			using TResultElem = decltype(_groupJoinSelector(std::declval<TElem>(), std::declval<std::shared_ptr<IEnumerable<TOtherElem>>>()));
			return std::shared_ptr<IEnumerable<TResultElem>>(new _GroupJoinEnumerable<TElem, TOtherElem, TResultElem, TElemKeySelector, TOtherKeySelector, TGroupJoinSelector>(enumerable, _otherEnumerable, _elemKeySelector, _otherKeySelector, _groupJoinSelector));
		}
	};

	template<typename TOtherElem, typename TElemKeySelector, typename TOtherKeySelector, typename TGroupJoinSelector>
	decltype(auto) groupJoin(
		std::shared_ptr<IEnumerable<TOtherElem>> otherEnumerable,
		TElemKeySelector elemKeySelector,
		TOtherKeySelector otherKeySelector,
		TGroupJoinSelector groupJoinSelector)
	{
		return _GroupJoinBuilder<TOtherElem, TElemKeySelector, TOtherKeySelector, TGroupJoinSelector>(otherEnumerable, elemKeySelector, otherKeySelector, groupJoinSelector);
	}
}

#endif