// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "UI\ReImagePanel.h"
#include "Core\ReRectEnveloper.h"
#include <QPainter>
#include <QWheelEvent>


namespace RE
{


// -----------------------------------------------------
// General
// -----------------------------------------------------
ReImagePanel::ReImagePanel( QWidget* _parent, int _imageSize )
: TSuper( _parent )
// Image item size.
, m_minItemSize( 16 )
, m_maxItemSize( 128 )
, m_currentItemSize( _imageSize )
, m_zoomStep( 4 )
// Spacing.
, m_topPadding( 0 )
, m_bottomPadding( 0 )
, m_leftPadding( 0 )
, m_rightPadding( 0 )
, m_vSpacing( 10 )
, m_hSpacing( 10 )
// Viewing.
, m_contentOrigin( 0, 0 )
, m_contentOriginBackup( 0, 0 )
, m_dragStart( -1, -1 )
, m_bv( 0, 0 )
{
}


ReImagePanel::~ReImagePanel()
{
	TItemListItor itor = m_itemList.begin();
	TItemListItor itorEnd = m_itemList.end();

	for( ; itor != itorEnd; ++itor )
	{
		ReImageItem* item = *itor;
		if( NULL != item )
		{
			if( NULL != item->m_image )
				delete item->m_image;
			delete item;
		}
	}

	m_itemList.clear();
}


// -----------------------------------------------------
// Override QWidget
// -----------------------------------------------------
void ReImagePanel::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	TItemListCItor itor = m_itemList.begin();
	TItemListCItor itorEnd = m_itemList.end();
		
	for( ; itor != itorEnd; ++itor )
	{
		const ReImageItem* item = *itor;
		if( NULL != item )
		{
			if( IsItemVisible( item ) )
			{
				QRect rect;
				rect.setLeft( m_contentOrigin.x() + item->m_pos.x() );
				rect.setRight( rect.left() + m_currentItemSize );
				rect.setTop( m_contentOrigin.y() + item->m_pos.y() );
				rect.setBottom( rect.top() + m_currentItemSize );
				painter.drawImage( rect, *item->m_image );
			}
		}
	}

	// Debug.
	//painter.setPen( QColor( 255, 255, 255 ) );
	//QSize bv = GetBv();
	//painter.drawRect( m_contentOrigin.x(), m_contentOrigin.y(), bv.width(), bv.height() );
}


void ReImagePanel::resizeEvent( QResizeEvent* _event )
{
	RefreshItemLayout();
}


void ReImagePanel::wheelEvent( QWheelEvent* _event )
{
	int degrees = _event->delta() / 8;
	int stepCount = degrees / 15;

	m_currentItemSize += m_zoomStep * stepCount;
	if( m_currentItemSize > m_maxItemSize )
		m_currentItemSize = m_maxItemSize;
	else if( m_currentItemSize < m_minItemSize )
		m_currentItemSize = m_minItemSize;

	RefreshItemLayout();
}


void ReImagePanel::mousePressEvent( QMouseEvent* _event )
{
	if( HasContent() )
	{
		if( ( Qt::MidButton == _event->button() ) ||
			( Qt::LeftButton == _event->button() && Qt::ControlModifier & _event->modifiers() ) )
		{
			m_contentOriginBackup = m_contentOrigin;
			m_dragStart = _event->pos();
		}
	}
}


void ReImagePanel::mouseReleaseEvent( QMouseEvent* _event )
{
	if( HasContent() )
	{
		if( IsDragging() )
			m_dragStart.setX( -1 );
	}
}


void ReImagePanel::mouseMoveEvent( QMouseEvent* _event )
{
	if( HasContent() )
	{
		if( IsDragging() )
		{
			// Dragging: vertical only.
			QPoint delta = _event->pos() - m_dragStart;
			m_contentOrigin = m_contentOriginBackup + delta;

			update();
		}
	}
}


QSize ReImagePanel::minimumSizeHint() const
{
	return QSize( m_leftPadding + m_rightPadding + m_minItemSize, 0 );
}


// -----------------------------------------------------
// ImagePanel
// -----------------------------------------------------
void ReImagePanel::Clear()
{
	TItemListItor itor = m_itemList.begin();
	TItemListItor itorEnd = m_itemList.end();

	for( ; itor != itorEnd; ++itor )
	{
		ReImageItem* item = *itor;
		if( NULL != item )
		{
			item->m_status = ReImageItem::EStatus_Recycled;
			m_itemPool.push_back( item );
		}
	}

	m_itemList.clear();
}


void ReImagePanel::Upload( const QList< QString >& _idArray )
{
	QList< QString >::const_iterator itor = _idArray.begin();
	QList< QString >::const_iterator itorEnd = _idArray.end();

	for( ; itor != itorEnd; ++itor )
	{
		const QString& id = *itor;
		ReImageItem* item = FromPool();
		item->m_image->load( id );
		item->m_path = id;
		item->m_status = ReImageItem::EStatus_Loaded;
		m_itemList.push_back( item );
	}

	RefreshItemLayout();
}


void ReImagePanel::Tick( float _delta )
{
	bool isDirty = false;

	// Update item positioning.
	TItemListItor itor = m_itemList.begin();
	TItemListItor itorEnd = m_itemList.end();	

	for( ; itor != itorEnd; ++itor )
	{
		ReImageItem* item = *itor;
		if( NULL != item )
		{
			static float sXSpeed = 200.0f;
			static float sYSpeed = 200.0f;
			int x = item->m_pos.x();
			int newX = item->m_newPos.x();			
			int y = item->m_pos.y();
			int newY = item->m_newPos.y();

			if( x != newX )
			{
				bool sign = x < newX;
				x += sign ? 10 : -10;
				if( sign != ( x < newX ) )
					x = newX;

				item->m_pos.setX( x );
				isDirty = true;
			}

			if( y != newY )
			{				
				bool sign = y < newY;
				y += sign ? 10 : -10;
				if( sign != ( y < newY ) )
					y = newY;

				item->m_pos.setY( y );
				isDirty = true;
			}
		}
	}

	// Rect enveloping.
	if( HasContent() && !IsDragging() )
	{
		int newViewportOriginX = m_contentOrigin.x();
		int newViewportOriginY = m_contentOrigin.y();
		QSize bv = GetBv();
		if( ReRectEnveloper::sTick( newViewportOriginX, newViewportOriginY, m_contentOrigin.x(), m_contentOrigin.y(),
			bv.width(), bv.height(), 0, 0, width(), height(), ReRectEnveloper::EAlign_MiddleH ) )
		{
			m_contentOrigin.setX( newViewportOriginX );
			m_contentOrigin.setY( newViewportOriginY );
			isDirty = true;
		}
	}

	if( isDirty )
		update();
}


// -----------------------------------------------------
// Utility
// -----------------------------------------------------
ReImagePanel::ReImageItem* ReImagePanel::FromPool()
{
	ReImageItem* item = NULL;

	if( m_itemPool.size() > 0 )
	{
		item = m_itemPool.back();
		item->m_pos = item->m_newPos = QPoint( 0, 0 );
		m_itemPool.pop_back();
	}
	else
	{
		static QString sDummyPath = tr( "unknown" );
		QImage* image = new QImage();
		item = new ReImageItem( image, sDummyPath );
	}

	return item;
}


void ReImagePanel::RefreshItemLayout()
{
	if( m_itemList.size() > 0 )
	{
		TItemListItor itor = m_itemList.begin();
		TItemListItor itorEnd = m_itemList.end();

		int colCount = 1;
		int lastX = m_currentItemSize;
		while( lastX + m_hSpacing + m_currentItemSize <= width() ) 
		{
			++colCount;
			lastX += ( m_hSpacing + m_currentItemSize );
		}

		int rowCount = m_itemList.size() / colCount + ( ( 0 != ( m_itemList.size() % colCount ) ) ? 1 : 0 );
		m_grid.setWidth( colCount );
		m_grid.setHeight( rowCount );
		bool quit = false;	

		for( int i = 0; !quit && itor != itorEnd; ++itor, ++i )
		{
			ReImageItem* item = *itor;
			if( NULL != item && NULL != item->m_image )
			{
				int col = i % colCount;
				int row = i / colCount;

				int top = row * ( m_currentItemSize + m_vSpacing );
				int left = ( 0 == col ) ? 0 : col * ( m_currentItemSize + m_hSpacing );

				item->m_newPos.setX( left );
				item->m_newPos.setY( top );
			}
		}
	}
}


QSize ReImagePanel::GetBv() const
{
	// The bounding volume should contain all the items and the spacing between them
	// plus the surrounding paddings.
	QSize bv;

	if( HasContent() )
	{
		bv.setWidth( ( m_grid.width() - 1 ) * ( m_currentItemSize + m_hSpacing ) + m_currentItemSize + m_leftPadding + m_rightPadding );
		bv.setHeight( ( m_grid.height() - 1 ) * ( m_currentItemSize + m_vSpacing ) + m_currentItemSize + m_topPadding + m_bottomPadding );
	}

	return bv;
}


bool ReImagePanel::IsItemVisible( const ReImageItem* _item ) const
{
	if( NULL != _item )
	{
		QPoint pos = m_contentOrigin + _item->m_pos;

		return ( pos.x() > -m_currentItemSize && pos.x() < width() ) && ( pos.y() > -m_currentItemSize && pos.y() < height() );
	}
	else
	{
		return false;
	}
}


bool ReImagePanel::IsDragging() const
{
	return -1 != m_dragStart.x();
}


bool ReImagePanel::HasContent() const
{
	return m_itemList.size() > 0;
}


}	// namespace RE
