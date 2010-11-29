// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_RECTENVELOPER_H_
#define _RE_EDITOR_RECTENVELOPER_H_
// ----------------------------------------------------------------------------
// This utility helps to achieve the sliding panel effect on iPhone.
// ----------------------------------------------------------------------------


namespace RE
{


class ReRectEnveloper
{
public:
	enum eAlignment
	{
		EAlign_MiddleH		= 0x0001,
		EAlign_MiddleV		= 0x0002,
		EAlign_Left			= 0x0004,
		EAlign_Right		= 0x0008,
		EAlign_Top			= 0x0010,
		EAlign_Bottom		= 0x0020,
		EAlign_MaxShift		= 5
	};

public:
	static bool			sTick(	int& _newX, int& _newY,
								int _posX, int _posY, int _width, int _height,
								int _left, int _top, int _right, int _bottom,								
								int _alignmentMask );

	static bool			sHasAlignment( int _alignmentMask, eAlignment _alignment );
};


}		// namespace RE
#endif	// _RE_EDITOR_RECTENVELOPER_H_
