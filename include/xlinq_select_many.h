#ifndef XLINQ_SELECT_MANY_H_
#define XLINQ_SELECT_MANY_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TElem, typename TManyElem, typename TManySelect, typename TSelect>
	class _SelectManyEnumerable : public IEnumerable<TSelect>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _source;
		std::function<TManySelect(TElem)> _manySelector;
		std::function<TSelect(TManyElem)> _selector;
		
	public:
		_SelectManyEnumerable(std::shared_ptr<IEnumerable<TElem>> source, std::function<TManySelect(TElem)> manySelector, std::function<TSelect(TManyElem)> selector)
			: _source(source), _manySelector(manySelector), _selector(selector) {}

		class _SelectManyEnumerator : public IEnumerator<TSelect>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			std::function<TManySelect(TElem)> _manySelector;
			std::function<TSelect(TManyElem)> _selector;
			std::shared_ptr<IEnumerator<TManyElem>> _currentEnumerator;

		public:
			_SelectManyEnumerator(std::shared_ptr<IEnumerator<TElem>> source, std::function<TManySelect(TElem)> manySelector, std::function<TSelect(TManyElem)> selector)
				: _source(source), _manySelector(manySelector), _selector(selector) {}

			virtual bool next() override
			{
				if (_currentEnumerator)
				{
					if (_currentEnumerator->next())
						return true;
					_currentEnumerator.reset();
				}
				while (!_currentEnumerator)
				{
					if (_source->next())
					{
						_currentEnumerator = _manySelector(_source->current())->getEnumerator();
						if (_currentEnumerator->next())
							return true;
						_currentEnumerator.reset();
					}
					else return false;
				}
				return false;
			}

			virtual TSelect current() override
			{
				return _selector(_currentEnumerator->current());
			}
		};

		virtual std::shared_ptr<IEnumerator<TSelect>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TSelect>>(new _SelectManyEnumerable::_SelectManyEnumerator(_source->getEnumerator(), _manySelector, _selector));
		}
	};

	template<typename TManySelector, typename TSelector>
	class _SelectMany
	{
	private:
		TManySelector _manySelector;
		TSelector _selector;
	public:
		_SelectMany(TManySelector manySelector, TSelector selector) : _manySelector(manySelector), _selector(selector) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			using TManySelect = decltype(_manySelector(std::declval<TElem>()));
			using TManyElem = decltype(std::declval<TManySelect>()->getEnumerator()->current());
			using TSelect = decltype(_selector(std::declval<TManyElem>()));
			return std::shared_ptr<IEnumerable<TSelect>>(new _SelectManyEnumerable<TElem, TManyElem, TManySelect, TSelect>(enumerable, _manySelector, _selector));
		}
	};

	template<typename TManySelector, typename TSelector>
	_SelectMany<TManySelector, TSelector> selectMany(TManySelector manySelector, TSelector selector)
	{
		return _SelectMany<TManySelector, TSelector>(manySelector, selector);
	}
}

#endif