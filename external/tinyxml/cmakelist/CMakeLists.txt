set( library_name "tinyxml" )
message( STATUS "generate library: " ${library_name} )

set( src_list 
    ../tinystr.cpp
    ../tinyxml.cpp
    ../tinyxmlerror.cpp
    ../tinyxmlparser.cpp
    )

set( inc_list
    ../tinystr.h
    ../tinyxml.h
    )
			
	


#foreach( F ${src_list} ${inc_list} )
#    message( ${F} )
#endforeach( F )
add_library( ${library_name} STATIC ${src_list} ${inc_list}  )

