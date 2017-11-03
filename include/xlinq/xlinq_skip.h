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
*	@file xlinq_skip.h
*	Skipping collection elements.
*	@author TrolleY
*/
#ifndef XLINQ_SKIP_H_
#define XLINQ_SKIP_H_

#include <memory>
#include <cassert>
#include "xlinq_base.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem, typename TPredicate>
		class _SkipWhileEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			bool _omitPredicate;
			TPredicate _predicate;
		public:
			_SkipWhileEnumerator(std::shared_ptr<IEnumerator<TElem>> source, TPredicate predicate)
				: _source(source), _omitPredicate(false), _predicate(predicate)
			{}

			bool next() override
			{
				do
				{
					if (!_source || !_source->next())
					{
						_source = nullptr;
						return false;
					}
				} while (!_omitPredicate && _predicate(_source->current()));
				_omitPredicate = true;
				return true;
			}

			TElem current() override
			{
				if (!_source) throw IterationFinishedException();
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SkipWhileEnumerator<TElem, TPredicate>>(other);
				if (!pother)
					return false;
				return this->_omitPredicate == pother->_omitPredicate &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _SkipWhileEnumerator<TElem, TPredicate>(this->_source->clone(), this->_predicate);
				ptr->_omitPredicate = this->_omitPredicate;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};

		template<typename TElem, typename TPredicate>
		class _SkipWhileEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _source;
			TPredicate _predicate;
		public:
			_SkipWhileEnumerable(std::shared_ptr<IEnumerable<TElem>> source, TPredicate predicate)
				: _source(source), _predicate(predicate)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _SkipWhileEnumerator<TElem, TPredicate>(_source->getEnumerator(), _predicate));
			}
		};

		template<typename TPredicate>
		class _SkipWhileBuilder
		{
		private:
			TPredicate _predicate;
		public:
			_SkipWhileBuilder(TPredicate predicate) : _predicate(predicate) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_SkipWhileEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_SkipWhileEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_SkipWhileEnumerable<TElem, TPredicate>(enumerable, _predicate));
			}
		};

		template<typename TElem>
		class _SkipEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			int _items;
		public:
			_SkipEnumerator(std::shared_ptr<IEnumerator<TElem>> source, int items)
				: _source(source), _items(items)
			{}

			bool next() override
			{
				while (_items > 0)
				{
					if (!_source->next())
					{
						_items = 0;
						return false;
					}
					--_items;
				}

				return _source->next();
			}

			TElem current() override
			{
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SkipEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_items == pother->_items &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _SkipEnumerator<TElem>(this->_source->clone(), this->_items));
			}
		};

		template<typename TElem>
		class _SkipEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _source;
			int _items;
		public:
			_SkipEnumerable(std::shared_ptr<IEnumerable<TElem>> source, int items)
				: _source(source), _items(items)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _SkipEnumerator<TElem>(_source->getEnumerator(), _items));
			}
		};

		template<typename TElem>
		class _SkipRandomAccessEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerator<TElem>> _source;
			int _size;
			int _items;
			int _index;
		public:
			_SkipRandomAccessEnumerator(std::shared_ptr<IRandomAccessEnumerator<TElem>> source, int size, int items, int index)
				: _source(source), _size(size), _items(items), _index(index)
			{
				assert(size >= 0);
				assert(index >= -1);
				assert(index >= _items - 1);
			}

			bool advance(int step) override
			{
				if (!step) return true;
				int newIndex = _index + step;
				if (newIndex >= _size)
				{
					_source->advance(_size - _index);
					_index = _size;
					return false;
				}
				if (newIndex < _items)
				{
					_source->advance(_items - 1 - _index);
					_index = _items - 1;
					return false;
				}
				_source->advance(step);
				_index = newIndex;
				return true;
			}

			bool next() override 
			{
				if (_index == _size) throw IterationFinishedException();
				return advance(1); 
			}

			bool back() override 
			{
				if (_index < _items ) throw IterationNotStartedException();
				return advance(-1); 
			}

			TElem current() override
			{
				if (_index < _items) throw IterationNotStartedException();
				if (_index == _size) throw IterationFinishedException();
				return _source->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SkipRandomAccessEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_size == pother->_size &&
					this->_items == pother->_items &&
					this->_index == pother->_index &&
					this->_source->equals(pother->_source);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _SkipRandomAccessEnumerator<TElem>(
					std::dynamic_pointer_cast<IRandomAccessEnumerator<TElem>>(this->_source->clone()),
					this->_size,
					this->_items,
					this->_index));
			}

			int distance_to(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SkipRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_source->distance_to(pother->_source);
			}

			bool less_than(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SkipRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_source->less_than(pother->_source);
			}

			bool greater_than(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_SkipRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_source->greater_than(pother->_source);
			}
		};

		template<typename TElem>
		class _SkipRandomAccessEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerable<TElem>> _source;
			int _items;
		public:
			_SkipRandomAccessEnumerable(std::shared_ptr<IRandomAccessEnumerable<TElem>> source, int items)
				: _source(source), _items(items)
			{}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _SkipRandomAccessEnumerator<TElem>(_source->getEnumeratorAt(_items - 1), _source->size(), _items, _items - 1));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				auto index = _source->size();
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _SkipRandomAccessEnumerator<TElem>(_source->getEndEnumerator(), _source->size(), _items, index));
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int index) override
			{
				index += _items;
				index = index > _source->size() ? _source->size() : index;
				return std::shared_ptr<IRandomAccessEnumerator<TElem>>(new _SkipRandomAccessEnumerator<TElem>(_source->getEnumeratorAt(index), _source->size(), _items, index));
			}

			int size() override
			{
				return _source->size() - _items;
			}
		};

		class _SkipBuilder
		{
		private:
			int _items;
		public:
			XLINQ_INLINE _SkipBuilder(int items) : _items(items) {}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_SkipEnumerable<TElem>(enumerable, _items));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new internal::_SkipEnumerable<TElem>(enumerable, _items));
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IRandomAccessEnumerable<TElem>>(new internal::_SkipRandomAccessEnumerable<TElem>(enumerable, _items));
			}
		};
	}
	/*@endcond*/

	/**
	*	Skips collection elements until first element not satisfying predicate occurs.
	*	This function may be used to filter collection elements. It allows
	*	to ommit collection elements as long as predicate returns true.
	*	@param predicate Function used to filter elements of source collection.
	*	@return Builder of skip_while expression.
	*/
	template<typename TPredicate>
	XLINQ_INLINE internal::_SkipWhileBuilder<TPredicate> skip_while(TPredicate predicate)
	{
		return internal::_SkipWhileBuilder<TPredicate>(predicate);
	}

	/**
	*	Shortens collection by skipping specified number of elements from beginning of collection.
	*	This function may be used to enumerate over contiguous elements ommiting few first elements.
	*	@param items Number of elements to skip.
	*	@return Builder of skip expression.
	*/
	XLINQ_INLINE internal::_SkipBuilder skip(int items)
	{
		return internal::_SkipBuilder(items);
	}
}

#endif