// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101115.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_H_
#define _RE_CLIP_H_


#include <QLabel>
#include <QRect>
#include <list>
#include "Ui\ReBaseWidget.h"


namespace RE
{


class ReClipNode;


// -----------------------------------------------------------------------------
// ReClipWidget
// -----------------------------------------------------------------------------
class ReClipWidget : public ReBaseWidget< QLabel >
{
	Q_OBJECT
	typedef ReBaseWidget< QLabel >	TSuper;
	typedef ReDragInfo::eDragType	TDragType;
	typedef ReDragInfo::eResize		TResizeType;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReClipWidget( QWidget* _parent = NULL );
	~ReClipWidget();

	ReClipNode*			GetModelData() const				{ return m_modelData; }
	void				SetModelData( ReClipNode* _data )	{ m_modelData = _data; }

	int					GetZoomFactor() const				{ return m_zoomFactor; }
	void				SetZoomFactor( int _z )				{ m_zoomFactor = _z; }

	void				ShowOutline( bool _show )			{ m_isShowOutline = _show; }

	// -------------------------------------------------------------------------
	// Override.
	// -------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );
	virtual void		moveEvent( QMoveEvent* _event );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:	

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReClipNode*			m_modelData;

	int					m_zoomFactor;	
	bool				m_isShowOutline;

public:
	static QSize		ms_minSize;
};


}
#endif	// _RE_CLIP_H_
