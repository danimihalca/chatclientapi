project(chatClientAPI)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DBUILDING_LIBRARY)

find_package (Threads)

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})

SET (HEADER_FILES inc/common.h
                  inc/IChatClient.h
                  inc/ChatClient.h
                  inc/ChatClientImpl.h)
SET (SOURCE_FILES src/ChatClient.cpp
                  src/ChatClientImpl.cpp)

LINK_DIRECTORIES( ${LIBRARY_INSTALL_PATH})


SET (LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
               libwebsockets)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBRARIES})


SET (PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/inc/IChatClient.h
                    ${PROJECT_SOURCE_DIR}/inc/ChatClient.h
                    ${PROJECT_SOURCE_DIR}/inc/common.h)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${LIBRARY_INSTALL_PATH})
INSTALL(FILES ${PUBLIC_HEADERS} DESTINATION ${HEADERS_INSTALL_PATH})
