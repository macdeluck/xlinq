#ifndef XLINQ_SELECT_H_
#define XLINQ_SELECT_H_

#include <memory>
#include "xlinq_base.h"

namespace xlinq
{
	namespace internal
	{
		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectEnumerator : public IEnumerator<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IEnumerator<TElem>> _source;

		public:
			_SelectEnumerator(TSelector selector, std::shared_ptr<IEnumerator<TElem>> source)
				: _selector(selector), _source(source) {}

			bool next() override
			{
				return _source->next();
			}

			TSelect current() override
			{
				return _selector(_source->current());
			}
		};

		template<typename TSelector, typename TElem, typename TSelect>
		class _SelectEnumerable : public IEnumerable<TSelect>
		{
		private:
			TSelector _selector;
			std::shared_ptr<IEnumerable<TElem>> _source;

		public:
			_SelectEnumerable(TSelector selector, std::shared_ptr<IEnumerable<TElem>> source)
				: _selector(selector), _source(source) {}

			std::shared_ptr<IEnumerator<TSelect>> getEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TSelect>>(new _SelectEnumerator<TSelector, TElem, TSelect>(_selector, _source->getEnumerator()));
			}
		};

		template<typename TSelector, typename TElem>
		struct selecttypeinfo
		{
			typedef decltype(std::declval<TSelector>()((std::declval<TElem>()))) TSelect;
			typedef std::shared_ptr<IEnumerable<TSelect>> TSelectEnumerable;
		};

		template<typename TSelector>
		class _SelectBuilder
		{
		private:
			TSelector _selector;
		
		public:
			_SelectBuilder(TSelector selector) : _selector(selector) {}

			template<typename TElem>
			auto build(std::shared_ptr<IEnumerable<TElem>> enumerable) -> typename selecttypeinfo<TSelector, TElem>::TSelectEnumerable
			{
				typedef typename selecttypeinfo<TSelector, TElem>::TSelect TSelect;
				typedef typename selecttypeinfo<TSelector, TElem>::TSelectEnumerable TSelectEnumerable;
				return TSelectEnumerable(new _SelectEnumerable<TSelector, TElem, TSelect>(_selector, enumerable));
			}
		};
	}

	template<typename TSelector>
	internal::_SelectBuilder<TSelector> select(TSelector selector)
	{
		return internal::_SelectBuilder<TSelector>(selector);
	}
}

#endif