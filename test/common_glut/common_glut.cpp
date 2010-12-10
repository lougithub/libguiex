/** 
* @file glut_common.cpp
* @brief test by glut
* @author ken
* @date 2010-12-10
*/

//============================================================================//
// include
//============================================================================// 

#include <libguiex_framework/guiframework.h>

#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

//============================================================================//
// declare
//============================================================================// 
using namespace guiex;
const int32 g_nScreenWidth = 1024;
const int32 g_nScreenHeight = 768;
CGUIFrameworkBase* g_pFramework = NULL;
std::map<int,EKeyCode> g_mapKey_Glut2Guiex;
CGUITimer g_aOldTimer;
extern CGUIFrameworkBase* CreateFramework( const CGUISize& rScreenSize, const char* pDataPath );

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
void QuitApp()
{
	if( g_pFramework )
	{
		g_pFramework->Release();
		delete g_pFramework;
		g_pFramework = NULL;
	}

	exit( 0 );
}
//------------------------------------------------------------------------------
void MouseMotion(int x, int y)
{
	CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(CGUIVector2(x,y));
}
//------------------------------------------------------------------------------
void MouseWheel(int wheel, int direction, int x, int y)
{
	CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeWheel(wheel);
}
//------------------------------------------------------------------------------
void MouseButton(int button, int state, int x, int y)
{
	switch(button)
	{
	case  GLUT_LEFT_BUTTON:
		if (state == GLUT_UP)
		{
			CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_LEFT, MOUSE_UP );
		}
		else
		{
			CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_LEFT, MOUSE_DOWN );
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP)
		{
			CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_RIGHT, MOUSE_UP );
		}
		else
		{
			CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_RIGHT, MOUSE_DOWN );
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_UP)
		{
			CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_MIDDLE, MOUSE_UP );
		}
		else
		{
			CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_MIDDLE, MOUSE_DOWN );
		}
		break;
	}
}
//------------------------------------------------------------------------------
void KeyChar(unsigned char key, int x, int y)
{
	// extract some keys may be handled via key code and generate those too
	int k = key;
	switch (key)
	{
	case 0x1B:  // Escape
		QuitApp();
		break;

	case 0x08:  // backspace
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(KC_BACK, KEY_DOWN);
		break;

	case 0x7F:  // delete
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(KC_DELETE, KEY_DOWN);
		break;

	case 0x0D:  // CR (Return)
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(KC_ENTER, KEY_DOWN);
		break;

	default:
		// inject Character code
		break;
	}
}
//------------------------------------------------------------------------------
void KeyUpChar(unsigned char key, int x, int y)
{
	// extract some keys may be handled via key code and generate those too
	int k = key;
	switch (key)
	{
	case 0x08:  // backspace
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(KC_BACK, KEY_UP);
		break;

	case 0x7F:  // delete
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(KC_DELETE, KEY_UP);
		break;

	case 0x0D:  // CR (Return)
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(KC_ENTER, KEY_UP);
		break;

	default:
		// inject Character code
		break;
	}
}
//------------------------------------------------------------------------------
void keySpecial(int key, int x, int y)
{
	std::map<int,EKeyCode>::iterator itor = g_mapKey_Glut2Guiex.find(key);
	if( itor != g_mapKey_Glut2Guiex.end())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(itor->second, KEY_DOWN);
	}
}
//------------------------------------------------------------------------------
void DrawFrame(void)
{
	// do updates
	CGUITimer aCurTimer;
	aCurTimer.UpdateTime();
	float fDeltaTime = (aCurTimer - g_aOldTimer) / 1000.f;
	g_aOldTimer = aCurTimer;

	g_pFramework->Update( fDeltaTime );

	// do rendering for this frame.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	g_pFramework->Render();
	glutPostRedisplay();
	glutSwapBuffers();
}
//------------------------------------------------------------------------------
void keyUpSpecial(int key, int x, int y)
{
	std::map<int,EKeyCode>::iterator itor = g_mapKey_Glut2Guiex.find(key);
	if( itor != g_mapKey_Glut2Guiex.end())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(itor->second, KEY_UP);
	}
}
//------------------------------------------------------------------------------
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
void setVSync(int interval=1)
{
	const char *extensions = (const char*)glGetString( GL_EXTENSIONS );

	if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
	{
		return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
	}
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

		if( wglSwapIntervalEXT )
		{
			wglSwapIntervalEXT(interval);
		}
	}
}
//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	g_mapKey_Glut2Guiex[GLUT_KEY_F1] = KC_F1;
	g_mapKey_Glut2Guiex[GLUT_KEY_F2] = KC_F2;
	g_mapKey_Glut2Guiex[GLUT_KEY_F3] = KC_F3;
	g_mapKey_Glut2Guiex[GLUT_KEY_F4] = KC_F4;
	g_mapKey_Glut2Guiex[GLUT_KEY_F5] = KC_F5;
	g_mapKey_Glut2Guiex[GLUT_KEY_F6] = KC_F6;
	g_mapKey_Glut2Guiex[GLUT_KEY_F7] = KC_F7;
	g_mapKey_Glut2Guiex[GLUT_KEY_F8] = KC_F8;
	g_mapKey_Glut2Guiex[GLUT_KEY_F9] = KC_F9;
	g_mapKey_Glut2Guiex[GLUT_KEY_F10] = KC_F10;
	g_mapKey_Glut2Guiex[GLUT_KEY_F11] = KC_F11;
	g_mapKey_Glut2Guiex[GLUT_KEY_F12] = KC_F12;
	g_mapKey_Glut2Guiex[GLUT_KEY_LEFT] = KC_LEFT;
	g_mapKey_Glut2Guiex[GLUT_KEY_UP] = KC_UP;
	g_mapKey_Glut2Guiex[GLUT_KEY_RIGHT] = KC_RIGHT;
	g_mapKey_Glut2Guiex[GLUT_KEY_DOWN] = KC_DOWN;
	g_mapKey_Glut2Guiex[GLUT_KEY_PAGE_UP] = KC_PAGEUP;
	g_mapKey_Glut2Guiex[GLUT_KEY_PAGE_DOWN] = KC_PAGEDOWN;
	g_mapKey_Glut2Guiex[GLUT_KEY_HOME] = KC_HOME;
	g_mapKey_Glut2Guiex[GLUT_KEY_END] = KC_END;
	g_mapKey_Glut2Guiex[GLUT_KEY_INSERT] = KC_INSERT;

	// Do GLUT init
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowSize( g_nScreenWidth, g_nScreenHeight );
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( "libguiex test" );
	glutSetCursor( GLUT_CURSOR_NONE );
	glutDisplayFunc( DrawFrame );
	glutMouseFunc( MouseButton );
	//glutMouseWheelFunc(MouseWheel);
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( MouseMotion );
	//glutReshapeFunc(reshape);
	glutKeyboardFunc( KeyChar );
	glutSpecialFunc( keySpecial );
	glutKeyboardUpFunc( KeyUpChar );
	glutSpecialUpFunc( keyUpSpecial );

	setVSync(0);

	// Set the clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glViewport(0,0,g_nScreenWidth,g_nScreenHeight);

	g_pFramework = CreateFramework( CGUISize( g_nScreenWidth, g_nScreenHeight ), "D:/_ipad_/_project_/libguiex_googlecode/data/test/" );
	g_pFramework->Initialize();

	glutMainLoop();

	return 0;
}
