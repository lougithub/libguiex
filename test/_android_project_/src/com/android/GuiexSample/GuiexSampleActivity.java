package com.android.GuiexSample;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class GuiexSampleActivity extends Activity
{
    GuiexSampleView mView;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
    	Log.v("GuiexSampleActivity", "onCreate");
    	
        super.onCreate(savedInstanceState);
        mView = new GuiexSampleView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause()
	{
    	Log.i("GuiexSampleActivity", "onPause");
    	
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() 
	{
    	Log.i("GuiexSampleActivity", "onResume");
    	
        super.onResume();
        mView.onResume();
    }
}
