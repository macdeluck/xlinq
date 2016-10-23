#ifndef XLINQ_MAX_H_
#define XLINQ_MAX_H_

#include "xlinq_base.h"
#include <limits>

namespace xlinq
{
	class _MaxBuilder
	{
	private:
		template<typename TElem>
		decltype(auto) max(std::shared_ptr<IEnumerable<TElem>> enumerable, TElem maxBase)
		{
			for (auto it = enumerable->getEnumerator(); it->next();)
			{
				if (maxBase < it->current())
					maxBase = it->current();
			}
			return maxBase;
		}
	public:
		decltype(auto) build(std::shared_ptr<IEnumerable<char>> enumerable)
		{
			return max(enumerable, std::numeric_limits<char>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned char>> enumerable)
		{
			return max(enumerable, std::numeric_limits<unsigned char>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<short>> enumerable)
		{
			return max(enumerable, std::numeric_limits<short>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned short>> enumerable)
		{
			return max(enumerable, std::numeric_limits<unsigned short>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<int>> enumerable)
		{
			return max(enumerable, std::numeric_limits<int>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned int>> enumerable)
		{
			return max(enumerable, std::numeric_limits<unsigned int>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long>> enumerable)
		{
			return max(enumerable, std::numeric_limits<long>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long>> enumerable)
		{
			return max(enumerable, std::numeric_limits<unsigned long>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long long>> enumerable)
		{
			return max(enumerable, std::numeric_limits<long long>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long long>> enumerable)
		{
			return max(enumerable, std::numeric_limits<unsigned long long>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<float>> enumerable)
		{
			return max(enumerable, std::numeric_limits<float>::min());
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<double>> enumerable)
		{
			return max(enumerable, std::numeric_limits<double>::min());
		}
	};

	decltype(auto) max()
	{
		return _MaxBuilder();
	}
}

#endif