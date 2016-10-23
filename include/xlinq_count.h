#ifndef XLINQ_COUNT_H_
#define XLINQ_COUNT_H_

#include "xlinq_base.h"

namespace xlinq
{
	class _Count
	{
	public:
		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			unsigned int count = 0;
			for (auto it = enumerable->getEnumerator(); it->next(); )
			{
				count++;
			}
			return count;
		}
	};

	decltype(auto) count()
	{
		return _Count();
	}
}

#endif