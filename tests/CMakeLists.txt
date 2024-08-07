include_directories (${GTEST_INCLUDE_DIRS})
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../include")

set (SRC
    tupletTests.cpp
    arrayTests.cpp
    const_arrayTests.cpp
    binhexTests.cpp ../source/binhex.cpp
    textTests.cpp ../source/text.cpp
    )

if (ENABLE_TrackingAllocator)
    set (SRC ${SRC} ../source/TrackingAllocator.cpp)
endif()

add_executable (n88utilTests ${SRC})

target_link_libraries (n88utilTests
    ${GTEST_BOTH_LIBRARIES})

if (ENABLE_TrackingAllocator)
    target_link_libraries (n88utilTests
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries (n88utilTests pthread)
    if (GLIBC_VERSION)
        if (GLIBC_VERSION VERSION_LESS "2.17")
            target_link_libraries (n88utilTests ${RT_LIBRARY})
        endif ()
    endif()
endif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

add_test (NAME N88UtilTests COMMAND $<TARGET_FILE:n88utilTests>)
