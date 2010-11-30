// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101119.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_DATA_H_
#define _RE_CLIP_DATA_H_
// -----------------------------------------------------------------------------
// Use float type to store UV info, so that when the image gets scaled, the 
// graphics still match, and this is more convenient to handle zooming related
// features.
// -----------------------------------------------------------------------------
#include <QStandardItem>
#include <QPointF>
#include <QSizeF>


namespace RE
{


class ReClipData : public QStandardItem
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef QStandardItem	TSuper;

	ReClipData();

	// -------------------------------------------------------------------------
	// Override.
	// -------------------------------------------------------------------------
public:
	//virtual void		setData( const QVariant& _value, int _role = Qt::UserRole + 1 );

	// -------------------------------------------------------------------------
	// Interface.
	// -------------------------------------------------------------------------
public:
	QString				GetName() const			{ return m_name; }
	QString&			GetNameRef()			{ return m_name; }
	QPointF				GetOffset() const		{ return m_offset; }
	QSizeF				GetSize() const			{ return m_size; }
	QPointF&			GetOffsetRef()			{ return m_offset; }
	QSizeF&				GetSizeRef()			{ return m_size; }

	void				Flush();

protected:
	QString				m_name;
	QPointF				m_offset;
	QSizeF				m_size;
	bool				m_isFlushingData;

	static int			ms_nextId;
};


}
#endif	// _RE_CLIP_DATA_H_
