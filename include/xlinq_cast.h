#ifndef XLINQ_CAST_H_
#define XLINQ_CAST_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TDest>
	class _CastBuilder
	{
	public:
		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return enumerable ^ select([](TElem elem) { return (TDest)elem; });
		}
	};

	template<typename TDest>
	decltype(auto) cast()
	{
		return _CastBuilder<TDest>();
	}
}

#endif