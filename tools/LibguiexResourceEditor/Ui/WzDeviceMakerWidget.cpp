#include "WzAfxEditor.h"
#include <QPainter>
#include <QString>
#include <QResizeEvent>
#include "UI\WzDeviceMakerWidget.h"
#include "UI\WzSpriteWidget.h"


#define M_Pointf2Point( p )	QPoint( p.x(), p.y() )
#define M_Sizef2Size( s )	QSize( s.width(), s.height() )


namespace RE
{


// -----------------------------------------------------
// General
// -----------------------------------------------------
WzDeviceMakerWidget::WzDeviceMakerWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_image( NULL )
, m_currentSprite( NULL )
{
	m_image = new QImage( tr( ":/image/device/device_iphone.png" ) );
	if( !m_image->isNull() )
	{
		m_image->createMaskFromColor( qRgb( 255, 255, 255 ), Qt::MaskOutColor );
	}

	m_deviceWidget = new WzSpriteWidget( this );
	m_deviceWidget->EnableDragMove( true );
	m_deviceWidget->SetImage( m_image );
	m_deviceWidget->resize( m_image->width(), m_image->height() );	
	m_deviceWidget->setMouseTracking( true );
	m_deviceWidget->SetDebugInfo( tr( "DeviceWidget" ) );
	m_deviceWidget->EnableDebugInfo( false );
	m_deviceWidget->show();

	m_rectWidget = new WzSpriteWidget( m_deviceWidget );
	m_rectWidget->EnableDragMove( true );
	m_rectWidget->EnableDragResize( true );
	m_rectWidget->resize( 150, 150 );
	m_rectWidget->setMouseTracking( true );
	m_rectWidget->EnableDebugInfo( false );
	m_rectWidget->SetDebugInfo( tr( "RectWidget" ) );
	m_rectWidget->show();

	EnableDebugInfo( true );
	setMouseTracking( true );
}


void WzDeviceMakerWidget::Tick( float _delta )
{
	QPointF localPos;
	
	//m_debugInfo = QString( "P(%1,%2) Z(%3)" )
	//	.arg( m_deviceInfo.m_pos.x() )
	//	.arg( m_deviceInfo.m_pos.y() )
	//	.arg( m_zoomingInfo.m_scalar );
}


// -----------------------------------------------------
// Override QWidget
// -----------------------------------------------------
void WzDeviceMakerWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );
	QRect destRect;
	destRect.setLeft( m_deviceInfo.m_offset.x() + m_deviceInfo.m_pos.x() );
	destRect.setTop( m_deviceInfo.m_offset.y() + m_deviceInfo.m_pos.y() );
	destRect.setRight( destRect.left() + m_deviceInfo.m_visualSize.width() );
	destRect.setBottom( destRect.top() + m_deviceInfo.m_visualSize.height() );
	painter.drawImage( destRect, *m_image );

	destRect.setLeft( m_rectInfo.m_offset.x() + m_rectInfo.m_pos.x() );
	destRect.setTop( m_rectInfo.m_offset.y() + m_rectInfo.m_pos.y() );
	destRect.setRight( destRect.left() + m_rectInfo.m_size.width() );
	destRect.setBottom( destRect.top() + m_rectInfo.m_size.height() );
	painter.fillRect( destRect, QColor( 0, 255, 0, 100 ) );
}


void WzDeviceMakerWidget::resizeEvent( QResizeEvent* _event )
{
	if( !m_deviceInfo.m_isInitialized )
	{
		m_deviceInfo.m_size = m_image->size();
		m_deviceInfo.m_pos.setX( ( _event->size().width() - m_deviceInfo.m_size.width() ) / 2 );
		m_deviceInfo.m_pos.setY( ( _event->size().height() - m_deviceInfo.m_size.height() ) / 2 );
		m_deviceInfo.m_isInitialized = true;

		CalcVisualStats();
	}

	if( _event->size() != m_size )
	{
		m_size = _event->size();

		// TODO...
	}

	update();
}


void WzDeviceMakerWidget::mousePressEvent( QMouseEvent* _event )
{
	bool isDirty = false;
	QPoint pos = _event->pos();

	QWidget* child = childAt( pos );

	if( Qt::MidButton == _event->button() )
	{
		m_dragDeviceInfo.m_isDragging = true;
		m_dragDeviceInfo.m_cursorPosBackup = _event->pos();
		m_dragDeviceInfo.m_targetPosBackup = M_Pointf2Point( m_deviceInfo.m_pos );
	}
	else if( Qt::LeftButton == _event->button() )
	{		
		WzSpriteInfo* sprite = HitSelect( pos.x(), pos.y() );

		if( sprite == &m_rectInfo )
		{
			m_dragRectInfo.m_isDragging = true;
			m_dragRectInfo.m_cursorPosBackup = pos;
			m_dragRectInfo.m_targetPosBackup = M_Pointf2Point( m_rectInfo.m_pos );
		}
		else
		{
			m_rectInfo.m_pos = pos;
			m_rectInfo.m_offset = _event->pos() - m_deviceInfo.m_pos;
			m_rectInfo.m_size = QSize( 0, 0 );

			isDirty = true;
		}
	}

	if( isDirty )
		update();
}


void WzDeviceMakerWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		m_dragDeviceInfo.m_isDragging = false;
	}

	if( Qt::LeftButton == _event->button() )
	{
		m_dragRectInfo.m_isDragging = false;
	}
}


void WzDeviceMakerWidget::mouseMoveEvent( QMouseEvent* _event )
{
	bool isDirty = false;
	m_cursorPos = _event->pos();

	QWidget* child = childAt( m_cursorPos );

	if( m_dragDeviceInfo.m_isDragging )
	{
		// Dragging the device ( the whole panel ).
		QPoint delta = m_cursorPos - m_dragDeviceInfo.m_cursorPosBackup;
		m_deviceInfo.m_pos = m_dragDeviceInfo.m_targetPosBackup + delta;

		isDirty = true;
	}
	else if( m_dragRectInfo.m_isDragging )
	{
		// Dragging the rect widget.
		QPoint delta = m_cursorPos - m_dragRectInfo.m_cursorPosBackup;
		m_rectInfo.m_pos = m_dragRectInfo.m_targetPosBackup + delta;

		isDirty = true;
	}
	else if( Qt::LeftButton & _event->buttons() )
	{
		// Resizing the rect widget.
		QPoint size = m_cursorPos - M_Pointf2Point( m_rectInfo.m_pos );
		m_rectInfo.m_size.setWidth( size.x() );
		m_rectInfo.m_size.setHeight( size.y() );

		isDirty = true;
	}

	if( isDirty )
		update();
}


void WzDeviceMakerWidget::wheelEvent( QWheelEvent* _event )
{
	int degrees = _event->delta() / 8;
	int stepCount = degrees / 15;

	if( 0 != ( Qt::ControlModifier & _event->modifiers() ) )
		m_zoomingInfo.m_scalar += m_zoomingInfo.m_smallStep * stepCount;
	else
		m_zoomingInfo.m_scalar += m_zoomingInfo.m_bigStep * stepCount;

	if( m_zoomingInfo.m_scalar < 0.2f )
		m_zoomingInfo.m_scalar = 0.2f;
	else if( m_zoomingInfo.m_scalar > 5.0f )
		m_zoomingInfo.m_scalar = 5.0f;

	CalcVisualStats();
	update();
}


// --------------------------------------------------------------------------------------------
// Utilities
// --------------------------------------------------------------------------------------------
WzDeviceMakerWidget::WzSpriteInfo* WzDeviceMakerWidget::HitSelect( int _x, int _y ) const
{
	return NULL;
}


void WzDeviceMakerWidget::CalcVisualStats()
{
	m_deviceInfo.m_visualSize = m_deviceInfo.m_size * m_zoomingInfo.m_scalar;
}


}	// namespace RE
