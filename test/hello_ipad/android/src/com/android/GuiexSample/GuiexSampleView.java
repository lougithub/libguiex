package com.android.GuiexSample;


import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

class GuiexSampleView extends GLSurfaceView 
{
    public GuiexSampleView(Context context) 
	{
        super(context);
        init( );
    }

    private void init() 
	{
    	setEGLConfigChooser(5, 6, 5, 0, 0, 0); 
    	
        /* Set the renderer responsible for frame rendering */
        setRenderer(new Renderer());
    }

//    private static void checkEglError(String prompt, EGL10 egl) 
//	{
//        int error;
//        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) 
//		{
//            Log.e("GuiexSampleView", String.format("%s: EGL error: 0x%x", prompt, error));
//        }
//    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        public void onDrawFrame(GL10 gl) 
		{
        	//Log.i("GuiexSampleView", "onDrawFrame");
        	 GuiexSampleLib.step();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) 
		{
        	Log.i("GuiexSampleView", "onSurfaceChanged");
            GuiexSampleLib.init(width, height); 
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) 
		{
            // Do nothing.
        }
    }
}
