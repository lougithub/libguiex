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
     public static native void init(int width, int height);
     public static native void step();
}
