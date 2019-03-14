cmake_minimum_required(VERSION 3.1)
project(myapp) # プロジェクト名を決める。 
set(CMAKE_CXX_STANDARD 14) # C++14で書くよCMakeに教えてあげる。 

# add_definitions(-DFOO)

# find_packegeが便利そうなんだけど、ただでは使えないんだな。
# find_package(OpenCV REQUIRED) #OpenCV見つけてきて 
# include_directories(${OpenCV_INCLUDE_DIRS}) #インクルードディレクトリ指定 
# add_executable(Main main.cpp) #吐き出す実行ファイルと使うソースコードの指定 
# target_link_libraries(Main ${OpenCV_LIBRARIES}) #ライブラリとリンク

find_library(CORE_FOUND CoreFoundation)
if (NOT CORE_FOUND)
    message(FATAL_ERROR "CoreFoundation not found")
endif()


include_directories(.)
include_directories(external_lib/Photon-MacOSX)
include_directories(external_lib/Photon-MacOSX/Common-cpp/inc)
#include_directories(AFTER SYSTEM)

# link_directories(/path1/to/lib /path2/to/lib)

link_directories(PHOTONLIB ./external_lib/Photon-MacOSX/Common-cpp/lib/libCommon-cpp_debug_macosx.a ./external_lib/Photon-MacOSX/LoadBalancing-cpp/lib/libLoadBalancing-cpp_debug_macosx.a)

add_executable(myapp main.cpp)
# target_link_libraries(foo bar)
target_link_libraries(myapp ${CORE_FOUND} ${PHOTONLIB})

IF(APPLE)
        ADD_DEFINITIONS(-D__MACOSX_CORE__)
ENDIF(APPLE)