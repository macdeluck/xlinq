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
*	@file xlinq_base.h
*	Base xlinq definitions and expressions.
*	@author TrolleY
*/
#ifndef XLINQ_BASE_H_
#define XLINQ_BASE_H_

#include <cstddef>
#include <memory>
#include <type_traits>
#include "xlinq_defs.h"
#include "xlinq_exception.h"

#define _XLINQ_GET_ENUMERATOR(TEnumerator) \
	std::shared_ptr<TEnumerator> getEnumerator() \
	{ \
		std::shared_ptr<IEnumerator<TElem>> res = this->createEnumerator(); \
		return std::static_pointer_cast<TEnumerator>(res); \
	}

#define _XLINQ_GET_END_ENUMERATOR(TEnumerator) \
	std::shared_ptr<TEnumerator> getEndEnumerator() \
	{ \
		std::shared_ptr<IEnumerator<TElem>> res = this->createEndEnumerator(); \
		return std::static_pointer_cast<TEnumerator>(res); \
	}

#define _XLINQ_GET_ENUMERATOR_AT(TEnumerator) \
	std::shared_ptr<TEnumerator> getEnumeratorAt(int index) \
	{ \
		std::shared_ptr<IEnumerator<TElem>> res = this->createEnumeratorAt(index); \
		return std::static_pointer_cast<TEnumerator>(res); \
	}

namespace xlinq
{
	/**
	*	Interface for enumerator.
	*	This is the basic enumerator interface which is used to
	*	obtain elements from collection. New enumerator instance
	*	should point a guard member, so call of next is required
	*	before iteration starts. Enumerator should support deffered
	*	execution whenever it is possible.
	*/
	template<typename TElem>
	class IEnumerator
	{
	public:
		/**
		*	Type returned by enumerator.
		*	This member stores type which is returned during enumeration.
		*	It is designed to be not-constant and not-reference type.
		*/
		typedef TElem ElemType;
		
		/**
		*	Virtual destructor of object.
		*/
		virtual ~IEnumerator() {}

		/**
		*	Moves enumeration to next element.
		*	This method moves an enumeration to next element. After creation
		*	enumerator should point at begin enumeration guard so call of this method
		*	is required to start enumeration. This method should return false
		*	if collection does not have more elements and end enumeration guard
		*	has been reached. It should support deffered execution whenever it is possible
		*	and return whenever it is safe to return next collection element.
		*	@return true, if next element of enumeration has been found. Otherwise
		*	false.
		*/
		virtual bool next() XLINQ_ABSTRACT;

		/**
		*	Returns current element of enumeration.
		*	This method is used to access current element. It is not allowed
		*	to call this method before first call of #next method and after
		*	#next method returning false.
		*	@throws IterationNotStartedException when called before first call of #next method.
		*	@throws IterationFinishedException when called after #next method returned false.
		*	@return current enumeration element.
		*/
		virtual TElem current() XLINQ_ABSTRACT;
	};

	/**
	*	Interface for enumerator.
	*	This enumerator interface allows to traverse collection
	*	in two ways. When the iteration is finished (end guard has been met), it may
	*	continue to go backward until the begin guard occurance. It should support
	*	deffered execution whenever it is possible and stop its execution whenever
	*	it is safe to return next collection element.
	*/
	template<typename TElem>
	class IBidirectionalEnumerator : public IEnumerator<TElem>
	{
	public:
		/**
		*	Moves enumeration to previous element.
		*	This method moves an enumeration to previous element. This method should return false
		*	if collection does not have more elements and begin enumeration guard
		*	has been reached. It should support deffered execution whenever it is possible
		*	and stop its execution whenever it is safe to return previous collection element.
		*	@return true, if next element of enumeration has been found. Otherwise
		*	false.
		*/
		virtual bool back() XLINQ_ABSTRACT;
	};

	template<typename TElem>
	class IRandomAccessEnumerator : public IBidirectionalEnumerator<TElem>
	{
	public:
		/**
		*	Moves enumeration forward or backward by given number of steps.
		*	This function allows to traverse collection ommiting elements in constant time.
		*	To move forward step should be positive value, and to move backward step shoud
		*	be negative value. If step is equal to 0 method should always return true and
		*	exit immediately. If given step is greater than number of elements left,
		*	then enumerator should point to begin or end guard respectively and return false.
		*	@param step Number of steps to advance enumerator.
		*	@return true, if new enumerator position is within collection. Otherwise false.
		*/
		virtual bool advance(int step) XLINQ_ABSTRACT;
	};

	/**
	*	Interface for enumerable.
	*	This is basic enumerable interface which is used to obtain pointer to IEnumerator
	*	instance.
	*/
	template<typename TElem>
	class IEnumerable
	{
	protected:
		/**
		*	@todo comments
		*	Accesses enumerator.
		*	This method allows to access pointer to enumerator, which may be used
		*	to traverse collection. Each of enumerator instances created from single
		*	collection should work separately (they should not have any impact on each other).
		*	@return New IEnumerator instance.
		*/
		virtual std::shared_ptr<IEnumerator<TElem>> createEnumerator() XLINQ_ABSTRACT;

	public:
		/**
		*	Type returned by enumerator.
		*	This member stores type which is returned during enumeration.
		*	It is designed to be not-constant and not-reference type.
		*/
		typedef TElem ElemType;

		_XLINQ_GET_ENUMERATOR(IEnumerator<TElem>)
	};

	/**
	*	Interface for bidirectional enumerable.
	*	This is basic enumerable interface which is used to obtain pointer to
	*	IBidirectionalEnumerator instance.
	*	@todo comments
	*/
	template<typename TElem>
	class IBidirectionalEnumerable : public IEnumerable<TElem>
	{
	protected:
		/**
		*	Accesses enumerator pointing at the end of enumerable.
		*	This method allows to access pointer to enumerator, which may be used
		*	to traverse collection starting from last element. Since enumerator will point at the
		*	end of collection the call of back method is required to traverse collection. Each of
		*	enumerator instances created from single collection should work separately
		*	(they should not have any impact on each other).
		*	@return New IBidirectionalEnumerator instance.
		*/
		virtual std::shared_ptr<IBidirectionalEnumerator<TElem>> createEndEnumerator() XLINQ_ABSTRACT;
	public:
		_XLINQ_GET_ENUMERATOR(IBidirectionalEnumerator<TElem>)

		_XLINQ_GET_END_ENUMERATOR(IBidirectionalEnumerator<TElem>)
	};

	/**
	*	Interface for random access enumerable.
	*	This is basic enumerable interface which is used to obtain pointer to
	*	IRandomAccessEnumerator instance.
	*	@todo comments
	*/
	template<typename TElem>
	class IRandomAccessEnumerable : public IBidirectionalEnumerable<TElem>
	{
	protected:
		/**
		*	Accesses enumerator pointing at the given element of enumerable.
		*	This method allows to access pointer to enumerator, which may be used
		*	to traverse collection starting from any element. Since enumerator will point at the
		*	end of collection the call of back method is required to traverse collection. Each of
		*	enumerator instances created from single collection should work separately
		*	(they should not have any impact on each other).
		*	@return New IRandomAccessEnumerator instance.
		*/
		virtual std::shared_ptr<IRandomAccessEnumerator<TElem>> createEnumeratorAt(int elementIndex) XLINQ_ABSTRACT;
	public:
		_XLINQ_GET_ENUMERATOR(IRandomAccessEnumerator<TElem>)

		_XLINQ_GET_END_ENUMERATOR(IRandomAccessEnumerator<TElem>)

		_XLINQ_GET_ENUMERATOR_AT(IRandomAccessEnumerator<TElem>)
	};

	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<bool isRandomAccess, bool isBidirectional, bool isEnumerable, typename TElem> struct EnumerableTypeSelectorHelper {};
		template<typename TElem> struct EnumerableTypeSelectorHelper<true, true, true, TElem> { typedef IRandomAccessEnumerable<TElem> type; };
		template<typename TElem> struct EnumerableTypeSelectorHelper<false, true, true, TElem> { typedef IBidirectionalEnumerable<TElem> type; };
		template<typename TElem> struct EnumerableTypeSelectorHelper<false, false, true, TElem> { typedef IEnumerable<TElem> type; };

		template<typename Type>
		struct EnumerableTypeSelector
		{
			typedef typename EnumerableTypeSelectorHelper<
				std::is_base_of<IRandomAccessEnumerable<typename Type::ElemType>, Type>::value,
				std::is_base_of<IBidirectionalEnumerable<typename Type::ElemType>, Type>::value,
				std::is_base_of<IEnumerable<typename Type::ElemType>, Type>::value,
				typename Type::ElemType>::type type;
		};

		class _GetEnumeratorBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerator<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumerator();
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerator<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumerator();
			}

			template<typename TElem>
			std::shared_ptr<IEnumerator<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumerator();
			}
		};

		class _GetEndEnumeratorBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerator<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return enumerable->getEndEnumerator();
			}

			template<typename TElem>
			std::shared_ptr<IBidirectionalEnumerator<TElem>> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				return enumerable->getEndEnumerator();
			}
		};

		class _GetEnumeratorAtBuilder
		{
			int _elementIndex;
		public:
			_GetEnumeratorAtBuilder(int elementIndex) : _elementIndex(elementIndex) {}

			template<typename TElem>
			std::shared_ptr<IRandomAccessEnumerator<TElem>> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumeratorAt(_elementIndex);
			}
		};
		
		template<typename TValue, typename TBuilder>
		auto build(std::shared_ptr<TValue> ptr, TBuilder builder) -> decltype(builder.build(std::declval<std::shared_ptr<typename EnumerableTypeSelector<TValue>::type>>()))
		{
			return builder.build((std::shared_ptr<typename EnumerableTypeSelector<TValue>::type>)ptr);
		}
	}
	/*@endcond*/

	/**
	*	Nice syntax operator for executing xlinq command.
	*	This operator should be used to build xlinq queries.
	*	@param obj The enumeration object. It should be pointer to IEnumerable or
	*	deriving class.
	*	@param builder The expression builder object. It should have build method defined.
	*/
	template<typename TValue, typename TBuilder>
	auto operator>>(std::shared_ptr<TValue> obj, TBuilder builder) -> decltype(build(obj, builder))
	{
		return internal::build(obj, builder);
	}

	/**
	*	Function extracting enumerator from enumerable.
	*	@return Builder of getEnumerator expression.
	*/
	XLINQ_INLINE internal::_GetEnumeratorBuilder getEnumerator()
	{
		return internal::_GetEnumeratorBuilder();
	}

	/**
	*	Function extracting end enumerator from bidirectional enumerable.
	*	@return Builder of getEndEnumerator expression.
	*/
	XLINQ_INLINE internal::_GetEndEnumeratorBuilder getEndEnumerator()
	{
		return internal::_GetEndEnumeratorBuilder();
	}

	/**
	*	Function extracting enumerator at given position from random access enumerable.
	*	@param elementIndex index of element to set enumerator pointing to
	*	@return Builder of getEnumeratorAt expression.
	*/
	XLINQ_INLINE internal::_GetEnumeratorAtBuilder getEnumeratorAt(int elementIndex)
	{
		return internal::_GetEnumeratorAtBuilder(elementIndex);
	}
}

#endif