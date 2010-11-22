/** 
* @file guiwgtframe.h
* @brief just like mfc's frame. is a title now.
* @author ken
* @date 2006-07-20
*/


#ifndef __GUI_WGTFRAME_20060720_H__
#define __GUI_WGTFRAME_20060720_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtFrame
	* @brief frame, can be used as a title bar, can be
	* dragged to move.
	* used image name:
	*		-FRAME_TITLEBAR_TOP or FRAME_TITLEBAR_BOTTOM or FRAME_TITLEBAR_LEFT or FRAME_TITLEBAR_RIGHT
	*		-FRAME_DECORATE_0
	*		-FRAME_DECORATE_1
	*		-FRAME_BORDER_LEFT
	*		-FRAME_BORDER_TOP
	*		-FRAME_BORDER_BOTTOM
	*		-FRAME_BORDER_RIGHT
	*		-FRAME_BORDER_TOPLEFT
	*		-FRAME_BORDER_TOPRIGHT
	*		-FRAME_BORDER_BOTTOMLEFT
	*		-FRAME_BORDER_BOTTOMRIGHT
	*/
	class GUIEXPORT CGUIWgtFrame : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtFrame( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief create this widget
		*/
		virtual void Create();

		/**
		* @brief whether this frame has title bar.
		* hasn't title bar means this frame couldn't move
		*/
		bool	HasTitlebar() const;

		/**
		* @brief whether this title bar has decorate.
		*/
		bool	HasTitlebarDecorate();

		/**
		* @brief whether show client or not
		*/
		bool	IsClientVisible() const;

		/**
		* @brief whether title bar is horizonal 
		*/
		bool		IsHorizonalTitlebar() const;

		/**
		* @brief whether title bar is vertical 
		*/
		bool		IsVerticalTitlebar() const;

		/**
		* @brief is this point in the title bar
		* @param rPos global position
		*/
		bool		IsPointInTitlebar( const CGUIVector2& rPos);

		/**
		* @brief test whether the point is in this widget
		* @param rPos in pixel format
		*/
		virtual bool	HitTest( const CGUIVector2& rPos) ;

		/**
		* @brief location of border
		*/
		enum EBorderLocation
		{
			FRAME_BORDER_TOPLEFT = 0,			
			FRAME_BORDER_TOPRIGHT,		
			FRAME_BORDER_BOTTOMLEFT,		
			FRAME_BORDER_BOTTOMRIGHT,		
			FRAME_BORDER_TOP,				
			FRAME_BORDER_LEFT,			
			FRAME_BORDER_BOTTOM,			
			FRAME_BORDER_RIGHT,
			FRAME_BORDER_NONE,			
		};

		/**
		* @brief location of border
		*/
		void	SetBorderDynamic(uint32 eBorder, bool bDynamic);

		bool	IsBorderDynamic(uint32 eBorder) const;

		/**
		* @brief load widget config from property
		*/
		virtual CGUIProperty*	GenerateProperty(const CGUIString& rName, const CGUIString& rType );

		/** 
		* @brief process property
		*/
		virtual void			ProcessProperty( const CGUIProperty* pProperty);

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtFrame( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		///initialize frame
		void				InitFrame();

		/// render
		virtual void		RenderSelf(IGUIInterfaceRender* pRender);

		///update rect if it is dirty
		virtual void		UpdateDirtyRect();	

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:
		virtual uint32		OnMouseMove( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDbClick( CGUIEventMouse* pEvent );

		virtual uint32		OnDragBegin( CGUIEventDrag* pEvent );
		virtual uint32		OnDragProcess( CGUIEventDrag* pEvent );
		virtual uint32		OnDragEnd( CGUIEventDrag* pEvent );

	protected:

		/**
		* @brief location of title bar
		*/
		enum ETitlebarLocation
		{
			TITLEBAR_NONE,
			TITLEBAR_TOP,
			TITLEBAR_BOTTOM,
			TITLEBAR_LEFT,
			TITLEBAR_RIGHT,
		};


		/**
		* @brief get the title bar's rect
		*/
		const CGUIRect&	GetTitlebarRect();

		/**
		* @brief get the border's rect
		*/
		const CGUIRect&	GetBorderRect(int32 eBorder);

		/**
		* @brief get the decorate of title bar
		* @param nIdx 0 or 1
		*/
		const CGUIRect&	GetDecorateRect(int32 nIdx);

		/**
		* @brief get the border type under the point.
		* @param rPos global position
		*/
		EBorderLocation		GetBorderType( const CGUIVector2& rPos);

		/**
		* @brief show or hide the client area
		*/
		void		ShowClient( bool bShow );

		//!< move top border to resize frame
		void		MoveTopBorder(real deltaY);

		//!< move top border to resize frame
		void		MoveBottomBorder(real deltaY);

		//!< move top border to resize frame
		void		MoveLeftBorder(real deltaX);

		//!< move top border to resize frame
		void		MoveRightBorder(real deltaX);

		//!< move top left border to resize frame
		void		MoveTopLeftBorder(real deltaX, real deltaY);

		//!< move bottom left border to resize frame
		void		MoveBottomLeftBorder(real deltaX, real deltaY);

		//!< move top right border to resize frame
		void		MoveTopRightBorder(real deltaX,real deltaY);

		//!< move bottom right border to resize frame
		void		MoveBottomRightBorder(real deltaX,real deltaY);

		//!< get border width
		real		GetBorderWidth(EBorderLocation eBorder)
		{
			return	m_aBorderInfo[eBorder].m_aSize.m_fWidth*GetDerivedScale().m_fWidth;
		}

		//!< get border height
		real		GetBorderHeight(EBorderLocation eBorder)
		{
			return	m_aBorderInfo[eBorder].m_aSize.m_fHeight*GetDerivedScale().m_fHeight;
		}

		//!< get border size
		CGUISize	GetBorderSize(EBorderLocation eBorder)
		{
			return	m_aBorderInfo[eBorder].m_aSize*GetDerivedScale();
		}

	private:

		enum EBorderType
		{
			eBorder_Static,	///< this border couldn't be drag to resize frame
			eBorder_Dynamic,///< this border could be drag to resize frame
			eBorder_None	///< havn't border
		};
		struct
		{
			CGUISize			m_aSize;
			EBorderType			m_aType;
			CGUIImage*			m_pImageInfo;
			CGUIRect			m_aRenderRect;
		} m_aBorderInfo[FRAME_BORDER_NONE];

		CGUIImage*			m_pImageTitleBar;			///image of title bar
		CGUIImage*			m_pImageDecorate[2];		///image of decorate

		ETitlebarLocation	m_eTitlebarType;			///< type of title bar
		CGUISize			m_aSizeTitleBar;			///< the size of title bar
		CGUISize			m_aSizeDecorate[2];			///< get the decorate of title bar

		CGUIRect			m_aTitleBarRect;			///<rendable rect of title bar
		CGUIRect			m_aDecorateRect[2];			///<rendable rect of decorate bar

		bool				m_bShowClient;				///< whether this frame show client or not
		bool				m_bHasDecorate;				///< whether title bar has decorate

		//for drag
		EBorderLocation		m_eDragSizing;				///< whether this frame is being sized, FRAME_BORDER_NONE for no sizing now
		CGUIVector2			m_aStartMousePoint;			///< the start mouse point when begin to drag size
		CGUIVector2			m_aStartPos;				///< the start point of frame
		CGUISize			m_aStartSize;				///< the start size of frame

	protected:
		static CGUIString ms_strType;

	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtFrame);

}//namespace guiex

#endif //__GUI_WGTFRAME_20060720_H__

