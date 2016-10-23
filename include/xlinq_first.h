#ifndef XLINQ_FIRST_H_
#define XLINQ_FIRST_H_

#include "xlinq_base.h"

namespace xlinq
{
	class _First
	{
	public:
		template<typename TElem>
		TElem build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			auto it = enumerable->getEnumerator();
			it->next();
			return it->current();
		}
	};

	decltype(auto) first()
	{
		return _First();
	}
}

#endif