// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101115.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_CLIPIMAGE_H_
#define _RE_EDITOR_CLIPIMAGE_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Ui\ReBaseWidget.h"
#include "Ui\ReClipWidget.h"
#include "Core\ReItemGroup.h"
#include "Core\ReWidgetSelection.h"
#include <QLabel>


namespace RE
{


class ReClipNodeGroup;
class ReClipModel;
class ReZoomInfo;


class ReClipImage : public ReBaseWidget< QLabel >
{
	Q_OBJECT
	typedef ReBaseWidget< QLabel >	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReClipImage( ReZoomInfo* _zoomInfo, QWidget* _parent = NULL );
	~ReClipImage();

	void				InitFromModelData( ReClipNodeGroup* _modelData );
	ReClipNodeGroup*	GetModelData() const { return m_modelData; }

	//const QString&		GetImagePath() const { return m_imagePath; }
	//void				SetImagePath( const QString& _path );

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		keyPressEvent( QKeyEvent* _event );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void				OnZoom( int _scalar );
	void				OnDelete();

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	ReClipWidget*		CreateClip();
	bool				IsClipValid( const ReClipWidget* _clip ) const;
	void				HandleResize( const QPoint _pos );
	void				HandleClipping();
	void				Reset();

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	class ReDragInfoEx : public ReDragInfo
	{
		typedef ReDragInfo TSuper;

	public:
		ReDragInfoEx(): m_clipWidget( NULL ), m_isCreating( false ) {}

		virtual void	Stop()			{ m_isCreating = false; m_clipWidget = NULL; TSuper::Stop(); }
		bool			IsCreating()	{ return m_isCreating; }
		void			StartCreate()	{ m_isCreating = true; }

		ReClipWidget*	GetClip() const					{ return m_clipWidget; }
		void			SetClip( ReClipWidget* _clip )	{ m_clipWidget = _clip; }

	protected:
		ReClipWidget*	m_clipWidget;
		bool			m_isCreating;
	};


	class ReClipSelection : public ReWidgetSelection
	{
		typedef ReWidgetSelection TSuper;

	public:
		virtual void	Add( QWidget* _widget );
		virtual void	Remove( QWidget* _widget );
		virtual void	Clear();
	};

	// Data.
	ReClipNodeGroup*	m_modelData;
	QString				m_imagePath;

	// UI.
	typedef ReItemGroup< ReClipWidget* >	TClipList;
	typedef TClipList::TItemListItor		TClipListItor;
	typedef TClipList::TItemListCItor		TClipListCItor;

	TClipList			m_clipList;			// Keep a list of all children.
	ReClipSelection		m_selection;
	ReDragInfoEx		m_dragInfo;
	int					m_zoomScalar;
};


}
#endif	// _RE_EDITOR_CLIPIMAGE_H_
