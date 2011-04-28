set( target_name "zlib" )
message( STATUS "generate library: " ${target_name} )


set(ZLIB_PUBLIC_HDRS
    ../zconf.h
    ../zlib.h
)
set(ZLIB_PRIVATE_HDRS
    ../crc32.h
    ../deflate.h
    ../gzguts.h
    ../inffast.h
    ../inffixed.h
    ../inflate.h
    ../inftrees.h
    ../trees.h
    ../zutil.h
)
set(ZLIB_SRCS
    ../adler32.c
    ../compress.c
    ../crc32.c
    ../deflate.c
    ../gzclose.c
    ../gzlib.c
    ../gzread.c
    ../gzwrite.c
    ../inflate.c
    ../infback.c
    ../inftrees.c
    ../inffast.c
    ../trees.c
    ../uncompr.c
    ../zutil.c
)

include_directories( ../ )
add_library(${target_name} STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
set_target_properties(${target_name} PROPERTIES FOLDER external)

