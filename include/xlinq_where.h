#ifndef XLINQ_WHERE_H_
#define XLINQ_WHERE_H_

#include "xlinq_base.h"
#include <functional>

namespace xlinq
{
	template<typename TElem>
	class _WhereEnumerable : public IEnumerable<TElem>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _source;
		std::function<bool(TElem)> _predicate;
	public:
		_WhereEnumerable(std::shared_ptr<IEnumerable<TElem>> source, std::function<bool(TElem)> predicate)
			: _source(source), _predicate(predicate) {}

		class _WhereEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			std::function<bool(TElem)> _predicate;
		public:
			_WhereEnumerator(std::shared_ptr<IEnumerator<TElem>> source, std::function<bool(TElem)> predicate)
				: _source(source), _predicate(predicate) {}

			virtual bool next() override
			{
				while (_source->next())
				{
					if (_predicate(_source->current()))
						return true;
				}
				return false;
			}

			virtual TElem current() override
			{
				return _source->current();
			}
		};

		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TElem>>(new _WhereEnumerable::_WhereEnumerator(_source->getEnumerator(), _predicate));
		}
	};

	template<typename TPredicate>
	class _Where
	{
	private:
		TPredicate _predicate;
	public:
		_Where(TPredicate predicate) : _predicate(predicate) {}

		template<typename TElem>
		std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new _WhereEnumerable<TElem>(enumerable, _predicate));
		}
	};

	template<typename TPredicate>
	decltype(auto) where(TPredicate predicate)
	{
		return _Where<TPredicate>(predicate);
	}
}

#endif