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
*	@file xlinq_gather.h
*	Gathering the collection elements.
*	@author TrolleY
*/
#ifndef XLINQ_GATHER_H_
#define XLINQ_GATHER_H_

#include "xlinq_base.h"
#include "xlinq_from.h"
#include <memory>
#include <vector>
#include <list>

namespace xlinq
{
	/*@cond INTERNAL*/
	namespace internal
	{
		template<typename TElem>
		class _Gatherer
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			std::shared_ptr<std::list<TElem>> _list;

		public:
			_Gatherer(std::shared_ptr<IEnumerator<TElem>> enumerator)
				: _enumerator(enumerator), _list(new std::list<TElem>()) {}

			bool next()
			{
				if (_enumerator)
				{
					if (_enumerator->next())
					{
						_list->push_back(_enumerator->current());
						return true;
					}
					else _enumerator = nullptr;
				}
				return false;
			}

			bool finished() { return !_enumerator; }

			typename std::list<TElem>::const_iterator begin() { return _list->begin(); }

			typename std::list<TElem>::const_iterator end() { return _list->end(); }
		};

		template<typename TElem>
		class _LazyGatherEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<_Gatherer<TElem>> _gatherer;
			typename std::list<TElem>::const_iterator _it;
			bool _started;
			bool _finished;

		public:
			_LazyGatherEnumerator(std::shared_ptr<_Gatherer<TElem>> gatherer)
				: _gatherer(gatherer), _it(gatherer->begin()), _started(false), _finished(false) {}

			bool next() override
			{
				if (_finished) throw IterationFinishedException();
				auto itn = _it;
				auto ite = _gatherer->end();
				if ((_it == ite) || (_started && (++itn == ite)))
				{
					if (!_gatherer->finished() && _gatherer->next())
					{
						if (!_started)
							_it = _gatherer->begin();
						else ++_it;
						_started = true;
						return true;
					}
					_finished = true;
					_started = true;
					return false;
				}
				else if (_started)
					++_it;
				_started = true;
				return true;
			}

			TElem current() override
			{
				if (!_started) throw IterationNotStartedException();
				if (_finished) throw IterationFinishedException();
				return *_it;
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_LazyGatherEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_it == pother->_it &&
					this->_started == pother->_started &&
					this->_finished == pother->_finished;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _LazyGatherEnumerator<TElem>(_gatherer);
				ptr->_it = this->_it;
				ptr->_started = this->_started;
				ptr->_finished = this->_finished;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};

		template<typename TElem>
		class _LazyGatherEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<_Gatherer<TElem>> _gatherer;

		public:
			_LazyGatherEnumerable(std::shared_ptr<IEnumerator<TElem>> enumerator)
				: _gatherer(std::shared_ptr<_Gatherer<TElem>>(new _Gatherer<TElem>(enumerator))) {}
			
			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _LazyGatherEnumerator<TElem>(_gatherer));
			}
		};

		class _GatherBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto vec = std::shared_ptr<std::vector<TElem>>(new std::vector<TElem>());
				for (auto it = enumerable->getEnumerator(); it->next();)
				{
					vec->push_back(it->current());
				}
				return from(vec);
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};

		class _LazyGatherBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new _LazyGatherEnumerable<TElem>(enumerable->getEnumerator()));
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}

			template<typename TElem>
			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
			}
		};
	}
	/*@endcond*/

	/**
	*	Gathers elements of collection.
	*	This function allows to gather elements of collection to improve query performance,
	*	or to copy collection elements preventing from deallocating enumeration source collection.
	*	It is used to intentionally omit effects, advantages and disadvantages of deffered execution.
	*	@return Builder of gather expression.
	*/
	XLINQ_INLINE internal::_GatherBuilder gather()
	{
		return internal::_GatherBuilder();
	}

	/**
	*	Gathers elements of collection while manually executing iteration. Next time uses
	*	already stored elements ignoring source enumerable.
	*	This function allows to gather elements of collection to improve query performance.
	*	It is used to intentionally omit effects, advantages and disadvantages of deffered execution,
	*	allowing to store already quered items, when query is called second time ignoring source
	*	enumerable.
	*	@return Builder of gather expression.
	*/
	XLINQ_INLINE internal::_LazyGatherBuilder lazy_gather()
	{
		return internal::_LazyGatherBuilder();
	}
}

#endif