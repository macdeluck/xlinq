#ifndef XLINQ_BASE_H_
#define XLINQ_BASE_H_

#include <memory>
#include "xlinq_defs.h"
#include "xlinq_exception.h"

namespace xlinq
{
	/**
		Interface for enumerator.
	*/
	template<typename TElem>
	class IEnumerator
	{
	public:
		typedef TElem ElemType;
		
		virtual ~IEnumerator() {}

		virtual bool next() XLINQ_ABSTRACT;

		virtual TElem current() XLINQ_ABSTRACT;
	};

	/**
		Interface for enumerable.
	*/
	template<typename TElem>
	class IEnumerable
	{
	public:
		typedef TElem ElemType;
		
		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() XLINQ_ABSTRACT;
	};

	template<typename TValue>
	struct typeinfo
	{
		typedef typename std::remove_const<
				typename std::remove_reference<typename TValue::ElemType>::type>::type ElemType;
		
		typedef std::shared_ptr<IEnumerable<ElemType>> BaseType;
		
		XLINQ_INLINE static BaseType cast(std::shared_ptr<TValue> value)
		{
			return (BaseType)value;
		}
	};

	/**
		Simple builder extracting enumerator from enumerable.
	*/
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

	template<typename TValue, typename TBuilder>
	auto operator>>(std::shared_ptr<TValue> ptr, TBuilder builder) -> decltype(build(ptr, builder))
	{
		return build(ptr, builder);
	}

	/**
		Function extracting enumerator from enumerable.
		Example: auto enumerator = enumerable >> getEnumerator();
	*/
	XLINQ_INLINE _GetEnumeratorBuilder getEnumerator()
	{
		return _GetEnumeratorBuilder();
	}
}

#endif