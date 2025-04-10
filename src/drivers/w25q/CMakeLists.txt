cmake_minimum_required(VERSION 3.17)

set(SCRS
    ${CMAKE_CURRENT_LIST_DIR}/w25q.cpp
)

add_library(w25q_driver INTERFACE)

target_sources(w25q_driver INTERFACE ${SCRS})
