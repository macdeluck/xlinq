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
*	@file xlinq_from_container.h
*	Creating enumerable object from containers passed by reference.
*	@author TrolleY
*/
#ifndef XLINQ_FROM_CONTAINER_H_
#define XLINQ_FROM_CONTAINER_H_

#include <cstdlib>
#include <memory>
#include <iterator>
#include <cassert>
#include "xlinq_base.h"
#include "xlinq_exception.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TIterator, typename TElem>
		class _StlEnumerator : public IEnumerator<TElem>
		{
		protected:
			TIterator _begin, _end;
			bool _started;

			void assert_finished()
			{
				if (_started && _begin == _end)
					throw IterationFinishedException();
			}

		public:
			_StlEnumerator(TIterator begin, TIterator end) : _begin(begin), _end(end), _started(false) {}

			bool next() override
			{
				assert_finished();
				if (!_started)
					_started = true;
				else ++_begin;
				return _begin != _end;
			}

			TElem current() override
			{
				if (!_started)
				{
					throw IterationNotStartedException();
				}
				assert_finished();
				return *_begin;
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_StlEnumerator<TIterator, TElem>>(other);
				if (!pother) return false;
				return pother->_begin == this->_begin &&
					pother->_end == this->_end &&
					pother->_started == this->_started;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _StlEnumerator<TIterator, TElem>(this->_begin, this->_end);
				ptr->_started = this->_started;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};

		template<typename TIterator, typename TElem>
		class _StlBidirectionalEnumerator : public IBidirectionalEnumerator<TElem>
		{
		protected:
			TIterator _begin, _end, _current;
			bool _started;

			void assert_finished()
			{
				if (_started && _current == _end)
					throw IterationFinishedException();
			}

			void assert_started()
			{
				if (!_started)
					throw IterationNotStartedException();
			}

		public:
			_StlBidirectionalEnumerator(TIterator begin, TIterator end, bool atBegin = true) : _begin(begin), _end(end), _current(atBegin ? begin : end), _started(!atBegin) {}

			bool next() override
			{
				assert_finished();
				if (!_started)
					_started = true;
				else ++_current;
				return _current != _end;
			}

			bool back() override
			{
				assert_started();
				if (_current != _begin)
					--_current;
				else _started = false;
				return _started;
			}

			TElem current() override
			{
				assert_started();
				assert_finished();
				return *_current;
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_StlBidirectionalEnumerator<TIterator, TElem>>(other);
				if (!pother) return false;
				return pother->_begin == this->_begin &&
					pother->_current == this->_current &&
					pother->_end == this->_end &&
					pother->_started == this->_started;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _StlBidirectionalEnumerator<TIterator, TElem>(this->_begin, this->_end);
				ptr->_current = this->_current;
				ptr->_started = this->_started;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};

		template<typename TIterator, typename TElem>
		class _StlRandomAccessEnumerator : public IRandomAccessEnumerator<TElem>
		{
		protected:
			TIterator _begin, _end, _current;
			bool _started;

			void assert_finished()
			{
				if (_started && _current == _end)
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
				if (_current == _end && _started)
					return false;

				if (!_started)
				{
					_started = true;
					step--;
				}
				auto dist = std::distance(_current, _end);
				if (step < dist)
				{
					_current += step;
					return true;
				}
				else
				{
					_current = _end;
					return false;
				}
			}

			bool advance_backward(int step)
			{
				assert(step < 0);
				if (_current == _begin && !_started)
					return false;

				auto dist = std::distance(_current, _begin);
				if (step > dist)
				{
					_current += step;
					return true;
				}
				else if (step == dist)
				{
					_current = _begin;
					return true;
				}
				else
				{
					_started = false;
					_current = _begin;
					return false;
				}
			}

		public:
			_StlRandomAccessEnumerator(TIterator begin, TIterator end) : _begin(begin), _end(end), _current(begin), _started(false) {}
			_StlRandomAccessEnumerator(TIterator begin, TIterator end, TIterator current) : _begin(begin), _end(end), _current(current), _started(true) {}

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

			TElem current() override
			{
				assert_started();
				assert_finished();
				return *_current;
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<_StlRandomAccessEnumerator<TIterator, TElem>>(other);
				if (!pother) return false;
				return pother->_begin == this->_begin &&
					pother->_current == this->_current &&
					pother->_end == this->_end &&
					pother->_started == this->_started;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new _StlRandomAccessEnumerator<TIterator, TElem>(this->_begin, this->_end, this->_current);
				ptr->_started = this->_started;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};

		template<typename TContainer, typename TElem>
		class _StlEnumerable : public IEnumerable<TElem>
		{
		private:
			TContainer& _container;
		public:
			_StlEnumerable(TContainer& container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlEnumerator<iterator, TElem>(_container.begin(), _container.end()));
			}
		};

		template<typename TContainer, typename TElem>
		class _StlBidirectionalEnumerable : public IBidirectionalEnumerable<TElem>
		{
		private:
			TContainer& _container;
		public:
			_StlBidirectionalEnumerable(TContainer& container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlBidirectionalEnumerator<iterator, TElem>(_container.begin(), _container.end()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _StlBidirectionalEnumerator<iterator, TElem>(_container.begin(), _container.end(), false));
			}
		};

		template<typename TContainer, typename TElem>
		class _StlRandomAccessEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			TContainer& _container;
		public:
			_StlRandomAccessEnumerable(TContainer& container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlRandomAccessEnumerator<iterator, TElem>(_container.begin(), _container.end()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new _StlRandomAccessEnumerator<iterator, TElem>(_container.begin(), _container.end(), _container.end()));
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				auto result = this->getEnumerator();
				result->advance(elementIndex + 1);
				return result;
			}

			int size() override
			{
				return (int)_container.size();
			}
		};

		template<typename iterator_tag, typename TContainer, typename TElem>
		struct StlEnumerableSelectorHelper
		{
		};

		template<typename TContainer, typename TElem>
		struct StlEnumerableSelectorHelper<std::forward_iterator_tag, TContainer, TElem>
		{
			typedef _StlEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlEnumerableSelectorHelper<std::bidirectional_iterator_tag, TContainer, TElem>
		{
			typedef _StlBidirectionalEnumerable<TContainer, TElem> enumerable;
		};
		
		template<typename TContainer, typename TElem>
		struct StlEnumerableSelectorHelper<std::random_access_iterator_tag, TContainer, TElem>
		{
			typedef _StlRandomAccessEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct StlEnumerableSelector
		{
			typedef typename std::iterator_traits<typename TContainer::iterator> traits;
			typedef typename StlEnumerableSelectorHelper<typename traits::iterator_category, TContainer, typename TContainer::value_type>::enumerable type;
		};
	}
	/*@endcond*/
	
	/**
	*	Creates enumerable from STL container.
	*	This function may be used to create enumerable from STL container.
	*	It is implemented using deffered execution so traversing of the container
	*	will wait until its next element will be requested. Please note, that
	*	enumeration will fail if container will be deallocated.
	*	@param container Source container.
	*	@return Enumerable from container.
	*/
	template<typename TContainer>
	auto from(TContainer& container) -> std::shared_ptr<typename internal::EnumerableTypeSelector<typename internal::StlEnumerableSelector<TContainer, typename TContainer::value_type>::type>::type>
	{
		return std::shared_ptr<typename internal::EnumerableTypeSelector<typename internal::StlEnumerableSelector<TContainer, typename TContainer::value_type>::type>::type>(new typename internal::StlEnumerableSelector<TContainer, typename TContainer::value_type>::type(container));
	}
}

#endif