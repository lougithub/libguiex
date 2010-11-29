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


class ReClipData;


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

	ReClipData*			GetModelData() const				{ return m_modelData; }
	void				SetModelData( ReClipData* _data )	{ m_modelData = _data; }
	void				PullData();
	void				PushData();

	int					GetZoomFactor() const				{ return m_zoomFactor; }
	void				SetZoomFactor( int _z )				{ m_zoomFactor = _z; }

	TResizeType			CalcResizeType( const QPoint& _point ) const;
	void				ShowOutline( bool _show )			{ m_isShowOutline = _show; }

	// -------------------------------------------------------------------------
	// Override.
	// -------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:	

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReClipData*			m_modelData;

	int					m_zoomFactor;	
	bool				m_isShowOutline;

public:
	static QSize		ms_minSize;
};


}
#endif	// _RE_CLIP_H_
