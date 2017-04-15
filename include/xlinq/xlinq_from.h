#ifndef XLINQ_FROM_H_
#define XLINQ_FROM_H_

#include <cstdlib>
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

	template<typename TElem, size_t SIZE>
	class _ArrayEnumerator : public IEnumerator<TElem>
	{
	private:
		TElem* _begin;
		size_t _index;
		bool _started;

		void assert_finished()
		{
			if (_index == SIZE)
				throw IterationFinishedException();
		}

	public:
		_ArrayEnumerator(TElem* begin) : _begin(begin), _index(0), _started(false) {}

		bool next() override
		{
			assert_finished();
			if (!_started)
				_started = true;
			else
			{
				++_begin;
				++_index;
			}
			return _index != SIZE;
		}

		TElem current()
		{
			if (!_started) throw IterationNotStartedException();
			assert_finished();
			return *_begin;
		}
	};

	template<typename TElem, size_t SIZE>
	class _ArrayEnumerable : public IEnumerable<TElem>
	{
	private:
		TElem(&_array)[SIZE];
	public:
		_ArrayEnumerable(TElem(&array)[SIZE]) : _array(array) {}

		std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TElem>>(new _ArrayEnumerator<TElem, SIZE>((TElem*)_array));
		}
	};

	template<typename TElem, size_t SIZE>
	std::shared_ptr<IEnumerable<TElem>> from(TElem(&array)[SIZE])
	{
		return std::shared_ptr<IEnumerable<TElem>>(new _ArrayEnumerable<TElem, SIZE>(array));
	}

	template<typename TContainer>
	auto from(TContainer& container) -> std::shared_ptr<IEnumerable<typename TContainer::value_type>>
	{
		return std::shared_ptr<IEnumerable<typename TContainer::value_type>>(new _StlEnumerable<TContainer, typename TContainer::value_type>(container));
	}
}

#endif