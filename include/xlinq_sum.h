#ifndef XLINQ_SUM_H_
#define XLINQ_SUM_H_

#include "xlinq_base.h"

namespace xlinq
{
	class _SumBuilder
	{
	private:
		template<typename TElem>
		decltype(auto) sum(std::shared_ptr<IEnumerable<TElem>> enumerable, TElem sumBase)
		{
			for (auto it = enumerable->getEnumerator(); it->next();)
				sumBase += it->current();
			return sumBase;
		}
	public:
		decltype(auto) build(std::shared_ptr<IEnumerable<char>> enumerable)
		{
			return sum(enumerable, static_cast<char>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned char>> enumerable)
		{
			return sum(enumerable, static_cast<unsigned char>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<short>> enumerable)
		{
			return sum(enumerable, static_cast<short>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned short>> enumerable)
		{
			return sum(enumerable, static_cast<unsigned short>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<int>> enumerable)
		{
			return sum(enumerable, static_cast<int>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned int>> enumerable)
		{
			return sum(enumerable, static_cast<unsigned int>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long>> enumerable)
		{
			return sum(enumerable, static_cast<long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long>> enumerable)
		{
			return sum(enumerable, static_cast<unsigned long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long long>> enumerable)
		{
			return sum(enumerable, static_cast<long long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long long>> enumerable)
		{
			return sum(enumerable, static_cast<unsigned long long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<float>> enumerable)
		{
			return sum(enumerable, static_cast<float>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<double>> enumerable)
		{
			return sum(enumerable, static_cast<double>(0));
		}
	};

	decltype(auto) sum()
	{
		return _SumBuilder();
	}
}

#endif