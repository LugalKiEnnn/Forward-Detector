
set(INCLUDE_DIRECTORIES
 ${BASE_INCLUDE_DIRECTORIES}
 ${CMAKE_SOURCE_DIR}/mcstack
 ${CMAKE_SOURCE_DIR}/bmndata
 ${CMAKE_SOURCE_DIR}/FD
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${Boost_INCLUDE_DIRS}
 ${ROOT_INCLUDE_DIR} 
)

include_directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
 ${ROOT_LIBRARY_DIR}
 ${FAIRROOT_LIBRARY_DIR}
)
 
link_directories(${LINK_DIRECTORIES})

# List of source files
set(SRCS
BmnFD.cxx 
BmnFDPoint.cxx  
BmnFDContFact.cxx  
BmnFDGeo.cxx 
BmnFDGeoPar.cxx
)

Set(HEADERS)
Set(LINKDEF BmnFDLinkDef.h)
Set(LIBRARY_NAME FD)
Set(DEPENDENCIES Core Base)

GENERATE_LIBRARY()
