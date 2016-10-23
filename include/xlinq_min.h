#ifndef XLINQ_MIN_H_
#define XLINQ_MIN_H_

#include "xlinq_base.h"
#include <limits>

namespace xlinq
{
	class _MinBuilder
	{
	private:
		template<typename TElem>
		decltype(auto) min(std::shared_ptr<IEnumerable<TElem>> enumerable, TElem minBase)
		{
			for (auto it = enumerable->getEnumerator(); it->next();)
			{
				if (minBase > it->current())
					minBase = it->current();
			}
			return minBase;
		}
	public:
		decltype(auto) build(std::shared_ptr<IEnumerable<char>> enumerable)
		{
			return min(enumerable, std::numeric_limits<char>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned char>> enumerable)
		{
			return min(enumerable, std::numeric_limits<unsigned char>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<short>> enumerable)
		{
			return min(enumerable, std::numeric_limits<short>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned short>> enumerable)
		{
			return min(enumerable, std::numeric_limits<unsigned short>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<int>> enumerable)
		{
			return min(enumerable, std::numeric_limits<int>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned int>> enumerable)
		{
			return min(enumerable, std::numeric_limits<unsigned int>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long>> enumerable)
		{
			return min(enumerable, std::numeric_limits<long>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long>> enumerable)
		{
			return min(enumerable, std::numeric_limits<unsigned long>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long long>> enumerable)
		{
			return min(enumerable, std::numeric_limits<long long>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long long>> enumerable)
		{
			return min(enumerable, std::numeric_limits<unsigned long long>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<float>> enumerable)
		{
			return min(enumerable, std::numeric_limits<float>::max());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<double>> enumerable)
		{
			return min(enumerable, std::numeric_limits<double>::max());
		}
	};

	decltype(auto) min()
	{
		return _MinBuilder();
	}
}

#endif