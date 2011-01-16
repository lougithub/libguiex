namespace guiex
{
	class CGUIWgtBox2DBase : public CGUIWidget
	{
	};
	
	class CGUIWgtBox2DBodyBase : public CGUIWgtBox2DBase
	{
	};
	
	class CGUIWgtBox2DDynamicBody : public CGUIWgtBox2DBodyBase
	{
	public:
		void SetVelocity( real fVelocityX, real fVelocityY );
	};
	
	class CGUIWgtBox2DStaticBody : public CGUIWgtBox2DBodyBase
	{
	};
}
