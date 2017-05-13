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

#include <memory>
#include "xlinq_defs.h"
#include "xlinq_exception.h"

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
		*	Destructor defined to avoid diamond inheritance problem.
		*/
		virtual ~IEnumerator() {}

		/**
		*	Moves enumeration to next element.
		*	This method moves an enumeration to next element. Enumeration
		*	should start at the begin enumeration guard so call of this method
		*	is required to begin enumeration. This method should return false
		*	if collection does not have more elemends and end enumeration guard
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
	*	Interface for enumerable.
	*	This is basic enumerable interface which is used to obtain pointer to IEnumerator
	*	instance.
	*/
	template<typename TElem>
	class IEnumerable
	{
	public:
		/**
		*	Type returned by enumerator.
		*	This member stores type which is returned during enumeration.
		*	It is designed to be not-constant and not-reference type.
		*/
		typedef TElem ElemType;
		
		/**
		*	Accesses enumerator.
		*	This method allows to access pointer to enumerator, which may be used
		*	to traverse collection. Each of enumerator instances created from single
		*	collection should work separately (they should not have any impact on each other).
		*	@return New IEnumerator instance.
		*/
		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() XLINQ_ABSTRACT;
	};

	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TEnumerable>
		struct typeinfo
		{
			typedef typename std::remove_const<
				typename std::remove_reference<typename TEnumerable::ElemType>::type>::type ElemType;

			typedef std::shared_ptr<IEnumerable<ElemType>> BaseType;

			XLINQ_INLINE static BaseType cast(std::shared_ptr<TEnumerable> value)
			{
				return (BaseType)value;
			}
		};

		class _GetEnumeratorBuilder
		{
		public:
			template<typename TElem>
			std::shared_ptr<IEnumerator<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				return enumerable->getEnumerator();
			}
		};

		template<typename TValue, typename TBuilder>
		auto build(std::shared_ptr<TValue> ptr, TBuilder builder) -> decltype(builder.build(typeinfo<TValue>::cast(ptr)))
		{
			return builder.build(typeinfo<TValue>::cast(ptr));
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
}

#endif