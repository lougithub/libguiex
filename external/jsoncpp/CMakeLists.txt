set( target_name "json" )
message( STATUS "generate library: " ${target_name} )

set( src_list 
    src/lib_json/json_reader.cpp 
    src/lib_json/json_value.cpp 
    src/lib_json/json_writer.cpp 
    src/lib_json/json_internalarray.inl 
    src/lib_json/json_internalmap.inl
    src/lib_json/json_valueiterator.inl 
	)

set( inc_list
    include/json/autolink.h
    include/json/config.h
    include/json/features.h
    include/json/forwards.h
    include/json/json.h
    include/json/reader.h
    include/json/value.h
    include/json/writer.h
	)
	
include_directories( include )
add_library( ${target_name} STATIC ${src_list} ${inc_list}  )

set_target_properties(${target_name} PROPERTIES FOLDER external)

