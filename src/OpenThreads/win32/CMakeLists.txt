# This file should only be included when WIN32

SET(LIB_NAME OpenThreads)
SET(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
IF(NOT OSG_DETERMINE_WIN_VERSION)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0400)
ENDIF(NOT OSG_DETERMINE_WIN_VERSION)
SOURCE_GROUP("Header Files" FILES ${LIB_PUBLIC_HEADERS})
SET_SOURCE_FILES_PROPERTIES(${LIB_PUBLIC_HEADERS} PROPERTIES HEADER_FILE_ONLY ON)

ADD_LIBRARY(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    HandleHolder.h
    Win32BarrierPrivateData.h
    WIN32Condition.cpp
    Win32ConditionPrivateData.h
    Win32Mutex.cpp
    Win32MutexPrivateData.h
    Win32Thread.cpp
    Win32ThreadBarrier.cpp
    Win32ThreadPrivateData.h
    ../common/Version.cpp
    ../common/Atomic.cpp
    ${OPENTHREADS_VERSIONINFO_RC}
)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES FOLDER "OSG Core")
IF(OPENTHREADS_SONAMES)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES VERSION ${OPENTHREADS_VERSION} SOVERSION ${OPENTHREADS_SOVERSION})
ENDIF()

# Since we're building different platforms binaries in 
# their respective directories, we need to set the 
# link directory so it can find this location.
LINK_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
)

IF(MSVC AND OSG_MSVC_VERSIONED_DLL)
    HANDLE_MSVC_DLL(ot ${OPENTHREADS_SOVERSION})
ENDIF()

SET(INSTALL_BINDIR bin)

INSTALL(
    TARGETS OpenThreads
    ARCHIVE DESTINATION lib COMPONENT libopenthreads-dev
    LIBRARY DESTINATION lib COMPONENT libopenthreads
    RUNTIME DESTINATION ${INSTALL_BINDIR} COMPONENT libopenthreads
)
INSTALL(
    FILES ${OpenThreads_PUBLIC_HEADERS}
    DESTINATION include/OpenThreads
    COMPONENT libopenthreads-dev
)

IF(MSVC AND DYNAMIC_OPENSCENEGRAPH)
    INSTALL(FILES $<TARGET_PDB_FILE:${LIB_NAME}> DESTINATION ${INSTALL_BINDIR} COMPONENT libopenthreads)
ENDIF(MSVC AND DYNAMIC_OPENSCENEGRAPH)

#commented out# INCLUDE(ModuleInstall OPTIONAL)
