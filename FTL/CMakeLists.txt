CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

#Specify build types
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "Configurations" FORCE)
#This is the project name
PROJECT(FTL)
#Finding all the folders in a directory

#SET(CMAKE_VERBOSE_MAKEFILE TRUE)
SET(PREPROCESSOR_DEF "")

####################BUILD TYPE####################
#Mutually exclusive only one can be set true
#Expect undefined results if multiple are true

####################BUILD TYPE####################

#######################DO NOT TOUCH##############################
#Get the folder name
GET_FILENAME_COMPONENT(ProjectName ${CMAKE_CURRENT_LIST_DIR} NAME)
#Replacing space with _ from the above directory
STRING(REPLACE " " "_" ProjectName ${ProjectName})

# Getting every file and putting it in the variable SRCS
FILE(GLOB SRCS *.h *.hpp *.c *.cpp)

#######################DO NOT TOUCH##############################


#####################FINDING FILES AND FOLDER NAMES #########################

# Getting every file and putting it in the variable SRCS
FILE(GLOB_RECURSE SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.h *.hpp *.c *.cpp)


FOREACH(items IN ITEMS ${SRCS})
  GET_FILENAME_COMPONENT(filePath "${items}" PATH)
  STRING(REPLACE "/" "\\" pathOf "${filePath}")
  SOURCE_GROUP("${pathOf}" FILES "${items}")
ENDFOREACH(items IN ITEMS ${SRCS})

##########Adding Preprocessor Defines##########
FOREACH(predef ${PREPROCESSOR_DEF})
  ADD_DEFINITIONS(-D"${predef}")
ENDFOREACH(predef)
##########Adding Preprocessor Defines##########

##########Setting Compiler Flags#########
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEFAULT_FLAGS_DEBUG}")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${DEFAULT_FLAGS_RELEASE}")
IF(WIN32)
  IF(MINGW)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_FLAGS_DEBUG}")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${GCC_FLAGS_RELEASE}")
  ELSEIF(MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MSVC_FLAGS_DEBUG}")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_FLAGS_RELEASE}")
  ENDIF(MINGW)
ELSEIF(UNIX)
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${UNIX_FLAGS_DEBUG}")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} ${UNIX_FLAGS_RELEASE}")
ENDIF(WIN32)
##########Setting Compiler Flags#########

#################################DONT TOUCH####################################
############################ExternalDependencies###############################
#Checking Compiler/64-32/Debug-Release
INCLUDE_DIRECTORIES("${EX_DEPEND}/${ProjectName}/include")
IF(EX_PLATFORM EQUAL 64)
  IF(MINGW)
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/mingw/64/De")
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/mingw/64/Re")
  ELSEIF(MSVC)
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/msvc/64/De")
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/msvc/64/Re")
  ENDIF(MINGW)
ELSEIF(EX_PLATFORM EQUAL 32)
  IF(MINGW)
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/mingw/32/De")
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/mingw/32/Re")
  ELSEIF(MSVC)
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/msvc/32/De")
    LINK_DIRECTORIES("${EX_DEPEND}/${ProjectName}/lib/msvc/32/Re")
  ENDIF(MINGW)
ENDIF(EX_PLATFORM EQUAL 64)
############################ExternalDependencies###############################
#################################DONT TOUCH####################################
ADD_EXECUTABLE(${ProjectName} ${SRCS})

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

#INCLUDE(cotire)
SET(EX_DEPEND "${CMAKE_SOURCE_DIR}/../ExternalDependencies")

############OUTPUT FILES LOCATION######################
#Where the files will be located after being being built
#ARCHIVE = STATIC, LIBRARY = Module libs, RUNTIME = EXE
SET(ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib64")
SET(LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib64")
SET(RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin64")

SET(ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib64")
SET(LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib64")
SET(RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin64")
############OUTPUT FILES LOCATION######################

#Checking to see if compiled in 32/64
IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

IF( EX_PLATFORM EQUAL 64 )
  MESSAGE( "Outputting to lib64 and bin64" )
  IF(CMAKE_BUILD_TYPE MATCHES Debug)
  # ---------- Setup output Directories -------------------------
  SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY_DEBUG} CACHE PATH
    "Single Directory for all Libraries")

  # --------- Setup the Executable output Directory -------------
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_DEBUG} CACHE PATH
    "Single Directory for all Executables.")

  # --------- Setup the Executable output Directory -------------
  SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_OUTPUT_DIRECTORY_DEBUG} CACHE PATH
    "Single Directory for all static libraries.")
  ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    # ---------- Setup output Directories -------------------------
  SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY_RELEASE} CACHE PATH
    "Single Directory for all Libraries")

  # --------- Setup the Executable output Directory -------------
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_RELEASE} CACHE PATH
    "Single Directory for all Executables.")

  # --------- Setup the Executable output Directory -------------
  SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_OUTPUT_DIRECTORY_RELEASE} CACHE PATH
    "Single Directory for all static libraries.")
  ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
ELSE( EX_PLATFORM EQUAL 64 )
# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
   ${PROJECT_BINARY_DIR}/lib CACHE PATH
   "Single Directory for all Libraries")

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
   ${PROJECT_BINARY_DIR}/bin CACHE PATH
   "Single Directory for all Executables.")

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
   ${PROJECT_BINARY_DIR}/lib CACHE PATH
   "Single Directory for all static libraries.")
ENDIF( EX_PLATFORM EQUAL 64 )

################FINDING LIBRARY PACKAGES###########################


################FINDING LIBRARY PACKAGES###########################

#######################DEFAULT FLAGS##############################
SET(DEFAULT_FLAGS_DEBUG   "-std=c++14 -Wextra -Wall -pedantic ")
SET(DEFAULT_FLAGS_RELEASE "-std=c++14 -Wextra -Wall -pedantic ")
#######################DEFAULT FLAGS##############################

#######################GCC FLAGS##############################
SET(GCC_FLAGS_DEBUG   "-std=c++14 -Wextra -Wall -pedantic ")
SET(GCC_FLAGS_RELEASE "-std=c++14 -Wextra -Wall -pedantic ")
#######################GCC FLAGS##############################

#######################UNIX FLAGS##############################
SET(UNIX_FLAGS_DEBUG   "-std=c++14 -Wextra -Wall -pedantic ")
SET(UNIX_FLAGS_RELEASE "-std=c++14 -Wextra -Wall -pedantic")
#######################UNIX FLAGS##############################

#######################MSVC FLAGS##############################
SET(MSVC_FLAGS_DEBUG "/W3 /MP")
SET(MSVC_FLAGS_RELEASE "/W3 /MP")
#######################MSVC FLAGS##############################

################GLOBAL INCLUDE DIR###########################
INCLUDE_DIRECTORIES("${EX_DEPEND}/Global_Include")
################GLOBAL INCLUDE DIR###########################

#If we're using CLion
FOREACH(subdir ${SUBDIRS})
	IF(subdir STREQUAL ".idea")
	ELSE()
		ADD_SUBDIRECTORY(${subdir})
	ENDIF(subdir STREQUAL ".idea")
ENDFOREACH()
