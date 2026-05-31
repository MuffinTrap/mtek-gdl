#pragma once

#if defined(USE_ANGEL_AS_CPP)
#	ifdef __cplusplus
	extern "C" {
#	endif
	// When angelscript is used as C++ code, the script functions are declared here
	void angelscript_init();
	void angelscript_frame(float deltatime);
	void angelscript_quit();

#	ifdef __cplusplus
	}
#	endif
#endif
