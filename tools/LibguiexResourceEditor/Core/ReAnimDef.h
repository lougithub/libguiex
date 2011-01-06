#ifndef _RE_ANIM_DEF_H_
#define _RE_ANIM_DEF_H_


#include <QString>
#include <QObject>
#include <QGraphicsItem>


namespace RE
{


// -----------------------------------------------------------------------------
// Constants.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Enumerations.
// -----------------------------------------------------------------------------
enum eGraphicsItemType
{
	EGraphicsItemType_Base	= QGraphicsItem::UserType + 1,
	EGraphicsItemType_Tool	= QGraphicsItem::UserType + 2,
};


enum eTrackType
{
	ETrackType_Translation,
	ETrackType_Rotation,
	ETrackType_Scale,
	ETrackType_Alpha,
	ETrackType_Count
};


enum eEditMode
{
	EEditMode_Select,
	EEditMode_Move,
	EEditMode_Rotate,
	EEditMode_Scale,
	EEditMode_Alpha,
	EEditMode_Count
};


// -----------------------------------------------------------------------------
// Utility functions.
// -----------------------------------------------------------------------------
inline QString gEditModeToString( eEditMode _mode )
{
	if( EEditMode_Move == _mode )
	{
		return QObject::tr( "Move" );
	}
	else if( EEditMode_Rotate == _mode )
	{
		return QObject::tr( "Rotate" );
	}
	else if( EEditMode_Scale == _mode )
	{
		return QObject::tr( "Scale" );
	}
	else if( EEditMode_Alpha == _mode )
	{
		return QObject::tr( "Alpha" );
	}
	else if( EEditMode_Select == _mode )
	{
		return QObject::tr( "Select" );
	}
	else
	{
		return QObject::tr( "Error" );
	}
}


inline QString gTrackTypeToString( eTrackType _type )
{
	QString result = QObject::tr( "Track" );
	switch( _type )
	{
	case ETrackType_Translation:
		result = QObject::tr( "Translation" );
		break;

	case ETrackType_Rotation:
		result = QObject::tr( "Rotation" );
		break;

	case ETrackType_Scale:
		result = QObject::tr( "Scale" );
		break;

	case ETrackType_Alpha:
		result = QObject::tr( "Alpha" );
		break;

	default:
		break;
	}

	return result;
}


inline int gDataValueCount( eTrackType _tracType )
{
	int result = 0;

	switch( _tracType )
	{
	case ETrackType_Translation:
		result = 2;
		break;

	case ETrackType_Rotation:
		result = 1;
		break;

	case ETrackType_Scale:
		result = 2;
		break;

	case ETrackType_Alpha:
		result = 1;
		break;

	default:
		break;
	}

	return result;
}


}
#endif	// _RE_ANIM_DEF_H_
