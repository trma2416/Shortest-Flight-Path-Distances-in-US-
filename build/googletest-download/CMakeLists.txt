cmake_minimum_required(VERSION 3.14)

project(googletest-download NONE)

include(ExternalProject)
ExternalProject_Add(googletest
	GIT_REPOSITORY    https://github.com/google/googletest.git
	GIT_TAG           release-1.10.0
	SOURCE_DIR        "/Users/pc/Documents/CSPB/CSPB 2270/Final_project/build/googletest-src"
	BINARY_DIR        "/Users/pc/Documents/CSPB/CSPB 2270/Final_project/build/googletest-build"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND     ""
	INSTALL_COMMAND   ""
	TEST_COMMAND      ""
)
