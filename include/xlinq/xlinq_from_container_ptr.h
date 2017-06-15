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
*	@file xlinq_from_container_ptr.h
*	Creating enumerable object from containers passed by ptr.
*	@author TrolleY
*/
#ifndef XLINQ_FROM_CONTAINER_PTR_H_
#define XLINQ_FROM_CONTAINER_PTR_H_

#include <cstdlib>
#include <memory>
#include <iterator>
#include "xlinq_base.h"
#include "xlinq_exception.h"
#include "xlinq_from_container.h"

namespace xlinq
{
	/**
	*	Creates enumerable from pointer to STL container.
	*	This function may be used to create enumerable from pointer to STL container.
	*	It is implemented using deffered execution so traversing of the collection
	*	will wait until its next element will be requested. Please note, that
	*	enumeration will fail if container will be deallocated.
	*	@param container Source STL container.
	*	@return Enumerable from container.
	*/
	template<typename TContainer>
	auto from(TContainer* container) -> std::shared_ptr<typename internal::EnumerableTypeSelector<typename internal::StlEnumerableSelector<TContainer, typename TContainer::value_type>::type>::type>
	{
		return std::shared_ptr<typename internal::EnumerableTypeSelector<typename internal::StlEnumerableSelector<TContainer, typename TContainer::value_type>::type>::type>(new typename internal::StlEnumerableSelector<TContainer, typename TContainer::value_type>::type(*container));
	}
}

#endif