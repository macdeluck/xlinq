#ifndef XLINQ_BASE_H_
#define XLINQ_BASE_H_

#include <memory>

namespace xlinq
{
	template<typename TElem>
	class IEnumerator
	{
	public:
		using Ptr = std::shared_ptr<IEnumerator<TElem>>;

		virtual bool next() =0;

		virtual TElem current() =0;
	};

	template<typename TElem>
	class IEnumerable
	{
	public:
		using Ptr = std::shared_ptr<IEnumerable<TElem>>;
		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() =0;
	};

	template<typename TElem, typename TOther>
	decltype(auto) build(std::shared_ptr<IEnumerable<TElem>>& enumerable, TOther& other)
	{
		return other.build(enumerable);
	}

	template<typename TElem, typename TOther>
	decltype(auto) operator^(std::shared_ptr<IEnumerable<TElem>>& enumerable, TOther& other)
	{
		return other.build(enumerable);
	}
	
	class _GetEnumeratorBuilder
	{
	public:
		template<typename TElem>
		std::shared_ptr<IEnumerator<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return enumerable->getEnumerator();
		}
	};

	decltype(auto) getEnumerator()
	{
		return _GetEnumeratorBuilder();
	}
}

#endif