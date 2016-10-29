#ifndef XLINQ_TAKEWHILE_H_
#define XLINQ_TAKEWHILE_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TElem, typename TTakeWhilePredicate>
	class _TakeWhileEnumerable : public IEnumerable<TElem>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _enumerable;
		TTakeWhilePredicate _predicate;

	public:
		_TakeWhileEnumerable(std::shared_ptr<IEnumerable<TElem>> enumerable, TTakeWhilePredicate predicate)
			: _enumerable(enumerable), _predicate(predicate) {}

		class _TakeWhileEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			TTakeWhilePredicate _predicate;

		public:
			_TakeWhileEnumerator(std::shared_ptr<IEnumerator<TElem>> enumerator, TTakeWhilePredicate predicate)
				: _enumerator(enumerator), _predicate(predicate) {}

			virtual bool next() override
			{
				while (_enumerator->next())
				{
					if (_predicate(_enumerator->current()))
						return true;
					return false;
				}
				return false;
			}

			virtual TElem current() override
			{
				return _enumerator->current();
			}
		};

		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TElem>>(new _TakeWhileEnumerable::_TakeWhileEnumerator(_enumerable->getEnumerator(), _predicate));
		}
	};

	template<typename TTakeWhilePredicate>
	class _TakeWhileBuilder
	{
		TTakeWhilePredicate _predicate;
	public:
		_TakeWhileBuilder(TTakeWhilePredicate predicate) : _predicate(predicate) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new _TakeWhileEnumerable<TElem, TTakeWhilePredicate>(enumerable, _predicate));
		}
	};

	template<typename TTakeWhilePredicate>
	decltype(auto) takeWhile(TTakeWhilePredicate predicate)
	{
		return _TakeWhileBuilder<TTakeWhilePredicate>(predicate);
	}
}

#endif
