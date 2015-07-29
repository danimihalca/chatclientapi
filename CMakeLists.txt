project(chatClientAPI)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DBUILDING_LIBRARY)

find_package (Threads)

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})

SET (HEADER_FILES inc/chat_client_common.h
                  inc/ChatClient/IChatClient.h
                  inc/ChatClient/IChatClientListener.h
                  inc/ChatClient/ChatClient.h
                  inc/ChatClient/ChatClientImpl.h
                  inc/WebsocketClient/IWebsocketClient.h
                  inc/WebsocketClient/IWebsocketClientListener.h
                  inc/WebsocketClient/WebsocketClient.h
                  inc/WebsocketCallback/IWebsocketCallbackListener.h
                  inc/WebsocketCallback/websocket_callback.h
                  inc/JsonProtocol/JsonFactory.h
                  inc/JsonProtocol/common_protocol.h)
SET (SOURCE_FILES src/ChatClient/ChatClient.cpp
                  src/ChatClient/ChatClientImpl.cpp
                  src/WebsocketClient/WebsocketClient.cpp
                  src/WebsocketCallback/websocket_callback.cpp
                  src/JsonProtocol/JsonFactory.cpp)

LINK_DIRECTORIES( ${LIBRARY_INSTALL_PATH})


SET (LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
               z websockets jsoncpp)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBRARIES})


SET (PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/inc/chat_client_common.h)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${LIBRARY_INSTALL_PATH})
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/inc/ChatClient DESTINATION ${HEADERS_INSTALL_PATH}
        PATTERN "ChatClientImpl.h" EXCLUDE
        PATTERN "inc/ChatClient/common_protocol.h" EXCLUDE
        PATTERN "JsonFactory" EXCLUDE)
INSTALL(FILES ${PUBLIC_HEADERS} DESTINATION ${HEADERS_INSTALL_PATH})
