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
*	@file xlinq_stl.h
*	Treating IEnumerable as STL container.
*	@author TrolleY
*/
#ifndef XLINQ_STL_H_
#define XLINQ_STL_H_

#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem>
		class _StlIterator
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			bool _finished;

		public:
			typedef int difference_type;
			typedef TElem value_type;
			typedef TElem& reference_type;
			typedef TElem* pointer;
			typedef std::forward_iterator_tag iterator_category;

			_StlIterator() : _enumerator(nullptr), _finished(true) {}

			_StlIterator(std::shared_ptr<IEnumerator<TElem>> enumerator, bool finished) : _enumerator(enumerator), _finished(finished) {}

			_StlIterator(const _StlIterator<TElem>& other)
			{
				this->_enumerator = other._enumerator->clone();
			}

			_StlIterator<TElem>& operator++()
			{
				_finished = !_enumerator->next();
				return *this;
			}

			TElem operator*() const
			{
				return _enumerator->current();
			}

			friend void swap(_StlIterator<TElem>& lhs, _StlIterator<TElem>& rhs)
			{
				auto tmp = lhs->_finished;
				lhs->_finished = rhs->_finished;
				rhs->_finished = tmp;
				std::swap(lhs->_enumerator, rhs->_enumerator);
			}

			_StlIterator<TElem> operator++(int)
			{
				_StlIterator<TElem> res(_enumerator->clone(), _finished);
				_finished = !_enumerator->next();
				return res;
			}

			pointer operator->() const;

			friend bool operator==(const _StlIterator<TElem>& lhs, const _StlIterator<TElem>& rhs)
			{
				if (lhs._finished && rhs._finished) return true;
				return lhs._finished == rhs._finished &&
					lhs._enumerator->equals(rhs._enumerator);
			}

			friend bool operator!=(const _StlIterator<TElem>& lhs, const _StlIterator<TElem>& rhs)
			{
				return !(lhs == rhs);
			}
		};

		template<typename TElem>
		class _StlContainer
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _enumerable;

		public:
			typedef _StlIterator<TElem> iterator;
			
			_StlContainer(std::shared_ptr<IEnumerable<TElem>> enumerable) : _enumerable(enumerable) {}

			iterator begin()
			{
				auto enumerator = _enumerable->getEnumerator();
				return iterator(enumerator, enumerator->next());
			}

			iterator end()
			{
				return iterator();
			}
		};

		class _StlBuilder
		{
		public:
			template<typename TElem>
			_StlContainer<TElem> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return _StlContainer<TElem>(enumerable);
			}

			template<typename TElem>
			_StlContainer<TElem> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return _StlContainer<TElem>(enumerable);
			}

			template<typename TElem>
			_StlContainer<TElem> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return _StlContainer<TElem>(enumerable);
			}
		};
	}
	/*@endcond*/

	XLINQ_INLINE internal::_StlBuilder stl()
	{
		return internal::_StlBuilder();
	}
}

#endif