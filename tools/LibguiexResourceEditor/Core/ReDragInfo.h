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


template< class P, class S >
class ReDragInfo_
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
	ReDragInfo_(): m_cursorPosBackup( 0, 0 ), m_itemPosBackup( 0, 0 ), m_itemSizeBackup( 0, 0 ),
		m_dragType( EDrag_None ), m_resizeType( EResize_None ) {}

	// Common.
	void			SetCursorPosBackup( const P& _pos )	{ m_cursorPosBackup = _pos; }
	void			SetItemPosBackup( const P& _pos )	{ m_itemPosBackup = _pos; }
	void			SetItemSizeBackup( const S& _size )	{ m_itemSizeBackup = _size; }	
	const P&		GetCursorPosBackup() const			{ return m_cursorPosBackup; }
	const P&		GetItemPosBackup() const			{ return m_itemPosBackup; }
	const S&		GetItemSizeBackup() const			{ return m_itemSizeBackup; }
	virtual void	Stop()								{ m_dragType = EDrag_None; }

	// Move.
	void			StartMove()							{ m_dragType = EDrag_Move; }	
	bool			IsMoving() const					{ return EDrag_Move == m_dragType; }

	// Resize.
	void			StartResize( eResize _type )		{ if( EResize_None != _type ) { m_resizeType = _type; m_dragType = EDrag_Resize; } }
	eResize			GetResizeType() const				{ return m_resizeType; }
	bool			IsResizing() const					{ return EDrag_Resize == m_dragType; }
	bool			IsResizeL() const					{ return EResize_Left == m_resizeType; }
	bool			IsResizeT() const					{ return EResize_Top == m_resizeType; }
	bool			IsResizeR() const					{ return EResize_Right == m_resizeType; }
	bool			IsResizeB() const					{ return EResize_Bottom == m_resizeType; }
	bool			IsResizeLT() const					{ return EResize_LeftTop == m_resizeType; }
	bool			IsResizeRT() const					{ return EResize_RightTop == m_resizeType; }
	bool			IsResizeRB() const					{ return EResize_RightBottom == m_resizeType; }
	bool			IsResizeLB() const					{ return EResize_LeftBottom == m_resizeType; }

protected:
	P				m_cursorPosBackup;
	P				m_itemPosBackup;
	S				m_itemSizeBackup;
	eDragType		m_dragType;
	eResize			m_resizeType;
};


typedef ReDragInfo_< QPoint, QSize >	ReDragInfo;
typedef ReDragInfo_< QPointF, QSizeF >	ReDragInfoF;


}
#endif	// _RE_DRAG_INFO_H_
