namespace guiex
{
	class CGUIAs
	{
	protected:
		CGUIAs();
		virtual ~CGUIAs();
	};
	
	template< class T >
	class CGUIAsInterpolation : public CGUIAs
	{
	protected:
		CGUIAsInterpolation();
		virtual ~CGUIAsInterpolation();
	};
	%template(CGUIInterpolationBase_real) CGUIAsInterpolation<real>;
	%template(CGUIInterpolationBase_size) CGUIAsInterpolation<CGUISize>;
	%template(CGUIInterpolationBase_Vector3) CGUIAsInterpolation<CGUIVector3>;
	%template(CGUIInterpolationBase_Vector2) CGUIAsInterpolation<CGUIVector2>;
	%template(CGUIInterpolationBase_Color) CGUIAsInterpolation<CGUIColor>;
	
	class CGUIAsAlpha : public CGUIAsInterpolation<real>
	{
	protected:
		CGUIAsAlpha();
		virtual ~CGUIAsAlpha();
	};

	class CGUIAsScale : public CGUIAsInterpolation<CGUISize>
	{
	protected:
		CGUIAsScale();
		virtual ~CGUIAsScale();
	};
	
	class CGUIAsRotation: public CGUIAsInterpolation<CGUIVector3>
	{
	protected:
		CGUIAsRotation();
		virtual ~CGUIAsRotation();
	};	
	
	class CGUIAsPosition : public CGUIAsInterpolation<CGUIVector2>
	{
	protected:
		CGUIAsPosition();
		virtual ~CGUIAsPosition();
	};
	
	class CGUIAsColor : public CGUIAsInterpolation<CGUIColor>
	{
	protected:
		CGUIAsColor();
		virtual ~CGUIAsColor();
	};
	
	class CGUIAsContainer : public CGUIAs
	{
	protected:
		CGUIAsContainer();
		virtual ~CGUIAsContainer();
	};
}
