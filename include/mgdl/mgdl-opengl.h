#pragma once

/**
 * @file mgdl-opengl.h
 * @brief includes the OpenGL needed by platform
 */
#ifdef GEKKO
    #include <opengx.h>
    #include <GL/glu.h>
#else
	// Needed on non-Wii platforms to silence warnings about old OpenGL
	#define GL_SILENCE_DEPRECATION 1

    #ifdef __APPLE__
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
        #include <GLUT/glut.h>
    #elif defined(MGDL_PLATFORM_WINDOWS)
#       include <Windows.h>
#       include <gL/GL.h>
#       include <gL/GLU.h>
#   else
        #include <GL/gl.h>
        #include <GL/glu.h>
        #include <GL/glut.h>
    #endif
#endif

