#include "StdAfxEditor.h"
#include "Core\ReAnimTrack.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimTrack::ReAnimTrack()
: m_isSortDirty( false )
{
}


ReAnimTrack::~ReAnimTrack()
{
	TFrameListCItor itor = m_frames.begin();
	TFrameListCItor itorEnd = m_frames.end();
	for( ; itor != itorEnd; ++itor )
	{
		delete *itor;
	}

	m_frames.clear();
}


ReAnimFrame* ReAnimTrack::CreateFrame( qreal _time )
{
	ReAnimFrame* result = DoCreateFrame( _time );
	result->SetName( QString( QObject::tr( "Frame" ) ) );
	result->SetParent( this );
	m_frames.push_back( result );
	m_isSortDirty = true;

	return result;
}


void ReAnimTrack::DeleteFrame( ReAnimFrame* _frame )
{
	if( NULL != _frame )
	{
		TFrameListItor itor = m_frames.begin();
		TFrameListItor itorEnd = m_frames.end();
		for( ; itor != itorEnd; ++itor )
		{
			if( _frame == *itor )
			{
				m_frames.erase( itor );
				delete _frame;
				break;
			}
		}
	}
}


ReAnimFrame* ReAnimTrack::GetFrameByIndex( int _index )
{
	ReAnimFrame* result = NULL;

	if( IsDirty() )
		Sort();

	if( _index >= 0 && _index < m_frames.size() )
		result = m_frames.value( _index );

	return result;
}


bool ReAnimTrack::HasFrame( const ReAnimFrame* _frame ) const
{
	bool result = false;

	TFrameListCItor itor = m_frames.begin();
	TFrameListCItor itorEnd = m_frames.end();
	for( ; itor != itorEnd && !result; ++itor )
		if( _frame == *itor )
			result = true;

	return result;
}


bool ReAnimTrack::Interpolate( qreal _time, QVariant& _result, bool _allowExterpolate )
{
	bool isOk = false;

	ReAnimFrame* left = NULL;
	ReAnimFrame* right = NULL;
	GetNearestFrames( _time, left, right );

	if( NULL != left && NULL != right )
	{
		if( left == right )
		{
			_result = left->GetDataVariant();
		}
		else
		{
			qreal factor = 1.0f - ( _time - left->GetTime() ) / ( right->GetTime() - left->GetTime() );
			QVariant leftValue = left->GetDataVariant();
			QVariant rightValue = right->GetDataVariant();
			_result = InterpolateFrameValue( leftValue, rightValue, factor );
		}

		isOk = true;
	}
	else
	{
		if( _allowExterpolate )
		{
			if( NULL != left )
			{
				_result = left->GetDataVariant();
				isOk = true;
			}
			else if( NULL != right )
			{
				_result = right->GetDataVariant();
				isOk = true;
			}
		}
	}

	return isOk;
}


// -----------------------------------------------------------------------------
// Override ReAnimNode.
// -----------------------------------------------------------------------------
int ReAnimTrack::IndexOfChild( const ReAnimNode* _child )
{
	int result = -1;

	if( NULL != _child )
	{
		if( IsDirty() )
			Sort();

		TFrameListCItor itor = m_frames.begin();
		TFrameListCItor itorEnd = m_frames.end();
		for( int i = 0; itor != itorEnd; ++itor, ++i )
		{
			if( *itor == _child )
			{
				result = i;
				break;
			}
		}
	}

	return result;
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReAnimTrack::GetNearestFrames( qreal _time, ReAnimFrame*& _left, ReAnimFrame*& _right )
{
	if( NULL != ( &_left ) || NULL != ( &_right ) )
	{
		int minLeft = 999999;
		int minRight = 999999;

		TFrameListItor itor = m_frames.begin();
		TFrameListItor itorEnd = m_frames.end();
		for( ; itor != itorEnd; ++itor )
		{
			ReAnimFrame* frame = *itor;
			qreal time = frame->GetTime();
			if( time == _time )
			{
				if( NULL != ( &_left ) )
					_left = frame;
				if( NULL != ( &_right ) )
					_right = frame;
			}
			else if( time < _time )
			{
				if( NULL != ( &_left ) )
				{
					int delta = _time - time;
					if( delta < minLeft )
					{
						minLeft = delta;
						_left = frame;
					}
				}
			}
			else
			{
				if( NULL != ( &_right ) )
				{
					int delta = time - _time;
					if( delta < minRight )
					{
						minRight = delta;
						_right = frame;
					}
				}
			}
		}
	}
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
