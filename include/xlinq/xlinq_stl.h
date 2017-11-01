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
	/**
	*	Iterator created from IEnumerator implementing C++ forward iterator concept.
	*	This class implements C++ forward iterator concept and allows to use
	*	all read-only algrithms from <algorithm> header.
	*/
	template<typename TElem>
	class XlinqIterator
	{
	private:
		std::shared_ptr<IEnumerator<TElem>> _enumerator;
		bool _finished;

	public:
		/**
		*	Iterator category to support std::iterator_traits.
		*/
		typedef std::forward_iterator_tag iterator_category;

		/**
		*	Iterator value type to support std::iterator_traits.
		*/
		typedef TElem value_type;

		/**
		*	Iterator difference type to support std::iterator_traits.
		*/
		typedef int difference_type;

		/**
		*	Iterator pointer type to support std::iterator_traits.
		*/
		typedef TElem* pointer;

		/**
		*	Iterator reference type to support std::iterator_traits.
		*/
		typedef TElem& reference;

		/**
		*	Creates empty finished iterator.
		*/
		XlinqIterator() : _enumerator(nullptr), _finished(true) {}

		/**
		*	Creates forward iterator from enumerator.
		*	This constructor allows to create STL-like iterator from given enumerator.
		*	@param enumerator Source enumerator
		*	@param finished Is enumerator equal to end.
		*/
		XlinqIterator(std::shared_ptr<IEnumerator<TElem>> enumerator, bool finished) : _enumerator(enumerator), _finished(finished) {}

		/**
		*	Creates copy of iterator.
		*	This constructor creates deep copy of iterator. The iteration of copy
		*	is independent of iteration of source.
		*	@param other iterator to copy
		*/
		XlinqIterator(const XlinqIterator<TElem>& other)
		{
			this->_enumerator = other._enumerator ? other._enumerator->clone() : nullptr;
			this->_finished = other._finished;
		}

		/**
		*	Moves iterator to the instance.
		*	This constructor creates shallow copy of iterator. The source iterator
		*	is no longer valid.
		*	@param other iterator to move
		*/
		XlinqIterator(const XlinqIterator<TElem>&& other)
		{
			this->_enumerator = std::move(other._enumerator);
			this->_finished = other._finished;
		}

		/**
		*	Assigns copy of the instance.
		*	This operator creates deep copy of iterator. The iteration of copy
		*	is independent of iteration of source.
		*	@param other iterator to copy
		*	@return copy of given iterator
		*/
		XlinqIterator<TElem> operator=(const XlinqIterator<TElem>& other)
		{
			this->_enumerator = other._enumerator ? other._enumerator->clone() : nullptr;
			this->_finished = other._finished;
			return *this;
		}

		/**
		*	Moves iterator to the next element.
		*	This pre-increment operator advances to the next element and
		*	then returns itself.
		*	@return self instance after moving to next element
		*/
		XlinqIterator<TElem>& operator++()
		{
			_finished = !_enumerator->next();
			return *this;
		}

		/**
		*	Returns element iterator points to.
		*	This operation allows to get element iterator points to.
		*	@return element iterator points to
		*/
		TElem operator*() const
		{
			return _enumerator->current();
		}

		/**
		*	Swaps iterators between themselves.
		*	This function allows to exchange iterators contents between themselves.
		*	@param lhs left side iterator
		*	@param rhs right side iterator
		*/
		friend void swap(XlinqIterator<TElem>& lhs, XlinqIterator<TElem>& rhs)
		{
			auto tmp = lhs->_finished;
			lhs->_finished = rhs->_finished;
			rhs->_finished = tmp;
			std::swap(lhs->_enumerator, rhs->_enumerator);
		}

		/**
		*	Creates copy of itself and then moves iterator to the next element.
		*	This post-increment operator copies itself and advances to the next 
		*	element.
		*	@return copy of instance before moving to next element
		*/
		XlinqIterator<TElem> operator++(int)
		{
			XlinqIterator<TElem> res(_enumerator ? _enumerator->clone() : nullptr, _finished);
			_finished = !_enumerator->next();
			return res;
		}

		/**
		*	Checks if iterators points to the same element.
		*	This function allows to compare iterator for their equality. Iterators are equal
		*	it they point to the same element.
		*	@return True, if iterators point to the same element.
		*/
		friend bool operator==(const XlinqIterator<TElem>& lhs, const XlinqIterator<TElem>& rhs)
		{
			if (lhs._finished && rhs._finished) return true;
			if (lhs._finished != rhs._finished) return false;
			return lhs._finished == rhs._finished &&
				(((bool)lhs._enumerator) == ((bool)rhs._enumerator)) &&
				((!lhs._enumerator) || lhs._enumerator->equals(rhs._enumerator));
		}

		/**
		*	Checks if iterators does not point to the same element.
		*	This function allows to compare iterator for their inequality. Iterators are equal
		*	it they point to the same element.
		*	@return True, if iterators does not poin to the same element.
		*/
		friend bool operator!=(const XlinqIterator<TElem>& lhs, const XlinqIterator<TElem>& rhs)
		{
			return !(lhs == rhs);
		}
	};

	/**
	*	STL-like container allowing to get STL-like iterator from IEnumerable
	*	This class implements C++ container concept allowing to get STL-like iterator.
	*/
	template<typename TElem>
	class XlinqContainer
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _enumerable;

	public:
		/**
		*	Type of iterator.
		*/
		typedef XlinqIterator<TElem> iterator;

		/**
		*	Creates new instance of container from given enumerable.
		*	@param enumerable Enumerable to create container from.
		*/
		XlinqContainer(std::shared_ptr<IEnumerable<TElem>> enumerable) : _enumerable(enumerable) {}

		/**
		*	Gets iterator pointing to beggining of collection.
		*	@return iterator pointing to beggining of collection
		*/
		iterator begin()
		{
			auto enumerator = _enumerable->getEnumerator();
			return ++iterator(enumerator, false);
		}

		/**
		*	Gets iterator pointing to end of collection.
		*	@return iterator pointing to end of collection (guard)
		*/
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
	*	@return stl expression builder
	*/
	XLINQ_INLINE internal::_StlBuilder stl()
	{
		return internal::_StlBuilder();
	}
}

#endif