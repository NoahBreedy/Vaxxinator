cmake_minimum_required(VERSION 3.16)
project(Vaxxinator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/StateMachine.cpp
    src/states/MainMenu.cpp
)

#target_include_directories(${PROJECT_NAME}
#    PRIVATE ${SDL2_INCLUDE_DIRS}
#)

#target_link_libraries(${PROJECT_NAME}
#    PRIVATE ${SDL2_LIBRARIES}
#)

# Copy assets folder next to the executable (needed later for when we include images)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
