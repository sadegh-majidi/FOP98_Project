/*
	# =================================================================
	Author:          <Mohammad Sadegh Majidi Yazdi>
	Create date:     <27 December 2019>
	Description:     <Chat Application - Phase 2/3 - Server - Fundamentals of programming 98 Project>
	# =================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cJSON.h>
#include <winsock2.h>
#include "cJSON.c"


#define MAX 1000
#define PORT 12345
#define SA struct sockaddr

typedef struct LoginUsers
{
    char username[100];
    char AuthToken[40];
    char currentChannel[100];
    int numberOfReadedMessages;
    struct LoginUsers* next;
} User;

typedef struct CreatedChannels
{
    char name[100];
    User *members;
    int numberOfMembers;
    struct CreatedChannels *next;
} Channel;

int numberOfUsers = 0;
int numberOfChannels = 0;
User *userlist = NULL;
Channel *channelList = NULL;

void ContactWithServer(char*);

//------------------------------------------------------------
cJSON* Register(char command[1000]);
cJSON* Login(char command[1000]);
cJSON* CreateChannel(char command[1000]);
cJSON* JoinChannel(char command[1000]);
cJSON* Logout(char command[1000]);
cJSON* SendMes(char command[1000]);
cJSON* LeaveChannel(char command[1000]);
cJSON* Refresh(char command[1000]);
cJSON* ChannelMembers(char command[1000]);
cJSON* SearchUser(char command[1000]);
cJSON* SearchMessage(char command[1000]);
//------------------------------------------------------------

int main()
{
    // Creating necessary directories and files for server and database (configuration and readme and timeout)
    struct stat st = {0};
    if(stat("Resources", &st) == -1)
        system("mkdir Resources");
    if(stat("Resources/Users", &st) == -1)
        system("cd Resources & mkdir Users");
    if(stat("Resources/Channels", &st) == -1)
        system("cd Resources & mkdir Channels");
    //Creating connection
    int server_socket, client_socket;
    struct sockaddr_in server, client;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }

    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify


    char buffer[MAX];
    char MesRespond[1000000] = "";
    int n;
    while (true)
    {
        int len = sizeof(client);
        client_socket = accept(server_socket, (SA *)&client, &len);
        if (client_socket < 0)
        {
            printf("Server accceptance failed...\n");
            exit(0);
        }
        memset(buffer, 0, sizeof(buffer));

        // Read the message from client and copy it to buffer
        recv(client_socket, buffer, sizeof(buffer), 0);

        //
        User *userList;
        char commandKey[20] = "";
        sscanf(buffer, "%s %*s", commandKey);
        if(strcmp(commandKey, "register") == 0)
        {
            cJSON *response = Register(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "login") == 0)
        {
            cJSON *response = Login(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "create") == 0)
        {
            cJSON *response = CreateChannel(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "join") == 0)
        {
            cJSON *response = JoinChannel(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "logout") == 0)
        {
            cJSON *response = Logout(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "send") == 0)
        {
            cJSON *response = SendMes(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "refresh") == 0)
        {
            cJSON *response = Refresh(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "channel") == 0)
        {
            cJSON *response = ChannelMembers(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "leave") == 0)
        {
            cJSON *response = LeaveChannel(buffer);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
        else if(strcmp(commandKey, "search") == 0)
        {
            char commandSubKey[20] = "";
            sscanf(buffer, "%*s %s %*s", commandSubKey);
            if(strcmp(commandSubKey, "user") == 0)
            {
                cJSON *response = SearchUser(buffer);
                strcpy(MesRespond ,cJSON_PrintUnformatted(response));
                send(client_socket, MesRespond, sizeof(MesRespond), 0);
            }
            else if(strcmp(commandSubKey, "message") == 0)
            {
                cJSON *response = SearchMessage(buffer);
                strcpy(MesRespond ,cJSON_PrintUnformatted(response));
                send(client_socket, MesRespond, sizeof(MesRespond), 0);
            }
            else
            {
                cJSON *response = cJSON_CreateObject();
                cJSON *type = cJSON_CreateString("Error");
                cJSON *content = cJSON_CreateString("Invalid statement.");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", content);
                strcpy(MesRespond ,cJSON_PrintUnformatted(response));
                send(client_socket, MesRespond, sizeof(MesRespond), 0);
            }
        }
        else
        {
            cJSON *response = cJSON_CreateObject();
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("Invalid statement.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            strcpy(MesRespond ,cJSON_PrintUnformatted(response));
            send(client_socket, MesRespond, sizeof(MesRespond), 0);
        }
    }

    // Close the socket
    closesocket(server_socket);
    //
    return 0;
}

//---------------------------------------------------------------------------
///Register: Receive client request for register and return Server Response To Client and state of registering
cJSON* Register(char command[1000])
{
    FILE *CheckExistance;
    cJSON* response = cJSON_CreateObject();
    char username[100] = "";
    char password[100] = "";
    sscanf(command, "%*s %[^,], %[^\n]", username, password);
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
    FILE *CheckExistance;
    cJSON* response = cJSON_CreateObject();
    char username[100] = "";
    char password[100] = "";
    sscanf(command, "%*s %[^,], %[^\n]", username, password);
    char filePath[150] = "Resources/Users/";
    strcat(filePath, username);
    strcat(filePath, ".user.json");
    filePath[strlen(filePath)] = '\0';
    if((CheckExistance = fopen(filePath, "r")) != NULL)
    {
        char userSavedData[250] = "";
        int counter = 0;
        char temp;
        while(!feof(CheckExistance))
        {
            temp = fgetc(CheckExistance);
            userSavedData[counter] = temp;
            counter++;
        }
        userSavedData[counter] = '\0';
        cJSON *userDataJSON = cJSON_Parse(userSavedData);
        cJSON *realUsername = cJSON_GetObjectItemCaseSensitive(userDataJSON, "username");
        cJSON *realPassword = cJSON_GetObjectItemCaseSensitive(userDataJSON, "password");
        if(strcmp(realPassword->valuestring, password) == 0 && strcmp(realUsername->valuestring, username) == 0)
        {
            char AuthToken[33];
            srand(time(NULL));
            for(int i = 0; i < 32; i++)
                AuthToken[i] = (char)((rand()%(126 - 33)) + 33);
            AuthToken[32] = '\0';

            if(numberOfUsers == 0)
            {
                userlist = (User*)malloc(sizeof(User));
                strcpy(userlist->username, username);
                strcpy(userlist->AuthToken, AuthToken);
                strcpy(userlist->currentChannel, "");
                userlist->numberOfReadedMessages = 0;
                userlist->next = NULL;
                numberOfUsers++;
                fclose(CheckExistance);
                cJSON *type = cJSON_CreateString("AuthToken");
                cJSON *content = cJSON_CreateString(AuthToken);
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", content);
                return response;
            }
            else
            {
                User *current = userlist;
                int flag = 1;
                while(current != NULL)
                {
                    if(strcmp(current->username, username) == 0)
                    {
                        flag = 0;
                        break;
                    }
                    current = current->next;
                }
                if(flag == 1)
                {
                    current = userlist;
                    while(current->next != NULL)
                    {
                        current = current->next;
                    }
                    current->next = (User*)malloc(sizeof(User));
                    strcpy(current->next->username, username);
                    strcpy(current->next->AuthToken, AuthToken);
                    strcpy(current->next->currentChannel, "");
                    current->next->numberOfReadedMessages = 0;
                    current->next->next = NULL;
                    numberOfUsers++;
                    fclose(CheckExistance);
                    cJSON *type = cJSON_CreateString("AuthToken");
                    cJSON *content = cJSON_CreateString(AuthToken);
                    cJSON_AddItemToObject(response, "type", type);
                    cJSON_AddItemToObject(response, "content", content);
                    return response;
                }
                else
                {
                    fclose(CheckExistance);
                    cJSON *type = cJSON_CreateString("Error");
                    cJSON *content = cJSON_CreateString("You are already logged in.");
                    cJSON_AddItemToObject(response, "type", type);
                    cJSON_AddItemToObject(response, "content", content);
                    return response;
                }
            }
        }
        else
        {
            fclose(CheckExistance);
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("Wrong password.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        fclose(CheckExistance);
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("Username is not valid.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///CreateChannel: Receive client request for creating a channel and return Server Response To Client and state of creating
cJSON* CreateChannel(char command[1000])
{
    FILE *CheckExistance;
    cJSON* response = cJSON_CreateObject();
    char name[100] = "";
    char AuthToken[40] = "";
    sscanf(command, "%*s %*s %[^,], %[^\n]", name, AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") == 0)
        {
            char filePath[150] = "Resources/Channels/";
            strcat(filePath, name);
            strcat(filePath, ".channel.json");
            filePath[strlen(filePath)] = '\0';
            if((CheckExistance = fopen(filePath, "r")) == NULL)
            {
                fclose(CheckExistance);
                FILE *channel;
                if((channel = fopen(filePath, "w")) != NULL)
                {
                    char strContent[250] = "";
                    strcat(strContent, current->username);
                    strcat(strContent, " created ");
                    strcat(strContent, name);
                    strcat(strContent, ".");
                    strContent[strlen(strContent)] = '\0';
                    cJSON* jsonObj = cJSON_CreateObject();
                    cJSON* nameJSON = cJSON_CreateString(name);
                    cJSON* messagesArray = cJSON_CreateArray();
                    cJSON* serverMessage = cJSON_CreateObject();
                    cJSON* sender = cJSON_CreateString("server");
                    cJSON* contentMes = cJSON_CreateString(strContent);
                    cJSON_AddItemToObject(serverMessage, "sender", sender);
                    cJSON_AddItemToObject(serverMessage, "content", contentMes);
                    cJSON_AddItemToArray(messagesArray, serverMessage);
                    cJSON_AddItemToObject(jsonObj,"messages", messagesArray);
                    cJSON_AddItemToObject(jsonObj, "name", nameJSON);
                    fprintf(channel,cJSON_PrintUnformatted(jsonObj));
                    fclose(channel);
                    cJSON *type = cJSON_CreateString("Successful");
                    cJSON *content = cJSON_CreateString("");
                    cJSON_AddItemToObject(response, "type", type);
                    cJSON_AddItemToObject(response, "content", content);
                    strcpy(current->currentChannel, name);
                    if(numberOfChannels == 0)
                    {
                        channelList = (Channel*)malloc(sizeof(Channel));
                        strcpy(channelList->name, name);
                        channelList->members = (User *)malloc(sizeof(User));
                        strcpy(channelList->members->username, current->username);
                        strcpy(channelList->members->AuthToken, current->AuthToken);
                        strcpy(channelList->members->currentChannel, name);
                        channelList->members->numberOfReadedMessages = 0;
                        channelList->members->next = NULL;
                        channelList->numberOfMembers = 0;
                        channelList->numberOfMembers++;
                        channelList->next = NULL;
                        numberOfChannels++;
                    }
                    else
                    {
                        Channel *currentList = channelList;
                        while(currentList->next != NULL)
                        {
                            currentList = currentList->next;
                        }
                        currentList->next = (Channel*)malloc(sizeof(Channel));
                        strcpy(currentList->next->name, name);
                        currentList->next->members = (User *)malloc(sizeof(User));
                        strcpy(currentList->next->members->username, current->username);
                        strcpy(currentList->next->members->AuthToken, current->AuthToken);
                        strcpy(currentList->next->members->currentChannel, name);
                        currentList->next->members->numberOfReadedMessages = 0;
                        currentList->next->members->next = NULL;
                        currentList->next->numberOfMembers = 0;
                        currentList->next->numberOfMembers++;
                        currentList->next->next = NULL;
                        numberOfChannels++;
                    }
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
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You are in another channel.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User with this Token is not logged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///JoinChannel: Receive client request for join in a channel and return Server Response To Client and state of join
cJSON* JoinChannel(char command[1000])
{
    FILE *CheckExistance;
    cJSON* response = cJSON_CreateObject();
    char name[100] = "";
    char AuthToken[40] = "";
    sscanf(command, "%*s %*s %[^,], %[^\n]", name, AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") == 0)
        {
            char filePath[150] = "Resources/Channels/";
            strcat(filePath, name);
            strcat(filePath, ".channel.json");
            filePath[strlen(filePath)] = '\0';
            if((CheckExistance = fopen(filePath, "r")) != NULL)
            {
                char channelSavedData[1000000] = "";
                int counter = 0;
                char tempChar;
                while(!feof(CheckExistance))
                {
                    tempChar = fgetc(CheckExistance);
                    channelSavedData[counter] = tempChar;
                    counter++;
                }
                channelSavedData[counter] = '\0';
                cJSON *channelDataJSON = cJSON_Parse(channelSavedData);
                cJSON *messagesArray = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "messages");
                cJSON *channelNameJSON = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "name");
                fclose(CheckExistance);
                char serverMessage[150] = "";
                strcat(serverMessage, current->username);
                strcat(serverMessage, " joined.");
                serverMessage[strlen(serverMessage)] = '\0';
                cJSON *serverMesJSON = cJSON_CreateString(serverMessage);
                cJSON *sender = cJSON_CreateString("server");
                cJSON *jsonObj = cJSON_CreateObject();
                cJSON_AddItemToObject(jsonObj, "sender", sender);
                cJSON_AddItemToObject(jsonObj, "content", serverMesJSON);
                cJSON *tempArray = cJSON_CreateArray();
                cJSON *tempObj;
                cJSON *tempObj2;
                char helpString[1000] = "";
                char helpString2[1000] = "";
                cJSON_ArrayForEach(tempObj, messagesArray)
                {
                    tempObj2 = cJSON_CreateObject();
                    strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "sender")));
                    helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "sender")))] = '\0';
                    strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                    helpString[strlen(helpString2)-2] = '\0';
                    cJSON_AddItemToObject(tempObj2, "sender", cJSON_CreateString(helpString));
                    strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "content")));
                    helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "content")))] = '\0';
                    strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                    helpString[strlen(helpString2)-2] = '\0';
                    cJSON_AddItemToObject(tempObj2,"content", cJSON_CreateString(helpString));
                    cJSON_AddItemToArray(tempArray,tempObj2);
                }
                cJSON_AddItemToArray(tempArray, jsonObj);
                cJSON* writeFileJson = cJSON_CreateObject();
                cJSON_AddItemToObject(writeFileJson, "messages", tempArray);
                cJSON_AddItemToObject(writeFileJson, "name", channelNameJSON);
                FILE *channel;
                if((channel = fopen(filePath, "w")) != NULL)
                {
                    fprintf(channel, cJSON_PrintUnformatted(writeFileJson));
                    fclose(channel);
                    cJSON *type = cJSON_CreateString("Successful");
                    cJSON *content = cJSON_CreateString("");
                    cJSON_AddItemToObject(response, "type", type);
                    cJSON_AddItemToObject(response, "content", content);
                    strcpy(current->currentChannel, name);
                    current->currentChannel[strlen(name)] = '\0';
                    int channelFlag = 0;
                    Channel *targetChannel = channelList;
                    while(targetChannel != NULL)
                    {
                        if(strcmp(targetChannel->name, name) == 0)
                        {
                            channelFlag = 1;
                            break;
                        }
                        targetChannel = targetChannel->next;
                    }
                    if(channelFlag == 1)
                    {
                        if (targetChannel->numberOfMembers < 1)
                        {
                            targetChannel->members = (User*)malloc(sizeof(User));
                            strcpy(targetChannel->members->username, current->username);
                            strcpy(targetChannel->members->AuthToken, current->AuthToken);
                            strcpy(targetChannel->members->currentChannel, name);
                            targetChannel->members->numberOfReadedMessages = 0;
                            targetChannel->members->next = NULL;
                        }
                        else
                        {
                            User *temp = targetChannel->members;
                            while(temp->next != NULL)
                            {
                                temp = temp->next;
                            }
                            temp->next = (User*)malloc(sizeof(User));
                            strcpy(temp->next->username, current->username);
                            strcpy(temp->next->AuthToken, current->AuthToken);
                            strcpy(temp->next->currentChannel, name);
                            temp->next->numberOfReadedMessages = 0;
                            temp->next->next = NULL;
                        }
                        targetChannel->numberOfMembers++;
                        return response;
                    }
                    else
                    {
                        if(numberOfChannels > 0)
                        {
                            Channel *helpChannel = channelList;
                            while(helpChannel->next != NULL)
                            {
                                helpChannel = helpChannel->next;
                            }
                            helpChannel->next = (Channel*)malloc(sizeof(Channel));
                            strcpy(helpChannel->next->name, name);
                            helpChannel->next->numberOfMembers = 1;
                            helpChannel->next->members = (User*)malloc(sizeof(User));
                            strcpy(helpChannel->next->members->username, current->username);
                            strcpy(helpChannel->next->members->AuthToken, current->AuthToken);
                            strcpy(helpChannel->next->members->currentChannel, name);
                            helpChannel->next->members->numberOfReadedMessages = 0;
                            helpChannel->next->members->next = NULL;
                            helpChannel->next->next = NULL;
                            numberOfChannels++;
                        }
                        else
                        {
                            channelList = (Channel*)malloc(sizeof(Channel));
                            strcpy(channelList->name, name);
                            channelList->members = (User *)malloc(sizeof(User));
                            strcpy(channelList->members->username, current->username);
                            strcpy(channelList->members->AuthToken, current->AuthToken);
                            strcpy(channelList->members->currentChannel, name);
                            channelList->members->numberOfReadedMessages = 0;
                            channelList->members->next = NULL;
                            channelList->numberOfMembers = 0;
                            channelList->numberOfMembers++;
                            channelList->next = NULL;
                            numberOfChannels++;
                        }
                        return response;
                    }

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
                cJSON *content = cJSON_CreateString("Channel not found.");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", content);
                return response;
            }
        }
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You are in another channel.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User with this Token is not logged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///Logout: Receive client request for logout from his/her account and return Server Response To Client and state of logout
cJSON* Logout(char command[1000])
{
    cJSON* response = cJSON_CreateObject();
    char AuthToken[40] = "";
    sscanf(command, "%*s %[^\n]", AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        int counter = 1;
        current = userlist;
        User *tempUser = userlist;
        while(strcmp(tempUser->AuthToken, AuthToken) != 0)
        {
            tempUser = tempUser->next;
            counter++;
        }
        if(counter == 1)
        {
            if(numberOfUsers == 1)
            {
                strcpy(userlist->username, "");
                strcpy(userlist->AuthToken, "");
                strcpy(userlist->currentChannel, "");
                free(userlist);
                numberOfUsers--;
            }
            else
            {
                User *nextNode = userlist->next;
                strcpy(userlist->username, "");
                strcpy(userlist->AuthToken, "");
                strcpy(userlist->currentChannel, "");
                free(userlist);
                userlist = nextNode;
                numberOfUsers--;
            }
        }
        else if(counter == numberOfUsers && numberOfUsers > 1)
        {
            tempUser = userlist;
            while(tempUser->next->next != NULL)
                tempUser = tempUser->next;
            strcpy(tempUser->next->username, "");
            strcpy(tempUser->next->AuthToken, "");
            strcpy(tempUser->next->currentChannel, "");
            free(tempUser->next);
            tempUser->next = NULL;
            numberOfUsers--;
        }
        else
        {
            for (int i = 1; i < counter; i++)
                current = current->next;
            tempUser = current->next;
            current->next = tempUser->next;
            strcpy(tempUser->username, "");
            strcpy(tempUser->AuthToken, "");
            strcpy(tempUser->currentChannel, "");
            free(tempUser);
            numberOfUsers--;
        }
        cJSON *type = cJSON_CreateString("Successful");
        cJSON *content = cJSON_CreateString("");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("Authentication failed!");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///SendMes: Receive client request for sending message in a channel and return Server Response To Client and state of sending
cJSON* SendMes(char command[1000])
{
    cJSON* response = cJSON_CreateObject();
    char AuthToken[40] = "";
    char message[500] = "";
    sscanf(command, "%*s %[^,], %[^\n]",message ,AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") != 0)
        {
            char filePath[150] = "Resources/Channels/";
            strcat(filePath, current->currentChannel);
            strcat(filePath, ".channel.json");
            filePath[strlen(filePath)] = '\0';
            FILE *readChannelData = fopen(filePath, "r");
            char channelSavedData[1000000] = "";
            int counter = 0;
            char tempChar;
            while(!feof(readChannelData))
            {
                tempChar = fgetc(readChannelData);
                channelSavedData[counter] = tempChar;
                counter++;
            }
            channelSavedData[counter] = '\0';
            fclose(readChannelData);
            cJSON *channelDataJSON = cJSON_Parse(channelSavedData);
            cJSON *messagesArray = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "messages");
            cJSON *channelNameJSON = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "name");
            cJSON *messToAdd = cJSON_CreateObject();
            cJSON *sender = cJSON_CreateString(current->username);
            cJSON *clientMes = cJSON_CreateString(message);
            cJSON_AddItemToObject(messToAdd, "sender", sender);
            cJSON_AddItemToObject(messToAdd, "content", clientMes);
            cJSON *tempArray = cJSON_CreateArray();
            cJSON *tempObj;
            cJSON *tempObj2;
            char helpString[1000] = "";
            char helpString2[1000] = "";
            cJSON_ArrayForEach(tempObj, messagesArray)
            {
                tempObj2 = cJSON_CreateObject();
                strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "sender")));
                helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "sender")))] = '\0';
                strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                helpString[strlen(helpString2)-2] = '\0';
                cJSON_AddItemToObject(tempObj2, "sender", cJSON_CreateString(helpString));
                strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "content")));
                helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "content")))] = '\0';
                strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                helpString[strlen(helpString2)-2] = '\0';
                cJSON_AddItemToObject(tempObj2,"content", cJSON_CreateString(helpString));
                cJSON_AddItemToArray(tempArray,tempObj2);
            }
            cJSON_AddItemToArray(tempArray, messToAdd);
            cJSON* writeFileJson = cJSON_CreateObject();
            cJSON_AddItemToObject(writeFileJson, "messages", tempArray);
            cJSON_AddItemToObject(writeFileJson, "name", channelNameJSON);
            FILE *writeChannelData;
            if((writeChannelData = fopen(filePath, "w")) != NULL)
            {
                fprintf(writeChannelData, cJSON_PrintUnformatted(writeFileJson));
                fclose(writeChannelData);
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
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You are not in any channel now.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User whit this token is not loged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///LeaveChannel: Receive client request for leaving from a channel and return Server Response To Client and state of leaving
cJSON* LeaveChannel(char command[1000])
{
    cJSON* response = cJSON_CreateObject();
    char AuthToken[40] = "";
    sscanf(command, "%*s %[^\n]",AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") != 0)
        {
            char filePath[150] = "Resources/Channels/";
            strcat(filePath, current->currentChannel);
            strcat(filePath, ".channel.json");
            filePath[strlen(filePath)] = '\0';
            FILE *readChannelData = fopen(filePath, "r");
            char channelSavedData[1000000] = "";
            int counter = 0;
            char tempChar;
            while(!feof(readChannelData))
            {
                tempChar = fgetc(readChannelData);
                channelSavedData[counter] = tempChar;
                counter++;
            }
            channelSavedData[counter] = '\0';
            fclose(readChannelData);
            char message[150] = "";
            strcat(message, current->username);
            strcat(message, " leaved the channel.");
            message[strlen(message)] = '\0';
            cJSON *channelDataJSON = cJSON_Parse(channelSavedData);
            cJSON *messagesArray = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "messages");
            cJSON *channelNameJSON = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "name");
            cJSON *messToAdd = cJSON_CreateObject();
            cJSON *sender = cJSON_CreateString("server");
            cJSON *serverMes = cJSON_CreateString(message);
            cJSON_AddItemToObject(messToAdd, "sender", sender);
            cJSON_AddItemToObject(messToAdd, "content", serverMes);
            cJSON *tempArray = cJSON_CreateArray();
            cJSON *tempObj;
            cJSON *tempObj2;
            char helpString[1000] = "";
            char helpString2[1000] = "";
            cJSON_ArrayForEach(tempObj, messagesArray)
            {
                tempObj2 = cJSON_CreateObject();
                strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "sender")));
                helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "sender")))] = '\0';
                strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                helpString[strlen(helpString2)-2] = '\0';
                cJSON_AddItemToObject(tempObj2, "sender", cJSON_CreateString(helpString));
                strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "content")));
                helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempObj, "content")))] = '\0';
                strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                helpString[strlen(helpString2)-2] = '\0';
                cJSON_AddItemToObject(tempObj2,"content", cJSON_CreateString(helpString));
                cJSON_AddItemToArray(tempArray,tempObj2);
            }
            cJSON_AddItemToArray(tempArray, messToAdd);
            cJSON *writeFileJSON = cJSON_CreateObject();
            cJSON_AddItemToObject(writeFileJSON,"messages", tempArray);
            cJSON_AddItemToObject(writeFileJSON, "name", channelNameJSON);
            FILE *writeChannelData;
            if((writeChannelData = fopen(filePath, "w")) != NULL)
            {
                fprintf(writeChannelData, cJSON_PrintUnformatted(writeFileJSON));
                fclose(writeChannelData);

                Channel *targetChannel = channelList;
                while(strcmp(targetChannel->name, current->currentChannel) != 0)
                    targetChannel = targetChannel->next;
                User *channelMembers = targetChannel->members;
                counter = 1;
                while(strcmp(channelMembers->AuthToken, current->AuthToken) != 0)
                {
                    counter++;
                    channelMembers = channelMembers->next;
                }
                if(counter == 1)
                {
                    if(targetChannel->numberOfMembers == 1)
                    {
                        strcpy(targetChannel->members->username, "");
                        strcpy(targetChannel->members->AuthToken, "");
                        strcpy(targetChannel->members->currentChannel, "");
                        free(targetChannel->members);
                        targetChannel->numberOfMembers--;
                    }
                    else
                    {
                        User *nextNode = targetChannel->members->next;
                        strcpy(targetChannel->members->username, "");
                        strcpy(targetChannel->members->AuthToken, "");
                        strcpy(targetChannel->members->currentChannel, "");
                        free(targetChannel->members);
                        targetChannel->members = nextNode;
                        targetChannel->numberOfMembers--;
                    }
                }
                else if(counter == channelMembers && targetChannel->members > 1)
                {
                    User *tempUser = targetChannel->members;
                    while (tempUser->next->next != NULL)
                        tempUser = tempUser->next;
                    strcpy(tempUser->next->username, "");
                    strcpy(tempUser->next->AuthToken, "");
                    strcpy(tempUser->next->currentChannel, "");
                    free(tempUser->next);
                    tempUser->next = NULL;
                    targetChannel->numberOfMembers--;
                }
                else
                {
                    User *targetUser = targetChannel->members;
                    User *tempUser;
                    for(int i = 1; i < counter; i++)
                        targetUser = targetUser->next;
                    tempUser = targetUser->next;
                    targetUser->next = tempUser->next;
                    strcpy(tempUser->username, "");
                    strcpy(tempUser->AuthToken, "");
                    strcpy(tempUser->currentChannel, "");
                    free(tempUser);
                    targetChannel->numberOfMembers--;
                }
                strcpy(current->currentChannel, "");
                current->numberOfReadedMessages = 0;
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
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You can not leave channel.You are not in any channel now.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User whit this token is not loged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///Refresh: Receive client request for showing unread messages and return Server Response To Client and state of showing messages
cJSON* Refresh(char command[1000])
{
    cJSON* response = cJSON_CreateObject();
    char AuthToken[40] = "";
    sscanf(command, "%*s %[^\n]",AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") != 0)
        {
            Channel *targetChannel = channelList;
            int Existflag = 0;
            while(targetChannel != NULL)
            {
                if(strcmp(targetChannel->name, current->currentChannel) == 0)
                {
                    Existflag = 1;
                    break;
                }
                targetChannel = targetChannel->next;
            }
            if (Existflag)
            {
                char filePath[150] = "Resources/Channels/";
                strcat(filePath, current->currentChannel);
                strcat(filePath, ".channel.json");
                filePath[strlen(filePath)] = '\0';
                FILE *readChannelData = fopen(filePath, "r");
                char channelSavedData[1000000] = "";
                int counter = 0;
                char tempChar;
                while(!feof(readChannelData))
                {
                    tempChar = fgetc(readChannelData);
                    channelSavedData[counter] = tempChar;
                    counter++;
                }
                channelSavedData[counter] = '\0';
                fclose(readChannelData);
                cJSON *channelDataJSON = cJSON_Parse(channelSavedData);
                cJSON *messagesArray = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "messages");
                cJSON *channelNameJSON = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "name");
                cJSON *unreadMessages = cJSON_CreateArray();
                cJSON *tempMessage = NULL;
                int Mescounter = 1;
                int numberOfReadedMessages = current->numberOfReadedMessages;
                cJSON *tempObj2;
                char helpString[1000] = "";
                char helpString2[1000] = "";
                cJSON_ArrayForEach(tempMessage, messagesArray)
                {
                    if(Mescounter > numberOfReadedMessages)
                    {
                        tempObj2 = cJSON_CreateObject();
                        strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "sender")));
                        helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "sender")))] = '\0';
                        strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                        helpString[strlen(helpString2)-2] = '\0';
                        cJSON_AddItemToObject(tempObj2, "sender", cJSON_CreateString(helpString));
                        strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "content")));
                        helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "content")))] = '\0';
                        strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                        helpString[strlen(helpString2)-2] = '\0';
                        cJSON_AddItemToObject(tempObj2,"content", cJSON_CreateString(helpString));
                        cJSON_AddItemToArray(unreadMessages, tempObj2);
                        current->numberOfReadedMessages++;
                    }
                    Mescounter++;
                }
                cJSON *type = cJSON_CreateString("List");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", unreadMessages);
                return response;
            }
            else
            {
                cJSON *type = cJSON_CreateString("Error");
                cJSON *content = cJSON_CreateString("There is no channel with this informations.");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", content);
                return response;
            }
        }
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You are not in any channel now.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User whit this token is not loged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

///ChannelMembers: Receive client request for showing channel members and return Server Response To Client and state of showing members
cJSON* ChannelMembers(char command[1000])
{
    cJSON* response = cJSON_CreateObject();
    char AuthToken[40] = "";
    sscanf(command, "%*s %*s %[^\n]",AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") != 0)
        {
            Channel *targetChannel = channelList;
            int Existflag = 0;
            while(targetChannel != NULL)
            {
                if(strcmp(targetChannel->name, current->currentChannel) == 0)
                {
                    Existflag = 1;
                    break;
                }
                targetChannel = targetChannel->next;
            }
            if (Existflag)
            {
                cJSON *memberArray = cJSON_CreateArray();
                cJSON *temp;
                User *tempUser = targetChannel->members;
                for(int i = 0; i < targetChannel->numberOfMembers; i++)
                {
                    temp = cJSON_CreateString(tempUser->username);
                    cJSON_AddItemToArray(memberArray, temp);
                    tempUser = tempUser->next;
                }
                cJSON *type = cJSON_CreateString("List");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", memberArray);
                return response;
            }
            else
            {
                cJSON *type = cJSON_CreateString("Error");
                cJSON *content = cJSON_CreateString("There is no channel with this informations.");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", content);
                return response;
            }
        }
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You are not in any channel now.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User whit this token is not loged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

cJSON* SearchUser(char command[1000])
{
    cJSON* response = cJSON_CreateObject();
    char AuthToken[40] = "";
    char ExpectedUser[40] = "";
    sscanf(command, "%*s %*s %[^,], %[^\n]",ExpectedUser ,AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") != 0)
        {
            Channel *targetChannel = channelList;
            int Existflag = 0;
            while(targetChannel != NULL)
            {
                if(strcmp(targetChannel->name, current->currentChannel) == 0)
                {
                    Existflag = 1;
                    break;
                }
                targetChannel = targetChannel->next;
            }
            if (Existflag)
            {
                User *tempUser = targetChannel->members;
                int findFlag = 0;
                for(int i = 0; i < targetChannel->numberOfMembers; i++)
                {
                    if(strcmp(tempUser->username, ExpectedUser) == 0)
                    {
                        findFlag = 1;
                        break;
                    }
                    tempUser = tempUser->next;
                }
                if(findFlag == 1)
                {
                    cJSON *content = cJSON_CreateString("user found.");
                    cJSON_AddItemToObject(response, "content", content);
                }
                else
                {
                    cJSON *content = cJSON_CreateString("user not found.");
                    cJSON_AddItemToObject(response, "content", content);
                }
                return response;
            }
            else
            {
                cJSON *type = cJSON_CreateString("Error");
                cJSON *content = cJSON_CreateString("There is no channel with this informations.");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", content);
                return response;
            }
        }
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You are not in any channel now.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User whit this token is not loged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}

cJSON* SearchMessage(char command[1000])
{
    cJSON* response = cJSON_CreateObject();
    char AuthToken[40] = "";
    char ExpectedMessage[40] = "";
    sscanf(command, "%*s %*s %[^,], %[^\n]",ExpectedMessage ,AuthToken);
    User *current = userlist;
    int flag = 0;
    while(current != NULL)
    {
        if(strcmp(current->AuthToken, AuthToken) == 0)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        if(strcmp(current->currentChannel, "") != 0)
        {
            Channel *targetChannel = channelList;
            int Existflag = 0;
            while(targetChannel != NULL)
            {
                if(strcmp(targetChannel->name, current->currentChannel) == 0)
                {
                    Existflag = 1;
                    break;
                }
                targetChannel = targetChannel->next;
            }
            if (Existflag)
            {
                char filePath[150] = "Resources/Channels/";
                strcat(filePath, current->currentChannel);
                strcat(filePath, ".channel.json");
                filePath[strlen(filePath)] = '\0';
                FILE *readChannelData = fopen(filePath, "r");
                char channelSavedData[1000000] = "";
                int counter = 0;
                char tempChar;
                while(!feof(readChannelData))
                {
                    tempChar = fgetc(readChannelData);
                    channelSavedData[counter] = tempChar;
                    counter++;
                }
                channelSavedData[counter] = '\0';
                fclose(readChannelData);
                cJSON *channelDataJSON = cJSON_Parse(channelSavedData);
                cJSON *messagesArray = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "messages");
                cJSON *channelNameJSON = cJSON_GetObjectItemCaseSensitive(channelDataJSON, "name");
                cJSON *foundMessages = cJSON_CreateArray();
                cJSON *tempMessage = NULL;
                int Mescounter = 1;
                int numberOfReadedMessages = current->numberOfReadedMessages;
                cJSON *tempObj2;
                char helpString[1000] = "";
                char helpString2[1000] = "";
                cJSON_ArrayForEach(tempMessage, messagesArray)
                {
                      char str1[500] = "";
                      char str2[500] = "";
                      int foundFlag = 0;
                      strcpy(str2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "content")));
                      str2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "content")))] = '\0';
                      strncpy(str1, &str2[1], strlen(str2)-2);
                      char * pch;
                      pch = strtok (str1," ");
                      while (pch != NULL)
                      {
                        if(strcmp(ExpectedMessage, pch) == 0)
                        {
                            foundFlag = 1;
                            break;
                        }
                        pch = strtok (NULL, " ");
                      }
                      if(foundFlag == 1)
                        {
                            tempObj2 = cJSON_CreateObject();
                            strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "sender")));
                            helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "sender")))] = '\0';
                            strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                            helpString[strlen(helpString2)-2] = '\0';
                            cJSON_AddItemToObject(tempObj2, "sender", cJSON_CreateString(helpString));
                            strcpy(helpString2, cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "content")));
                            helpString2[strlen(cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(tempMessage, "content")))] = '\0';
                            strncpy(helpString, &helpString2[1], strlen(helpString2)-2);
                            helpString[strlen(helpString2)-2] = '\0';
                            cJSON_AddItemToObject(tempObj2,"content", cJSON_CreateString(helpString));
                            cJSON_AddItemToArray(foundMessages, tempObj2);
                        }

                }
                cJSON *type = cJSON_CreateString("List");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", foundMessages);
                return response;
            }
            else
            {
                cJSON *type = cJSON_CreateString("Error");
                cJSON *content = cJSON_CreateString("There is no channel with this informations.");
                cJSON_AddItemToObject(response, "type", type);
                cJSON_AddItemToObject(response, "content", content);
                return response;
            }
        }
        else
        {
            cJSON *type = cJSON_CreateString("Error");
            cJSON *content = cJSON_CreateString("You are not in any channel now.");
            cJSON_AddItemToObject(response, "type", type);
            cJSON_AddItemToObject(response, "content", content);
            return response;
        }
    }
    else
    {
        cJSON *type = cJSON_CreateString("Error");
        cJSON *content = cJSON_CreateString("User whit this token is not loged in.");
        cJSON_AddItemToObject(response, "type", type);
        cJSON_AddItemToObject(response, "content", content);
        return response;
    }
}
