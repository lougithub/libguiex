/** 
* @file common_android.cpp
* @author ken
* @date 2011-05-10
*/

//============================================================================//
// include
//============================================================================// 
#include <jni.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <libguiex_framework/guiframework.h>


//============================================================================//
// declare
//============================================================================// 
guiex::CGUIFrameworkBase* g_pFramework = NULL;
guiex::CGUITimer g_aOldTimer;


extern guiex::CGUIFrameworkBase* CreateFramework( );

//============================================================================//
// function
//============================================================================// 
extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	guiex::CGUISystem::SetJavaVM( vm );

	return JNI_VERSION_1_4;
}
//------------------------------------------------------------------------------
extern "C" void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	guiex::CGUISystem::SetJavaVM( NULL );
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_SetApkPath(JNIEnv * env, jobject obj, jstring apkPath)
{
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	guiex::CGUIFramework_Android::SetApkPath( str );
	env->ReleaseStringUTFChars(apkPath, str);
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{    
	if( NULL == g_pFramework )
	{
		g_pFramework = CreateFramework( );
		g_pFramework->Initialize( guiex::CGUIIntSize( guiex::uint32(width), guiex::uint32(height)), "assets" );
	}

	g_aOldTimer.UpdateTime();
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_Exit(JNIEnv * env, jobject obj )
{    
	if( g_pFramework )
	{
		g_pFramework->Release();
		delete g_pFramework;
		g_pFramework = NULL;
	}
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_Step(JNIEnv * env, jobject obj)
{
	guiex::CGUITimer aCurTimer;
	aCurTimer.UpdateTime();
	guiex::real fDeltaTime = (aCurTimer - g_aOldTimer) / 1000.f;
	g_aOldTimer = aCurTimer;

	g_pFramework->Update( fDeltaTime );

	// do rendering for this frame.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	g_pFramework->Render();
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_onPause(JNIEnv * env, jobject obj)
{
	guiex::GSystem->UnloadAllResource();
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_onResume(JNIEnv * env, jobject obj)
{
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_OnFingerUp(JNIEnv * env, jobject obj, float x, float y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(guiex::CGUIVector2(x,y));
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_LEFT, guiex::MOUSE_UP );
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_OnFingerDown(JNIEnv * env, jobject obj, float x, float y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(guiex::CGUIVector2(x,y));
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_LEFT, guiex::MOUSE_DOWN );
}
//------------------------------------------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_org_guiex_lib_GuiexLibNative_OnFingerMove(JNIEnv * env, jobject obj, float x, float y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(guiex::CGUIVector2(x,y));
}
//------------------------------------------------------------------------------
