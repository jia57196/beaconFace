cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME_STR mnisten)
project(${PROJECT_NAME_STR} C CXX)

# ----------------------------------------------------------------------------
#   Basic Configuration
# ----------------------------------------------------------------------------
SET(EXTRA_C_FLAGS " ${EXTRA_C_FLAGS} -std=c++11 -Wall -Wpedantic")
SET(EXTRA_C_FLAGS_RELEASE " -O3 ")
SET(EXTRA_C_FLAGS_DEBUG " -g3 ")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS}")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_C_FLAGS_RELEASE}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_EXE_LINKER_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${EXTRA_EXE_LINKER_FLAGS_RELEASE}")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${EXTRA_EXE_LINKER_FLAGS_DEBUG}")

find_package( OpenCV REQUIRED )

#-------------------
# set common include folder for module
#-------------------
set(MNISTEN_INCLUDES ${PROJECT_SOURCE_DIR}/include)

#------------------------------------------------
# TARGETS
#------------------------------------------------
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} ${MNISTEN_INCLUDES})
LINK_LIBRARIES(${REQUIRED_LIBRARIES} )

#------------------------------------------------------
# Boost Lib Names and Dirs
#------------------------------------------------------
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.55.0 COMPONENTS filesystem system program_options regex) 


file(GLOB MNISTN_SRC_FILES        ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(mnisten ${MNISTN_SRC_FILES}) 
target_link_libraries(mnisten ${Boost_LIBRARIES} ${OpenCV_LIBS} )


#mnisten -d ~/00-code/tiny-cnn/data/lfw2 -o lfwa -n 0 -s 112x112