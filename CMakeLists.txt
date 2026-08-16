cmake_minimum_required(VERSION 3.16)
project(Jobbie LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/models.cpp
    src/storage.cpp
    src/styles.cpp
    src/job_card.cpp
    src/kanban_column.cpp
    src/kanban_board.cpp
    src/job_dialog.cpp
    src/confirm_dialog.cpp
    src/main_window.cpp
    src/native_dialog.cpp
)

set(HEADERS
    src/config.h
    src/models.h
    src/storage.h
    src/styles.h
    src/job_card.h
    src/kanban_column.h
    src/kanban_board.h
    src/job_dialog.h
    src/confirm_dialog.h
    src/main_window.h
    src/native_dialog.h
)

set(RESOURCES
    resources/resources.qrc
)

add_executable(jobbie ${SOURCES} ${HEADERS} ${RESOURCES})

target_link_libraries(jobbie PRIVATE Qt6::Widgets)

# Place the compiled binary directly in the project root directory
set_target_properties(jobbie PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
