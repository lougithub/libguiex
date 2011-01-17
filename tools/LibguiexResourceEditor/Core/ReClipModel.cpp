// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101119.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReClipModel.h"
#include "Ui\ReClipUiInfo.h"
#include <QPixmap>
#include <QFile>
#include <QXmlStreamWriter>


namespace RE
{


// -------------------------------------------------------------------------
// ReClipNode.
// -------------------------------------------------------------------------
void ReClipNode::SetOffset( const QPoint& _offset )
{
	m_offset = _offset;

	ReClipModel* clipModel = GetModel();
	clipModel->OnClipChanged( this );
}


void ReClipNode::SetSize( const QSize& _size )
{
	m_size = _size;

	ReClipModel* clipModel = GetModel();
	clipModel->OnClipChanged( this );
}


// -------------------------------------------------------------------------
// General.
// -------------------------------------------------------------------------
ReClipModel::ReClipModel( QWidget* _parent /* = NULL */ )
: TSuper( ( QObject* )_parent )
{
	m_rootNode = new ReClipGroupNode( this );
}


ReClipModel::~ReClipModel()
{
	delete m_rootNode;
}


void ReClipModel::Clear()
{
	m_rootNode->DestroyChildren();
}


void ReClipModel::OnClipChanged( ReClipNode* _clip )
{
	ReClipGroupNode* group = ( ReClipGroupNode* )_clip->GetParent();
	QModelIndex groupIndex = index( m_rootNode->GetChildIndex( group ), 0, QModelIndex() );
	QModelIndex clipIndex = index( group->GetChildIndex( _clip ), 0, groupIndex );
	QModelIndex clipIndex2 = index( group->GetChildIndex( _clip ), EColumn_Count - 1, groupIndex );
	emit dataChanged( clipIndex, clipIndex2 );
}


bool ReClipModel::Export( const QString& _filePath ) const
{
	bool result = false;

	QFile file( _filePath );
	if( file.open( QFile::WriteOnly | QFile::Text ) )
	{
		QXmlStreamWriter writer( &file );
		writer.setAutoFormatting( true );
		writer.writeStartDocument();

		int groupCount = GetGroupCount();
		for( int i = 0; i < groupCount; ++i )
		{
			ReClipGroupNode* group = GetGroupByIndex( i );
			writer.writeStartElement( "property" );
			{
				QString imagePath = group->GetImageId();
				writer.writeAttribute( "name", imagePath );
				writer.writeAttribute( "type", "CGUIImageGroupDefine" );

				int clipCount = group->GetChildrenCount();
				for( int j = 0; j < clipCount; ++j )
				{
					ReClipNode* clip = ( ReClipNode* )group->GetChild( j );
					int scalar = clip->GetZoomScalar();

					writer.writeStartElement( "property" );
					{
						writer.writeAttribute( "name", clip->GetName() );
						writer.writeAttribute( "type", "CGUIImageDefine" );				
						{
							writer.writeStartElement( "property" );
							{
								writer.writeAttribute( "name", "path" );
								writer.writeAttribute( "type", "CGUIString" );
								writer.writeAttribute( "value", imagePath );
							}
							writer.writeEndElement();

							writer.writeStartElement( "property" );
							{
								writer.writeAttribute( "name", "uv" );
								writer.writeAttribute( "type", "CGUIRect" );
								writer.writeAttribute( "value", QString( "%1,%2,%3,%4" ).
									arg( clip->GetOffset().x() / scalar ).
									arg( clip->GetOffset().y() / scalar ).
									arg( clip->GetSize().width() / scalar ).
									arg( clip->GetSize().height() / scalar ) );
							}
							writer.writeEndElement();

							writer.writeStartElement( "property" );
							{
								writer.writeAttribute( "name", "orientation" );
								writer.writeAttribute( "type", "EImageOrientation" );
								writer.writeAttribute( "value", "eImageOrientation_Normal" );
							}
							writer.writeEndElement();
						}
					}
					writer.writeEndElement();
				}
			}
		}

		writer.writeEndDocument();
		result = true;
	}

	return result;
}


bool ReClipModel::Import( const QString& _filePath )
{
	bool result = false;

	QFile file( _filePath );
	if( file.open( QFile::ReadOnly | QFile::Text ) )
	{

	}

	return result;
}


// -------------------------------------------------------------------------
// Override QAbstractItemModel.
// -------------------------------------------------------------------------
QVariant ReClipModel::data( const QModelIndex& _index, int _role /* = Qt::DisplayRole */ ) const
{
	QVariant result = QVariant();
	
	if( _index.isValid() )
	{
		if( Qt::DisplayRole == _role )
		{
			TNode* node = IndexToNode( _index );
			if( EColumn_Name == _index.column() )
			{
				result = node->GetName();
			}
			else if( EColumn_Detail == _index.column() )
			{
				result = node->GetDetail();
			}
		}
		else if( Qt::DecorationRole == _role )
		{
			if( EColumn_Preview == _index.column() )
			{
				TNode* node = IndexToNode( _index );
				ReClipGroupNode* group = dynamic_cast< ReClipGroupNode* >( node );
				QPixmap pixmap;
				if( NULL != group )
				{
					pixmap = *group->GetImage();
				}
				else
				{
					ReClipNode* clip = ( ReClipNode* )node;
					group = ( ReClipGroupNode* )clip->GetParent();
					QPoint offset = clip->GetOffset() / group->GetZoomScalar();
					QSize size = clip->GetSize() / group->GetZoomScalar();
					pixmap = group->GetImage()->copy( offset.x(), offset.y(), size.width(), size.height() );
				}

				// Scale to fit the desired preview window.
				int w = pixmap.width();
				int h = pixmap.height();
				if( w > CLIP_PREVIEW_WIDTH && h > CLIP_PREVIEW_HEIGHT )
				{
					result = pixmap.scaled( CLIP_PREVIEW_WIDTH, CLIP_PREVIEW_HEIGHT );
				}
				else if( w > CLIP_PREVIEW_WIDTH )
				{
					result = pixmap.scaledToWidth( CLIP_PREVIEW_WIDTH );
				}
				else if( h > CLIP_PREVIEW_HEIGHT )
				{
					result = pixmap.scaledToHeight( CLIP_PREVIEW_HEIGHT );
				}
				else
				{
					result = pixmap;
				}
			}
		}
		else if( Qt::TextAlignmentRole == _role )
		{
			result = Qt::AlignLeft;
		}
	}

	return result;
}


bool ReClipModel::setData( const QModelIndex& _index, const QVariant& _value, int _role /* = Qt::EditRole */ )
{
	//ReClipData* clipData = dynamic_cast< ReClipData* >( TSuper::item( _index.row() ) );
	////QString name = data( index( _index.row(), EColumn_Name, QModelIndex() ) ).toString();
	//QString name = data( index( _index.row(), EColumn_Preview, QModelIndex() ) ).toString();
	//qreal x = data( index( _index.row(), EColumn_X, QModelIndex() ) ).toDouble();
	//qreal y = data( index( _index.row(), EColumn_Y, QModelIndex() ) ).toDouble();
	//qreal w = data( index( _index.row(), EColumn_W, QModelIndex() ) ).toDouble();
	//qreal h = data( index( _index.row(), EColumn_H, QModelIndex() ) ).toDouble();

	//clipData->GetNameRef() = name;
	//clipData->GetOffsetRef().setX( x );
	//clipData->GetOffsetRef().setY( y );
	//clipData->GetSizeRef().setWidth( w );
	//clipData->GetSizeRef().setHeight( h );

	//return TSuper::setData( _index, _value, _role );

	return false;
}


QModelIndex ReClipModel::index( int _row, int _column, const QModelIndex& _parent ) const
{
	QModelIndex result = QModelIndex();

	if( _row >= 0 && _column >= 0 )
	{
		if( _parent.isValid() )
		{
			ReClipGroupNode* group = ( ReClipGroupNode* )IndexToNode( _parent );
			if( _row < group->GetChildrenCount() )
			{
				result = createIndex( _row, _column, group->GetChild( _row ) );	
			}
		}
		else
		{
			if( _row < GetGroupCount() )
			{
				result = createIndex( _row, _column, GetGroupByIndex( _row ) );
			}			
		}
	}

	return result;
}


QModelIndex ReClipModel::parent( const QModelIndex& _index ) const
{
	QModelIndex result = QModelIndex();

	if( _index.isValid() )
	{
		// When Qt is doing some basic sanity check at start at ( set model ),
		// the node return here could be null.
		TNode* node = IndexToNode( _index );
		if( NULL != node )
		{
			ReClipNode* clip = dynamic_cast< ReClipNode* >( node );
			if( NULL != clip )
			{
				ReClipGroupNode* group = ( ReClipGroupNode* )clip->GetParent();
				result = createIndex( m_rootNode->GetChildIndex( group ), 0, group );
			}
		}
	}

	return result;
}


bool ReClipModel::insertRows( int _row, int _count, const QModelIndex& _parent )
{
	bool result = false;

	beginInsertRows( _parent, _row, _row + _count - 1 );

	if( _row >= 0 && _count > 0 )
	{
		if( _parent.isValid() )
		{
			// We are creating new clips.
			ReClipGroupNode* group = ( ReClipGroupNode* )IndexToNode( _parent );
			ReClipNode* clip = new ReClipNode( this );
			clip->SetParent( group );
			clip->SetName( QString( tr( "Clip %1" ) ).arg( group->GetNextChildId() ) );
			group->AddNode( clip );
		}
		else
		{
			// We are creating new clip groups.
			ReClipGroupNode* group = new ReClipGroupNode( this );
			group->SetParent( m_rootNode );
			group->SetName( QString( tr( "Group %1" ) ).arg( m_rootNode->GetNextChildId() ) );
			m_rootNode->AddNode( group );
		}

		result = true;		
	}

	endInsertRows();

	return result;
}


bool ReClipModel::removeRows( int _row, int _count, const QModelIndex& _parent )
{
	bool result = false;

	beginRemoveRows( _parent, _row, _row + _count - 1 );

	if( _row >= 0 && _count > 0 )
	{
		if( _parent.isValid() )
		{
			// We are removing existing clips.
			ReClipGroupNode* group = ( ReClipGroupNode* )IndexToNode( _parent );
			TNode* clip = group->GetChild( _row );
			group->RemoveNode( clip );
			delete clip;
		}
		else
		{
			// We are removing existing clip groups.
			ReClipGroupNode* group = ( ReClipGroupNode* )m_rootNode->GetChild( _row );
			m_rootNode->RemoveNode( group );
			delete group;
		}

		result = true;		
	}

	endRemoveRows();

	return result;
}


int ReClipModel::rowCount( const QModelIndex& _parent ) const
{
	TNode* node = IndexToNode( _parent );
	
	return node->GetChildrenCount();
}


int ReClipModel::columnCount( const QModelIndex& _parent ) const
{
	return EColumn_Count;
}

QVariant ReClipModel::headerData( int _section, Qt::Orientation _orientation, int _role ) const
{
	QVariant result = QVariant();

	if( Qt::DisplayRole == _role )
	{
		if( Qt::Horizontal == _orientation )
		{
			if( EColumn_Preview == _section )
				result = tr( "Preview" );
			else if( EColumn_Name == _section )
				result = tr( "Name" );
			else if( EColumn_Detail == _section )
				result = tr( "Detail" );
			//else if( EColumn_X == _secntion )
			//	result = tr( "X" );
			//else if( EColumn_Y == _section )
			//	result = tr( "Y" );
			//else if( EColumn_W == _section )
			//	result = tr( "W" );
			//else if( EColumn_H == _section )
			//	result = tr( "H" );
		}
	}	

	return result;
}


QMimeData* ReClipModel::mimeData( const QModelIndexList& _indexes ) const
{
	return TSuper::mimeData( _indexes );
}


ReClipModel::TFlags ReClipModel::flags( const QModelIndex& _index ) const
{
	TFlags result = TSuper::flags( _index );
	if( EColumn_Preview == _index.column() )
	{
		result |= Qt::ItemIsDragEnabled;
	}
	else
	{
		result |= Qt::ItemIsEditable;
		result &= ~Qt::ItemIsDragEnabled;
	}

	return result;
}


// -------------------------------------------------------------------------
// Utilities.
// -------------------------------------------------------------------------
ReClipGroupNode* ReClipModel::GetGroupByIndex( int _index ) const
{
	if( m_rootNode->GetChildrenCount() > 0 )
	{
		const ReClipGroupNode* group = ( ReClipGroupNode* )m_rootNode->GetChild( _index );
		return const_cast< ReClipGroupNode* >( group );
	}
	else
	{
		return NULL;
	}
}


ReClipGroupNode* ReClipModel::GetGroupById( const QString& _id )
{
	ReClipGroupNode* result = NULL;

	int groupCount = GetGroupCount();
	for( int i = 0; i < groupCount; ++i )
	{
		ReClipGroupNode* group = GetGroupByIndex( i );
		if( 0 == group->GetImageId().compare( _id ) )
		{
			result = group;
			break;
		}
	}

	return result;
}


ReClipGroupNode* ReClipModel::CreateGroup( const QString& _id )
{
	ReClipGroupNode* group = GetGroupById( _id );
	if( NULL == group )
	{
		int groupCount = GetGroupCount();
		insertRow( groupCount, QModelIndex() );

		group = ( ReClipGroupNode* )m_rootNode->GetChild( groupCount );
		group->SetImageId( _id );
	}

	return group;
}


ReClipNode* ReClipModel::CreateClip( ReClipGroupNode* _group )
{
	ReClipNode* clip = NULL;

	int groupIndex = m_rootNode->GetChildIndex( _group );
	if( groupIndex >= 0 )
	{
		int clipCount = _group->GetChildrenCount();
		insertRow( clipCount, index( groupIndex, 0, QModelIndex() ) );

		clip = ( ReClipNode* )_group->GetChild( clipCount );
	}

	return clip;
}


void ReClipModel::DestroyGroup( ReClipGroupNode* _group )
{
	removeRow( m_rootNode->GetChildIndex( _group ), QModelIndex() );
}


void ReClipModel::DestroyClip( ReClipNode* _clip )
{
	ReClipGroupNode* group = ( ReClipGroupNode* )_clip->GetParent();
	QModelIndex parentIndex = index( m_rootNode->GetChildIndex( group ), 0, QModelIndex() );
	removeRow( group->GetChildIndex( _clip ), parentIndex );
}


ReClipModel::TNode* ReClipModel::IndexToNode( const QModelIndex& _index ) const
{
	return _index.isValid() ? static_cast< TNode* >( _index.internalPointer() ) : m_rootNode;
}


}
