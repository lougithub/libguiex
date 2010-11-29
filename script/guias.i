namespace guiex
{
	class CGUIAs
	{
	public:
		const CGUIString&		GetAsName() const;
		CGUIWidget*	GetReceiver();
		void		PushSuccessor( CGUIAs* pAs);
		virtual uint32	Process() = 0;
	};

	class CGUIAsAlpha : public CGUIAs
	{
	public:
		void	SetAlphaSequence(real fStartValue, real fEndValue, uint32 nFrames, uint32 nDelayTime);
	};
	CGUIAsAlpha* CreateAs_CGUIAsAlpha(CGUIWidget* pReceiver);

	class CGUIAsScale: public CGUIAs
	{
	public:
		void	SetScaleSequence(const CGUISize& fStartScale, const CGUISize& fEndScale, uint32 nFrames, uint32 nDelayTime);
	};
	CGUIAsScale* CreateAs_CGUIAsScale(CGUIWidget* pReceiver);

}
