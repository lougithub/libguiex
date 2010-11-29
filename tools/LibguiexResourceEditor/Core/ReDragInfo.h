// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101111.
// -----------------------------------------------------------------------------
#ifndef _RE_DRAG_INFO_H_
#define _RE_DRAG_INFO_H_


#include <QPoint>
#include <QSize>


namespace RE
{


class ReDragInfo
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
	ReDragInfo(): m_cursorPosBackup( 0, 0 ), m_itemPosBackup( 0, 0 ), m_itemSizeBackup( 0, 0 ),
		m_dragType( EDrag_None ), m_resizeType( EResize_None ) {}

	// Common.
	void			SetCursorPosBackup( const QPoint& _pos )	{ m_cursorPosBackup = _pos; }
	void			SetItemPosBackup( const QPoint& _pos )		{ m_itemPosBackup = _pos; }
	void			SetItemSizeBackup( const QSize& _size )		{ m_itemSizeBackup = _size; }	
	const QPoint&	GetCursorPosBackup() const					{ return m_cursorPosBackup; }
	const QPoint&	GetItemPosBackup() const					{ return m_itemPosBackup; }
	const QSize&	GetItemSizeBackup() const					{ return m_itemSizeBackup; }

	// Move.
	void			StartMove()						{ m_dragType = EDrag_Move; }
	void			StopMove()						{ Stop(); }				
	void			Stop()							{ m_dragType = EDrag_None; }
	bool			IsMoving() const				{ return EDrag_Move == m_dragType; }

	// Resize.
	void			StartResize( eResize _type )	{ if( EResize_None != _type ) { m_resizeType = _type; m_dragType = EDrag_Resize; } }
	eResize			GetResizeType() const			{ return m_resizeType; }
	void			StopResize()					{ Stop(); }
	bool			IsResizing() const				{ return EDrag_Resize == m_dragType; }
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
	QPoint			m_itemPosBackup;
	QSize			m_itemSizeBackup;
	eDragType		m_dragType;
	eResize			m_resizeType;
};

}
#endif	// _RE_DRAG_INFO_H_
