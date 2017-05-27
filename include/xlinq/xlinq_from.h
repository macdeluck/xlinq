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
				if (step > 0)
				{
					if (_current == _end)
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
				else
				{
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

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlEnumerator<iterator, TElem>(_container.begin(), _container.end()));
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
				auto result = this->getEnumerator();
				result->advance(_container.size() + 1);
				return result;
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				auto result = this->getEnumerator();
				result->advance(elementIndex + 1);
				return result;
			}
		};

		template<typename iterator_tag, typename TContainer, typename TElem>
		struct stl_enumerable_selector_helper
		{
			typedef _StlEnumerable<TContainer, TElem> enumerable;
		};
		
		template<typename TContainer, typename TElem>
		struct stl_enumerable_selector_helper<std::random_access_iterator_tag, TContainer, TElem>
		{
			typedef _StlRandomAccessEnumerable<TContainer, TElem> enumerable;
		};

		template<typename TContainer, typename TElem>
		struct stl_enumerable_selector
		{
			typedef typename std::iterator_traits<typename TContainer::iterator> traits;
			typedef typename stl_enumerable_selector_helper<typename traits::iterator_category, TContainer, typename TContainer::value_type>::enumerable type;
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

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				typedef typename TContainer::iterator iterator;
				return std::shared_ptr<IEnumerator<TElem>>(new _StlPointerEnumerator<TContainer, iterator, TElem>(_container, _container->begin(), _container->end()));
			}
		};

		template<typename TElem>
		class _ArrayEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			TElem* _begin;
			size_t _size;
			size_t _index;
			bool _started;

			void assert_finished()
			{
				if (_index == _size)
					throw IterationFinishedException();
			}

			void assert_started()
			{
				if (!_started)
					throw IterationNotStartedException();
			}

		public:
			_ArrayEnumerator(TElem* begin, size_t size) : _begin(begin), _size(size), _index(0), _started(false) {}

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
				if (step > 0)
				{
					if (_index == _size)
						return false;

					if (!_started)
					{
						_started = true;
						step--;
					}
					auto dist = _size - _index;
					if ((size_t)step < dist)
					{
						_index += step;
						return true;
					}
					else
					{
						_index = _size;
						return false;
					}
				}
				else
				{
					if ((_index == 0) && !_started)
						return false;
					step = -step;
					if ((size_t)step < _index)
					{
						_index -= step;
						return true;
					}
					else if ((size_t)step == _index)
					{
						_index = 0;
						return true;
					}
					else
					{
						_started = false;
						_index = 0;
						return false;
					}
				}
			}

			TElem current()
			{
				assert_started();
				assert_finished();
				return *(_begin + _index);
			}
		};

		template<typename TElem>
		class _ArrayEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			TElem* _array;
			size_t _size;
		public:
			_ArrayEnumerable(TElem* array, size_t size) : _array(array), _size(size) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new _ArrayEnumerator<TElem>(_array, _size));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				auto result = this->getEnumerator();
				result->advance(_size + 1);
				return result;
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				auto result = this->getEnumerator();
				result->advance(elementIndex + 1);
				return result;
			}
		};

		template<typename TEnumerable, typename TElemType>
		struct EnumerableOnlyFilterHelper
		{
			typedef typename EnumerableTypeSelector<TEnumerable>::type type;
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
	std::shared_ptr<IRandomAccessEnumerable<TElem>> from(TElem(&array)[SIZE])
	{
		return std::shared_ptr<IRandomAccessEnumerable<TElem>>(new internal::_ArrayEnumerable<TElem>((TElem*)array, SIZE));
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
	*	This function returns cleaned type from given enumerable.
	*	@param enumerable Source enumerable
	*	@return Given pointer.
	*/
	template<typename TEnumerable>
	std::shared_ptr<typename internal::EnumerableOnlyFilterHelper<TEnumerable, typename TEnumerable::ElemType>::type> from(std::shared_ptr<TEnumerable> enumerable)
	{
		return (std::shared_ptr<typename internal::EnumerableOnlyFilterHelper<TEnumerable, typename TEnumerable::ElemType>::type>)enumerable;
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
	auto from(TContainer& container) -> std::shared_ptr<typename internal::stl_enumerable_selector<TContainer, typename TContainer::value_type>::type>
	{
		return std::shared_ptr<typename internal::stl_enumerable_selector<TContainer, typename TContainer::value_type>::type>(new typename internal::stl_enumerable_selector<TContainer, typename TContainer::value_type>::type(container));
	}
}

#endif