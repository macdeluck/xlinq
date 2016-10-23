#ifndef XLINQ_AGGREGATE_H_
#define XLINQ_AGGREGATE_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TElem, typename TAggregator>
	class _AggregateBuilder
	{
	private:
		TElem _sumBase;
		TAggregator _aggregator;
	public:
		_AggregateBuilder(TElem sumBase, TAggregator aggregator) : _sumBase(sumBase), _aggregator(aggregator) {}

		template<typename TDestElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TDestElem>> enumerable)
		{
			TDestElem result = (TDestElem)TElem(_sumBase);
			for (auto it = enumerable->getEnumerator(); it->next();)
				result = _aggregator(result, it->current());
			return result;
		}
	};

	template<typename TElem, typename TAggregator>
	decltype(auto) aggregate(TElem baseElem, TAggregator aggregator)
	{
		return _AggregateBuilder<TElem, TAggregator>(baseElem, aggregator);
	}
}

#endif