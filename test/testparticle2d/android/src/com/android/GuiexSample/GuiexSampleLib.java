package com.android.GuiexSample;


// Wrapper for native library

public class GuiexSampleLib {

     static {
         System.loadLibrary("guiex_sample");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void Init(int width, int height);
     public static native void Step();
     public static native void SetApkPath( String apkFilePath ); 
     
     public static native void OnFingerUp( float x, float y );
     public static native void OnFingerDown( float x, float y );
     public static native void OnFingerMove( float x, float y );
}
