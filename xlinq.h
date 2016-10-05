#ifndef ENUMERABLE_H_
#define ENUMERABLE_H_

#include <memory>

namespace xlinq
{
	template<typename TElem>
	class IEnumerator
	{
	public:
		using Ptr = std::shared_ptr<IEnumerator<TElem>>;

		virtual bool hasNext() =0;

		virtual void next() =0;

		virtual const TElem current() =0;
	};

	template<typename TElem>
	class IEnumerable
	{
	public:
		using Ptr = std::shared_ptr<IEnumerable<TElem>>;
		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() =0;
	};

	template<typename TContainer, typename TElem>
	class StdEnumerator : public IEnumerator<TElem>
	{
	public:
		typedef decltype(std::begin(std::declval<TContainer>())) IteratorType;
		typedef std::shared_ptr<StdEnumerator<TContainer, TElem>> Ptr;
	private:
		IteratorType _begin;
		IteratorType _end;
	public:
		StdEnumerator(IteratorType begin, IteratorType end) : _begin(begin), _end(end) {}
		virtual bool hasNext() override
		{
			return _begin != _end;
		}

		virtual void next() override
		{
			++_begin;
		}

		virtual const TElem current() override
		{
			return *_begin;
		}
	};

	template<typename TContainer, typename TElem>
	class StdEnumerable : public IEnumerable<TElem>
	{
	public:
		typedef decltype(std::begin(std::declval<TContainer>())) IteratorType;
		typedef std::shared_ptr<StdEnumerable<TContainer, TElem>> Ptr;
	private:
		TContainer& _container;
	public:
		StdEnumerable(TContainer& container) : _container(container) {}
		virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
		{
			return IEnumerator<TElem>::Ptr(new StdEnumerator<TContainer, TElem>(std::begin(_container), std::end(_container)));
		}
	};

	template<typename TContainer>
	decltype(auto) from(TContainer& container)
	{
		using TRefElem = decltype(*std::begin(std::declval<TContainer>()));
		using TElem = typename std::remove_reference<TRefElem>::type;
		return std::shared_ptr<IEnumerable<TElem>>(new StdEnumerable<TContainer, TElem>(container));
	}

	template<typename TElem, typename TOther>
	decltype(auto) operator^(std::shared_ptr<IEnumerable<TElem>>& enumerable, TOther& other)
	{
		return other.build(enumerable);
	}

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

	template<typename TElem, typename TSelect>
	class _SelectEnumerable : public IEnumerable<TSelect>
	{
	private:
		std::function<TSelect(TElem)> _selector;
		std::shared_ptr<IEnumerable<TElem>> _source;

		class _SelectEnumerator : public IEnumerator<TSelect>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;
			std::function<TSelect(TElem)> _selector;
		public:
			_SelectEnumerator(std::shared_ptr<IEnumerator<TElem>> source, std::function<TSelect(TElem)> selector)
				: _source(source), _selector(selector) {}

			virtual bool hasNext() override
			{
				return _source->hasNext();
			}

			virtual void next() override
			{
				_source->next();
			}

			virtual const TSelect current() override
			{
				return _selector(_source->current());
			}
		};
	public:
		_SelectEnumerable(std::shared_ptr<IEnumerable<TElem>> source, std::function<TSelect(TElem)> selector)
			: _source(source), _selector(selector) {}

		virtual std::shared_ptr<IEnumerator<TSelect>> getEnumerator() override
		{
			return std::shared_ptr<IEnumerator<TSelect>>(new _SelectEnumerable::_SelectEnumerator(_source->getEnumerator(), _selector));
		}
	};

	template<typename TSelector>
	class _Select
	{
	private:
		TSelector _selector;
	public:
		_Select(TSelector selector) : _selector(selector) {}

		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			using TSelect = decltype(_selector(enumerable->getEnumerator()->current()));
			return std::shared_ptr<IEnumerable<TSelect>>(new _SelectEnumerable<TElem, TSelect>(enumerable, _selector));
		}
	};

	template<typename TSelector>
	_Select<TSelector> select(TSelector selector)
	{
		return _Select<TSelector>(selector);
	}

	class _Iter
	{
	public:
		template<typename TElem>
		std::shared_ptr<IEnumerator<TElem>> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return enumerable->getEnumerator();
		}
	};

	decltype(auto) iter()
	{
		return _Iter();
	}

	class _Count
	{
	public:
		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			unsigned int count = 0;
			for (auto it = enumerable->getEnumerator(); it->hasNext(); it->next())
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

	template<typename TElem>
	class StdContainer
	{
	private:
		std::shared_ptr<IEnumerable<TElem>> _source;
	public:
		StdContainer(std::shared_ptr<IEnumerable<TElem>> source) : _source(source) {}

		class iterator : public std::iterator<std::input_iterator_tag, TElem>
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _source;

		public:
			typedef std::input_iterator_tag iterator_category;

			iterator(std::shared_ptr<IEnumerator<TElem>> source) : _source(source) {}
			TElem operator*() const { return _source->current(); }
			iterator& operator++() { _source->next(); return *this; }
			bool operator==(const iterator& other) const
			{
				return (_source != nullptr && _source->hasNext()) == (other._source != nullptr && other._source->hasNext());
			}
			bool operator!=(const iterator& other) const
			{
				return !(*this == other);
			}
		};
		typedef iterator const_iterator;

		iterator begin()
		{
			return iterator(_source->getEnumerator());
		}

		iterator end()
		{
			return iterator(nullptr);
		}
	};

	class _StdContainerBuilder
	{
	public:
		template<typename TElem>
		decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
		{
			return StdContainer<TElem>(enumerable);
		}
	};

	decltype(auto) toStdContainer()
	{
		return _StdContainerBuilder();
	}
}

#endif