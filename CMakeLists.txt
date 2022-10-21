cmake_minimum_required(VERSION 3.13)
project(colisao_bolas_2D)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

if(WIN32)
    # change this to your SDL path on Windows
    set(SDL2_DIR "C:/SDL2-2.0.18/x86_64-w64-mingw32")
    set(SDL2_INCLUDE_DIR ${SDL2_DIR}/include/SDL2)
    set(SDL2_LIBRARY_DIR ${SDL2_DIR}/lib)
    include_directories(include ${SDL2_INCLUDE_DIR})

    # change this to your SDL_image path on Windows
    set(SDL2_IMAGE_DIR "C:/SDL2_image-2.0.5/x86_64-w64-mingw32")
    set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_IMAGE_DIR}/include/SDL2)
    set(SDL2_IMAGE_LIBRARY_DIR ${SDL2_IMAGE_DIR}/lib)
    include_directories(include ${SDL2_IMAGE_INCLUDE_DIR})
endif()
# if(UNIX)
#     # not sure this is running on Linux
#     find_package(SDL2 REQUIRED)
#     find_package(SDL2_image REQUIRED)
#     include_directories(include ${SDL2_LIBRARY_DIR})
#     include_directories(include ${SDL2_IMAGE_LIBRARY_DIR})
# endif()

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "./")

if(WIN32)
    target_link_libraries(${PROJECT_NAME} -lmingw32 ${SDL2_LIBRARY_DIR}/libSDL2main.a ${SDL2_LIBRARY_DIR}/libSDL2.dll.a ${SDL2_IMAGE_LIBRARY_DIR}/libSDL2_image.dll.a) #don't use -mwindows
    file(COPY ${SDL2_DIR}/bin/SDL2.dll DESTINATION ./)
    file(COPY ${SDL2_IMAGE_DIR}/bin/SDL2_image.dll DESTINATION ./)
    message(${SDL2_IMAGE_DIR}/bin/*.dll)
    file(GLOB_RECURSE DLL_LIBS ${SDL2_IMAGE_DIR}/bin/*.dll)
    file(COPY ${DLL_LIBS} DESTINATION ./)
    file(COPY assets DESTINATION ./)
endif()
# if(UNIX)
#     target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
# endif()