#ifndef XLINQ_SKIPWHILE_H_
#define XLINQ_SKIPWHILE_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TElem, typename TSkipWhilePredicate>
	class _SkipWhileEnumerable : public IEnumerable<TElem>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _enumerable;
		TSkipWhilePredicate _predicate;

	public:
		_SkipWhileEnumerable(std::shared_ptr<IEnumerable<TElem>> enumerable, TSkipWhilePredicate predicate)
			: _enumerable(enumerable), _predicate(predicate) {}

		class _SkipWhileEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			TSkipWhilePredicate _predicate;
			bool _predicateDone;

		public:
			_SkipWhileEnumerator(std::shared_ptr<IEnumerator<TElem>> enumerator, TSkipWhilePredicate predicate)
				: _enumerator(enumerator), _predicate(predicate), _predicateDone(false) {}

			virtual bool next() override
			{
				if (!_predicateDone)
				{
					while (_enumerator->next())
					{
						if (!_predicate(_enumerator->current()))
						{
							_predicateDone = true;
							return true;
						}
					}
					return false;
				}
				return _enumerator->next();
			}

			virtual TElem current() override
			{
				return _enumerator->current();
			}
		};

		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TElem>>(new _SkipWhileEnumerable::_SkipWhileEnumerator(_enumerable->getEnumerator(), _predicate));
		}
	};

	template<typename TSkipWhilePredicate>
	class _SkipWhileBuilder
	{
		TSkipWhilePredicate _predicate;
	public:
		_SkipWhileBuilder(TSkipWhilePredicate predicate) : _predicate(predicate) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new _SkipWhileEnumerable<TElem, TSkipWhilePredicate>(enumerable, _predicate));
		}
	};

	template<typename TSkipWhilePredicate>
	decltype(auto) skipWhile(TSkipWhilePredicate predicate)
	{
		return _SkipWhileBuilder<TSkipWhilePredicate>(predicate);
	}
}

#endif