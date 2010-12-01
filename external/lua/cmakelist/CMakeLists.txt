set( library_name "lua" )
message( STATUS "generate library: " ${library_name} )

set( src_list 
    ../src/lapi.c
    ../src/lauxlib.c
    ../src/lbaselib.c
    ../src/lcode.c
    ../src/ldblib.c
    ../src/ldebug.c
    ../src/ldo.c
    ../src/ldump.c
    ../src/lfunc.c
    ../src/lgc.c
    ../src/linit.c
    ../src/liolib.c
    ../src/llex.c
    ../src/lmathlib.c
    ../src/lmem.c
    ../src/loadlib.c
    ../src/lobject.c
    ../src/lopcodes.c
    ../src/loslib.c
    ../src/lparser.c
    ../src/lstate.c
    ../src/lstring.c
    ../src/lstrlib.c
    ../src/ltable.c
    ../src/ltablib.c
    ../src/ltm.c
    ../src/lundump.c
    ../src/lvm.c
    ../src/lzio.c
    )

set( inc_list
    ../src/lapi.h
    ../src/lauxlib.h
    ../src/lcode.h
    ../src/ldebug.h
    ../src/ldo.h
    ../src/lfunc.h
    ../src/lgc.h
    ../src/llex.h
    ../src/lmem.h
    ../src/lobject.h
    ../src/lopcodes.h
    ../src/lparser.h
    ../src/lstate.h
    ../src/lstring.h
    ../src/ltable.h
    ../src/ltm.h
    ../src/lundump.h
    ../src/lvm.h
    ../src/lzio.h
    )
	


#foreach( F ${src_list} ${inc_list} )
#    message( ${F} )
#endforeach( F )
add_library( ${library_name} STATIC ${src_list} ${inc_list}  )

