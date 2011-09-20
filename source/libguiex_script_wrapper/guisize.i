namespace guiex
{
	class CGUISize
	{
	public:
		CGUISize();
		CGUISize(real fWidth, real fHeight);
		CGUISize(const CGUISize& rSize);

		void SetSize( const CGUISize& rSize);
		void SetValue( real fWidth, real fHeight );

		void SetWidth( real fWidth);
		real GetWidth( ) const ;

		void SetHeight( real fHeight);
		real GetHeight( ) const ;

		bool IsEqualZero( ) const;
		bool IsEqual( const CGUISize& rSize ) const;
	};
}

