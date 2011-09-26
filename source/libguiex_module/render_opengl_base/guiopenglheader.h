/** 
* @file guiopenglheader.h
* @brief 
* @author ken
* @date 2011-09-26
*/

#ifndef __GUI_OPENGL_HEADER_20110926_H__
#define __GUI_OPENGL_HEADER_20110926_H__

#include <libguiex_core/guibase.h>

#if defined(GUIEX_TARGET_WIN32)
#	if defined(GUIEX_RENDER_OPENGL)
#		include <windows.h>
#		include <GL/glew.h>
#		include <GL/gl.h>
#		include <GL/glu.h>
#	elif defined(GUIEX_RENDER_OPENGL_ES1)
#		include <GLES/gl.h>
#		include <GLES/glext.h>
#		include <EGL/egl.h>
#	elif defined(GUIEX_RENDER_OPENGL_ES2)
#		include <GLES2/gl2.h>
#		include <GLES2/gl2ext.h>
#		include <EGL/egl.h>
#	else
#		error "unknown render type"	
#	endif
#elif defined(GUIEX_TARGET_IOS)
#	if defined(GUIEX_RENDER_OPENGL_ES1)
#		include <OpenGLES/ES1/gl.h>
#		include <OpenGLES/ES1/glext.h>
#	elif defined(GUIEX_RENDER_OPENGL_ES2)
#		error "not implemented"	
#	else
#		error "unknown render type"	
#	endif
#elif defined(GUIEX_TARGET_MACOS)
#	if defined(GUIEX_RENDER_OPENGL)
#		include <OpenGL/gl.h>
#		include <OpenGL/glu.h>
#	else
#		error "unknown render type"	
#	endif
#elif defined(GUIEX_TARGET_ANDROID)
#	if defined(GUIEX_RENDER_OPENGL_ES1)
#		include <GLES/gl.h>
#		include <GLES/glext.h>
#	elif defined(GUIEX_RENDER_OPENGL_ES2)
#		error "not implemented"	
#	else
#		error "unknown render type"	
#	endif
#else
#	error "unknown target"	
#endif

#endif //__GUI_OPENGL_HEADER_20110926_H__
