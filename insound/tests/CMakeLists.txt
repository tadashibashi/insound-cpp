project (insound-tests)

file (GLOB_RECURSE INSOUND_TEST_SRC ./*.test.cpp ./main.cpp)
add_executable (${PROJECT_NAME} ${INSOUND_TEST_SRC})

target_link_libraries (${PROJECT_NAME} PRIVATE insound-core Catch2::Catch2)

add_test(${PROJECT_NAME} ${PROJECT_NAME})

set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(STATIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/static)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h)
