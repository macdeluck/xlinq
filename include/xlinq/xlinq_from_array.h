/*
MIT License

Copyright (c) 2017 TrolleY

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/**
*	@file xlinq_from_array.h
*	Creating enumerable object from arrays.
*	@author TrolleY
*/
#ifndef XLINQ_FROM_ARRAY_H_
#define XLINQ_FROM_ARRAY_H_

#include <cstdlib>
#include <memory>
#include <cassert>
#include "xlinq_base.h"
#include "xlinq_exception.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem>
		class _ArrayEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			TElem* _begin;
			int _size;
			int _index;
			bool _started;

			void assert_finished()
			{
				if (_started && _index == _size)
					throw IterationFinishedException();
			}

			void assert_started()
			{
				if (!_started)
					throw IterationNotStartedException();
			}

			bool advance_forward(int step)
			{
				assert(step > 0);
				if (_index == _size && _started)
					return false;

				if (!_started)
				{
					_started = true;
					step--;
				}
				auto dist = _size - _index;
				if (step < dist)
				{
					_index += step;
					return true;
				}
				else
				{
					_index = _size;
					return false;
				}
			}

			bool advance_backward(int step)
			{
				assert(step < 0);
				if ((_index == 0) && !_started)
					return false;

				step = -step;
				if (step < _index)
				{
					_index -= step;
					return true;
				}
				else if (step == _index)
				{
					_index = 0;
					return true;
				}
				else
				{
					_started = false;
					_index = 0;
					return false;
				}
			}

		public:
			_ArrayEnumerator(TElem* begin, int size) : _begin(begin), _size(size), _index(0), _started(false) {}

			bool next() override
			{
				assert_finished();
				return advance(1);
			}

			bool back() override
			{
				assert_started();
				return advance(-1);
			}

			bool advance(int step) override
			{
				if (!step) return true;
				return step > 0 ? advance_forward(step) : advance_backward(step);
			}

			TElem current()
			{
				assert_started();
				assert_finished();
				return *(_begin + _index);
			}
		};

		template<typename TElem>
		class _ArrayEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			TElem* _array;
			int _size;
		public:
			_ArrayEnumerable(TElem* array, int size) : _array(array), _size(size) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ArrayEnumerator<TElem>(_array, _size));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				auto result = this->getEnumerator();
				result->advance(_size + 1);
				return result;
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				auto result = this->getEnumerator();
				result->advance(elementIndex + 1);
				return result;
			}

			int size() override
			{
				return _size;
			}
		};
	}
	/*@endcond*/

	/**
	*	Creates enumerable from fixed size array.
	*	This function may be used to create enumerable from fixed size array.
	*	It is implemented using deffered execution so traversing of the array
	*	will wait until its next element will be requested. Please note, that
	*	enumeration will fail if array will be deallocated.
	*	@param array Source fixed size array.
	*	@return Enumerable from array.
	*/
	template<typename TElem, int SIZE>
	std::shared_ptr<IRandomAccessEnumerable<TElem>> from(TElem(&array)[SIZE])
	{
		return std::shared_ptr<IRandomAccessEnumerable<TElem>>(new internal::_ArrayEnumerable<TElem>((TElem*)array, SIZE));
	}
}

#endif