/*
MIT License

Copyright (c) 2017 TrolleY

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/**
*	@file xlinq_to_container.h
*	Converting IEnumerable to STL container.
*	@author TrolleY
*/
#ifndef XLINQ_TO_CONTAINER_H_
#define XLINQ_TO_CONTAINER_H_

#include "xlinq_base.h"
#include "xlinq_stl.h"
#include <vector>
#include <list>
#include <forward_list>

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		class _ToVectorBuilder
		{
		public:
			template<typename TElem>
			std::vector<TElem> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::vector<TElem>(container.begin(), container.end());
			}

			template<typename TElem>
			std::vector<TElem> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::vector<TElem>(container.begin(), container.end());
			}

			template<typename TElem>
			std::vector<TElem> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::vector<TElem>(container.begin(), container.end());
			}
		};

		class _ToListBuilder
		{
		public:
			template<typename TElem>
			std::list<TElem> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::list<TElem>(container.begin(), container.end());
			}

			template<typename TElem>
			std::list<TElem> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::list<TElem>(container.begin(), container.end());
			}

			template<typename TElem>
			std::list<TElem> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::list<TElem>(container.begin(), container.end());
			}
		};

		class _ToForwardListBuilder
		{
		public:
			template<typename TElem>
			std::forward_list<TElem> build(std::shared_ptr<IEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::forward_list<TElem>(container.begin(), container.end());
			}

			template<typename TElem>
			std::forward_list<TElem> build(std::shared_ptr<IBidirectionalEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::forward_list<TElem>(container.begin(), container.end());
			}

			template<typename TElem>
			std::forward_list<TElem> build(std::shared_ptr<IRandomAccessEnumerable<TElem>> enumerable)
			{
				auto container = enumerable >> stl();
				return std::forward_list<TElem>(container.begin(), container.end());
			}
		};
	}
	/*@endcond*/

	/**
	*	Converts IEnumerable to STL vector.
	*	This function allows to gather all enumerable elements to STL vector.
	*	@return to_vector expression builder
	*/
	XLINQ_INLINE internal::_ToVectorBuilder to_vector()
	{
		return internal::_ToVectorBuilder();
	}

	/**
	*	Converts IEnumerable to STL list.
	*	This function allows to gather all enumerable elements to STL list.
	*	@return to_list expression builder
	*/
	XLINQ_INLINE internal::_ToListBuilder to_list()
	{
		return internal::_ToListBuilder();
	}

	/**
	*	Converts IEnumerable to STL forward list.
	*	This function allows to gather all enumerable elements to STL forward list.
	*	@return to_forward_list expression builder
	*/
	XLINQ_INLINE internal::_ToForwardListBuilder to_forward_list()
	{
		return internal::_ToForwardListBuilder();
	}
}

#endif