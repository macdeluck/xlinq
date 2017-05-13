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
*	@file xlinq_from.h
*	Creating enumerable object from containers and arrays.
*	@author TrolleY
*/
#ifndef XLINQ_FROM_H_
#define XLINQ_FROM_H_

#include <cstdlib>
#include <memory>
#include <iterator>
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
		private:
			TIterator _begin, _end;
			bool _started;

			void assert_finished()
			{
				if (_begin == _end)
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
		};

		template<typename TIterator, typename TElem>
		class _StlRandomAccessEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			TIterator _begin, _end, _current;
			bool _started;

			void assert_finished()
			{
				if (_current == _end)
					throw IterationFinishedException();
			}

			void assert_started()
			{
				if (!_started)
					throw IterationNotStartedException();
			}

		public:
			_StlRandomAccessEnumerator(TIterator begin, TIterator end) : _begin(begin), _end(end), _current(begin), _started(false) {}

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
				if (_current == _begin)
				{
					_started = false;
					return false;
				}
				--_current;
				return true;
			}

			bool advance(int step) override
			{
				if (!step) return true;
				if (step > 0)
				{
					if (_current == _end)
						return false;

					if (!_started)
					{
						_started = true;
						step--;
					}
					if (step < std::distance(_end, _current))
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
				else
				{
					if (_current == _begin && !_started)
						return false;
					if (step < std::distance(_begin, _current))
					{
						_current += step;
						return true;
					}
					else if (step == std::distance(_begin, _current))
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
			}

			TElem current() override
			{
				assert_started();
				assert_finished();
				return *_current;
			}
		};

		template<typename TContainer, typename TElem>
		class _StlEnumerable : public IEnumerable<TElem>
		{
		private:
			TContainer& _container;
		public:
			_StlEnumerable(TContainer& container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlEnumerator<iterator, TElem>(_container.begin(), _container.end()));
			}
		};

		template<typename TContainer, typename TElem>
		class _StlRandomAccessEnumerable : public IEnumerable<TElem>
		{
		private:
			TContainer& _container;
		public:
			_StlRandomAccessEnumerable(TContainer& container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlRandomAccessEnumerator<iterator, TElem>(_container.begin(), _container.end()));
			}
		};

		template<typename iterator_tag, typename TContainer, typename TElem>
		struct stl_enumerable_selector
		{
		public:
			typedef _StlEnumerable<TContainer, TElem> enumerable;
		};
		
		template<typename TContainer, typename TElem>
		struct stl_enumerable_selector<std::random_access_iterator_tag, TContainer, TElem>
		{
		public:
			typedef _StlRandomAccessEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TIterator, typename TElem>
		class _StlPointerEnumerator : public _StlEnumerator<TIterator, TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerEnumerator(std::shared_ptr<TContainer> container, TIterator begin, TIterator end)
				: _StlEnumerator<TIterator, TElem>(begin, end), _container(container) {}
		};

		template<typename TContainer, typename TElem>
		class _StlPointerEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<TContainer> _container;
		public:
			_StlPointerEnumerable(std::shared_ptr<TContainer> container) : _container(container) {}

			std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlPointerEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end()));
			}
		};

		template<typename TElem, size_t SIZE>
		class _ArrayEnumerator : public IEnumerator<TElem>
		{
		private:
			TElem* _begin;
			size_t _index;
			bool _started;

			void assert_finished()
			{
				if (_index == SIZE)
					throw IterationFinishedException();
			}

		public:
			_ArrayEnumerator(TElem* begin) : _begin(begin), _index(0), _started(false) {}

			bool next() override
			{
				assert_finished();
				if (!_started)
					_started = true;
				else
				{
					++_begin;
					++_index;
				}
				return _index != SIZE;
			}

			TElem current()
			{
				if (!_started) throw IterationNotStartedException();
				assert_finished();
				return *_begin;
			}
		};

		template<typename TElem, size_t SIZE>
		class _ArrayEnumerable : public IEnumerable<TElem>
		{
		private:
			TElem(&_array)[SIZE];
		public:
			_ArrayEnumerable(TElem(&array)[SIZE]) : _array(array) {}

			std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ArrayEnumerator<TElem, SIZE>((TElem*)_array));
			}
		};
	}
	/*@endcond*/

	/**
	*	Creates enumerable from fixed size array.
	*	This function may be used to create enumerable from fixed size array.
	*	It is implemented using deffered execution so traversing of the array
	*	will wait until its next element will be requested. Please note, that
	*	enumeration will fail if array will be deallocated.
	*	@param array Source fixed size array.
	*	@return Enumerable from array.
	*/
	template<typename TElem, size_t SIZE>
	std::shared_ptr<IEnumerable<TElem>> from(TElem(&array)[SIZE])
	{
		return std::shared_ptr<IEnumerable<TElem>>(new internal::_ArrayEnumerable<TElem, SIZE>(array));
	}

	/**
	*	Creates enumerable from shared pointer to STL container.
	*	This function may be used to create enumerable from pointer to STL container.
	*	It is implemented using deffered execution so traversing of the collection
	*	will wait until its next element will be requested.
	*	@param container Source STL container.
	*	@return Enumerable from container.
	*/
	template<typename TContainer>
	auto from(std::shared_ptr<TContainer> container) -> std::shared_ptr<IEnumerable<typename TContainer::value_type>>
	{
		return std::shared_ptr<IEnumerable<typename TContainer::value_type>>(new internal::_StlPointerEnumerable<TContainer, typename TContainer::value_type>(container));
	}

	/**
	*	Creates enumerable from shared pointer to enumerable.
	*	This function just returns given enumerable.
	*	@param enumerable Source enumerable
	*	@return Given pointer.
	*/
	template<typename TEnumerable>
	auto from(std::shared_ptr<TEnumerable> enumerable) -> std::shared_ptr<IEnumerable<typename TEnumerable::ElemType>>
	{
		return (std::shared_ptr<IEnumerable<typename TEnumerable::ElemType>>)enumerable;
	}

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
	auto from(TContainer& container) -> std::shared_ptr<IEnumerable<typename TContainer::value_type>>
	{
		typedef typename std::iterator_traits<typename TContainer::iterator> traits;
		typedef typename internal::stl_enumerable_selector<typename traits::iterator_category, TContainer, typename TContainer::value_type> selector;
		return std::shared_ptr<IEnumerable<typename TContainer::value_type>>(new typename selector::enumerable(container));
	}
}

#endif