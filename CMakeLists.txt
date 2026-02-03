cmake_minimum_required(VERSION 3.16)
project(Vaxxinator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

include_directories(
   PUBLIC include/
   PUBLIC ${SDL2_INCLUDE_DIRS}
)

# Get the proper libraries to link basesd on platform
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Running on Linux")
    set(OS_LIBRARIES
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Running on macOS")
    set(OS_LIBRARIES
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Running on Windows")
    set(OS_LIBRARIES
        mingw32 
        SDL2main
    )
    
    # This is needed to automatically get the linker library directory on windows 
    get_target_property(SDL2_LIB_FILE SDL2::SDL2 IMPORTED_IMPLIB)
    get_filename_component(SDL2_LIBRARY_DIR "${SDL2_LIB_FILE}" DIRECTORY)
    
    link_directories(${SDL2_LIBRARY_DIR})

endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/StateMachine.cpp
    src/states/MainMenu.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${OS_LIBRARIES}
    SDL2 
)

# Copy assets folder next to the executable (needed later for when we include images)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
