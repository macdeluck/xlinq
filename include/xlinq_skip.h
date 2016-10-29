#ifndef XLINQ_SKIP_H_
#define XLINQ_SKIP_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TElem>
	class _SkipEnumerable : public IEnumerable<TElem>
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _enumerable;
		int _num;
	public:
		_SkipEnumerable(std::shared_ptr<IEnumerable<TElem>> enumerable, int num)
			: _enumerable(enumerable), _num(num) {}

		class _SkipEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			int _num;
		public:
			_SkipEnumerator(std::shared_ptr<IEnumerator<TElem>> enumerator, int num)
				: _enumerator(enumerator), _num(num) {}

			virtual bool next() override
			{
				if (_num >= 0)
				{
					while (_num >= 0)
					{
						if (_enumerator->next())
							_num--;
						else return false;
					}
					return true;
				}
				return _enumerator->next();
			}

			virtual TElem current() override
			{
				return _enumerator->current();
			}
		};

		std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TElem>>(new _SkipEnumerable::_SkipEnumerator(_enumerable->getEnumerator(), _num));
		}
	};

	class _SkipBuilder
	{
	private:
		int _num;

	public:
		_SkipBuilder(int num) : _num(num) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new _SkipEnumerable<TElem>(enumerable, _num));
		}
	};

	decltype(auto) skip(int num)
	{
		return _SkipBuilder(num);
	}
}

#endif