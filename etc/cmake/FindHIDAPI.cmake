#
# find module for HIDAPI
#
# (c) 2018 Hartmut Seichter
#
# Licensed under BSD-2-Clause
#

include(FindPackageHandleStandardArgs)

set(HIDAPI_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	$ENV{HIDAPI_ROOT}
	)

find_library(HIDAPI_LIBRARY
	NAMES hidapi hidapi-libusb
	PATHS ${HIDAPI_SEARCH_PATHS}
	PATH_SUFFIXES lib64 lib
	)

find_path(HIDAPI_INCLUDE_DIR
	NAMES hidapi.h
	PATHS ${HIDAPI_SEARCH_PATHS}
	PATH_SUFFIXES hidapi
	)

find_package_handle_standard_args(HIDAPI
	DEFAULT_MSG
	HIDAPI_LIBRARY
	HIDAPI_INCLUDE_DIR
	)

if(HIDAPI_FOUND)
	set(HIDAPI_LIBRARIES "${HIDAPI_LIBRARY}")
	set(HIDAPI_INCLUDE_DIRS "${HIDAPI_INCLUDE_DIR}")
endif()

mark_as_advanced(HIDAPI_INCLUDE_DIR HIDAPI_LIBRARY)
