// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "Core\ReRectEnveloper.h"


namespace
{
	enum eMonoAlignment
	{
		EMonoALign_None,
		EMonoAlign_Head,
		EMonoAlign_Tail,
		EMonoAlign_Middle
	};

	/* Note:
		- _size must be positive ( thus unsigned int );
		- _min must be no greater than _max;
		- _speed must be positive ( thus unsigned int ).
	*/
	int sMonoTick( int _pos, int _size, int _min, int _max, unsigned int _speed, eMonoAlignment _alignment )
	{
		int min = _min;
		int max = _max;
		int pos = _pos;
		int size = _size;

		int result = pos;

		if( EMonoAlign_Middle == _alignment )
		{
			int center = pos + size / 2;
			int half = ( min + max ) / 2;
			
			if( center < half )
			{
				center += _speed;
				if( center > half )
					center = half;
			}
			else if( center > half )
			{
				center -= _speed;
				if( center < half )
					center = half;
			}

			result = center - size / 2;
		}
		else if( EMonoAlign_Head == _alignment )
		{
			if( pos != min )
			{
				if( pos < min )
				{
					pos += _speed;
					if( pos > min )
						pos = min;
				}
				else
				{
					pos -= _speed;
					if( pos < min )
						pos = min;
				}
			}

			result = pos;
		}
		else if( EMonoAlign_Tail == _alignment )
		{
			int end = pos + size;
			if( end != max )
			{
				if( end < max )
				{
					end += _speed;
					if( end > max )
						end = max;
				}
				else
				{
					end -= _speed;
					if( end < max )
						end = max;
				}
			}

			result = end - size;
		}

		return result;
	}
}


namespace RE
{


bool ReRectEnveloper::sTick(
	int& _newX, int& _newY,
	int _posX, int _posY, int _width, int _height, 
	int _left, int _top, int _right, int _bottom,
	int _alignmentMask  )
{
	static int sSpeed = 20;

	if( sHasAlignment( _alignmentMask, EAlign_MiddleH ) )
	{
		_newX = sMonoTick( _posX, _width, _left, _right, sSpeed, EMonoAlign_Middle );
	}
	else if( sHasAlignment( _alignmentMask, EAlign_Left ) )
	{
		_newX = sMonoTick( _posX, _width, _left, _right, sSpeed, EMonoAlign_Head );
	}
	else if( sHasAlignment( _alignmentMask, EAlign_Right ) )
	{
		_newX = sMonoTick( _posX, _width, _left, _right, sSpeed, EMonoAlign_Tail );
	}

	if( sHasAlignment( _alignmentMask, EAlign_MiddleV ) )
	{
		_newY = sMonoTick( _posY, _height, _top, _bottom, sSpeed, EMonoAlign_Middle );
	}
	else if( sHasAlignment( _alignmentMask, EAlign_Top ) )
	{
		_newY = sMonoTick( _posY, _height, _top, _bottom, sSpeed, EMonoAlign_Head );
	}
	else if( sHasAlignment( _alignmentMask, EAlign_Bottom ) )
	{
		_newY = sMonoTick( _posY, _height, _top, _bottom, sSpeed, EMonoAlign_Tail );
	}

	return _newX != _posX || _newY != _posY;
}


bool ReRectEnveloper::sHasAlignment( int _alignmentMask, eAlignment _alignment )
{
	return 0 != ( _alignmentMask & _alignment );
}


}	// namespace RE
