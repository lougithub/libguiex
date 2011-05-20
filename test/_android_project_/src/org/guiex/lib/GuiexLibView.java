package org.guiex.lib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

public class GuiexLibView extends GLSurfaceView 
{
    public GuiexLibView(Context context) 
	{
        super(context);
        			
        init( context );
    }

    private void init( Context context ) 
	{
    	//setEGLConfigChooser(5, 6, 5, 0, 0, 8); 
		setEGLConfigChooser(8, 8, 8, 8, 8, 0); 
   	
    	
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = context.getPackageManager();
        
		try 
		{
		    appInfo = packMgmr.getApplicationInfo(context.getPackageName(), 0);
	    } 
		catch (NameNotFoundException e)
		{
		    e.printStackTrace();
		    throw new RuntimeException("Unable to locate assets, aborting...");
	    }
		apkFilePath = appInfo.sourceDir;
	    GuiexLibNative.SetApkPath( apkFilePath ); 
	    
	    setFocusableInTouchMode(true);
			
        /* Set the renderer responsible for frame rendering */
        setRenderer(new Renderer());
    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        public void onDrawFrame(GL10 gl) 
		{
        	//Log.i("GuiexLibView", "onDrawFrame");
        	 GuiexLibNative.Step();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) 
		{
        	Log.i("GuiexLibView", "onSurfaceChanged");
            GuiexLibNative.Init(width, height); 
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
        	GuiexLibNative.OnFingerDown(ev.getX(), ev.getY());
        	break;
        case  MotionEvent.ACTION_UP:
        	GuiexLibNative.OnFingerUp(ev.getX(), ev.getY());
        	break;
        case  MotionEvent.ACTION_MOVE:
        	GuiexLibNative.OnFingerMove(ev.getX(), ev.getY());
        	break;
        }
        return true;
    }
	
    @Override public void onPause()
	{
        super.onPause();
        GuiexLibNative.onPause( ); 
    }

    @Override public void onResume() 
	{
        super.onResume();
        GuiexLibNative.onResume( ); 
    }	
}
