#include "StdAfxEditor.h"
#include "Core\ReAnimTrack.h"
#include <algorithm>


namespace
{
	using namespace RE;

	//bool SortFramePredicate( const ReAnimFrame* _frameA, const ReAnimFrame* _frameB )
	//{
	//	return _frameA->GetTime() < _frameB->GetTime();
	//}

	//bool SortFramePredicate( const ReAnimFrame& _frameA, const ReAnimFrame& _frameB )
	//{
	//	return _frameA.GetTime() < _frameB.GetTime();
	//}

	class FrameSortPredicater
	{
	public:
		bool operator()( const ReAnimFrame* _frameA, const ReAnimFrame* _frameB ) const
		{
			return _frameA->GetTime() < _frameB->GetTime();
		}
	};
}


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimTrack::ReAnimTrack()
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

	m_frames.sort( FrameSortPredicater() );

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


qreal ReAnimTrack::GetTotalLength()
{
	qreal result = 0.0f;

	if( m_frames.size() > 0 )
	{
		m_frames.sort( FrameSortPredicater() );
		ReAnimFrame* lastFrame = m_frames.back();
		result = lastFrame->GetTime();
	}

	return result;
}


// -----------------------------------------------------------------------------
// Override ReAnimNode.
// -----------------------------------------------------------------------------
int ReAnimTrack::IndexOfChild( const ReAnimNode* _child )
{
	int result = -1;

	if( NULL != _child )
	{
		m_frames.sort( FrameSortPredicater() );

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
		qreal minLeft = 999999.9;
		qreal minRight = 999999.9;

		ReAnimFrame* left = NULL;
		ReAnimFrame* right = NULL;

		TFrameListItor itor = m_frames.begin();
		TFrameListItor itorEnd = m_frames.end();
		for( ; itor != itorEnd; ++itor )
		{
			ReAnimFrame* frame = *itor;
			qreal time = frame->GetTime();
			if( time == _time )
			{
				left = right = frame;
			}
			else if( time < _time )
			{
				qreal delta = _time - time;
				if( delta < minLeft )
				{
					minLeft = delta;
					_left = frame;
				}
			}
			else
			{
				qreal delta = time - _time;
				if( delta < minRight )
				{
					minRight = delta;
					_right = frame;
				}
			}
		}

		if( NULL != left && NULL != &_left )
			_left = left;
		if( NULL != right && NULL != &_right )
			_right = right;
	}
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
