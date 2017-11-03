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
*	@file xlinq_take.h
*	Cutting collection elements.
*	@author TrolleY
*/
#ifndef XLINQ_TAKE_H_
#define XLINQ_TAKE_H_

#include <memory>
#include <cassert>
#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem, typename TPredicate>
		class _TakeWhileEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			TPredicate _predicate;
		public:
			_TakeWhileEnumerator(std::shared_ptr<IEnumerator<TElem>> source, TPredicate predicate)
				: _source(source), _predicate(predicate)
			{}

			bool next() override
			{
				if (_source && _source->next() && _predicate(_source->current()))
					return true;
				_source = nullptr;
				return false;
			}

			TElem current() override
			{
				if (!_source) throw IterationFinishedException();
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_TakeWhileEnumerator<TElem, TPredicate>>(other);
				if (!pother)
					return false;
				return ((bool)this->_source) == ((bool)pother->_source) &&
					(!(this->_source) || this->_source->equals(pother->_source));
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _TakeWhileEnumerator<TElem, TPredicate>(this->_source->clone(), this->_predicate));
			}
		};

		template<typename TElem, typename TPredicate>
		class _TakeWhileEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _source;
			TPredicate _predicate;
		public:
			_TakeWhileEnumerable(std::shared_ptr<IEnumerable<TElem>> source, TPredicate predicate)
				: _source(source), _predicate(predicate)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _TakeWhileEnumerator<TElem, TPredicate>(_source->getEnumerator(), _predicate));
			}
		};

		template<typename TPredicate>
		class _TakeWhileBuilder
		{
		private:
			TPredicate _predicate;
		public:
			_TakeWhileBuilder(TPredicate predicate) : _predicate(predicate) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_TakeWhileEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_TakeWhileEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_TakeWhileEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}
		};

		template<typename TElem>
		class _TakeEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			int _maxItems;
		public:
			_TakeEnumerator(std::shared_ptr<IEnumerator<TElem>> source, int maxItems)
				: _source(source), _maxItems(maxItems)
			{}

			bool next() override
			{
				if (--_maxItems >= 0 && _source->next())
					return true;
				return false;
			}

			TElem current() override
			{
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_TakeEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_maxItems == pother->_maxItems &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _TakeEnumerator<TElem>(this->_source->clone(), this->_maxItems));
			}
		};

		template<typename TElem>
		class _TakeEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _source;
			int _maxItems;
		public:
			_TakeEnumerable(std::shared_ptr<IEnumerable<TElem>> source, int maxItems)
				: _source(source), _maxItems(maxItems)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _TakeEnumerator<TElem>(_source->getEnumerator(), _maxItems));
			}
		};

		template<typename TElem>
		class _TakeRandomAccessEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerator<TElem>> _source;
			int _maxItems;
			int _index;
		public:
			_TakeRandomAccessEnumerator(std::shared_ptr<IRandomAccessEnumerator<TElem>> source, int maxItems, int index)
				: _source(source), _maxItems(maxItems), _index(index)
			{
				assert(index >= -1);
				assert(index <= maxItems);
			}

			bool advance(int step) override
			{
				if (!step) return true;

				if (((step > 0) && (_index == _maxItems)) || ((step < 0) && (_index < 0)))
					return false;

				if ((_index + step) > _maxItems)
					step = _maxItems - step;

				if (_source->advance(step))
				{
					_index += step;
					return step > 0 ? _index < _maxItems : _index > -1;
				}
				else
				{
					_index = step > 0 ? _maxItems : -1;
					return false;
				}
			}

			bool next() override 
			{
				if (_index == _maxItems) throw IterationFinishedException();
				return advance(1); 
			}

			bool back() override 
			{
				if (_index < 0 ) throw IterationNotStartedException();
				return advance(-1); 
			}

			TElem current() override
			{
				if (_index < 0) throw IterationNotStartedException();
				if (_index == _maxItems) throw IterationFinishedException();
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_TakeRandomAccessEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_maxItems == pother->_maxItems &&
					this->_index == pother->_index &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _TakeRandomAccessEnumerator<TElem>(
					std::dynamic_pointer_cast<IRandomAccessEnumerator<TElem>>(this->_source->clone()),
					this->_maxItems,
					this->_index));
			}

			int distance_to(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_TakeRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_source->distance_to(pother->_source);
			}

			bool less_than(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_TakeRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_source->less_than(pother->_source);
			}

			bool greater_than(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_TakeRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_source->greater_than(pother->_source);
			}
		};

		template<typename TElem>
		class _TakeRandomAccessEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerable<TElem>> _source;
			int _maxItems;
		public:
			_TakeRandomAccessEnumerable(std::shared_ptr<IRandomAccessEnumerable<TElem>> source, int maxItems)
				: _source(source), _maxItems(maxItems)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _TakeRandomAccessEnumerator<TElem>(_source->getEnumerator(), _maxItems, -1));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				auto index = _source->size() > _maxItems ? _maxItems : _source->size();
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _TakeRandomAccessEnumerator<TElem>(_source->getEnumeratorAt(index), _maxItems, index));
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int index) override
			{
				index = index > _maxItems ? _maxItems : index;
				index = index > _source->size() ? _source->size() : index;
				return std::shared_ptr<IRandomAccessEnumerator<TElem>>(new _TakeRandomAccessEnumerator<TElem>(_source->getEnumeratorAt(index), _maxItems, index));
			}

			int size() override
			{
				return _maxItems;
			}
		};

		class _TakeBuilder
		{
		private:
			int _maxItems;
		public:
			XLINQ_INLINE _TakeBuilder(int maxItems) : _maxItems(maxItems) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_TakeEnumerable<TElem>(enumerable, _maxItems));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_TakeEnumerable<TElem>(enumerable, _maxItems));
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IRandomAccessEnumerable<TElem>>(new internal::_TakeRandomAccessEnumerable<TElem>(enumerable, _maxItems));
			}
		};
	}
	/*@endcond*/

	/**
	*	Allows to enumerate collection as long as given condition holds.
	*	This function may be used to filter collection elements. It is possible
	*	to iterate collection as long as predicate returns true.
	*	@param predicate Function used to filter elements of source collection.
	*	@return Builder of take_while expression.
	*/
	template<typename TPredicate>
	XLINQ_INLINE internal::_TakeWhileBuilder<TPredicate> take_while(TPredicate predicate)
	{
		return internal::_TakeWhileBuilder<TPredicate>(predicate);
	}

	/**
	*	Shortens collection to have specified maximum elements.
	*	This function may be used to enumerate over contiguous elements 
	*	from the start of a sequence.
	*	@param maxItems Maximum elements to iterate over.
	*	@return Builder of take expression.
	*/
	XLINQ_INLINE internal::_TakeBuilder take(int maxItems)
	{
		return internal::_TakeBuilder(maxItems);
	}
}

#endif