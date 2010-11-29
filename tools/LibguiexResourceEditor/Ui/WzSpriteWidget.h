#ifndef _RE_EDITOR_SPRITEWIDGET_H_
#define _RE_EDITOR_SPRITEWIDGET_H_

#include "UI\WzBaseWidget.h"

namespace RE
{


class WzSpriteWidget : public WzBaseWidget
{
	// -----------------------------------------------------
	// General
	// -----------------------------------------------------
	Q_OBJECT

	typedef WzBaseWidget	TSuper;

public:
	WzSpriteWidget( QWidget* _parent = NULL );

	void				SetImage( QImage* _image )			{ m_image = _image; }
	QImage*				GetImage() const					{ return m_image; }
	void				EnableDragMove( bool _b )			{ m_isDragMoveEnabled = _b; }
	void				EnableDragResize( bool _b )			{ m_isDragResizeEnabled = _b; }
	bool				IsDragMoveEnabled() const			{ return m_isDragMoveEnabled; }
	bool				IsDragResizeEnabled() const			{ return m_isDragResizeEnabled; }

	// -----------------------------------------------------
	// Resizing
	// -----------------------------------------------------
public:
	void				SetMinSize( const QSize& _size )	{ m_minSize = _size; }
	const QSize&		GetMinSize() const					{ return m_minSize; }
	void				SetResizePadding( int _padding )	{ m_resizePadding = _padding; }
	int					GetResizePadding() const			{ return m_resizePadding; }

	// -----------------------------------------------------
	// Override
	// -----------------------------------------------------
public:
	void				paintEvent( QPaintEvent* _event );
	void				mouseMoveEvent( QMouseEvent* _event );
	void				mousePressEvent( QMouseEvent* _event );
	void				mouseReleaseEvent( QMouseEvent* _event );
	void				moveEvent( QMoveEvent* _event );

	// -----------------------------------------------------
	// Internal Classes
	// -----------------------------------------------------
	// This drag info helps to ease the work of porting the dragging and resizing feature
	// to a none-Qt framework ( MFC for example ).
	class WzDragInfo
	{
	public:
		enum eDragType	{ EDrag_None, EDrag_Move, EDrag_Resize };

		enum eResize
		{
			EResize_None,
			EResize_Left,
			EResize_Top,
			EResize_Right,
			EResize_Bottom,
			EResize_LeftTop,
			EResize_RightTop,
			EResize_RightBottom,
			EResize_LeftBottom
		};

	public:
		WzDragInfo(): m_cursorPosBackup( 0, 0 ), m_widgetPosBackup( 0, 0 ), m_dragType( EDrag_None ), m_resizeType( EResize_None ) {}

		// Common.
		void			SetCursorPosBackup( const QPoint& _pos )	{ m_cursorPosBackup = _pos; }
		void			SetWidgetPosBackup( const QPoint& _pos )	{ m_widgetPosBackup = _pos; }
		const QPoint&	GetCursorPosBackup() const					{ return m_cursorPosBackup; }
		const QPoint&	GetWidgetPosBackup() const					{ return m_widgetPosBackup; }

		// Move.
		void			StartMove()						{ m_dragType = EDrag_Move; }
		void			StopMove()						{ Stop(); }				
		void			Stop()							{ m_dragType = EDrag_None; }
		bool			IsMoving()						{ return EDrag_Move == m_dragType; }

		// Resize.
		void			StartResize( eResize _type )	{ if( EResize_None != _type ) { m_resizeType = _type; m_dragType = EDrag_Resize; } }
		eResize			GetResizeType() const			{ return m_resizeType; }
		void			StopResize()					{ Stop(); }
		bool			IsResizing()					{ return EDrag_Resize == m_dragType; }
		bool			IsResizeL() const				{ return EResize_Left == m_resizeType; }
		bool			IsResizeT() const				{ return EResize_Top == m_resizeType; }
		bool			IsResizeR() const				{ return EResize_Right == m_resizeType; }
		bool			IsResizeB() const				{ return EResize_Bottom == m_resizeType; }
		bool			IsResizeLT() const				{ return EResize_LeftTop == m_resizeType; }
		bool			IsResizeRT() const				{ return EResize_RightTop == m_resizeType; }
		bool			IsResizeRB() const				{ return EResize_RightBottom == m_resizeType; }
		bool			IsResizeLB() const				{ return EResize_LeftBottom == m_resizeType; }

	protected:
		QPoint			m_cursorPosBackup;
		QPoint			m_widgetPosBackup;
		eDragType		m_dragType;
		eResize			m_resizeType;
	};

	// -----------------------------------------------------
	// Utilities
	// -----------------------------------------------------
protected:
	WzDragInfo::eResize	CalcResizeType( const QPoint& _localPos ) const;

	// -----------------------------------------------------
	// Variable
	// -----------------------------------------------------
protected:
	QImage*				m_image;

	// Compound
	WzSpriteWidget*		m_logicParent;
	QPoint				m_localPos;

	// Resizing
	int					m_resizePadding;
	WzDragInfo			m_dragInfo;
	QSize				m_minSize;
	bool				m_isDragMoveEnabled;
	bool				m_isDragResizeEnabled;
};


}		// namespace RE
#endif	// _RE_EDITOR_SPRITEWIDGET_H_
