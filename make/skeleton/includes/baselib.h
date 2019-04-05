#ifndef CPPAPE_RUNTIME_H
#define CPPAPE_RUNTIME_H

#ifndef __cplusplus
#error baselib.h can only be used with a C++ compiler
#endif


#include <cstddef>
#include "shared-src/ape/SharedInterface.h"
#undef NULL
#define NULL 0


// #define CPPAPE_RELEASE

#ifdef CPPAPE_RELEASE
#define CPPAPE_NOEXCEPT_IF_RELEASE noexcept
#else
#define CPPAPE_NOEXCEPT_IF_RELEASE
#endif



int printf(const char * fmt, ...);
[[noreturn]] void abort(const char* reason);

#define CPPAPE_CONCAT_IMPL( x, y ) x##y
#define CPPAPE_MACRO_CONCAT( x, y ) CPPAPE_CONCAT_IMPL( x, y )

#define printf_once(...) \
	static bool CPPAPE_MACRO_CONCAT(__once_flag, __COUNTER__) = false; \
	if(!(CPPAPE_MACRO_CONCAT(__once_flag, __COUNTER__))) \
	{	\
		CPPAPE_MACRO_CONCAT(__once_flag, __COUNTER__) = true; \
		printf(__VA_ARGS__); \
	}

namespace ape
{
	APE_SharedInterface& getInterface();

	struct StatusCode
	{
		static constexpr APE_Status
			Ok = STATUS_OK,
			Error = STATUS_ERROR,
			Wait = STATUS_WAIT,
			Silent = STATUS_SILENT,
			Ready = STATUS_READY,
			Disabled = STATUS_DISABLED,
			Handled = STATUS_HANDLED,
			NotImplemented = STATUS_NOT_IMPLEMENTED;

	};
}



#endif