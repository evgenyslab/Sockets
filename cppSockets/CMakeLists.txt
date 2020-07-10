cmake_minimum_required(VERSION 3.0.0)
project(CPPSockets)
add_compile_options(-std=c++11)

include_directories(
        include
)

add_subdirectory(tests)

#TOOO: create INTERFACE Library for header only lib:
add_library(${PROJECT_NAME} INTERFACE)

# get all source files:
file(GLOB SOURCES include/*.h*)

# get all application sources:
file(GLOB EXAMPLES examples/*)


# create executible for each example application
foreach(example_application ${EXAMPLES})
    # strip .cpp extension:
    get_filename_component(example_application_base ${example_application} NAME)
    string( REPLACE ".cpp" "" example_application_name ${example_application_base})


    add_executable(${example_application_name} ${example_application} ${SOURCES})
    target_link_libraries(${example_application_name} -pthread)

endforeach(example_application ${EXAMPLES})