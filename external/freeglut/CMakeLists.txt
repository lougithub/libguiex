set( target_name "freeglut" )
message( STATUS "generate library: " ${target_name} )

set( src_list 
	src/freeglut_callbacks.c
	src/freeglut_cursor.c
	src/freeglut_display.c
	src/freeglut_ext.c
	src/freeglut_font.c
	src/freeglut_font_data.c
	src/freeglut_gamemode.c
	src/freeglut_geometry.c
	src/freeglut_glutfont_definitions.c
	src/freeglut_init.c
	src/freeglut_input_devices.c
	src/freeglut_joystick.c
	src/freeglut_main.c
	src/freeglut_menu.c
	src/freeglut_misc.c
	src/freeglut_overlay.c
	src/freeglut_spaceball.c
	src/freeglut_state.c
	src/freeglut_stroke_mono_roman.c
	src/freeglut_stroke_roman.c
	src/freeglut_structure.c
	src/freeglut_teapot.c
	src/freeglut_videoresize.c
	src/freeglut_window.c
	)

set( inc_list
	include/GL/freeglut.h
	include/GL/freeglut_ext.h
	include/GL/freeglut_std.h
	include/GL/glut.h
	src/freeglut_internal.h
	src/freeglut_teapot_data.h
    )
	

include_directories( include )
add_library( ${target_name} STATIC ${src_list} ${inc_list}  )
set_target_properties( 
	${target_name}
	PROPERTIES
	COMPILE_DEFINITIONS FREEGLUT_STATIC
	)

set_target_properties(${target_name} PROPERTIES FOLDER external)

