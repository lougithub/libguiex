namespace guiex
{
	typedef uint32 GUIARGB;
	typedef uint32 GUIRGBA;

	class CGUIColor
	{
	public:
		CGUIColor();
		CGUIColor(const CGUIColor &rColor);
		CGUIColor(GUIARGB argb);
		CGUIColor(real fRed, real fGreen, real fBlue, real fAlpha=1.0f );

		void SetARGB(GUIARGB argb);
		GUIARGB	GetARGB() const;

		GUIRGBA GetRGBA() const;

		static GUIRGBA ARGB2RGBA( GUIARGB aColorARGB );

		void SetColor(real fRed, real fGreen, real fBlue, real fAlpha = 1.0f);
		void SetColor(const CGUIColor& rColor);

		real GetAlpha() const;
		void SetAlpha(real fAlpha);

		real GetRed() const;
		void SetRed(real fRed);

		real GetGreen() const;
		void SetGreen(real fGreen);

		real GetBlue() const;
		void SetBlue(real fBlue);

		void InvertColor(void);
		void InvertColorWithAlpha(void);
	};
}
