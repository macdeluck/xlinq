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
#include <type_traits>
#include "xlinq_base.h"
#include "xlinq_exception.h"

namespace xlinq
{
	template<typename TElem, int SIZE>
	auto from_array(TElem(&array)[SIZE])->std::shared_ptr<IRandomAccessEnumerable<typename std::remove_all_extents<typename std::remove_reference<decltype(array)>::type>::type>>;

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

		template<typename TElem, int SIZE>
		class _StdArrayEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			std::array<TElem, SIZE>& _array;
			int _index;
			bool _started;

			void assert_finished()
			{
				if (_started && _index == SIZE)
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
				if (_index == SIZE && _started)
					return false;

				if (!_started)
				{
					_started = true;
					step--;
				}
				auto dist = SIZE - _index;
				if (step < dist)
				{
					_index += step;
					return true;
				}
				else
				{
					_index = SIZE;
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
			_StdArrayEnumerator(std::array<TElem, SIZE>& array) : _array(array), _index(0), _started(false) {}

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
				return _array[_index];
			}
		};

		template<typename TElem, int SIZE>
		class _StdArrayEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			std::array<TElem, SIZE>& _array;
		public:
			_StdArrayEnumerable(std::array<TElem, SIZE>& array) : _array(array) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _StdArrayEnumerator<TElem, SIZE>(_array));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				auto result = this->getEnumerator();
				result->advance(SIZE + 1);
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
				return SIZE;
			}
		};

		template<typename TArray, typename TElem>
		class _ArrayOverArrayEnumerator : public IRandomAccessEnumerator<TElem>
		{
			TArray* _begin;
			int _size;
			int _index;
			bool _started;
			std::shared_ptr<IRandomAccessEnumerator<TElem>> _currentEnumerator;

			void assert_finished()
			{
				if (_started && _index == size())
					throw IterationFinishedException();
			}

			void assert_started()
			{
				if (!_started)
					throw IterationNotStartedException();
			}

			inline int arrIndex(int index) const
			{
				return index / array_size<TArray>::value;
			}

			bool advance_forward(int step)
			{
				assert(step > 0);
				if (_index == size() && _started)
					return false;
				
				auto cstep = step;
				if (!_started)
				{
					_started = true;
					step--;
				}

				auto dist = size() - _index;
				if (step < dist)
				{
					if (!_currentEnumerator || (arrIndex(_index) != arrIndex(_index + step)))
					{
						_currentEnumerator = std::shared_ptr<IRandomAccessEnumerator<TElem>>(from_array(_begin[arrIndex(_index + step)]) >> getEnumerator());
						if (arrIndex(_index) != arrIndex(_index + step))
						{
							auto localIndex = _index % array_size<TArray>::value;
							auto localLeftSteps = array_size<TArray>::value - (cstep == step ? 1 : 0) - localIndex;
							cstep = (cstep - localLeftSteps) % (array_size<TArray>::value + 1);
						}
					}
					assert(cstep > 0);
					_currentEnumerator->advance(cstep);
					_index += step;
					return true;
				}
				else
				{
					_index = size();
					_currentEnumerator = nullptr;
					return false;
				}
			}

			bool advance_backward(int step)
			{
				assert(step < 0);
				if ((_index == 0) && !_started)
					return false;

				step = -step;
				auto cstep = step;
				if (_index == size())
				{
					_index--;
					step--;
				}

				if (step <= _index)
				{
					if (!_currentEnumerator || (arrIndex(_index) != arrIndex(_index - step)))
					{
						_currentEnumerator = std::shared_ptr<IRandomAccessEnumerator<TElem>>(from_array(_begin[arrIndex(_index - step)]) >> getEndEnumerator());
						if (arrIndex(_index) != arrIndex(_index - step))
						{
							auto localIndex = _index % array_size<TArray>::value;
							cstep = (step - localIndex) % (array_size<TArray>::value + 1);
						}
					}
					assert(cstep > 0);
					_currentEnumerator->advance(-cstep);
					_index -= step;
					return true;
				}
				else
				{
					_started = false;
					_currentEnumerator = nullptr;
					_index = 0;
					return false;
				}
			}

			inline int size() const { return _size * array_size<TArray>::value; }

		public:
			_ArrayOverArrayEnumerator(TArray* begin, int size) : _begin(begin), _size(size), _index(0), _started(false) {}

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
				return _currentEnumerator->current();
			}
		};

		template<typename TArray, int SIZE, typename TElem>
		class _ArrayOverArrayEnumerable : public IRandomAccessEnumerable<TElem>
		{
			TArray* _array;
			int _size;
		public:
			_ArrayOverArrayEnumerable(TArray* array, int size) : _array(array), _size(size) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ArrayOverArrayEnumerator<TArray, TElem>(_array, _size));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				auto result = this->getEnumerator();
				result->advance(size() + 1);
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
				return _size * array_size<TArray>::value;
			}
		};

		template<typename TArray, int SIZE, bool elemIsArray, typename TElem>
		struct MultidimArrayEnumerableSelector
		{
			typedef _ArrayEnumerable<TElem> type;
		};

		template<typename TArray, int SIZE, typename TElem>
		struct MultidimArrayEnumerableSelector<TArray, SIZE, true, TElem>
		{
			typedef _ArrayOverArrayEnumerable<TArray, SIZE, TElem> type;
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

	/**
	*	Creates enumerable from new standard fixed size array.
	*	This function may be used to create enumerable from fixed size array.
	*	It is implemented using deffered execution so traversing of the array
	*	will wait until its next element will be requested. Please note, that
	*	enumeration will fail if array will be deallocated.
	*	@param array Source fixed size array.
	*	@return Enumerable from array.
	*/
	template<typename TElem, int SIZE>
	std::shared_ptr<IRandomAccessEnumerable<TElem>> from(std::array<TElem, SIZE>& array)
	{
		return std::shared_ptr<IRandomAccessEnumerable<TElem>>(new internal::_StdArrayEnumerable<TElem, SIZE>(array));
	}

	/**
	*	Creates enumerable from any dimension fixed size array.
	*	This function may be used to create enumerable from fixed size array.
	*	It is implemented using deffered execution so traversing of the array
	*	will wait until its next element will be requested. Please note, that
	*	enumeration will fail if array will be deallocated. Enumeration flattens
	*	array contents to single one.
	*	@param array Source multidimensional fixed size array.
	*	@return Enumerable from array.
	*/
	template<typename TElem, int SIZE>
	auto from_array(TElem(&array)[SIZE]) -> std::shared_ptr<IRandomAccessEnumerable<typename std::remove_all_extents<typename std::remove_reference<decltype(array)>::type>::type>>
	{
		typedef typename internal::MultidimArrayEnumerableSelector<
			TElem,
			SIZE,
			std::is_array<TElem>::value,
			typename std::remove_all_extents<typename std::remove_reference<decltype(array)>::type>::type>::type TEnumerable;
		return std::shared_ptr<IRandomAccessEnumerable<typename std::remove_all_extents<typename std::remove_reference<decltype(array)>::type>::type>>(new TEnumerable(array, SIZE));
	}
}

#endif