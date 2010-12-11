#ifndef _RE_ANIM_DEF_H_
#define _RE_ANIM_DEF_H_


#include <QString>
#include <QObject>


namespace RE
{


enum eTrackType
{
	ETrackType_Translation,
	ETrackType_Rotation,
	ETrackType_Scale,
	ETrackType_Alpha,
	ETrackType_Count
};


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
