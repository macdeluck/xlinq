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
*	@file xlinq_sequence_equals.h
*	Comparing sequence elements.
*	@author TrolleY
*/
#ifndef XLINQ_SEQUENCE_EQUALS_H_
#define XLINQ_SEQUENCE_EQUALS_H_

#include <memory>
#include <utility>
#include "xlinq_base.h"
#include "xlinq_from.h"

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TEnumerable, typename TSecondEnumerable, typename TComparer>
		bool build_sequence_equals(TEnumerable enumerable, TSecondEnumerable secondEnumerable, TComparer comparer)
		{
			bool fenext;
			bool senext;
			auto fe = enumerable->getEnumerator();
			auto se = from(secondEnumerable)->getEnumerator();
			while (fenext = fe->next(), senext = se->next(), fenext && senext)
				if (!comparer(fe->current(), se->current()))
					return false;
			return fenext == senext;
		}

		template<typename TSecondEnumerable>
		class _SequenceEqualsBuilder
		{
		private:
			TSecondEnumerable _secondEnumerable;
		
		public:
			_SequenceEqualsBuilder(TSecondEnumerable secondEnumerable) : _secondEnumerable(secondEnumerable) {}

			template<typename TElem>
			bool build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_sequence_equals(enumerable, _secondEnumerable, std::equal_to<TElem>());
			}

			template<typename TElem>
			bool build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_sequence_equals(enumerable, _secondEnumerable, std::equal_to<TElem>());
			}

			template<typename TElem>
			bool build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto sen = from(_secondEnumerable);
				auto srae = std::dynamic_pointer_cast<std::shared_ptr<IRandomAccessEnumerable<typename decltype(sen)::element_type::ElemType>>>(sen);
				if (srae && (srae->size() != enumerable->size()))
					return false;
				return build_sequence_equals(enumerable, sen, std::equal_to<TElem>());
			}
		};

		template<typename TSecondEnumerable, typename TComparer>
		class _SequenceEqualsWithEqCompBuilder
		{
		private:
			TSecondEnumerable _secondEnumerable;
			TComparer _comparer;

		public:
			_SequenceEqualsWithEqCompBuilder(TSecondEnumerable secondEnumerable, TComparer comparer) : _secondEnumerable(secondEnumerable), _comparer(comparer) {}

			template<typename TElem>
			bool build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return build_sequence_equals(enumerable, _secondEnumerable, _comparer);
			}

			template<typename TElem>
			bool build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return build_sequence_equals(enumerable, _secondEnumerable, _comparer);
			}

			template<typename TElem>
			bool build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto sen = from(_secondEnumerable);
				auto srae = std::dynamic_pointer_cast<std::shared_ptr<IRandomAccessEnumerable<typename decltype(sen)::element_type::ElemType>>>(sen);
				if (srae && (srae->size() != enumerable->size()))
					return false;
				return build_sequence_equals(enumerable, sen, _comparer);
			}
		};
	}
	/*@endcond*/

	/**
	*	Checks if collection elements sequence is equal.
	*	This function compares two sequences for exact equality.
	*	It iterates through both of collections and compares each of their elements.
	*	Returns false when it finds first not matching element or when one of collections
	*	is shorter than another. Otherwise needs to loop though all elements of both collections.
	*	If both of enumerables support random access then size if collections is compared first.
	*	@param enumerable Enumerable to compare with.
	*	@return Builder of sequence_equals expression.
	*/
	template<typename TEnumerable>
	internal::_SequenceEqualsBuilder<TEnumerable> sequence_equals(TEnumerable enumerable)
	{
		return internal::_SequenceEqualsBuilder<TEnumerable>(enumerable);
	}

	/**
	*	Checks if collection elements sequence is equal using specified element comparer.
	*	This function compares two sequences for exact equality.
	*	It iterates through both of collections and compares each of their elements.
	*	Returns false when it finds first not matching element or when one of collections
	*	is shorter than another. Otherwise needs to loop though all elements of both collections.
	*	If both of enumerables support random access then size if collections is compared first.
	*	@param enumerable Enumerable to compare with.
	*	@return Builder of sequence_equals expression.
	*/
	template<typename TEnumerable, typename TComparer>
	internal::_SequenceEqualsWithEqCompBuilder<TEnumerable, TComparer> sequence_equals(TEnumerable enumerable, TComparer comparer)
	{
		return internal::_SequenceEqualsWithEqCompBuilder<TEnumerable, TComparer>(enumerable, comparer);
	}
}

#endif