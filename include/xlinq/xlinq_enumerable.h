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
*	@file xlinq_enumerable.h
*	Enumerable definitions.
*	@author TrolleY
*/
#ifndef XLINQ_ENUMERABLE_H_
#define XLINQ_ENUMERABLE_H_

#include <vector>
#include "xlinq_base.h"
#include "xlinq_from.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem>
		class EmptyEnumerator : public IBidirectionalEnumerator<TElem>
		{
			bool _started;
		public:
			EmptyEnumerator(bool started) : _started(started) {}

			virtual bool next() override
			{
				if (_started) throw IterationFinishedException();
				_started = true;
				return false;
			}

			virtual bool back() override
			{
				if (!_started) throw IterationNotStartedException();
				_started = false;
				return false;
			}

			TElem current()
			{
				if (!_started) throw IterationNotStartedException();
				else throw IterationFinishedException();
			}
		};

		template<typename TElem>
		class EmptyEnumerable : public IBidirectionalEnumerable<TElem>
		{
			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new EmptyEnumerator<TElem>(false));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new EmptyEnumerator<TElem>(true));
			}
		};

		template<typename TElem>
		class InfiniteRepeatEnumerator : public IEnumerator<TElem>
		{
		protected:
			TElem _element;
			bool _started;
		public:
			InfiniteRepeatEnumerator(TElem element) : _element(element), _started(false) {}

			virtual bool next() override
			{
				_started = true;
				return true;
			}

			TElem current()
			{
				if (!_started) throw IterationNotStartedException();
				return _element;
			}
		};

		template<typename TElem>
		class RepeatEnumerator : public InfiniteRepeatEnumerator<TElem>
		{
			int _size;
		public:
			RepeatEnumerator(TElem element, int size) : InfiniteRepeatEnumerator<TElem>(element), _size(size + 1) {}

			virtual bool next() override
			{
				return InfiniteRepeatEnumerator<TElem>::next() && (--_size > 0);
			}
		};

		template<typename TElem>
		class InfiniteRepeatEnumerable : public IEnumerable<TElem>
		{
			TElem _element;
		public:
			InfiniteRepeatEnumerable(TElem element) : _element(element) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new InfiniteRepeatEnumerator<TElem>(_element));
			}
		};

		template<typename TElem>
		class RepeatEnumerable : public IEnumerable<TElem>
		{
			TElem _element;
			int _size;
		public:
			RepeatEnumerable(TElem element, int size) : _element(element), _size(size) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new RepeatEnumerator<TElem>(_element, _size));
			}
		};

		template<typename TElem>
		class RangeEnumerator : public IEnumerator<TElem>
		{
		protected:
			TElem _current;
			TElem _upper;
			bool _started;
		public:
			RangeEnumerator(TElem lower, TElem upper) : _current(lower), _upper(upper), _started(false) {}

			virtual bool next() override
			{
				if (_current == _upper) throw IterationFinishedException();
				if (!_started)
				{
					_started = true;
					return true;
				}
				return !(++_current == _upper);
			}

			TElem current()
			{
				if (!_started) throw IterationNotStartedException();
				return _current;
			}
		};

		template<typename TElem>
		class RangeEnumerable : public IEnumerable<TElem>
		{
			TElem _lower;
			TElem _upper;
			int _size;
		public:
			RangeEnumerable(TElem lower, TElem upper) : _lower(lower), _upper(upper) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new RangeEnumerator<TElem>(_lower, _upper));
			}
		};
	}
	/*@endcond*/

	/**
	*	Enumerable definitions.
	*	This class implements basic static enumerable factory methods.
	*/
	class Enumerable
	{
	public:
		/**
		*	Creates empty enumerable.
		*	This function creates empty enumerable.
		*	@return Empty enumerable.
		*/
		template<typename TElem>
		XLINQ_INLINE static std::shared_ptr<IBidirectionalEnumerable<TElem>> empty()
		{
			return std::shared_ptr<IBidirectionalEnumerable<TElem>>(new internal::EmptyEnumerable<TElem>());
		}

		/**
		*	Creates enumerable which gives infinite stream of repeating elements.
		*	This function creates enumerable whose elements won't end.
		*	It can cause infinite loop if user won't control stopping iteration.
		*	@param element Element to repeat.
		*	@return Infinite enumerable with repeating elements.
		*/
		template<typename TElem>
		XLINQ_INLINE static std::shared_ptr<IEnumerable<TElem>> repeat(TElem element)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new internal::InfiniteRepeatEnumerable<TElem>(element));
		}

		/**
		*	Creates enumerable which gives stream of repeating elements by number of time.
		*	This function creates a stream of copies of provided element. Element copies
		*	will occur in stream given number of times.
		*	@param element Element to repeat.
		*	@param size Number of element repetitions.
		*	@return Enumerable with repeating elements provided number of times.
		*/
		template<typename TElem>
		XLINQ_INLINE static std::shared_ptr<IEnumerable<TElem>> repeat(TElem element, int size)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new internal::RepeatEnumerable<TElem>(element, size));
		}

		/**
		*	Creates enumerable whose elements are in given range.
		*	This function creates an enumerable whose elements are in given range.
		*	Type of element must implement pre-increment and equality comparision operators.
		*	@param lower Lower bound of range (inclusive).
		*	@param upper Upper bound of range (exclusive).
		*	@return Enumerable with elements within given range.
		*/
		template<typename TElem>
		XLINQ_INLINE static std::shared_ptr<IEnumerable<TElem>> range(TElem lower, TElem upper)
		{
			return std::shared_ptr<IEnumerable<TElem>>(new internal::RangeEnumerable<TElem>(lower, upper));
		}
	};
}

#endif