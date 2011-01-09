// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101127.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_ENTITY_WIDGET_H_
#define _RE_ANIM_ENTITY_WIDGET_H_
// -----------------------------------------------------------------------------
// A ReAnimEntityWidget consists of a toggle button, a label and multiple suite 
// widget.
// Each suite widget consists of a few suite buttons and a track widget.
// Each suite button serves a specific purpose, like enable or disable the current
// track, or delete the current track.
// The track widget is where frames are edited.
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include "Ui\ReBaseWidget.h"
#include <QLabel>
#include <QMatrix>


class QPushButton;
class QLineEdit;


namespace RE
{


class ReAnimEntityWidget;
class ReAnimTrackWidget;
class ReAnimFrameWidget;
class ReRulerWidget;
class ReAnimEntity;


class ReAnimEntityWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	enum eSuiteButton
	{
		ESuiteButton_Enable,
		ESuiteButton_Count
	};

	class ReSuite
	{
	public:
		QPushButton*		m_buttons[ ESuiteButton_Count ];
		ReAnimTrackWidget*	m_track;
		eTrackType			m_type;

		ReSuite(): m_track( NULL )
		{			
			for( int i = 0; i < ESuiteButton_Count; ++i )
				m_buttons[ i ] = NULL;
		}
	};

	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	ReAnimEntityWidget( ReAnimEntity* _model, ReRulerWidget* _ruler, QWidget* _parent = NULL );

	// Model.
	ReAnimEntity*		GetModelData() const { return m_modelData; }
	void				SetModelData( ReAnimEntity* _entity ) { m_modelData = _entity; }

	// UI.
	int					GetTotalHeight() const;
	int					GetTrackCount() const;
	void				Highlight( bool _isHighlight );
	bool				IsHighlighted() const			{ return m_isHighlighted; }

	// Transform.
	bool				GetTranslationAt( int _cursor, QPointF& _result, bool _allowExterpolate );
	bool				GetRotationAt( int _cursor, qreal& _result, bool _allowExterpolate );
	bool				GetScaleAt( int _cursor, QPointF& _result, bool _allowExterpolate );
	bool				GetAlphaAt( int _cursor, qreal& _result, bool _allowExterpolate );
	bool				GetTransformAt( int _cursor, QTransform& _result, bool _allowExterpolate );

	// ----------------------------------------------------------------------------
	// Override QWidget.
	// ----------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );

	// -------------------------------------------------------------------------
	// Overrides ReBaseWidget.
	// -------------------------------------------------------------------------
public:
	virtual QMenu*		GetEditMenu() const	{ return m_editMenu; }

	// ----------------------------------------------------------------------------
	// Signals.
	// ----------------------------------------------------------------------------
signals:
	void				TotalHeightChanged();
	void				DataChangedAt( int _cursor );

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	void				OnContextMenu( const QPoint& _point );
	void				OnToggled( bool _isChecked );
	void				OnNewTranslationTrack();
	void				OnNewRotationTrack();
	void				OnNewScaleTrack();
	void				OnNewAlphaTrack();
	void				OnPositionChanged( const QPointF& _pos );
	void				OnRotationChanged( qreal _delta );
	void				OnScaleChanged( const QPointF& _scale );
	void				OnAlphaChanged( qreal _delta );

	// ----------------------------------------------------------------------------
	// Utilities.
	// ----------------------------------------------------------------------------
protected:
	void				InitMenu();
	void				CreateTrack( eTrackType _type );
	void				UpdateLayout();

	// ----------------------------------------------------------------------------
	// Variables.
	// ----------------------------------------------------------------------------
protected:
	ReAnimEntity*		m_modelData;

	ReRulerWidget*		m_ruler;
	QPushButton*		m_toggleButton;
	//QLabel*				m_label;
	QLineEdit*			m_nameEdit;
	ReSuite				m_suiteArray[ ETrackType_Count ];

	QMenu*				m_editMenu;
	bool				m_isHighlighted;
};


}
#endif	// _RE_ANIM_ENTITY_WIDGET_H_
