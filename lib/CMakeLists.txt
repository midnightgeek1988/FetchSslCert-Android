cmake_minimum_required(VERSION 3.4.1)

link_directories(/home/midnight/project/MyApplication/app/src/main/jniLibs)

add_library(open-ssl-handle
             SHARED
             src/main/cpp/native-lib.cpp )

add_library( libssl
             SHARED
             IMPORTED )

add_library( libcrypto
             SHARED
             IMPORTED )

add_library( libdecrepit
             SHARED
             IMPORTED )
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )

set_target_properties( # Specifies the target library.
                       libssl

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libssl.so )

set_target_properties( # Specifies the target library.
                       libcrypto

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libcrypto.so )

set_target_properties( # Specifies the target library.
                       libdecrepit

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libdecrepit.so )

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

target_link_libraries(open-ssl-handle
                       libssl)

target_link_libraries(open-ssl-handle
                       libcrypto)

target_link_libraries(open-ssl-handle
                       libdecrepit)

target_link_libraries( # Specifies the target library.
                       open-ssl-handle

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
