#ifndef _RE_EDITOR_DEVICEMAKERWIDGET_H_
#define _RE_EDITOR_DEVICEMAKERWIDGET_H_

#include "UI\WzBaseWidget.h"


namespace RE
{


class WzSpriteWidget;


class WzDeviceMakerWidget : public WzBaseWidget
{
	// --------------------------------------------------------------------------------------------
	// General
	// --------------------------------------------------------------------------------------------
	Q_OBJECT

	typedef WzBaseWidget	TSuper;

public:
	WzDeviceMakerWidget( QWidget* _parent = NULL );

	void				Tick( float _delta );

	// --------------------------------------------------------------------------------------------
	// Override 
	// --------------------------------------------------------------------------------------------
protected:
	void				paintEvent( QPaintEvent* _event );
	void				resizeEvent( QResizeEvent* _event );
	void				mousePressEvent( QMouseEvent* _event );
	void				mouseReleaseEvent( QMouseEvent* _event );
	void				mouseMoveEvent( QMouseEvent* _event );
	void				wheelEvent( QWheelEvent* _event );

	// --------------------------------------------------------------------------------------------
	// Internal Classes
	// --------------------------------------------------------------------------------------------
protected:
	class WzDragInfo
	{
	public:		
		QPoint			m_cursorPosBackup;
		QPoint			m_targetPosBackup;
		bool			m_isDragging;

		WzDragInfo(): m_cursorPosBackup( 0, 0 ), m_targetPosBackup( 0, 0 ), m_isDragging( false ) {}
	};

	class WzSpriteInfo
	{
	public:
		WzSpriteInfo*	m_parent;
		QPointF			m_pos;		// the center of the sprite, not the left top corner
		QSizeF			m_size;
		QPointF			m_offset;	// sprite from device, device from none
		QSizeF			m_visualSize;
		bool			m_isInitialized;

		WzSpriteInfo( WzSpriteInfo* _parent = NULL ): m_parent( _parent ), 
			m_pos( 0.0f, 0.0f ), m_size( 0.0f, 0.0f ), m_offset( 0.0f, 0.0f ), 
			m_visualSize( 0.0f, 0.0f ), m_isInitialized( false ) 
		{}

		bool			IsInitialize() const{ return m_isInitialized; }
		void			SetInitialized()	{ m_isInitialized = true; }

		QPointF			GetWorldPos() const	{ return m_pos + ( NULL != m_parent ? m_parent->GetWorldPos() : QPointF( 0.0f, 0.0f ) ); }
		const QPointF&	GetLocalPos() const	{ return m_pos; }
		const QSizeF&	GetSize() const		{ return m_size; }

		QPointF			WorldToLocal( const QPointF& _pos ) const	{ return _pos - GetWorldPos(); }
	};

	class WzZoomingInfo
	{
	public:
		qreal			m_scalar;
		qreal			m_smallStep;
		qreal			m_bigStep;
		QPointF			m_cursorPos;

		WzZoomingInfo(): m_scalar( 1.0f ), m_smallStep( 0.01f ), m_bigStep( 0.1f ), m_cursorPos( 0.0f, 0.0f ) {}
	};

	// --------------------------------------------------------------------------------------------
	// Utilities
	// --------------------------------------------------------------------------------------------
protected:
	WzSpriteInfo*		HitSelect( int _x, int _y ) const;
	void				CalcVisualStats();

	// --------------------------------------------------------------------------------------------
	// Variable
	// --------------------------------------------------------------------------------------------
protected:
	QImage*				m_image;
	QSize				m_size;

	WzZoomingInfo		m_zoomingInfo;
	WzDragInfo			m_dragDeviceInfo;
	WzDragInfo			m_dragRectInfo;
	WzSpriteInfo		m_deviceInfo;
	WzSpriteInfo		m_rectInfo;
	WzSpriteInfo*		m_currentSprite;

	WzSpriteWidget*		m_deviceWidget;
	WzSpriteWidget*		m_rectWidget;

	// Debug
	QPoint				m_cursorPos;
};


}		// namespace RE
#endif	// _RE_EDITOR_DEVICEMAKERWIDGET_H_
