namespace guiex
{
	class IGUIInterfaceSound
	{
	public:
		virtual void StopMusic( );
		virtual void PauseMusic( );
		virtual void ResumeMusic( );
		virtual void RewindMusic( );
		virtual bool IsPlayingMusic( );


		void PlayEffect( const CGUIString& rSoundName );
		void PlayMusic( const CGUIString& rMusicName, bool bLoop );
	protected:
		IGUIInterfaceSound();
		~IGUIInterfaceSound();
	};
}

