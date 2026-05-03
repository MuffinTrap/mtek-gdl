#pragma once

/**
 * @file scriptmgdl.h
 * @ingroup angelscript
 * @brief Registerintg mgdl to AngelScript
 */

#include <angelscript.h>
#include <mgdl/mgdl-types.h>


#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Registers mgdl types and functions to AngelScript
	 * @param as_engine AngelScript engine object
	 */
	void RegisterMGDL(asIScriptEngine* as_engine);

#ifdef __cplusplus
}
#endif


