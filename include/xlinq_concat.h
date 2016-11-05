#ifndef XLINQ_CONCAT_H_
#define XLINQ_CONCAT_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TElem>
	class _ConcatEnumerable : public IEnumerable<TElem>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _first;
		std::shared_ptr<IEnumerable<TElem>> _second;

	public:
		_ConcatEnumerable(std::shared_ptr<IEnumerable<TElem>> first, std::shared_ptr<IEnumerable<TElem>> second)
			: _first(first), _second(second) {}

		class _ConcatEnumerator : public IEnumerator<TElem>
		{
		private:
			bool _firstFinished;
			std::shared_ptr<IEnumerator<TElem>> _first;
			std::shared_ptr<IEnumerator<TElem>> _second;

		public:
			_ConcatEnumerator(std::shared_ptr<IEnumerator<TElem>> first, std::shared_ptr<IEnumerator<TElem>> second)
				: _firstFinished(false), _first(first), _second(second) {}

			virtual bool next() override
			{
				if (!_firstFinished)
				{
					if (_first->next())
						return true;
					_firstFinished = true;
				}
				return _second->next();
			}

			virtual TElem current() override
			{
				return _firstFinished ? _second->current() : _first->current();
			}
		};

		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TElem>>(new _ConcatEnumerable::_ConcatEnumerator(_first->getEnumerator(), _second->getEnumerator()));
		}
	};

	template<typename TElem>
	class _ConcatBuilder
	{
		std::shared_ptr<IEnumerable<TElem>> _other;
	public:
		_ConcatBuilder(std::shared_ptr<IEnumerable<TElem>> other) : _other(other) {}

		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new _ConcatEnumerable<TElem>(enumerable, _other));
		}
	};

	template<typename TElem>
	decltype(auto) concat(std::shared_ptr<IEnumerable<TElem>> other)
	{
		return _ConcatBuilder<TElem>(other);
	}
}

#endif