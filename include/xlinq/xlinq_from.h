#ifndef XLINQ_FROM_H_
#define XLINQ_FROM_H_

#include <memory>
#include <iterator>
#include "xlinq_base.h"
#include "xlinq_exception.h"

namespace xlinq
{
	template<typename TIterator, typename TElem>
	class _StlEnumerator : public IEnumerator<TElem>
	{
	private:
		TIterator _begin, _end;
		bool _started;

		void assert_finished()
		{
			if (_begin == _end)
				throw IterationFinishedException();
		}

	public:
		_StlEnumerator(TIterator begin, TIterator end) : _begin(begin), _end(end), _started(false) {}

		bool next() override
		{
			assert_finished();
			if (!_started)
				_started = true;
			else ++_begin;
			return _begin != _end;
		}

		TElem current() override
		{
			if (!_started)
			{
				throw IterationNotStartedException();
			}
			assert_finished();
			return *_begin;
		}
	};
}

#endif