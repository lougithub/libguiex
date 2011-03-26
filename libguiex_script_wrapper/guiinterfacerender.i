namespace guiex
{
	class IGUIInterfaceRender
	{
	public:
		virtual void SetWireFrame( bool bWireFrame);
		virtual bool IsWireFrame( ) const;

		virtual void EnableClip( bool bEnable );
		virtual bool IsEnableClip( ) const;


	protected:
		IGUIInterfaceRender();
		~IGUIInterfaceRender();
	};
}

