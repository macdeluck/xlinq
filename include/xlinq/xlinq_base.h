#ifndef XLINQ_BASE_H_
#define XLINQ_BASE_H_

#define XLINQ_INLINE inline
#define XLINQ_ABSTRACT = 0

#include <memory>
#include <exception>

namespace xlinq
{
	/**
		General exception for iteration errors.
	*/
	class IterationException : std::exception
	{
	public:
		IterationException() : std::exception("Iteration has failed for unknown reason.") {}

		IterationException(const char* message) : std::exception(message) {}
	};

	/**
		Exception indicating, that function cannot be called due to iteration has not been started.
	*/
	class IterationNotStartedException : IterationException
	{
	public:
		IterationNotStartedException() : IterationException("Call of function failed due to iteration hasn't been started yet.") {}
	};

	/**
		Exception indicating, that function cannot be called due to iteration has been finished.
	*/
	class IterationFinishedException : IterationException
	{
	public:
		IterationFinishedException() : IterationException("Call of function failed due to iteration has been finished.") {}
	};

	/**
		Helper class which cleans type from const and reference indicators.
	*/
	template<typename TValue>
	class _cleanup_type
	{
	public:
		typedef typename std::remove_const<
				typename std::remove_reference<TValue>::type>::type type;
	};

	/**
		Interface for enumerator.
	*/
	template<typename TElem>
	class IEnumerator
	{
	public:
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
		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() XLINQ_ABSTRACT;
	};

	/**
		Explicit creating new object using given builder.
	*/
	template<typename TEnumerable, typename TBuilder>
	decltype(auto) build(std::shared_ptr<TEnumerable> enumerable, TBuilder& builder)
	{
		typedef typename decltype(enumerable->getEnumerator()->current()) TElem;
		return builder.build((std::shared_ptr<IEnumerable<TElem>>)enumerable);
	}

	/**
		Nice syntax operator for creating new object using given builder.
	*/
	template<typename TEnumerable, typename TBuilder>
	decltype(auto) operator>>(std::shared_ptr<TEnumerable> enumerable, TBuilder& builder)
	{
		return build(enumerable, builder);
	}

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