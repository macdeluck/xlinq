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
*	@file xlinq_reverse.h
*	Reversing collection elements order.
*	@author TrolleY
*/
#ifndef XLINQ_REVERSE_H_
#define XLINQ_REVERSE_H_

#include <memory>
#include <list>
#include "xlinq_base.h"
#include "xlinq_from.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem>
		class _ReverseBidirectionalEnumerator : public IBidirectionalEnumerator<TElem>
		{
		private:
			std::shared_ptr<IBidirectionalEnumerator<TElem>> _source;

		public:
			_ReverseBidirectionalEnumerator(std::shared_ptr<IBidirectionalEnumerator<TElem>> source)
				: _source(source)
			{
			}

			bool next() override
			{
				return _source->back();
			}

			bool back() override
			{
				return _source->next();
			}

			TElem current() override
			{
				return _source->current();
			}
		};

		template<typename TElem>
		class _ReverseBidirectionalEnumerable : public IBidirectionalEnumerable<TElem>
		{
		private:
			std::shared_ptr<IBidirectionalEnumerable<TElem>> _source;
		public:
			_ReverseBidirectionalEnumerable(std::shared_ptr<IBidirectionalEnumerable<TElem>> source)
				: _source(source)
			{
			}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ReverseBidirectionalEnumerator<TElem>(_source->getEndEnumerator()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _ReverseBidirectionalEnumerator<TElem>(_source->getEnumerator()));
			}
		};

		template<typename TElem>
		class _ReverseRandomAccessEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerator<TElem>> _source;

		public:
			_ReverseRandomAccessEnumerator(std::shared_ptr<IRandomAccessEnumerator<TElem>> source)
				: _source(source)
			{
			}

			bool next() override
			{
				return _source->back();
			}

			bool back() override
			{
				return _source->next();
			}

			bool advance(int step) override
			{
				return _source->advance(-step);
			}

			TElem current() override
			{
				return _source->current();
			}
		};

		template<typename TElem>
		class _ReverseRandomAccessEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerable<TElem>> _source;
		public:
			_ReverseRandomAccessEnumerable(std::shared_ptr<IRandomAccessEnumerable<TElem>> source)
				: _source(source)
			{
			}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ReverseRandomAccessEnumerator<TElem>(_source->getEndEnumerator()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _ReverseRandomAccessEnumerator<TElem>(_source->getEnumerator()));
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				return std::shared_ptr<IRandomAccessEnumerator<TElem>>(new _ReverseRandomAccessEnumerator<TElem>(_source->getEnumeratorAt(_source->size() - elementIndex - 1)));
			}

			int size() override
			{
				return _source->size();
			}
		};

		class _ReverseBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto vec = std::shared_ptr<std::list<TElem>>(new std::list<TElem>());
				for (auto it = enumerable->getEnumerator(); it->next();)
				{
					vec->push_front(it->current());
				}
				return from(vec);
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IBidirectionalEnumerable<TElem>>(new _ReverseBidirectionalEnumerable<TElem>(enumerable));
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IRandomAccessEnumerable<TElem>>(new _ReverseRandomAccessEnumerable<TElem>(enumerable));
			}
		};
	}
	/*@endcond*/

	/**
	*	Reversees order of collection elements.
	*	This function may be used to reverse collection elements. The function is O(1)
	*	and does not gather collection elements until IEnumerable<T> is passed.
	*	If so, the function operates in O(n), where n is number of collection elements.
	*	@return Builder of skip_while expression.
	*/
	XLINQ_INLINE internal::_ReverseBuilder reverse()
	{
		return internal::_ReverseBuilder();
	}
}

#endif