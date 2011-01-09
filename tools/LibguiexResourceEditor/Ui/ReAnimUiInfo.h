#ifndef _RE_ANIM_UI_INFO_H_
#define _RE_ANIM_UI_INFO_H_


namespace RE
{
	static const int	ANIM_RULER_UNIT				= 2;
	static const int	ANIM_RULER_SNAP				= 10;
	static const int	ANIM_RULER_HEIGHT			= 36;

	static const int	ANIM_FRAME_WIDTH			= 9;
	static const int	ANIM_FRAME_HEIGHT			= 20;
	static const QSize	ANIM_FRAME_SIZE				= QSize( ANIM_FRAME_WIDTH, ANIM_FRAME_HEIGHT );
	static const int	ANIM_ENTITY_SUITE_HEIGHT	= 16;

	static const int	ANIM_FPS_DEFAULT			= 30;
	
	static const int	ANIM_PLAYER_BUTTON_SIZE		= 16;
	static const int	ANIM_HEADER_WIDTH			= ANIM_PLAYER_BUTTON_SIZE * 5;
	static const int	ANIM_CONSOLE_V_GAP			= 2;
	static const int	ANIM_CONSOLE_H_GAP			= 2;
	static const int	ANIM_PLAYER_PANEL_HEIGHT	= ANIM_PLAYER_BUTTON_SIZE * 2 + ANIM_CONSOLE_V_GAP;
}
#endif	// _RE_ANIM_UI_INFO_H_
