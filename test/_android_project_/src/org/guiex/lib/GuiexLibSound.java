package org.guiex.lib;

import java.util.HashMap;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.util.Log;

public class GuiexLibSound
{
	static private Context mContext;
	
	static private final int MAX_SIMULTANEOUS_STREAMS_DEFAULT = 5;
	static private final float SOUND_RATE = 1.0f;
	static private final int SOUND_PRIORITY = 1;
	static private final int SOUND_LOOP_TIME = 0;
	static private final int SOUND_QUALITY = 5;
	
	static private float mLeftVolume = 1.0f;
	static private float mRightVolume = 1.0f;
		
	static private final int INVALID_SOUND_ID = -1;
	static private final int INVALID_STREAM_ID = -1;
	
	//effect
	static private SoundPool mSoundPool = new SoundPool(MAX_SIMULTANEOUS_STREAMS_DEFAULT, AudioManager.STREAM_MUSIC, SOUND_QUALITY);
	// sound id and stream id map
	static private HashMap<Integer,Integer> mSoundIdStreamIdMap = new HashMap<Integer,Integer>();
	// sound path and sound id map
	static private HashMap<String,Integer> mPathSoundIDMap = new HashMap<String,Integer>();
	

	//music
	static private MediaPlayer mBackgroundMediaPlayer = null;
	static private String mCurrentMusicPath;
	
	static public void SetContext( Context context )
	{
		mContext = context;
	}
	
	static public int LoadEffect(String path)
	{
		int soundId = INVALID_SOUND_ID;
		
		Log.i("guiex", "in java: load effect£º "+ path );
		
		// if the sound is preloaded, pass it
		if (mPathSoundIDMap.get(path) != null)
		{
			soundId =  mPathSoundIDMap.get(path).intValue();
		}  
		else
		{
			try 
			{
				soundId = mSoundPool.load(mContext.getAssets().openFd(path), 0);
			} 
			catch(Exception e)
			{
				soundId = INVALID_SOUND_ID;
				Log.e("guiex", "error: " + e.getMessage(), e);
			}	
			
			if (soundId != INVALID_SOUND_ID)
			{
				// the sound is loaded but has not been played
				mSoundIdStreamIdMap.put(soundId, INVALID_STREAM_ID);
				
				// record path and sound id map
				mPathSoundIDMap.put(path, soundId);
			}
		}
				
		return soundId;
	}
	
	public void UnloadEffect(String path)
	{
		Log.i("guiex", "in java: unload effect" + path );
		
		// get sound id and remove from mPathSoundIDMap
		Integer soundId = mPathSoundIDMap.remove(path);
		
		if (soundId != null)
		{
			// unload effect
			mSoundPool.unload(soundId.intValue());
			
			// remove record from mSoundIdStreamIdMap
			mSoundIdStreamIdMap.remove(soundId);
		}
	}
	
	static public void PlayEffect(int soundId)
	{
		// play sound
		int streamId = mSoundPool.play(soundId, mLeftVolume, 
				mRightVolume, SOUND_PRIORITY, SOUND_LOOP_TIME, SOUND_RATE);
		
		// record sound id and stream id map
		mSoundIdStreamIdMap.put(soundId, streamId);
	}
	
	static public void StopEffect(int soundId)
	{
        Integer streamId = mSoundIdStreamIdMap.get(soundId);
        
        if (streamId != null && streamId.intValue() != INVALID_STREAM_ID)
        {
        	mSoundPool.stop(streamId.intValue());
        }
	}
	
	static public void PauseEffect(int soundId)
	{
        Integer streamId = mSoundIdStreamIdMap.get(soundId);
        
        if (streamId != null && streamId.intValue() != INVALID_STREAM_ID)
        {
        	mSoundPool.pause(streamId.intValue());
        }
    }	
	
	static public boolean IsPlayingEffect(int soundId)
	{
		return false;
	}
		
	
	static public void LoadMusic(String path)
	{
		if (mBackgroundMediaPlayer != null)
		{
			throw new RuntimeException("failed to load music, music is playing...");
		}
	
		try
		{			
			AssetFileDescriptor assetFileDescritor = mContext.getAssets().openFd(path);
			
			mBackgroundMediaPlayer = new MediaPlayer();
			mBackgroundMediaPlayer.setDataSource(assetFileDescritor.getFileDescriptor(), 
	        		assetFileDescritor.getStartOffset(), assetFileDescritor.getLength());
			mBackgroundMediaPlayer.prepare();
	        
			mBackgroundMediaPlayer.setVolume(mLeftVolume, mRightVolume);
			
			mCurrentMusicPath = path;
		}
		catch (Exception e)
		{
			mBackgroundMediaPlayer = null;
			mCurrentMusicPath = "";
            Log.e("guiex", "error: " + e.getMessage(), e);
        }
	}
	
	static public void UnloadMusic(String path)
	{
		if( !mCurrentMusicPath.equals(path) )
		{
			throw new RuntimeException("failed to unload music: " + path + ", current music is: " + mCurrentMusicPath );
		}
		
		if( mBackgroundMediaPlayer == null )
		{
			throw new RuntimeException("failed to unload music: " + path );
		}
		
		mBackgroundMediaPlayer.release();
		mBackgroundMediaPlayer = null;
		mCurrentMusicPath = "";
	}
	

	static public void PlayMusic( boolean isLoop )
	{
		if( mBackgroundMediaPlayer != null )
		{
			mBackgroundMediaPlayer.stop();
			mBackgroundMediaPlayer.setLooping(isLoop);
			try 
			{
				mBackgroundMediaPlayer.prepare();
				mBackgroundMediaPlayer.seekTo(0);
				mBackgroundMediaPlayer.start();	
			} 
			catch (Exception e)
			{
	            Log.e("guiex", "PlayMusic error: " + e.getMessage(), e);
			}	
		}		
	}
	
	static public void StopMusic()
	{
		if (mBackgroundMediaPlayer != null)
		{
			mBackgroundMediaPlayer.stop();
		}
	}
	
	static public void PauseMusic()
	{		
		if (mBackgroundMediaPlayer != null && mBackgroundMediaPlayer.isPlaying())
		{
			mBackgroundMediaPlayer.pause();
		}
	}
	
	static public void ResumeMusic()
	{
		if (mBackgroundMediaPlayer != null && mBackgroundMediaPlayer.isPlaying() == false )
		{
			mBackgroundMediaPlayer.start();
		}
	}
	
	static public void RewindMusic()
	{		
		if (mBackgroundMediaPlayer != null)
		{
			mBackgroundMediaPlayer.stop();			
			
			try 
			{
				mBackgroundMediaPlayer.prepare();
				mBackgroundMediaPlayer.seekTo(0);
				mBackgroundMediaPlayer.start();
			} 
			catch (Exception e)
			{
				Log.e("guiex", "RewindMusic: error state");
			}			
		}
	}
	
	static public boolean IsPlayingMusic()
	{
		if (mBackgroundMediaPlayer != null)
		{
			return mBackgroundMediaPlayer.isPlaying();
		}
		return false;
	}	
}
