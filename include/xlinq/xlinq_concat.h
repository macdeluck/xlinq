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
*	@file xlinq_concat.h
*	Concatenation of expressions.
*	@author TrolleY
*/
#ifndef XLINQ_CONCAT_H_
#define XLINQ_CONCAT_H_

#include <memory>
#include <cassert>
#include "xlinq_base.h"
#include "xlinq_from.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TElem>
		class ConcatEnumerator : public IEnumerator<TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _first;
			std::shared_ptr<IEnumerator<TElem>> _second;
			bool _firstFinished;

		public:
			ConcatEnumerator(std::shared_ptr<IEnumerator<TElem>> first, std::shared_ptr<IEnumerator<TElem>> second)
				: _first(first), _second(second), _firstFinished(false) {}

			bool next() override
			{
				if (!_firstFinished)
				{
					if (_first->next())
						return true;
					_firstFinished = true;
				}
				return _second->next();
			}

			TElem current() override
			{
				return _firstFinished ? _second->current() : _first->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<ConcatEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_first->equals(pother->_first) && 
					this->_firstFinished == pother->_firstFinished &&
					this->_second->equals(pother->_second);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new ConcatEnumerator<TElem>(this->_first->clone(), this->_second->clone());
				ptr->_firstFinished = this->_firstFinished;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}
		};

		template<typename TElem>
		class ConcatBidirectionalEnumerator : public IBidirectionalEnumerator<TElem>
		{
		private:
			std::shared_ptr<IBidirectionalEnumerator<TElem>> _first;
			std::shared_ptr<IBidirectionalEnumerator<TElem>> _second;
			bool _firstFinished;

		public:
			ConcatBidirectionalEnumerator(std::shared_ptr<IBidirectionalEnumerator<TElem>> first, std::shared_ptr<IBidirectionalEnumerator<TElem>> second, bool atEnd)
				: _first(first), _second(second), _firstFinished(atEnd) {}

			bool next() override
			{
				if (!_firstFinished)
				{
					if (_first->next())
						return true;
					_firstFinished = true;
				}
				return _second->next();
			}

			bool back() override
			{
				if (_firstFinished)
				{
					if (_second->back())
						return true;
					_firstFinished = false;
				}
				return _first->back();
			}

			TElem current() override
			{
				return _firstFinished ? _second->current() : _first->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<ConcatBidirectionalEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_first->equals(pother->_first) &&
					this->_firstFinished == pother->_firstFinished &&
					this->_second->equals(pother->_second);
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new ConcatBidirectionalEnumerator<TElem>(
					std::dynamic_pointer_cast<IBidirectionalEnumerator<TElem>>(this->_first->clone()), 
					std::dynamic_pointer_cast<IBidirectionalEnumerator<TElem>>(this->_second->clone()), 
					this->_firstFinished));
			}
		};

		template<typename TElem>
		class ConcatRandomAccessEnumerator : public IRandomAccessEnumerator<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerator<TElem>> _first;
			std::shared_ptr<IRandomAccessEnumerator<TElem>> _second;
			int _firstSize;
			int _secondSize;
			int _index;

		public:
			ConcatRandomAccessEnumerator(std::shared_ptr<IRandomAccessEnumerator<TElem>> first, std::shared_ptr<IRandomAccessEnumerator<TElem>> second, int firstSize, int secondSize)
				: _first(first), _second(second), _firstSize(firstSize), _secondSize(secondSize), _index(-1) {}

			bool next() override
			{
				if (_index < _firstSize)
				{
					if (_first->next())
					{
						++_index;
						return true;
					}
				}
				if (_second->next())
				{
					++_index;
					return true;
				}
				++_index;
				return false;
			}

			bool back() override
			{
				if (_index >= _firstSize)
				{
					if (_second->back())
					{
						--_index;
						return true;
					}
				}
				if (_first->back())
				{
					--_index;
					return true;
				}
				--_index;
				return false;
			}

			bool advance_forward(int step)
			{
				int stepLeft = step;
				int cindex = _index;
				if ((_index + step) < _firstSize + _secondSize)
					_index += step;
				else _index = _firstSize + _secondSize;

				if (cindex < _firstSize)
				{
					if (_first->advance(stepLeft))
						return true;
					stepLeft = step - (_firstSize - cindex) + 1;
				}
				return _second->advance(stepLeft) && step != 0;
			}

			bool advance_backward(int step)
			{
				int stepLeft = step;
				int cindex = _index;
				if ((_index + step) >= 0)
					_index += step;
				else _index = -1;

				if (cindex >= _firstSize)
				{
					if (_second->advance(stepLeft))
						return true;
					stepLeft = step + (cindex - _firstSize);
				}
				return _first->advance(stepLeft) && step != 0;
			}

			bool advance(int step) override
			{
				if (!step) return true;
				return step > 0 ? advance_forward(step) : advance_backward(step);
			}

			TElem current() override
			{
				return _index < _firstSize ? _first->current() : _second->current();
			}

			bool equals(std::shared_ptr<IEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<ConcatRandomAccessEnumerator<TElem>>(other);
				if (!pother)
					return false;
				return this->_first->equals(pother->_first) &&
					this->_second->equals(pother->_second) &&
					this->_firstSize == pother->_firstSize &&
					this->_secondSize == pother->_secondSize &&
					this->_index == pother->_index;
			}

			std::shared_ptr<IEnumerator<TElem>> clone() const override
			{
				auto ptr = new ConcatRandomAccessEnumerator<TElem>(
					std::dynamic_pointer_cast<IRandomAccessEnumerator<TElem>>(this->_first->clone()),
					std::dynamic_pointer_cast<IRandomAccessEnumerator<TElem>>(this->_second->clone()),
					this->_firstSize, 
					this->_secondSize);
				ptr->_index = this->_index;
				return std::shared_ptr<IEnumerator<TElem>>(ptr);
			}

			int distance_to(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<ConcatRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return pother->_index - this->_index;
			}

			bool less_than(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<ConcatRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_index < pother->_index;
			}

			bool greater_than(std::shared_ptr<IRandomAccessEnumerator<TElem>> other) const override
			{
				auto pother = std::dynamic_pointer_cast<ConcatRandomAccessEnumerator<TElem>>(other);
				assert(pother);
				return this->_index > pother->_index;
			}
		};

		template<typename TElem>
		class ConcatEnumerable : public IEnumerable<TElem>
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _first;
			std::shared_ptr<IEnumerable<TElem>> _second;

		public:
			ConcatEnumerable(std::shared_ptr<IEnumerable<TElem>> first, std::shared_ptr<IEnumerable<TElem>> second)
				: _first(first), _second(second) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new ConcatEnumerator<TElem>(_first->getEnumerator(), _second->getEnumerator()));
			}
		};

		template<typename TElem>
		class ConcatBidirectionalEnumerable : public IBidirectionalEnumerable<TElem>
		{
		private:
			std::shared_ptr<IBidirectionalEnumerable<TElem>> _first;
			std::shared_ptr<IBidirectionalEnumerable<TElem>> _second;

		public:
			ConcatBidirectionalEnumerable(std::shared_ptr<IBidirectionalEnumerable<TElem>> first, std::shared_ptr<IBidirectionalEnumerable<TElem>> second)
				: _first(first), _second(second) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new ConcatBidirectionalEnumerator<TElem>(_first->getEnumerator(), _second->getEnumerator(), false));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				return std::shared_ptr<IBidirectionalEnumerator<TElem>>(new ConcatBidirectionalEnumerator<TElem>(_first->getEndEnumerator(), _second->getEndEnumerator(), true));
			}
		};

		template<typename TElem>
		class ConcatRandomAccessEnumerable : public IRandomAccessEnumerable<TElem>
		{
		private:
			std::shared_ptr<IRandomAccessEnumerable<TElem>> _first;
			std::shared_ptr<IRandomAccessEnumerable<TElem>> _second;

		public:
			ConcatRandomAccessEnumerable(std::shared_ptr<IRandomAccessEnumerable<TElem>> first, std::shared_ptr<IRandomAccessEnumerable<TElem>> second)
				: _first(first), _second(second) {}

			std::shared_ptr<IEnumerator<TElem>> createEnumerator() override
			{
				return std::shared_ptr<IEnumerator<TElem>>(new ConcatRandomAccessEnumerator<TElem>(_first->getEnumerator(), _second->getEnumerator(), _first->size(), _second->size()));
			}

			std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() override
			{
				auto enumerator = std::shared_ptr<IRandomAccessEnumerator<TElem>>(new ConcatRandomAccessEnumerator<TElem>(_first->getEnumerator(), _second->getEnumerator(), _first->size(), _second->size()));
				enumerator->advance(_first->size() + _second->size() + 1);
				return enumerator;
			}

			std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) override
			{
				auto enumerator = std::shared_ptr<IRandomAccessEnumerator<TElem>>(new ConcatRandomAccessEnumerator<TElem>(_first->getEnumerator(), _second->getEnumerator(), _first->size(), _second->size()));
				enumerator->advance(elementIndex + 1);
				return enumerator;
			}

			int size() override
			{
				return _first->size() + _second->size();
			}
		};

		template<typename TElem>
		class _ConcatRandomAccessBuilder
		{
		private:
			std::shared_ptr<IRandomAccessEnumerable<TElem>> _source;

		public:
			_ConcatRandomAccessBuilder(std::shared_ptr<IRandomAccessEnumerable<TElem>> source) : _source(source) {}

			std::shared_ptr<IRandomAccessEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IRandomAccessEnumerable<TElem>>(new ConcatRandomAccessEnumerable<TElem>(enumerable, _source));
			}

			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IBidirectionalEnumerable<TElem>>(new ConcatBidirectionalEnumerable<TElem>(enumerable, _source));
			}

			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new ConcatEnumerable<TElem>(enumerable, _source));
			}
		};

		template<typename TElem>
		class _ConcatBidirectionalBuilder
		{
		private:
			std::shared_ptr<IBidirectionalEnumerable<TElem>> _source;

		public:
			_ConcatBidirectionalBuilder(std::shared_ptr<IBidirectionalEnumerable<TElem>> source) : _source(source) {}

			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IBidirectionalEnumerable<TElem>>(new ConcatBidirectionalEnumerable<TElem>(enumerable, _source));
			}

			std::shared_ptr<IBidirectionalEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IBidirectionalEnumerable<TElem>>(new ConcatBidirectionalEnumerable<TElem>(enumerable, _source));
			}

			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new ConcatEnumerable<TElem>(enumerable, _source));
			}
		};

		template<typename TElem>
		class _ConcatBuilder
		{
		private:
			std::shared_ptr<IEnumerable<TElem>> _source;

		public:
			_ConcatBuilder(std::shared_ptr<IEnumerable<TElem>> source) : _source(source) {}

			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new ConcatEnumerable<TElem>(enumerable, _source));
			}

			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new ConcatEnumerable<TElem>(enumerable, _source));
			}

			std::shared_ptr<IEnumerable<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return std::shared_ptr<IEnumerable<TElem>>(new ConcatEnumerable<TElem>(enumerable, _source));
			}
		};

		template<typename TEnumerable, typename TElem>
		struct ConcatBuilderSelectorHelper
		{
		};

		template<typename TElem>
		struct ConcatBuilderSelectorHelper<std::shared_ptr<IEnumerable<TElem>>, TElem>
		{
			typedef _ConcatBuilder<TElem> builder;
		};

		template<typename TElem>
		struct ConcatBuilderSelectorHelper<std::shared_ptr<IBidirectionalEnumerable<TElem>>, TElem>
		{
			typedef _ConcatBidirectionalBuilder<TElem> builder;
		};

		template<typename TElem>
		struct ConcatBuilderSelectorHelper<std::shared_ptr<IRandomAccessEnumerable<TElem>>, TElem>
		{
			typedef _ConcatRandomAccessBuilder<TElem> builder;
		};

		template<typename TContainer, typename TElem>
		struct ConcatBuilderSelector
		{
			typedef typename ConcatBuilderSelectorHelper<decltype(from(std::declval<TContainer*>())), TElem>::builder type;
		};
	}
	/*@endcond*/
	
	/**
	*	Function concatenating array with other enumerable into single collection.
	*	@param enumerable Enumerable which will be concatenated.
	*	@return Builder of concat expression.
	*/
	template<typename TElem, int SIZE>
	internal::_ConcatRandomAccessBuilder<TElem> concat(TElem(&array)[SIZE])
	{
		return internal::_ConcatRandomAccessBuilder<TElem>(from(array));
	}

	/**
	*	Function concatenating container with other enumerable into single collection.
	*	@param enumerable Enumerable which will be concatenated.
	*	@return Builder of concat expression.
	*/
	template<typename TContainer>
	typename internal::ConcatBuilderSelector<TContainer, typename TContainer::value_type>::type concat(TContainer& container)
	{
		return typename internal::ConcatBuilderSelector<TContainer, typename TContainer::value_type>::type(from(container));
	}

	/**
	*	Function concatenating container pointer with other enumerable into single collection.
	*	@param enumerable Enumerable which will be concatenated.
	*	@return Builder of concat expression.
	*/
	template<typename TContainer>
	typename internal::ConcatBuilderSelector<TContainer, typename TContainer::value_type>::type concat(TContainer* container)
	{
		return typename internal::ConcatBuilderSelector<TContainer, typename TContainer::value_type>::type(from(container));
	}

	/**
	*	Function concatenating container shared pointer with other enumerable into single collection.
	*	@param enumerable Enumerable which will be concatenated.
	*	@return Builder of concat expression.
	*/
	template<typename TContainer>
	typename internal::ConcatBuilderSelector<TContainer, typename TContainer::value_type>::type concat(std::shared_ptr<TContainer> container)
	{
		return typename internal::ConcatBuilderSelector<TContainer, typename TContainer::value_type>::type(from(container));
	}

	/**
	*	Function concatenating two enumerables into single collection.
	*	@param enumerable Enumerable which will be concatenated.
	*	@return Builder of concat expression.
	*/
	template<typename TEnumerable>
	auto concat(std::shared_ptr<TEnumerable> enumerable) -> typename internal::ConcatBuilderSelectorHelper<decltype(from(enumerable)), typename TEnumerable::ElemType>::builder
	{
		return typename internal::ConcatBuilderSelectorHelper<decltype(from(enumerable)), typename TEnumerable::ElemType>::builder(from(enumerable));
	}
}

#endif