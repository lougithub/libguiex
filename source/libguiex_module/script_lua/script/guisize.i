
namespace guiex
{

	class CGUISize
	{
	public:
		CGUISize();
		CGUISize(real fWidth, real fHeight);
		CGUISize(const CGUISize& rSize);
		~CGUISize();

		void	SetSize( const CGUISize& rSize);
		void	SetWidth( real fWidth);
		real	GetWidth( ) const ;
		void	SetHeight( real fHeight);
		real	GetHeight(  ) const ;

		bool operator==(const CGUISize& other) const;
		bool operator<(const CGUISize& other) const;
		CGUISize operator+(const CGUISize& other) const;
		CGUISize operator-(const CGUISize& other) const;
		CGUISize operator*(real fScalar) const;
		CGUISize operator/(real fScalar) const;


		real m_fWidth;
		real m_fHeight;
	};
}

