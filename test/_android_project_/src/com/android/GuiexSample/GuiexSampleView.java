package com.android.GuiexSample;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

class GuiexSampleView extends GLSurfaceView 
{
    public GuiexSampleView(Context context) 
	{
        super(context);
        			
        init( context );
    }

    private void init( Context context ) 
	{
    	setEGLConfigChooser(5, 6, 5, 0, 0, 0); 
    	
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = context.getPackageManager();
        
		try 
		{
		    appInfo = packMgmr.getApplicationInfo("com.android.GuiexSample", 0);
	    } 
		catch (NameNotFoundException e)
		{
		    e.printStackTrace();
		    throw new RuntimeException("Unable to locate assets, aborting...");
	    }
		apkFilePath = appInfo.sourceDir;
	    GuiexSampleLib.SetApkPath( apkFilePath ); 
			
        /* Set the renderer responsible for frame rendering */
        setRenderer(new Renderer());
    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        public void onDrawFrame(GL10 gl) 
		{
        	//Log.i("GuiexSampleView", "onDrawFrame");
        	 GuiexSampleLib.Step();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) 
		{
        	Log.i("GuiexSampleView", "onSurfaceChanged");
            GuiexSampleLib.Init(width, height); 
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) 
		{
            // Do nothing.
        }
    }
    
	@Override public boolean dispatchTouchEvent(MotionEvent ev) 
	{
        switch( ev.getAction())
        {
        case  MotionEvent.ACTION_DOWN:
        	GuiexSampleLib.OnFingerDown(ev.getX(), ev.getY());
        	break;
        case  MotionEvent.ACTION_UP:
        	GuiexSampleLib.OnFingerUp(ev.getX(), ev.getY());
        	break;
        case  MotionEvent.ACTION_MOVE:
        	GuiexSampleLib.OnFingerMove(ev.getX(), ev.getY());
        	break;
        }
        return true;
    }
	
    @Override public void onPause()
	{
        super.onPause();
        GuiexSampleLib.onPause( ); 
    }

    @Override public void onResume() 
	{
        super.onResume();
        GuiexSampleLib.onResume( ); 
    }	
}
