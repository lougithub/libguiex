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

extern "C"
{
    JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_Init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_Step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_SetApkPath(JNIEnv * env, jobject obj, jstring apkPath);
    JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_OnFingerUp(JNIEnv * env, jobject obj, float x, float y);
    JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_OnFingerDown(JNIEnv * env, jobject obj, float x, float y);
    JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_OnFingerMove(JNIEnv * env, jobject obj, float x, float y);
};

//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_SetApkPath(JNIEnv * env, jobject obj, jstring apkPath)
{
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	guiex::CGUIFramework_Android::SetApkPath( str );
	env->ReleaseStringUTFChars(apkPath, str);
}
//------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{    
	if( NULL == g_pFramework )
	{
		g_pFramework = CreateFramework( );
		g_pFramework->Initialize( guiex::CGUIIntSize( guiex::uint32(width), guiex::uint32(height)), "assets" );
	}

	g_aOldTimer.UpdateTime();
}
//------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_Step(JNIEnv * env, jobject obj)
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
JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_OnFingerUp(JNIEnv * env, jobject obj, float x, float y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(guiex::CGUIVector2(x,y));
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_LEFT, guiex::MOUSE_UP );
}
//------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_OnFingerDown(JNIEnv * env, jobject obj, float x, float y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(guiex::CGUIVector2(x,y));
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( guiex::MOUSE_LEFT, guiex::MOUSE_DOWN );
}
//------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_android_GuiexSample_GuiexSampleLib_OnFingerMove(JNIEnv * env, jobject obj, float x, float y)
{
	guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(guiex::CGUIVector2(x,y));
}
//------------------------------------------------------------------------------
