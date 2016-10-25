#ifndef XLINQ_AVERAGE_H_
#define XLINQ_AVERAGE_H_

#include "xlinq_base.h"
#include "xlinq_select.h"

namespace xlinq
{
	class _AverageBuilder
	{
	private:
		template<typename TElem>
		double average(std::shared_ptr<IEnumerable<TElem>> enumerable, TElem sumBase)
		{
			double count = 0.0;
			for (auto it = enumerable->getEnumerator(); it->next();)
			{
				sumBase += it->current();
				count += 1.0;
			}
			return sumBase / count;
		}
	public:
		decltype(auto) build(std::shared_ptr<IEnumerable<char>> enumerable)
		{
			return average(enumerable, static_cast<char>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned char>> enumerable)
		{
			return average(enumerable, static_cast<unsigned char>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<short>> enumerable)
		{
			return average(enumerable, static_cast<short>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned short>> enumerable)
		{
			return average(enumerable, static_cast<unsigned short>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<int>> enumerable)
		{
			return average(enumerable, static_cast<int>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned int>> enumerable)
		{
			return average(enumerable, static_cast<unsigned int>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long>> enumerable)
		{
			return average(enumerable, static_cast<long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long>> enumerable)
		{
			return average(enumerable, static_cast<unsigned long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<long long>> enumerable)
		{
			return average(enumerable, static_cast<long long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<unsigned long long>> enumerable)
		{
			return average(enumerable, static_cast<unsigned long long>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<float>> enumerable)
		{
			return average(enumerable, static_cast<float>(0));
		}

		decltype(auto) build(std::shared_ptr<IEnumerable<double>> enumerable)
		{
			return average(enumerable, static_cast<double>(0));
		}
	};

	template<typename TValueConverter>
	class _CustomAverageBuilder
	{
	private:
		TValueConverter _valueConverter;
	public:
		_CustomAverageBuilder(TValueConverter valueConverter) : _valueConverter(valueConverter) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return enumerable ^ select(_valueConverter) ^ average();
		}
	};

	template<typename TValueConverter>
	decltype(auto) average(TValueConverter valueConverter)
	{
		return _CustomAverageBuilder<TValueConverter>(valueConverter);
	}

	decltype(auto) average()
	{
		return _AverageBuilder();
	}
}

#endif