#include "StdAfxEditor.h"
#include "Ui\ReAnimGraphicsItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QEvent>


namespace
{
	inline qreal AlphaClamp( qreal _alpha )
	{
		if( _alpha < 0.0f )
			return 0.0f;
		else if( _alpha > 1.0f )
			return 1.0f;
		else
			return _alpha;
	}
}


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimGraphicsItem::ReAnimGraphicsItem( QGraphicsItem* _parent /* = NULL */ )
: TSuper( _parent )
, m_editMode( EEditMode_Select )
, m_rotationAccumulator( 0 )
, m_scaleAccumulator( 1.0f, 1.0f )
, m_scaleMultiplier( 0.01f )
, m_alpha( 1.0f )
, m_alphaMultiplier( 0.01f )
{
	setFlag( QGraphicsItem::ItemIsSelectable );
	setFlag( QGraphicsItem::ItemIsMovable, false );
	setFlag( QGraphicsItem::ItemIsFocusable );
}


void ReAnimGraphicsItem::SetEditMode( eEditMode _mode )
{
	if( m_editMode != _mode )
	{
		m_editMode = _mode;
		setFlag( ItemIsSelectable, true );
		update();
	}
}


void ReAnimGraphicsItem::SetAlpha( qreal _alpha )
{
	m_alpha = AlphaClamp( _alpha );
	update();
}


// -----------------------------------------------------------------------------
// Override QGraphicsItem.
// -----------------------------------------------------------------------------
bool ReAnimGraphicsItem::sceneEvent( QEvent* _event )
{
	if( QEvent::ToolTip == _event->type() )
	{
		QMatrix mx = matrix();
		setToolTip( QString( tr( "m11: %1\tm12: %2\r\nm21: %3\tm22: %4\r\ndx: %5\tdy: %6\r\n" ) )
			.arg( mx.m11() ).arg( mx.m12() )
			.arg( mx.m21() ).arg( mx.m22() )
			.arg( mx.dx() ).arg( mx.dy() ) );
	}

	return TSuper::sceneEvent( _event );
}


QRectF ReAnimGraphicsItem::boundingRect() const
{
	const int penWidth = 2;
	return QRectF( -10 - penWidth / 2, - 10 - penWidth / 2,
		20 + penWidth, 20 + penWidth );
}


void ReAnimGraphicsItem::paint( QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget /* = NULL */ )
{
	_painter->setPen( QColor( 255, 255, 255 ) );
	_painter->fillRect( boundingRect(), QColor( 255, 255, 0, ( int )( 255.0f * m_alpha ) ) );
	_painter->setPen( QColor( 0, 0, 255 ) );
	_painter->drawLine( QPointF( 0.0f, 0.0f ), QPointF( boundingRect().width() / 2, 0.0f ) );
	_painter->setPen( QColor( 255, 0, 0 ) );
	_painter->drawLine( QPointF( 0.0f, 0.0f ), QPointF( 0.0f, boundingRect().height() / 2 ) );

	int extra = _painter->pen().width() / 2;
	QRectF rect = boundingRect();
	rect.adjust( extra, extra, extra, extra );
	_painter->setPen( Qt::red );
	_painter->drawRect( rect );

	if( EEditMode_Select == m_editMode )
		_painter->drawText( 0, 0, QObject::tr( "O" ) );
	else if( EEditMode_Move == m_editMode )
		_painter->drawText( 0, 0, QObject::tr( "T" ) );
	else if( EEditMode_Rotate == m_editMode )
		_painter->drawText( 0, 0, QObject::tr( "R" ) );
	else if( EEditMode_Scale == m_editMode )
		_painter->drawText( 0, 0, QObject::tr( "S" ) );
	else if( EEditMode_Alpha == m_editMode )
		_painter->drawText( 0, 0, QObject::tr( "A" ) );
	else
		_painter->drawText( 0, 0, QObject::tr( "X" ) );
}


void ReAnimGraphicsItem::mousePressEvent( QGraphicsSceneMouseEvent* _event )
{
	if( EEditMode_Move == m_editMode )
	{
		m_transformBackup = transform();
		m_dragInfo.SetCursorPosBackup( _event->screenPos() );
		m_dragInfo.SetItemPosBackup( pos() );
		m_dragInfo.StartMove();
	}
	else if( EEditMode_Rotate == m_editMode )
	{
		m_transformBackup = transform();
		m_dragInfo.SetCursorPosBackup( _event->screenPos() );
		m_dragInfo.SetItemPosBackup( QPointF( 0.0f, 0.0f ) );		// Store rotation delta.
		m_dragInfo.StartMove();
	}
	else if( EEditMode_Scale == m_editMode )
	{
		m_transformBackup = transform();
		m_dragInfo.SetCursorPosBackup( _event->screenPos() );
		m_dragInfo.SetItemPosBackup( QPointF( 1.0f, 1.0f ) );		// Store scale.
		m_dragInfo.StartMove();
	}
	else if( EEditMode_Alpha == m_editMode )
	{		
		m_dragInfo.SetCursorPosBackup( _event->screenPos() );
		m_dragInfo.SetItemPosBackup( QPointF( m_alpha, 0.0f ) );	// Backup alpha and store alpha delta.
		m_dragInfo.StartMove();
	}

	TSuper::mousePressEvent( _event );
}


void ReAnimGraphicsItem::mouseReleaseEvent( QGraphicsSceneMouseEvent* _event )
{
	if( EEditMode_Move == m_editMode )
	{
		m_dragInfo.Stop();

		QPointF p = pos();
		QPointF sp = scenePos();

		if( m_dragInfo.GetItemPosBackup() != pos() )
			emit PositionChanged( pos() );
	}
	else if( EEditMode_Rotate == m_editMode )
	{
		m_dragInfo.Stop();

		qreal delta = m_dragInfo.GetItemPosBackup().y();
		if( !qIsNull( delta ) )
		{
			m_rotationAccumulator += delta;
			emit RotationChanged( m_rotationAccumulator );
		}
	}
	else if( EEditMode_Scale == m_editMode )
	{
		m_dragInfo.Stop();

		QPointF delta = m_dragInfo.GetItemPosBackup();
		if( !delta.isNull() )
		{
			m_scaleAccumulator.setX( m_scaleAccumulator.x() * delta.x() );
			m_scaleAccumulator.setY( m_scaleAccumulator.y() * delta.y() );
			emit ScaleChanged( m_scaleAccumulator );
		}
	}
	else if( EEditMode_Alpha == m_editMode )
	{
		m_dragInfo.Stop();

		QPointF alphaBackup = m_dragInfo.GetItemPosBackup();
		if( !qIsNull( alphaBackup.y() ) )
		{			
			//printf( "Emit Alpha: %f\r\n", m_alpha );	
			emit AlphaChanged( AlphaClamp( alphaBackup.x() + alphaBackup.y() ) );
		}
	}

	return TSuper::mouseReleaseEvent( _event );
}


void ReAnimGraphicsItem::mouseMoveEvent( QGraphicsSceneMouseEvent* _event )
{
	bool ignoreEvent = false;

	if( EEditMode_Move == m_editMode )
	{
		if( m_dragInfo.IsMoving() )
		{
			QPointF delta = _event->screenPos() - m_dragInfo.GetCursorPosBackup();
			setPos( m_dragInfo.GetItemPosBackup() + delta );
		}
	}
	else if( EEditMode_Rotate == m_editMode )
	{
		if( m_dragInfo.IsMoving() )
		{			
			QPointF delta = m_dragInfo.GetCursorPosBackup() - _event->screenPos();
			setTransform( m_transformBackup );
			rotate( delta.y() );

			m_dragInfo.SetItemPosBackup( delta );
		}
	}
	else if( EEditMode_Scale == m_editMode )
	{
		if( m_dragInfo.IsMoving() )
		{
			QPointF delta = m_dragInfo.GetCursorPosBackup() - _event->screenPos();
			setTransform( m_transformBackup );
			qreal factor = 1.0f + delta.y() * m_scaleMultiplier;
			scale( factor, factor );

			m_dragInfo.SetItemPosBackup( QPointF( factor, factor ) );
		}
	}
	else if( EEditMode_Alpha == m_editMode )
	{
		if( m_dragInfo.IsMoving() )
		{
			QPointF posDelta = m_dragInfo.GetCursorPosBackup() - _event->screenPos();
			qreal delta = posDelta.y() * m_alphaMultiplier;
			//printf( "Alpha: %f + %f = %f\r\n", m_alpha, delta, m_alpha + delta );
			QPointF alphaBackup = m_dragInfo.GetItemPosBackup();
			SetAlpha( alphaBackup.x() + delta );

			m_dragInfo.SetItemPosBackup( QPointF( alphaBackup.x(), delta ) );
		}
	}

	if( ignoreEvent )
		_event->ignore();
	else
		TSuper::mouseMoveEvent( _event );
}


void ReAnimGraphicsItem::keyPressEvent( QKeyEvent* _event )
{
	if( Qt::Key_Escape == _event->key() )
	{
		if( m_dragInfo.IsMoving() )
		{
			m_dragInfo.Stop();
			setTransform( m_transformBackup );
		}
	}
}


// -----------------------------------------------------------------------------
// Slots
// -----------------------------------------------------------------------------


}
