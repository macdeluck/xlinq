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
				throw new IterationFinishedException();
		}
	public:
		_StlEnumerator(TIterator begin, TIterator end) : _begin(begin), _end(end), _started(false) {}

		bool next() override
		{
			_started = true;
			assert_finished();
			return ++_begin == _end;
		}

		TElem current() override
		{
			if (!_started)
				throw new IterationNotStartedException();
			assert_finished();
			return *_begin;
		}
	};
}

#endif