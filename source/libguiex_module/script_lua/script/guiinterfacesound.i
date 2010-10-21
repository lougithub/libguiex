namespace guiex
{
	class IGUIInterfaceSound : public IGUIInterface
	{
	public:
		virtual int32	LoadEffect( int32 nIdx,  const CGUIString& rFileName) = 0;
		virtual void	UnloadEffect( int32 nIdx ) = 0;
		virtual int32	PlayEffect( int32 nIdx ) = 0;

	};


	IGUIInterfaceSound*	GetInterface_Sound( );
}

