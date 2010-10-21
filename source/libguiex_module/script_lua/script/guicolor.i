/***********************************************************
color
***********************************************************/


namespace guiex
{

	typedef uint32 GUIARGB;

	class CGUIColor
	{
	public:
		CGUIColor();
		CGUIColor(const CGUIColor &rColor);
		CGUIColor(GUIARGB argb);
		CGUIColor(real fRed, real fGreen, real fBlue, real fAlpha=1.0f );
		~CGUIColor();

		GUIARGB		GetARGB() const;
		void		SetARGB(GUIARGB argb);
		void		SetARGB(real fRed, real fGreen, real fBlue, real fAlpha = 1.0f);
		void		SetColor(const CGUIColor& rColor);

		real		GetAlpha() const;
		void		SetAlpha(real fAlpha);
		real		GetRed() const;
		void		SetRed(real fRed);
		real		GetGreen() const;
		void		SetGreen(real fGreen);
		real		GetBlue() const;
		void		SetBlue(real fBlue);

		void		InvertColor(void);
		void		InvertColorWithAlpha(void);


		CGUIColor	operator+(const CGUIColor& rColor) const;
		CGUIColor	operator+(GUIARGB argb) const;

		CGUIColor	operator-(const CGUIColor& rColor) const;
		CGUIColor	operator-(GUIARGB argb) const;

		bool		operator==(const CGUIColor &rColor ) const;
	};
}
