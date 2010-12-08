namespace guiex
{
	class CGUIAs
	{
	protected:
		CGUIAs();
		virtual ~CGUIAs();
	};
	
	template< class T >
	class CGUIInterpolationBase : public CGUIAs
	{
	protected:
		CGUIInterpolationBase();
		virtual ~CGUIInterpolationBase();
	};
	%template(CGUIInterpolationBase_real) CGUIInterpolationBase<real>;
	%template(CGUIInterpolationBase_size) CGUIInterpolationBase<CGUISize>;
	%template(CGUIInterpolationBase_Vector3) CGUIInterpolationBase<CGUIVector3>;
	%template(CGUIInterpolationBase_Vector2) CGUIInterpolationBase<CGUIVector2>;
	%template(CGUIInterpolationBase_Color) CGUIInterpolationBase<CGUIColor>;
	
	class CGUIAsAlpha : public CGUIInterpolationBase<real>
	{
	protected:
		CGUIAsAlpha();
		virtual ~CGUIAsAlpha();
	};

	class CGUIAsScale : public CGUIInterpolationBase<CGUISize>
	{
	protected:
		CGUIAsScale();
		virtual ~CGUIAsScale();
	};
	
	class CGUIAsRotation: public CGUIInterpolationBase<CGUIVector3>
	{
	protected:
		CGUIAsRotation();
		virtual ~CGUIAsRotation();
	};	
	
	class CGUIAsPosition : public CGUIInterpolationBase<CGUIVector2>
	{
	protected:
		CGUIAsPosition();
		virtual ~CGUIAsPosition();
	};
	
	class CGUIAsColor : public CGUIInterpolationBase<CGUIColor>
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
