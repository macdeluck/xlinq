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
*	@file xlinq_exception.h
*	Definitions of exceptions used by xlinq.
*	@author TrolleY
*/
#ifndef XLINQ_EXCEPTION_H_
#define XLINQ_EXCEPTION_H_

#include "xlinq_defs.h"
#include <exception>
#include <string>

namespace xlinq
{
	/**
	*	Base class for all exceptions.
	*	@author tune2fs
	*/
	class Exception : public std::exception
	{
	public:
		/** 
		*	Constructor (C strings).
		*	@param message C-style string error message.
		*	The string contents are copied upon construction.
		*   Hence, responsibility for deleting the char* lies
		*   with the caller.
		*/
		XLINQ_INLINE explicit Exception(const char* message) :
			msg_(message)
		{
		}

		/** 
		*	Constructor (C++ STL strings).
		*	@param message The error message.
		*/
		XLINQ_INLINE explicit Exception(const std::string& message) :
			msg_(message)
		{}

		/**
		*	Destructor.
		*	Virtual to allow for subclassing.
		*/
		XLINQ_INLINE virtual ~Exception() throw () {}

		/**
		*	Returns a pointer to the (constant) error description.
		*	@return A pointer to a const char*. The underlying memory
		*	is in posession of the Exception object. Callers must
		*	not attempt to free the memory.
		*/
		XLINQ_INLINE virtual const char* what() const throw () {
			return msg_.c_str();
		}

	protected:
		/**
		*	Error message.
		*/
		std::string msg_;
	};

	/**
	*	Error indicating, that iteration was not started yet.
	*	This error is thrown when call of method requires enumerator not pointing at
	*	begin enumeration guard.
	*/
	class IterationNotStartedException : public Exception
	{
	public:
		/**
		*	Constructor.
		*	Creates new instance of IterationNotStartedException with default error message.
		*/
		XLINQ_INLINE IterationNotStartedException() : Exception("Iteration has not been started.") {}
	};

	/**
	*	Error indicating, that iteration has been already finished.
	*	This error is thrown when call of method requires enumerator not pointing at
	*	end enumeration guard.
	*/
	class IterationFinishedException : public Exception
	{
	public:
		/**
		*	Constructor.
		*	Creates new instance of IterationFinishedException with default error message.
		*/
		XLINQ_INLINE IterationFinishedException() : Exception("Iteration has been finished.") {}
	};
}

#endif