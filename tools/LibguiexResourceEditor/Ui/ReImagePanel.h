// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_IMAGEPANEL_H_
#define _RE_EDITOR_IMAGEPANEL_H_


#include <QWidget>
#include <QString>
#include <QList>
#include "UI\ReBaseWidget.h"


namespace RE
{


class ReImagePanel : public ReBaseWidget< QWidget >
{
	Q_OBJECT

	typedef ReBaseWidget< QWidget >	TSuper;

	// -----------------------------------------------------
	// Class ReImageItem
	// -----------------------------------------------------
	class ReImageItem
	{
	public:
		enum eStatus	{ EStatus_Created, EStatus_Loaded, EStatus_NotLoaded, EStatus_LoadFailed, EStatus_Recycled };

	public:
		QImage*			m_image;
		QString			m_path;
		eStatus			m_status;

		QPoint			m_pos;
		QPoint			m_newPos;

	public:
		explicit ReImageItem( QImage* _image, const QString& _path, eStatus _status = EStatus_Created ):
			m_image( _image ), m_path( _path ), m_status( _status ),
			m_pos( 0, 0 ), m_newPos( 0, 0 ) {}
	};

	// -----------------------------------------------------
	// General
	// -----------------------------------------------------
public:
	ReImagePanel( QWidget* _parent = NULL, int _imageSize = 64 );
	~ReImagePanel();

	// -----------------------------------------------------
	// Override
	// -----------------------------------------------------
protected:
	void				paintEvent( QPaintEvent* _event );
	void				resizeEvent( QResizeEvent* _event );
	void				wheelEvent( QWheelEvent* _event );
	void				mousePressEvent( QMouseEvent* _event );
	void				mouseReleaseEvent( QMouseEvent* _event );
	void				mouseMoveEvent( QMouseEvent* _event );
	QSize				minimumSizeHint() const;


	// -----------------------------------------------------
	// ImagePanel
	// -----------------------------------------------------
public:
	void				Clear();
	void				Upload( const QList< QString >& _idArray );
	void				Tick( qreal _delta );

	// -----------------------------------------------------
	// Utility
	// -----------------------------------------------------
protected:	
	ReImageItem*		FromPool();
	void				RefreshItemLayout();
	QSize				GetBv() const;
	bool				IsItemVisible( const ReImageItem* _item ) const;
	bool				IsDragging() const;
	bool				HasContent() const;	

	// -----------------------------------------------------
	// Variable
	// -----------------------------------------------------
protected:
	typedef QList< ReImageItem* >		TItemList;
	typedef TItemList::Iterator			TItemListItor;
	typedef TItemList::const_iterator	TItemListCItor;

	TItemList			m_itemList;
	TItemList			m_itemPool;

	// Sizing.
	int					m_minItemSize;
	int					m_maxItemSize;	
	int					m_currentItemSize;
	int					m_zoomStep;

	// Spacing.
	int					m_topPadding;
	int					m_bottomPadding;
	int					m_leftPadding;
	int					m_rightPadding;
	int					m_vSpacing;
	int					m_hSpacing;

	// Viewing.
	QPoint				m_contentOrigin;
	QPoint				m_contentOriginBackup;
	QPoint				m_dragStart;
	QSize				m_grid;
	QSize				m_bv;

	int					m_alignmentMask;
};


}		// namespace RE
#endif	// _RE_EDITOR_IMAGEPANEL_H_
