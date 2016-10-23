#ifndef XLINQ_STD_H_
#define XLINQ_STD_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TContainer, typename TElem>
	class StdEnumerator : public IEnumerator<TElem>
	{
	public:
		typedef decltype(std::begin(std::declval<TContainer>())) IteratorType;
		typedef std::shared_ptr<StdEnumerator<TContainer, TElem>> Ptr;
	private:
		IteratorType _begin;
		IteratorType _end;
		bool _started;
	public:
		StdEnumerator(IteratorType begin, IteratorType end) : _begin(begin), _end(end), _started(false) {}

		virtual bool next() override
		{
			if (_started)
				++_begin;
			else _started = true;
			return _begin != _end;
		}

		virtual TElem current() override
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
		using TConstElem = typename std::remove_reference<TRefElem>::type;
		using TElem = typename std::remove_const<TConstElem>::type;
		return std::shared_ptr<IEnumerable<TElem>>(new StdEnumerable<TContainer, TElem>(container));
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
			int _elements;
			void _advance()
			{
				if (_source)
					_elements = _source->next() ? _elements + 1 : -1;
			}

		public:
			typedef std::input_iterator_tag iterator_category;

			iterator(std::shared_ptr<IEnumerator<TElem>> source, int elements = 0) : _source(source), _elements(elements) { _advance(); }
			TElem operator*() const { return _source->current(); }
			iterator& operator++() { _advance(); return *this; }
			bool operator==(const iterator& other) const
			{
				return _elements == other._elements;
			}
			bool operator!=(const iterator& other) const
			{
				return !(*this == other);
			}
		};
		typedef iterator const_iterator;

		iterator begin()
		{
			return iterator(_source->getEnumerator(), 0);
		}

		iterator end()
		{
			return iterator(nullptr, -1);
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

	decltype(auto) iter()
	{
		return _StdContainerBuilder();
	}
}

#endif