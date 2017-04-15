#ifndef XLINQ_EXCEPTION_H_
#define XLINQ_EXCEPTION_H_

#include <exception>
#include <string>

namespace xlinq
{
	/**
		Base class for all exceptions.
	*/
	class Exception : public std::exception
	{
	public:
		/** Constructor (C strings).
		*  @param message C-style string error message.
		*                 The string contents are copied upon construction.
		*                 Hence, responsibility for deleting the char* lies
		*                 with the caller.
		*/
		explicit Exception(const char* message) :
			msg_(message)
		{
		}

		/** Constructor (C++ STL strings).
		*  @param message The error message.
		*/
		explicit Exception(const std::string& message) :
			msg_(message)
		{}

		/** Destructor.
		* Virtual to allow for subclassing.
		*/
		virtual ~Exception() throw () {}

		/** Returns a pointer to the (constant) error description.
		*  @return A pointer to a const char*. The underlying memory
		*          is in posession of the Exception object. Callers must
		*          not attempt to free the memory.
		*/
		virtual const char* what() const throw () {
			return msg_.c_str();
		}

	protected:
		/** Error message.
		*/
		std::string msg_;
	};

	/**
		Error thrown when function must be called during iteration, but it was not started yet.
	*/
	class IterationNotStartedException : public Exception
	{
	public:
		IterationNotStartedException() : Exception("Iteration has not been started.") {}
	};

	/**
		Error thrown when function must be called during iteration, but it was already finished.
	*/
	class IterationFinishedException : public Exception
	{
	public:
		IterationFinishedException() : Exception("Iteration has been finished.") {}
	};
}

#endif