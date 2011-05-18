package org.guiex.lib;

public class GuiexLibNative
{

     static 
	 {
         System.loadLibrary("guiex_lib");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void Init(int width, int height);
     public static native void Step();
     public static native void SetApkPath( String apkFilePath ); 
     
     public static native void onPause();
     public static native void onResume();
     
     public static native void OnFingerUp( float x, float y );
     public static native void OnFingerDown( float x, float y );
     public static native void OnFingerMove( float x, float y );
}
