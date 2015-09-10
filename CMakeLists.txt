project(chatClientAPI)
cmake_minimum_required(VERSION 2.8)


if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


add_definitions(-DBUILDING_LIBRARY)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-DENABLE_LOG_DEBUG)
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
                  inc/protocol/IActionFactory.hpp
                  inc/protocol/INotificationParser.hpp
                  inc/protocol/JsonNotificationParser.hpp
                  inc/protocol/JsonActionFactory.hpp
                  inc/protocol/ActionObject/IActionObject.hpp
                  inc/protocol/ActionObject/JsonActionObject.hpp
                  inc/protocol/NotificationObject/RemovedByContactNotification.hpp
                  inc/protocol/NotificationObject/AddContactResponseNotification.hpp
                  inc/protocol/NotificationObject/AddRequestNotification.hpp
                  inc/protocol/NotificationObject/ContactStateChangedNotification.hpp
                  inc/protocol/NotificationObject/LoginResponseNotification.hpp
                  inc/protocol/NotificationObject/ReceiveContactsNotification.hpp
                  inc/protocol/NotificationObject/ReceiveMessageNotification.hpp
                  inc/protocol/NotificationObject/RegisterUpdateNotification.hpp)


SET (SOURCE_FILES src/ChatClient/ChatClient.cpp
                  src/ChatClient/ChatClientImpl.cpp
                  src/WebsocketClient/WebsocketClient.cpp
                  src/WebsocketCallback/websocket_callback.cpp
                  src/protocol/JsonNotificationParser.cpp
                  src/protocol/JsonActionFactory.cpp)

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
