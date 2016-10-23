#ifndef XLINQ_SELECT_H_
#define XLINQ_SELECT_H_

#include "xlinq_base.h"
#include <functional>

namespace xlinq
{
	template<typename TElem, typename TSelect>
	class _SelectEnumerable : public IEnumerable<TSelect>
	{
	private:
		std::function<TSelect(TElem)> _selector;
		std::shared_ptr<IEnumerable<TElem>> _source;

		class _SelectEnumerator : public IEnumerator<TSelect>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			std::function<TSelect(TElem)> _selector;
		public:
			_SelectEnumerator(std::shared_ptr<IEnumerator<TElem>> source, std::function<TSelect(TElem)> selector)
				: _source(source), _selector(selector) {}

			virtual bool next() override
			{
				return _source->next();
			}

			virtual TSelect current() override
			{
				return _selector(_source->current());
			}
		};
	public:
		_SelectEnumerable(std::shared_ptr<IEnumerable<TElem>> source, std::function<TSelect(TElem)> selector)
			: _source(source), _selector(selector) {}

		virtual std::shared_ptr<IEnumerator<TSelect>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TSelect>>(new _SelectEnumerable::_SelectEnumerator(_source->getEnumerator(), _selector));
		}
	};

	template<typename TSelector>
	class _Select
	{
	private:
		TSelector _selector;
	public:
		_Select(TSelector selector) : _selector(selector) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			using TSelect = decltype(_selector(enumerable->getEnumerator()->current()));
			return std::shared_ptr<IEnumerable<TSelect>>(new _SelectEnumerable<TElem, TSelect>(enumerable, _selector));
		}
	};

	template<typename TSelector>
	_Select<TSelector> select(TSelector selector)
	{
		return _Select<TSelector>(selector);
	}
}

#endif