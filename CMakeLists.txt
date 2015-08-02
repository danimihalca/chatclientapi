project(chatClientAPI)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-DBUILDING_LIBRARY)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-DLOG_DEBUG)
endif()
find_package (Threads)

IF (WIN32)
        SET(LIBWEBSOCKETS_LIBRARY websockets_shared)
ELSE()
        SET(LIBWEBSOCKETS_LIBRARY websockets)
ENDIF()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})
INCLUDE_DIRECTORIES(${CHAT_UTILS_PATH})

SET (HEADER_FILES inc/chat_client_common.hpp
                  inc/ChatClient/IChatClient.hpp
                  inc/ChatClient/IChatClientListener.hpp
                  inc/ChatClient/ChatClient.hpp
                  inc/ChatClient/ChatClientImpl.hpp
                  inc/WebsocketClient/IWebsocketClient.hpp
                  inc/WebsocketClient/IWebsocketClientListener.hpp
                  inc/WebsocketClient/WebsocketClient.hpp
                  inc/WebsocketCallback/IWebsocketCallbackListener.hpp
                  inc/WebsocketCallback/websocket_callback.hpp
                  inc/JsonProtocol/IClientJsonFactory.hpp
                  inc/JsonProtocol/IClientJsonParser.hpp
                  inc/JsonProtocol/ClientJsonParser.hpp
                  inc/JsonProtocol/ClientJsonFactory.hpp)
SET (SOURCE_FILES src/ChatClient/ChatClient.cpp
                  src/ChatClient/ChatClientImpl.cpp
                  src/WebsocketClient/WebsocketClient.cpp
                  src/WebsocketCallback/websocket_callback.cpp
                  src/JsonProtocol/ClientJsonParser.cpp
                  src/JsonProtocol/ClientJsonFactory.cpp)

LINK_DIRECTORIES( ${LIBRARY_INSTALL_PATH})


SET (LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
               z ${LIBWEBSOCKETS_LIBRARY} jsoncpp)

IF (${CMAKE_SYSTEM_NAME} MATCHES Android)
    SET( LIBRARIES "${LIBRARIES} -llog")
ENDIF()

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBRARIES})


SET (PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/inc/chat_client_common.hpp)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${LIBRARY_INSTALL_PATH})
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/inc/ChatClient DESTINATION ${HEADERS_INSTALL_PATH}
        PATTERN "ChatClientImpl.hpp" EXCLUDE
        PATTERN "JsonFactory" EXCLUDE)
INSTALL(FILES ${PUBLIC_HEADERS} DESTINATION ${HEADERS_INSTALL_PATH})
