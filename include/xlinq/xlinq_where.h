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
*	@file xlinq_where.h
*	Filtering elements of collection using given predicate.
*	@author TrolleY
*/
#ifndef XLINQ_WHERE_H_
#define XLINQ_WHERE_H_

#include <memory>
#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem, typename TPredicate>
		class _WhereEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			TPredicate _predicate;
		public:
			_WhereEnumerator(std::shared_ptr<IEnumerator<TElem>> source, TPredicate predicate)
				: _source(source), _predicate(predicate)
			{}

			bool next() override
			{
				while (_source->next())
					if (_predicate(_source->current()))
						return true;
				return false;
			}

			TElem current() override
			{
				return _source->current();
			}
		};

		template<typename TElem, typename TPredicate>
		class _WhereBidirectionalEnumerator : public IBidirectionalEnumerator<TElem>
		{
		private:
			std::shared_ptr<IBidirectionalEnumerator<TElem>> _source;
			TPredicate _predicate;
		public:
			_WhereBidirectionalEnumerator(std::shared_ptr<IBidirectionalEnumerator<TElem>> source, TPredicate predicate)
				: _source(source), _predicate(predicate)
			{}

			bool next() override
			{
				while (_source->next())
					if (_predicate(_source->current()))
						return true;
				return false;
			}

			bool back() override
			{
				while (_source->back())
					if (_predicate(_source->current()))
						return true;
				return false;
			}

			TElem current() override
			{
				return _source->current();
			}
		};

		template<typename TElem, typename TPredicate>
		class _WhereEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _source;
			TPredicate _predicate;
		public:
			_WhereEnumerable(std::shared_ptr<IEnumerable<TElem>> source, TPredicate predicate)
				: _source(source), _predicate(predicate)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _WhereEnumerator<TElem, TPredicate>(_source->getEnumerator(), _predicate));
			}
		};

		template<typename TElem, typename TPredicate>
		class _WhereBidirectionalEnumerable : public IBidirectionalEnumerable<TElem>
		{
		private:
			std::shared_ptr<IBidirectionalEnumerable<TElem>> _source;
			TPredicate _predicate;
		public:
			_WhereBidirectionalEnumerable(std::shared_ptr<IBidirectionalEnumerable<TElem>> source, TPredicate predicate)
				: _source(source), _predicate(predicate)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _WhereBidirectionalEnumerator<TElem, TPredicate>(_source->getEnumerator(), _predicate));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _WhereBidirectionalEnumerator<TElem, TPredicate>(_source->getEndEnumerator(), _predicate));
			}
		};

		template<typename TPredicate>
		class _WhereBuilder
		{
		private:
			TPredicate _predicate;
		public:
			_WhereBuilder(TPredicate predicate) : _predicate(predicate) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_WhereEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IBidirectionalEnumerable<TElem>>(new internal::_WhereBidirectionalEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IBidirectionalEnumerable<TElem>>(new internal::_WhereBidirectionalEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}
		};
	}
	/*@endcond*/

	/**
	*	Filters elements of collection using predicate.
	*	This function may be used to filter collection elements using given condition.
	*	It is implemented using deffered execution. The traversing of collection
	*	in looking for next element passing given criteria stops until next element
	*	will be requested.
	*	@param predicate Function used to filter elements of source collection.
	*	It is common to use lambda expression as selector.
	*	@return Builder of select expression.
	*/
	template<typename TPredicate>
	internal::_WhereBuilder<TPredicate> where(TPredicate predicate)
	{
		return internal::_WhereBuilder<TPredicate>(predicate);
	}
}

#endif