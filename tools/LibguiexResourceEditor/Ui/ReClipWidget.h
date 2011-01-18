// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101115.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_H_
#define _RE_CLIP_H_


#include <QWidget>
#include <QRect>
#include <list>
#include "Ui\ReBaseWidget.h"


namespace RE
{


class ReClipNode;


// -----------------------------------------------------------------------------
// ReClipWidget
// -----------------------------------------------------------------------------
class ReClipWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReClipWidget( QWidget* _parent = NULL );
	~ReClipWidget();

	void				InitFromModelData( ReClipNode* _modelData );
	ReClipNode*			GetModelData() const				{ return m_modelData; }
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

	bool				m_isShowOutline;
};


}
#endif	// _RE_CLIP_H_
