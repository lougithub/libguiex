// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101218.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_GRAPHICS_VIEW_H_
#define _RE_ANIM_GRAPHICS_VIEW_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QGraphicsView>
#include "Core\ReDragInfo.h"
#include "Core\ReAnimDef.h"
#include "Ui\ReAnimTools.h"


class QLabel;


namespace RE
{


class ReAnimGraphicsView : public QGraphicsView
{
	Q_OBJECT
	typedef QGraphicsView	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimGraphicsView( QGraphicsScene* _scene, QWidget* _parent = NULL );
	~ReAnimGraphicsView();

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		wheelEvent( QWheelEvent* _event );
	virtual void		keyPressEvent( QKeyEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );	

	// -------------------------------------------------------------------------
	// Override QGraphicsView.
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	// [SceneOrigin*Changed]: Return the negative value of the scene origin on viewport.
	void				SceneOriginXChanged( int _x );
	void				SceneOriginYChanged( int _y );
	void				SceneItemMoved( const QPoint& _itemPosInView );
	void				EditModeChanged();

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	// [RequestChangeSceneOrigin*]: The parameter is the negative value of the desired scene origin on viewport.
	void				RequestChangeSceneOriginX( int _x );
	void				RequestChangeSceneOriginY( int _y );

	void				OnSceneSelectionChanged();

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	void				ChangeEditMode( eEditMode _mode );

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
protected:
	ReAnimTool*			m_currentTool;
	ReAnimTool*			m_tools[ EEditMode_Count ];

	QLabel*				m_modeLabel;		// Not working? Come back later...
	eEditMode			m_editMode;

	ReDragInfoF			m_dragInfo;
	QPoint				m_sceneOriginInView;
	bool				m_isInitialized;
};


}
#endif	// _RE_ANIM_GRAPHICS_VIEW_H_
