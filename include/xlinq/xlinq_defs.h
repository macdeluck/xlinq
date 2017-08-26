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
*	@file xlinq_defs.h
*	Common xlinq symbols.
*	@author TrolleY
*/
#ifndef XLINQ_DEFS_H_
#define XLINQ_DEFS_H_

/**
*	Defines xlinq inline function.
*/
#define XLINQ_INLINE inline


/**
*	Defines xlinq abstract function.
*/
#define XLINQ_ABSTRACT = 0

namespace xlinq
{
	/**
	*	Calculates total size of any dimension array.
	*	This spetialization generates constant 1 for non array type.
	*/
	template<typename T>
	struct array_size : public std::integral_constant<int, 1> {};


	/**
	*	Calculates total size of any dimension array.
	*/
	template<typename T, std::size_t S>
	struct array_size<T[S]> : public std::integral_constant<int, S * array_size<T>::value> {};

	/**
	*	Resolves return type of unary function of given type.
	*	This function may be used to resolve return type of unary function.
	*/
	template<typename TFunction, typename TArgument>
	struct unaryreturntype
	{
		/**
		*	Return type of unary function.
		*/
		typedef decltype(std::declval<TFunction>()((std::declval<TArgument>()))) type;
	};
}

#endif