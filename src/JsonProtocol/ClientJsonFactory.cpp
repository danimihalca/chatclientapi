#include "JsonProtocol/ClientJsonFactory.hpp"

#include <JsonChatProtocol/common_json_protocol.hpp>
#include <Model/User.hpp>
#include <Model/Message.hpp>

#include <JsonProtocol/ActionJsonObject/ActionJsonObject.hpp>


IActionJsonObject* ClientJsonFactory::createLoginJson(const UserCredentials& userCredentials, USER_STATE state)
{
    Json::Value root;
    root[ACTION] = ACTION_LOGIN;
    Json::Value content;
    Json::Value userCredentialsJson;
    userCredentialsJson[USERNAME] = userCredentials.getUserName();
    userCredentialsJson[PASSWORD] = userCredentials.getPassword();
    content[USER_CREDENTIALS] = userCredentialsJson;
    content[STATE] = state;
    root[CONTENT] = content;

    return new ActionJsonObject(root);
}


IActionJsonObject* ClientJsonFactory::createRequestContactsJson()
{
    Json::Value root;
    root[ACTION] = ACTION_GET_CONTACTS;
    return new ActionJsonObject(root);

}

IActionJsonObject* ClientJsonFactory::createSendMessageJson(
    const Message& message)
{
    Json::Value root;
    root[ACTION] = ACTION_SEND_MESSAGE;
    Json::Value content;

    Json::Value messageJson;

    messageJson[RECEIVER_ID] = message.getReceiverId();
    messageJson[MESSAGE_TEXT] = message.getMessageText();

    content[MESSAGE] = messageJson;
    root[CONTENT] = content;
    return new ActionJsonObject(root);

}

IActionJsonObject* ClientJsonFactory::createAddContactJson(const std::string& userName)
{
    Json::Value root;
    root[ACTION] = ACTION_ADD_CONTACT;
    Json::Value content;

    content[USERNAME] = userName;
    root[CONTENT] = content;
    return new ActionJsonObject(root);

}

IActionJsonObject* ClientJsonFactory::createAddContactResolutionJson(const std::string& userName, bool accepted)
{
    Json::Value root;
    root[ACTION] = ACTION_ADD_CONTACT_RESOLUTION;
    Json::Value content;

    content[USERNAME] = userName;
    content[ACCEPTED] = accepted;
    root[CONTENT] = content;
    return new ActionJsonObject(root);

}

IActionJsonObject* ClientJsonFactory::createRemoveContactJson(int contactId)
{
    Json::Value root;
    root[ACTION] = ACTION_REMOVE_CONTACT;
    Json::Value content;

    content[ID] = contactId;
    root[CONTENT] = content;
    return new ActionJsonObject(root);

}


IActionJsonObject* ClientJsonFactory::createChangeStateJson(USER_STATE state)
{
    Json::Value root;
    root[ACTION] = ACTION_CHANGE_STATE;
    Json::Value content;

    content[STATE] = state;
    root[CONTENT] = content;
    return new ActionJsonObject(root);

}


IActionJsonObject* ClientJsonFactory::createUpdateUserJson(const User& user)
{
    Json::Value root;
    root[ACTION] = ACTION_UPDATE_USER;
    Json::Value content;
    Json::Value userJson;

    userJson[USERNAME] = user.getUserName();
    userJson[PASSWORD] = user.getPassword();
    userJson[FIRSTNAME] = user.getFirstName();
    userJson[LASTNAME] = user.getLastName();

    content[USER] = userJson;

    root[CONTENT] = content;
    return new ActionJsonObject(root);

}

IActionJsonObject* ClientJsonFactory::createRegisterUserJson(const User& user)
{
    Json::Value root;
    root[ACTION] = ACTION_REGISTER_USER;
    Json::Value content;
    Json::Value userJson;

    userJson[USERNAME] = user.getUserName();
    userJson[PASSWORD] = user.getPassword();
    userJson[FIRSTNAME] = user.getFirstName();
    userJson[LASTNAME] = user.getLastName();

    content[USER] = userJson;

    root[CONTENT] = content;
    return new ActionJsonObject(root);

}
