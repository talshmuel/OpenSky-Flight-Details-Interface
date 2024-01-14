cmake_minimum_required(VERSION 3.12)
project(runMe)

set(SOURCES
    airportsIcoa.c
    allFilghtsAircrafts.c
    fullSchedule.c
    updateDB.c
    main.c
    zipFiles.c
)

add_executable(runMe ${SOURCES})

target_link_libraries(runMe zip)
