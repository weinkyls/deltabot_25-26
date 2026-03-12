#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "c1lidarrpi" for configuration "Debug"
set_property(TARGET c1lidarrpi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(c1lidarrpi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libc1lidarrpi.a"
  )

list(APPEND _cmake_import_check_targets c1lidarrpi )
list(APPEND _cmake_import_check_files_for_c1lidarrpi "${_IMPORT_PREFIX}/lib/libc1lidarrpi.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
