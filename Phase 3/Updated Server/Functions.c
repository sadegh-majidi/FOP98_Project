#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include "cJson.c"

///Register: Receive client request for register and return Server Response To Client and state of registering
cJSON* Register(char command[1000])
{
    FILE *CheckExistance;
    cJSON* response = cJSON_CreateObject();
    char username[100] = "";
    char password[100] = "";
    sscanf(command, "%*s %[^,] %[^\n]s", username, password);
    char filePath[150] = "Resources/Users/";
    cJSON *JsonObj = cJSON_CreateObject();
    cJSON *usernameJSON = cJSON_CreateString(username);
    cJSON *passwordJSON = cJSON_CreateString(password);
    cJSON_AddItemToObject(JsonObj,"username", usernameJSON);
    cJSON_AddItemToObject(JsonObj,"password", passwordJSON);
    strcat(filePath, username);
    strcat(filePath, ".user.json");
    filePath[strlen(filePath)] = '\0';
    if((CheckExistance = fopen(filePath, "r")) == NULL)
    {
        fclose(CheckExistance);
        FILE *user;
        if((user = fopen(filePath, "w")) != NULL)
        {
            fprintf(user, cJSON_PrintUnformatted(JsonObj));
            fclose(user);
            cJSON *type = cJSON_CreateString("Successful");
            cJSON *content = cJSON_CreateString("");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("An error occured in opening files.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        fclose(CheckExistance);
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("this username is not available.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///Login: Receive client request for Login and return AuthToken for Successful login or Error for failure in login
cJSON* Login(char command[1000])
{

}

///Incompleted
cJSON* CreateChannel(char command[1000])
{
    FILE *CheckExistance;
    cJSON* response = cJSON_CreateObject();
    char name[100] = "";
    char AuthToken[40] = "";
    sscanf(command, "%*s %s %s", name, AuthToken);
    cJSON* jsonObj = cJSON_CreateObject();
    cJSON* nameJSON = cJSON_CreateString(name);
    cJSON* messagesArray = cJSON_CreateArray();
    cJSON* serverMessage = cJSON_CreateObject();
    cJSON* sender = cJSON_CreateString("server");
    //------------
    cJSON* content = cJSON_CreateString("creator created channelName.")
    //-------------
    cJSON_AddItemToObject(serverMessage, "sender", sender);
    cJSON_AddItemToObject(serverMessage, "content", content);
    cJSON_AddItemToArray(messagesArray, serverMessage);
    cJSON_AddItemToObject(jsonObj,"messages", messagesArray);
    cJSON_AddItemToObject(jsonObj, "name", nameJSON);
    char filePath[150] = "Resources/Channels/";
    username[strlen(name)-1] = '\0';
    strcat(filePath, name);
    strcat(filePath, ".channel.json");
    filePath[strlen(filePath)] = '\0';
    if((CheckExistance = fopen(filePath, "r")) == NULL)
    {
        fclose(CheckExistance);
        FILE *channel;
        if((channel = fopen(filePath, "w")) != NULL)
        {
            fprintf(channel,cJSON_PrintUnformatted(jsonObj));
            fclose(channel);
            cJSON *type = cJSON_CreateString("Successful");
            cJSON *content = cJSON_CreateString("");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("An error occured in opening files.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        fclose(CheckExistance);
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("Channel name is not available.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}
