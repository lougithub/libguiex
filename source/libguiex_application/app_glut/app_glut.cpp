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

#include <iostream>
#include <time.h>


#if defined(GUIEX_PLATFORM_WIN32)
#include <libguiex_module/ime_winapi/guiime_winapi.h>
#endif

#include <GL/freeglut.h>

//============================================================================//
// declare
//============================================================================// 
//using namespace guiex;
const guiex::int32 g_nScreenWidth = 1024;
const guiex::int32 g_nScreenHeight = 768;
guiex::CGUIFrameworkBase* g_pFramework = NULL;
std::map<int,guiex::EKeyCode> g_mapKey_Glut2Guiex;
guiex::CGUITimer g_aOldTimer;
extern guiex::CGUIFrameworkBase* GUIEXCreateFramework( );
extern const char* GUIEXGetDataDir();
int g_nVSync = 1;

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
#if defined GUIEX_PLATFORM_WIN32

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
 
#elif defined GUIEX_PLATFORM_MAC
void setVSync( int interval = 1 )
{
	// a swap interval of 1 causes SwapBuffers to wait until the 
	// next vertical sync, avoiding possible tears in your images.
	// Note that this limits your framerate to 60 fps or so,
	// so don't forget to turn it OFF if you are doing render timing.
	
	CGLContextObj cgl_context = CGLGetCurrentContext();
	CGLSetParameter(cgl_context, kCGLCPSwapInterval, &interval);
}
#endif
//------------------------------------------------------------------------------
void QuitApp()
{
	if( g_pFramework )
	{
		g_pFramework->Release();
		delete g_pFramework;
		g_pFramework = NULL;
	}

	exit(0);
}
//------------------------------------------------------------------------------
void mouseMotionCB(int x, int y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(guiex::CGUIVector2((guiex::real)x,(guiex::real)y));
}
//------------------------------------------------------------------------------
void mouseWheel(int wheel, int direction, int x, int y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeWheel(guiex::real(wheel));
}
//------------------------------------------------------------------------------
void mouseCB(int button, int state, int x, int y)
{
	try
	{

		switch(button)
		{
		case  GLUT_LEFT_BUTTON:
			if (state == GLUT_UP)
			{
				guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_LEFT, guiex::MOUSE_UP );
			}
			else
			{
				guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_LEFT, guiex::MOUSE_DOWN );
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_UP)
			{
				guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_RIGHT, guiex::MOUSE_UP );
			}
			else
			{
				guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_RIGHT, guiex::MOUSE_DOWN );
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_UP)
			{
				guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_MIDDLE, guiex::MOUSE_UP );
			}
			else
			{
				guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_MIDDLE, guiex::MOUSE_DOWN );
			}
			break;
		}
	}
	catch (guiex::CGUIBaseException& rError)
	{
#if defined(GUIEX_PLATFORM_WIN32)
		MessageBoxA( NULL,rError.what(), "error", MB_OK | MB_ICONHAND );
#endif
	}
}
//------------------------------------------------------------------------------
void keyboardCB(unsigned char key, int x, int y)
{
	// extract some keys may be handled via key code and generate those too
	switch (key)
	{
	case 0x1B:  // Escape
		QuitApp();
		break;

	case 0x08:  // backspace
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(guiex::KC_BACK, guiex::KEY_DOWN);
		break;

	case 0x7F:  // delete
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(guiex::KC_DELETE, guiex::KEY_DOWN);
		break;

	case 0x0D:  // CR (Return)
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(guiex::KC_ENTER, guiex::KEY_DOWN);
		break;

	case 'c':	//clip or not
		if( guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender())
		{
			guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender()->EnableClip( !guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender()->IsEnableClip() );
		}
		break;

	case 'w':	//wire frame or not
		if( guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender())
		{
			guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender()->SetWireFrame( !guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender()->IsWireFrame() );
		}
		break;

	case 'e':	//extra info or not
		if( guiex::GSystem )
		{
			guiex::GSystem->SetDrawExtraInfo( !guiex::GSystem->IsDrawExtraInfo() );
		}
		break;

//	case 'f':	//draw in full speed or not
//		g_nVSync = ( g_nVSync+1 ) % 2;
//		setVSync( g_nVSync );
//		break;
	
	case 'r':	//rotate screen
		guiex::GSystem->SetScreenOrientation( guiex::EScreenOrientation((guiex::GSystem->GetScreenOrientation() + 1) % (guiex::eScreenOrientation_LandscapeRight+1)));
		break;

	default:
		// inject Character code
		break;
	}
}
//------------------------------------------------------------------------------
void keyboardUpCB(unsigned char key, int x, int y)
{
	// extract some keys may be handled via key code and generate those too
	switch (key)
	{
	case 0x08:  // backspace
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(guiex::KC_BACK, guiex::KEY_UP);
		break;

	case 0x7F:  // delete
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(guiex::KC_DELETE, guiex::KEY_UP);
		break;

	case 0x0D:  // CR (Return)
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(guiex::KC_ENTER, guiex::KEY_UP);
		break;

	default:
		// inject Character code
		break;
	}
}
//------------------------------------------------------------------------------
void keySpecialCB(int key, int x, int y)
{
	std::map<int,guiex::EKeyCode>::iterator itor = g_mapKey_Glut2Guiex.find(key);
	if( itor != g_mapKey_Glut2Guiex.end())
	{
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(itor->second, guiex::KEY_DOWN);
	}
}
//------------------------------------------------------------------------------
void keyUpSpecialCB(int key, int x, int y)
{
	std::map<int,guiex::EKeyCode>::iterator itor = g_mapKey_Glut2Guiex.find(key);
	if( itor != g_mapKey_Glut2Guiex.end())
	{
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard()->ChangeKeyState(itor->second, guiex::KEY_UP);
	}
}
//------------------------------------------------------------------------------
void displayCB(void)
{
	// do updates
	guiex::CGUITimer aCurTimer;
	aCurTimer.UpdateTime();
	guiex::real fDeltaTime = (aCurTimer - g_aOldTimer) / 1000.f;
	g_aOldTimer = aCurTimer;

	g_pFramework->Update( fDeltaTime );

	// do rendering for this frame.
	g_pFramework->Render();
	glutPostRedisplay();
	glutSwapBuffers();
}
//------------------------------------------------------------------------------
void idleCB()
{
	glutPostRedisplay();
}
//------------------------------------------------------------------------------
void reshapeCB(int width, int height)
{
	guiex::GSystem->SetRawScreenSize(width,height);
}
//------------------------------------------------------------------------------
void RegisterKeyboard()
{
	g_mapKey_Glut2Guiex[GLUT_KEY_F1] = guiex::KC_F1;
	g_mapKey_Glut2Guiex[GLUT_KEY_F2] = guiex::KC_F2;
	g_mapKey_Glut2Guiex[GLUT_KEY_F3] = guiex::KC_F3;
	g_mapKey_Glut2Guiex[GLUT_KEY_F4] = guiex::KC_F4;
	g_mapKey_Glut2Guiex[GLUT_KEY_F5] = guiex::KC_F5;
	g_mapKey_Glut2Guiex[GLUT_KEY_F6] = guiex::KC_F6;
	g_mapKey_Glut2Guiex[GLUT_KEY_F7] = guiex::KC_F7;
	g_mapKey_Glut2Guiex[GLUT_KEY_F8] = guiex::KC_F8;
	g_mapKey_Glut2Guiex[GLUT_KEY_F9] = guiex::KC_F9;
	g_mapKey_Glut2Guiex[GLUT_KEY_F10] = guiex::KC_F10;
	g_mapKey_Glut2Guiex[GLUT_KEY_F11] = guiex::KC_F11;
	g_mapKey_Glut2Guiex[GLUT_KEY_F12] = guiex::KC_F12;
	g_mapKey_Glut2Guiex[GLUT_KEY_LEFT] = guiex::KC_LEFT;
	g_mapKey_Glut2Guiex[GLUT_KEY_UP] = guiex::KC_UP;
	g_mapKey_Glut2Guiex[GLUT_KEY_RIGHT] = guiex::KC_RIGHT;
	g_mapKey_Glut2Guiex[GLUT_KEY_DOWN] = guiex::KC_DOWN;
	g_mapKey_Glut2Guiex[GLUT_KEY_PAGE_UP] = guiex::KC_PAGEUP;
	g_mapKey_Glut2Guiex[GLUT_KEY_PAGE_DOWN] = guiex::KC_PAGEDOWN;
	g_mapKey_Glut2Guiex[GLUT_KEY_HOME] = guiex::KC_HOME;
	g_mapKey_Glut2Guiex[GLUT_KEY_END] = guiex::KC_END;
	g_mapKey_Glut2Guiex[GLUT_KEY_INSERT] = guiex::KC_INSERT;
}
//------------------------------------------------------------------------------
void exitCB()
{
	QuitApp();
}

//------------------------------------------------------------------------------
#if defined(GUIEX_PLATFORM_WIN32)
WNDPROC g_pfOldProc = NULL;
LRESULT CALLBACK MsgWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	try
	{
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceImeWithTypeCheck<guiex::IGUIIme_winapi>()->ProcessWindowMessage(hwnd, uMsg, wParam, lParam );
	}
	catch (guiex::CGUIBaseException& rError)
	{
		MessageBoxA( NULL,rError.what(), "error", MB_OK | MB_ICONHAND );
	}

	return(CallWindowProc(g_pfOldProc, hwnd, uMsg, wParam, lParam));
}
#endif

guiex::CGUIString GetGameDir(char** argv)
{
	//get data path
#if defined( GUIEX_PLATFORM_WIN32 )
	char fdir[_MAX_DIR];
	_splitpath( argv[0], NULL, fdir, NULL, NULL ); 
	guiex::CGUIString strDir = fdir;
	strDir += "../../data/";
	strDir += GUIEXGetDataDir();
#elif defined( GUIEX_PLATFORM_MAC )
	guiex::CGUIString strDir(dirname( argv[0]));
	strDir += "/../../";
	strDir += GUIEXGetDataDir();
#else
#	error "unknown platform"		
#endif
	return strDir;
}
//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
#if defined(GUIEX_PLATFORM_WIN32)
#ifdef	_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#endif
#endif

	srand( NULL );

	// Do GLUT init
	glutInitDisplayMode( GLUT_STENCIL | GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowSize( g_nScreenWidth, g_nScreenHeight );
	glutInitWindowPosition( 100, 100 );
	glutInit( &argc, argv );

	glutCreateWindow( (const char *)(L"libguiex demo"));
	glClearColor(0.7, 0.7, 0.7, 1.0);

	glutSetCursor( GLUT_CURSOR_INHERIT );

	glutReshapeFunc(reshapeCB);
	glutDisplayFunc( displayCB );
	glutIdleFunc(idleCB);
	glutMouseFunc( mouseCB );
	glutMouseWheelFunc(mouseWheel);
	glutMotionFunc( mouseMotionCB );
	glutPassiveMotionFunc( mouseMotionCB );
	glutKeyboardFunc( keyboardCB );
	glutSpecialFunc( keySpecialCB );
	glutKeyboardUpFunc( keyboardUpCB );
	glutSpecialUpFunc( keyUpSpecialCB );

	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );
	glutCloseFunc(QuitApp);

	//setVSync( g_nVSync );

	g_pFramework = GUIEXCreateFramework( );
	g_pFramework->Initialize( guiex::CGUIIntSize( g_nScreenWidth, g_nScreenHeight ), GetGameDir(argv).c_str() );
	RegisterKeyboard();

#if defined(GUIEX_PLATFORM_WIN32)
	HWND hWnd = GetTopWindow(NULL);
	g_pfOldProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)MsgWindowProc);
#endif

	g_aOldTimer.UpdateTime();
	glutMainLoop();

	return 0;
}
