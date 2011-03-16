set( target_name "glew" )
message( STATUS "generate library: " ${target_name} )

set( src_list 
    ../src/glew.c
    )

set( inc_list
	../include/GL/glew.h
    ../include/GL/wglew.h
    )
	

include_directories( ../include )
add_library( ${target_name} STATIC ${src_list} ${inc_list}  )
set_target_properties( 
	${target_name}
	PROPERTIES
	COMPILE_DEFINITIONS GLEW_STATIC
	)


