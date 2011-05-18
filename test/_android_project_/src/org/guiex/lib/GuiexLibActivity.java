package org.guiex.lib;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class GuiexLibActivity extends Activity
{
    GuiexLibView mView;
    
//    abstract public GuiexLibView CreateView();

    /** Called when the activity is first created. */
    @Override public void onCreate(Bundle savedInstanceState)
    {
    	Log.v("GuiexLibActivity", "onCreate");
    	
        super.onCreate(savedInstanceState);
        //mView = CreateView();
        mView = new GuiexLibView(getApplication());
        setContentView(mView);
    }

    @Override protected void onDestroy()
	 {
    	Log.v("GuiexLibActivity", "onDestroy");
		super.onDestroy();
		 
		android.os.Process.killProcess(android.os.Process.myPid());
	 }


    @Override protected void onPause()
	{
    	Log.i("GuiexLibActivity", "onPause");
    	
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() 
	{
    	Log.i("GuiexLibActivity", "onResume");
    	
        super.onResume();
        mView.onResume();
    }
}
