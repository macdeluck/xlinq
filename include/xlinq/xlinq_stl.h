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
	template<typename TElem>
	class XlinqIterator
	{
	private:
		std::shared_ptr<IEnumerator<TElem>> _enumerator;
		bool _finished;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef TElem value_type;
		typedef int difference_type;

		typedef TElem* pointer;
		typedef TElem& reference;

		XlinqIterator() : _enumerator(nullptr), _finished(true) {}

		XlinqIterator(std::shared_ptr<IEnumerator<TElem>> enumerator, bool finished) : _enumerator(enumerator), _finished(finished) {}

		XlinqIterator(const XlinqIterator<TElem>& other)
		{
			this->_enumerator = other._enumerator ? other._enumerator->clone() : nullptr;
			this->_finished = other._finished;
		}

		XlinqIterator<TElem> operator=(const XlinqIterator<TElem>& other)
		{
			this->_enumerator = other._enumerator ? other._enumerator->clone() : nullptr;
			this->_finished = other._finished;
			return *this;
		}

		XlinqIterator<TElem>& operator++()
		{
			_finished = !_enumerator->next();
			return *this;
		}

		TElem operator*() const
		{
			return _enumerator->current();
		}

		friend void swap(XlinqIterator<TElem>& lhs, XlinqIterator<TElem>& rhs)
		{
			auto tmp = lhs->_finished;
			lhs->_finished = rhs->_finished;
			rhs->_finished = tmp;
			std::swap(lhs->_enumerator, rhs->_enumerator);
		}

		XlinqIterator<TElem> operator++(int)
		{
			XlinqIterator<TElem> res(_enumerator ? _enumerator->clone() : nullptr, _finished);
			_finished = !_enumerator->next();
			return res;
		}

		friend bool operator==(const XlinqIterator<TElem>& lhs, const XlinqIterator<TElem>& rhs)
		{
			if (lhs._finished && rhs._finished) return true;
			if (lhs._finished != rhs._finished) return false;
			return lhs._finished == rhs._finished &&
				(((bool)lhs._enumerator) == ((bool)rhs._enumerator)) &&
				((!lhs._enumerator) || lhs._enumerator->equals(rhs._enumerator));
		}

		friend bool operator!=(const XlinqIterator<TElem>& lhs, const XlinqIterator<TElem>& rhs)
		{
			return !(lhs == rhs);
		}
	};

	template<typename TElem>
	class XlinqContainer
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _enumerable;

	public:
		typedef XlinqIterator<TElem> iterator;

		XlinqContainer(std::shared_ptr<IEnumerable<TElem>> enumerable) : _enumerable(enumerable) {}

		iterator begin()
		{
			auto enumerator = _enumerable->getEnumerator();
			return ++iterator(enumerator, false);
		}

		iterator end()
		{
			return iterator();
		}
	};

	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _StlBuilder
		{
		public:
			template<typename TElem>
			XlinqContainer<TElem> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return XlinqContainer<TElem>(enumerable);
			}

			template<typename TElem>
			XlinqContainer<TElem> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return XlinqContainer<TElem>(enumerable);
			}

			template<typename TElem>
			XlinqContainer<TElem> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return XlinqContainer<TElem>(enumerable);
			}
		};
	}
	/*@endcond*/

	/**
	*	Converts IEnumerable to STL-like container.
	*	This function alllows to convert enumerable to STL-like container.
	*	This allows use read-only algorithms and modern for each loop syntax.
	*/
	XLINQ_INLINE internal::_StlBuilder stl()
	{
		return internal::_StlBuilder();
	}
}

#endif