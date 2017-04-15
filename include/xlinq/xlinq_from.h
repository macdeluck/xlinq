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

	template<typename TContainer, typename TElem>
	class _StlEnumerable : public IEnumerable<TElem>
	{
	private:
		TContainer& _container;
	public:
		_StlEnumerable(TContainer& container) : _container(container) {}

		std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			typedef typename TContainer::iterator iterator;
			return std::shared_ptr<IEnumerator<TElem>>(new _StlEnumerator<iterator, TElem>(_container.begin(), _container.end()));
		}
	};

	template<typename TContainer>
	auto from(TContainer& container) -> std::shared_ptr<IEnumerable<typename TContainer::value_type>>
	{
		return std::shared_ptr<IEnumerable<typename TContainer::value_type>>(new _StlEnumerable<TContainer, typename TContainer::value_type>(container));
	}
}

#endif