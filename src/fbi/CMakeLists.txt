set(SUBPROJ_NAME "fbi")
set(SUBPROJ_NAMESPACE "mech")

set(${SUBPROJ_NAME}_MAJOR_VERSION 0)
set(${SUBPROJ_NAME}_MINOR_VERSION 1)
set(${SUBPROJ_NAME}_PATCH_VERSION 0)
set(${SUBPROJ_NAME}_VERSION
    ${${SUBPROJ_NAME}_MAJOR_VERSION}.${${SUBPROJ_NAME}_MINOR_VERSION}.${${SUBPROJ_NAME}_PATCH_VERSION})

# Set build type to library target
if(${SUBPROJ_NAME}_BUILD_SHARED AND ${${SUBPROJ_NAME}_BUILD_SHARED} STREQUAL "ON")
    set(${SUBPROJ_NAME}_TARGET_TYPE "SHARED")
else()
    set(${SUBPROJ_NAME}_TARGET_TYPE "STATIC")
endif()

string(TOLOWER ${${SUBPROJ_NAME}_TARGET_TYPE} ${SUBPROJ_NAME}_TARGET_TYPE_LOWER)

set(
    ${SUBPROJ_NAME}_HEADERS
    "fbi.hh"
    "BigInteger.hh"
    "BigInteger.inl"
    "BigIntegerAlgorithms.hh"
    "BigIntegerUtils.hh"
    "BigIntegerUtils.inl"
    "BigUnsigned.hh"
    "BigUnsigned.inl"
    "BigUnsignedInABase.hh"
    "NumberlikeArray.hh"
    "NumberlikeArray.inl"
    "Exception.hh")

set( 
    ${SUBPROJ_NAME}_SOURCES
    "BigInteger.cc"
    "BigIntegerAlgorithms.cc"
    "BigIntegerUtils.cc"
    "BigUnsigned.cc"
    "BigUnsignedInABase.cc"
    "Exception.cc")

set(
    ${SUBPROJ_NAME}_ALL_SRCS
    ${${SUBPROJ_NAME}_HEADERS}
    ${${SUBPROJ_NAME}_SOURCES})

add_library(
    ${SUBPROJ_NAME} 
    ${${SUBPROJ_NAME}_TARGET_TYPE} 
    ${${SUBPROJ_NAME}_ALL_SRCS})

set_target_properties(
    ${SUBPROJ_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED YES)

set_target_properties(
    ${SUBPROJ_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    OUTPUT_NAME              "${SUBPROJ_NAME}$<$<CONFIG:Debug>:d>")

target_include_directories(
    ${SUBPROJ_NAME}
    PRIVATE   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
              $<INSTALL_INTERFACE:include>)
    
# ############################################################### #
# Installing #################################################### #
# ############################################################### #

# Create export targets
install(
    TARGETS ${SUBPROJ_NAME}
    EXPORT  ${SUBPROJ_NAME}-targets)

# Install headers
install(
    FILES ${${SUBPROJ_NAME}_HEADERS}
    DESTINATION ${${SUBPROJ_NAME}_INSTALL_INCLUDE_PREFIX})

# Set out paths
install(
    TARGETS ${SUBPROJ_NAME}
    RUNTIME DESTINATION  ${${SUBPROJ_NAME}_INSTALL_BIN_PREFIX}
    ARCHIVE DESTINATION  ${${SUBPROJ_NAME}_INSTALL_LIB_PREFIX}
    LIBRARY DESTINATION  ${${SUBPROJ_NAME}_INSTALL_LIB_PREFIX})

set(SUBPROJ_TARGETS_FILE "${SUBPROJ_NAME}-${${SUBPROJ_NAME}_TARGET_TYPE_LOWER}-targets.cmake")

# Create config-targets cmake file
install(
    EXPORT      ${SUBPROJ_NAME}-targets
    FILE        ${SUBPROJ_TARGETS_FILE}
    NAMESPACE   ${SUBPROJ_NAMESPACE}::
    DESTINATION ${${SUBPROJ_NAME}_INSTALL_CMAKE_PREFIX})

# Create config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${SUBPROJ_NAME}-config-version.cmake"
    VERSION ${cmake-test-library_VERSION}
    COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
    "${PROJECT_ROOT_DIR}/cmake/${SUBPROJ_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${SUBPROJ_NAME}-config.cmake"
    INSTALL_DESTINATION ${${SUBPROJ_NAME}_INSTALL_CMAKE_PREFIX})

# Install config files
install(
    FILES
        "${PROJECT_BINARY_DIR}/${SUBPROJ_NAME}-config.cmake"
        "${PROJECT_BINARY_DIR}/${SUBPROJ_NAME}-config-version.cmake"
    DESTINATION ${${SUBPROJ_NAME}_INSTALL_CMAKE_PREFIX})