#pragma once

#ifdef GEKKO
    #include <opengx.h>
    #include <GL/glu.h>
#else
	// Needed on non-Wii platforms to silence warnings about old OpenGL
	#define GL_SILENCE_DEPRECATION 1

    #if defined(__APPLE__)
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
        #include <GLUT/glut.h>
    #elif defined(__MINGW_32__)
#       include <Windows.h>
#       include <gL/GL.h>
#       include <gL/GLU.h>
#   elif defined(__linux__)
        #include <GL/gl.h>
        #include <GL/glu.h>
        #include <GL/glut.h>
    #endif
#endif

