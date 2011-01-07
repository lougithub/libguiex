namespace guiex
{
	class IGUIInterfaceSound
	{
	public:
		virtual void StopMusic( );
		virtual void PauseMusic( );
		virtual bool IsPlayingMusic( );


		void PlayEffect( const CGUIString& rSoundName );
		void PlayMusic( const CGUIString& rMusicName );

	protected:
		IGUIInterfaceSound();
		~IGUIInterfaceSound();
	};
}

