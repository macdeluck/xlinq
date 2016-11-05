#ifndef XLINQ_OFTYPE_H_
#define XLINQ_OFTYPE_H_

#include "xlinq_base.h"
#include "xlinq_where.h"

namespace xlinq
{
	template<typename TSelect>
	class _OfTypeBuilder
	{
	public:
		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<std::shared_ptr<TElem>>> enumerable)
		{
			return enumerable ^ where([](std::shared_ptr<TElem>& elem) -> bool 
			{ 
				return std::dynamic_pointer_cast<TSelect>(elem) != nullptr; 
			});
		}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem*>> enumerable)
		{
			return enumerable ^ where([](TElem* elem) -> bool
			{
				return dynamic_cast<TSelect*>(elem) != nullptr;
			});
		}
	};

	template<typename TSelect>
	decltype(auto) oftype()
	{
		return _OfTypeBuilder<TSelect>();
	}
}

#endif