/*
	# =================================================================
	Author:          <Mohammad Sadegh Majidi Yazdi>
	Create date:     <27 December 2019>
	Description:     <Chat Application - Phase 1/3 - Fundamentals of programming 98 Project>
	# =================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include "cJSON.c"
#include <string.h>
#include <winsock2.h>
#include <winuser.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr


//functions Declarations
char * ContactWithServer(char *);
void AccountMenu();
void Register();
void Login();
void UserInterface(char*);
void CreateChannel(char*);
void JoinChannel(char*);
void Logout(char*);
void ChatMenu(char*);
void SendMeesage(char*);
void Refresh(char*);
void ChannelMembers(char*);
void LeaveChannel(char*);
void SearchMember(char*);
void SearchMessage(char*);

//global server message
char serverMes[1000] = "\0";


int main()
{
    AccountMenu();
    return 0;
}
//function definitions
char* ContactWithServer(char *clientMes)
{
    int client_socket;
	struct sockaddr_in servaddr;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2,2);
	err = WSAStartup(wVersionRequested, &wsaData);

	 if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        exit(0);
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	if(connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
    send(client_socket,clientMes, strlen(clientMes),0);
    serverMes[0] = '\0';
    int read = recv(client_socket, serverMes, sizeof(serverMes), 0);
    if (read < 0)
        printf("recv failed: %d\n", WSAGetLastError());
	closesocket(client_socket);
	return serverMes;
}

void AccountMenu()
{
    while(true)
    {
        system("cls");
        system("color 02");
        printf(":::::::::: ::::::::  :::::::::        ::::::::  :::::::::      :::     ::::    ::::  \n");
        printf(":+:       :+:    :+: :+:    :+:      :+:    :+: :+:    :+:   :+: :+:   +:+:+: :+:+:+ \n");
        printf("+:+       +:+    +:+ +:+    +:+      +:+        +:+    +:+  +:+   +:+  +:+ +:+:+ +:+ \n");
        printf(":#::+::#  +#+    +:+ +#++:++#+       :#:        +#++:++#:  +#++:++#++: +#+  +:+  +#+ \n");
        printf("+#+       +#+    +#+ +#+             +#+   +#+# +#+    +#+ +#+     +#+ +#+       +#+ \n");
        printf("#+#       #+#    #+# #+#             #+#    #+# #+#    #+# #+#     #+# #+#       #+# \n");
        printf("###        ########  ###              ########  ###    ### ###     ### ###       ### \n\n");
        printf("Account Menu:\n");
        printf("1: Register    #\n");
        printf("2: Login\n");
        printf("3: Exit\n");
        HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD Pos = {15, 9};
        SetConsoleCursorPosition(hconsole,Pos);
        int keyValue1 =0;
        int keyValue2 =0;
        int temp = 1;
        int flag = 1;
        while(flag)
        {
            keyValue1 = getch();
            if(keyValue1 == 0xE0)
            {
                keyValue2 = getch();
                if(keyValue2 == 72)
                {
                    if(temp == 2)
                    {
                        COORD lastPos = {15, 9};
                        printf("\b  ");
                        SetConsoleCursorPosition(hconsole,lastPos);
                        printf("#");
                        temp--;
                    }
                    else if(temp == 3)
                    {
                        COORD lastPos = {15, 10};
                        printf("\b  ");
                        SetConsoleCursorPosition(hconsole,lastPos);
                        printf("#");
                        temp--;
                    }
                    else if(temp == 1)
                    {
                        COORD lastPos = {15, 11};
                        printf("\b  ");
                        SetConsoleCursorPosition(hconsole,lastPos);
                        printf("#");
                        temp = 3;
                    }
                }
                else if(keyValue2 == 80)
                {
                    if(temp == 1)
                    {
                        COORD lastPos = {15, 10};
                        printf("\b  ");
                        SetConsoleCursorPosition(hconsole,lastPos);
                        printf("#");
                        temp++;
                    }
                    else if(temp == 2)
                    {
                        COORD lastPos = {15, 11};
                        printf("\b  ");
                        SetConsoleCursorPosition(hconsole,lastPos);
                        printf("#");
                        temp++;
                    }
                    else if(temp == 3)
                    {
                        COORD lastPos = {15, 9};
                        printf("\b  ");
                        SetConsoleCursorPosition(hconsole,lastPos);
                        printf("#");
                        temp = 1;
                    }
                }
            }
            else if(keyValue1 == 13)
                flag = 0;
        }
        if (temp == 1)
        {
            Register();
            break;
        }
        else if (temp == 2)
        {
            Login();
            break;
        }
        else if (temp == 3)
        {
            printf("\nAre you sure to exit? (y/n)\n");
            char status = getch();
            if(status == 'y')
                exit(0);
            else
                continue;
        }
        else
        {
            printf("Error. Invalid selection. Please try again.\n");
            continue;
        }
    }

}

void Register()
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char* username = (char*)malloc(100 * sizeof(char));
    char* password = (char*)malloc(100 * sizeof(char));
    *username = '\0';
    *password = '\0';
    char regUser[210] = "register ";
    printf("Enter Username:\n");
    scanf("%[^\n]%*c", username);
    printf("Enter Password:\n");
    scanf("%[^\n]%*c", password);
    strcat(regUser, username);
    strcat(regUser, ", ");
    strcat(regUser, password);
    strcat(regUser,"\n");
    buffer = ContactWithServer(regUser);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "Successful") == 0)
        {
            printf("You are successfully registered.\n");
            printf("Press any key to continue...\n");
            getch();
            AccountMenu();
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            AccountMenu();
        }
    }
    free(buffer);
    free(username);
    free(password);
	cJSON_Delete(json);
	cJSON_Delete(type);
}

void Login()
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char* username = (char*)malloc(100 * sizeof(char));
    char* password = (char*)malloc(100 * sizeof(char));
    char AuthToken[50] = "";
    *username = '\0';
    *password = '\0';
    char logUser[210] = "login ";
    printf("Enter Username:\n");
    scanf("%[^\n]%*c", username);
    printf("Enter Password:\n");
    scanf("%[^\n]%*c", password);
    strcat(logUser, username);
    strcat(logUser, ", ");
    strcat(logUser, password);
    strcat(logUser, "\n");
    buffer = ContactWithServer(logUser);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "AuthToken") == 0)
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            strcpy(AuthToken, content->valuestring);
            printf("Successful login.\n");
            printf("Press any key to continue...\n");
            getch();
            UserInterface(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            Login();
        }
    }
    free(buffer);
    free(username);
    free(password);
	cJSON_Delete(json);
	cJSON_Delete(type);
}

void UserInterface(char *AuthToken)
{
    system("cls");
    system("color 03");
    printf("1: Create Channel   #\n2: Join Channel\n3: Logout\n");
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Pos = {20, 0};
    SetConsoleCursorPosition(hconsole,Pos);
    int keyValue1 =0;
    int keyValue2 =0;
    int temp = 1;
    int flag = 1;
    while(flag)
    {
        keyValue1 = getch();
        if(keyValue1 == 0xE0)
        {
            keyValue2 = getch();
            if(keyValue2 == 72)
            {
                if(temp == 2)
                {
                    COORD lastPos = {20, 0};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp--;
                }
                else if(temp == 3)
                {
                    COORD lastPos = {20, 1};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp--;
                }
                else if(temp == 1)
                {
                    COORD lastPos = {20, 2};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp = 3;
                }
            }
            else if(keyValue2 == 80)
            {
                if(temp == 1)
                {
                    COORD lastPos = {20, 1};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp++;
                }
                else if(temp == 2)
                {
                    COORD lastPos = {20, 2};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp++;
                }
                else if(temp == 3)
                {
                    COORD lastPos = {20, 0};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp = 1;
                }
            }
        }
        else if(keyValue1 == 13)
            flag = 0;
    }
    switch(temp)
    {
        case 1:
            CreateChannel(AuthToken);
            break;
        case 2:
            JoinChannel(AuthToken);
            break;
        case 3:
            Logout(AuthToken);
            break;
        default:
            printf("Error.Invalid selection.Please Try again.\n");
            UserInterface(AuthToken);
            break;
    }
}

void CreateChannel(char *AuthToken)
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char channelName[50] = {0};
    char createChannel[60] = "create channel ";
    printf("Please enter your channel name:\n");
    scanf("%[^\n]%*c", channelName);
    strcat(createChannel, channelName);
    strcat(createChannel, ", ");
    strcat(createChannel, AuthToken);
    strcat(createChannel, "\n");
    buffer = ContactWithServer(createChannel);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "Successful") == 0)
        {
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            UserInterface(AuthToken);
        }
    }
    free(buffer);
	cJSON_Delete(json);
	cJSON_Delete(type);
}

void JoinChannel(char *AuthToken)
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char channelName[50] = {0};
    char joinChannel[60] = "join channel ";
    printf("Please enter your channel name:\n");
    scanf("%[^\n]%*c", channelName);
    strcat(joinChannel, channelName);
    strcat(joinChannel, ", ");
    strcat(joinChannel, AuthToken);
    strcat(joinChannel, "\n");
    buffer = ContactWithServer(joinChannel);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "Successful") == 0)
        {
            printf("Press any key to continue...\n");
            getch();
           ChatMenu(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            UserInterface(AuthToken);
        }
    }
    free(buffer);
	cJSON_Delete(json);
	cJSON_Delete(type);
}

void Logout(char *AuthToken)
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char logOutUser[50]= "logout ";
    strcat(logOutUser, AuthToken);
    strcat(logOutUser, "\n");
    buffer = ContactWithServer(logOutUser);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "Successful") == 0)
        {
            printf("Press any key to continue...\n");
            getch();
            AccountMenu(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            UserInterface(AuthToken);
        }
    }
    free(buffer);
    free(logOutUser);
	cJSON_Delete(json);
	cJSON_Delete(type);
}

void ChatMenu(char *AuthToken)
{
    system("cls");
    system("color 04");
    printf("1: Send Message       #\n2: Refresh\n3: Channel Members\n4: Search Member\n5: Search Messages\n6: Leave Channel\n");
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Pos = {22, 0};
    SetConsoleCursorPosition(hconsole,Pos);
    int keyValue1 =0;
    int keyValue2 =0;
    int temp = 1;
    int flag = 1;
    while(flag)
    {
        keyValue1 = getch();
        if(keyValue1 == 0xE0)
        {
            keyValue2 = getch();
            if(keyValue2 == 72)
            {
                if(temp == 2)
                {
                    COORD lastPos = {22, 0};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp--;
                }
                else if(temp == 3)
                {
                    COORD lastPos = {22, 1};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp--;
                }
                else if(temp == 4)
                {
                    COORD lastPos = {22, 2};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp--;
                }
                else if(temp == 5)
                {
                    COORD lastPos = {22, 3};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp--;
                }
                else if(temp == 6)
                {
                    COORD lastPos = {22, 4};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp--;
                }
                else if(temp == 1)
                {
                    COORD lastPos = {22, 5};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp = 6;
                }
            }
            else if(keyValue2 == 80)
            {
                if(temp == 1)
                {
                    COORD lastPos = {22, 1};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp++;
                }
                else if(temp == 2)
                {
                    COORD lastPos = {22, 2};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp++;
                }
                else if(temp == 3)
                {
                    COORD lastPos = {22, 3};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp++;
                }
                else if(temp == 4)
                {
                    COORD lastPos = {22, 4};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp++;
                }
                else if(temp == 5)
                {
                    COORD lastPos = {22, 5};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp++;
                }
                else if(temp == 6)
                {
                    COORD lastPos = {22, 0};
                    printf("\b  ");
                    SetConsoleCursorPosition(hconsole,lastPos);
                    printf("#");
                    temp = 1;
                }
            }
        }
        else if(keyValue1 == 13)
            flag = 0;
    }
    switch(temp)
    {
        case 1:
            SendMeesage(AuthToken);
            break;
        case 2:
            Refresh(AuthToken);
            break;
        case 3:
            ChannelMembers(AuthToken);
            break;
        case 4:
            SearchMember(AuthToken);
            break;
        case 5:
            SearchMessage(AuthToken);
            break;
        case 6:
            LeaveChannel(AuthToken);
            break;
        default:
            printf("Error.Invalid selection.Please Try again.\n");
            UserInterface(AuthToken);
            break;
    }
}

void SendMeesage(char *AuthToken)
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char Message[500] = {0};
    char sendMeesage[550] = "send ";
    printf("Please enter your message:\n");
    scanf("%[^\n]%*c", Message);
    strcat(sendMeesage, Message);
    strcat(sendMeesage, ", ");
    strcat(sendMeesage, AuthToken);
    strcat(sendMeesage, "\n");
    buffer = ContactWithServer(sendMeesage);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "Successful") == 0)
        {
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
        else
        {
            printf("No Respond!\n");
            printf("Press any key to continue...\n");
            getch();
            SendMeesage(AuthToken);
        }
    }
    free(buffer);
	cJSON_Delete(json);
	cJSON_Delete(type);
}

void Refresh(char *AuthToken)
{
    system("cls");
    char refreshChannel[50] = "refresh ";
    char* buffer = (char*)malloc(1000 * sizeof(char));
    strcat(refreshChannel, AuthToken);
    strcat(refreshChannel, "\n");
    buffer = ContactWithServer(refreshChannel);
    const cJSON *json = cJSON_Parse(buffer);
	const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
	if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "List") == 0)
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            if (content != NULL)
            {
                cJSON *tempJson = NULL;
                cJSON_ArrayForEach(tempJson, content)
                {
                    cJSON *sender = cJSON_GetObjectItemCaseSensitive(tempJson, "sender");
                    cJSON *message = cJSON_GetObjectItemCaseSensitive(tempJson, "content");
                    printf("%s : %s\n", sender->valuestring, message->valuestring);
                }
            }
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
    }
    free(buffer);
    cJSON_Delete(json);
    cJSON_Delete(type);
}

void ChannelMembers(char *AuthToken)
{
    system("cls");
    char members[50] = "channel members ";
    char* buffer = (char*)malloc(1000 * sizeof(char));
    strcat(members, AuthToken);
    strcat(members, "\n");
    buffer = ContactWithServer(members);
    const cJSON *json = cJSON_Parse(buffer);
	const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
	if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "List") == 0)
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);

            if (content != NULL)
            {
                cJSON *item = NULL;
                cJSON_ArrayForEach(item, content)
                {
                    printf("%s\n", item->valuestring);
                }
            }

            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
    }
    free(buffer);
    cJSON_Delete(json);
    cJSON_Delete(type);
}

void LeaveChannel(char *AuthToken)
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char leave[50] = "leave ";
    strcat(leave, AuthToken);
    strcat(leave, "\n");
    buffer = ContactWithServer(leave);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "Successful") == 0)
        {
            printf("Press any key to continue...\n");
            getch();
            UserInterface(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            UserInterface(AuthToken);
        }
    }
    free(buffer);
	cJSON_Delete(json);
	cJSON_Delete(type);
}

void SearchMember(char *AuthToken)
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char ExpectedMember[101] = "";
    char searchCommand[150] = "search user ";
    printf("Please enter your expected username: ");
    scanf("%s", ExpectedMember);
    strcat(searchCommand, ExpectedMember);
    strcat(searchCommand, ", ");
    strcat(searchCommand, AuthToken);
    strcat(searchCommand, "\n");
    searchCommand[strlen(searchCommand)] = '\0';
    buffer = ContactWithServer(searchCommand);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
    if((content->valuestring != NULL))
    {
        if(strcmp(content->valuestring, "user found.") == 0)
        {
            printf("User with this username exist in this channel.\n");
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
        else
        {
            printf("There is no member in this channel with this username.\n");
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
    }
    free(buffer);
	cJSON_Delete(json);
	cJSON_Delete(content);
}

void SearchMessage(char *AuthToken)
{
    system("cls");
    char* buffer = (char*)malloc(1000 * sizeof(char));
    char ExpectedWord[201] = "";
    char searchCommand[150] = "search message ";
    printf("Please enter your expected word to search: ");
    scanf("%s", ExpectedWord);
    strcat(searchCommand, ExpectedWord);
    strcat(searchCommand, ", ");
    strcat(searchCommand, AuthToken);
    strcat(searchCommand, "\n");
    searchCommand[strlen(searchCommand)] = '\0';
    buffer = ContactWithServer(searchCommand);
    cJSON *json = cJSON_Parse(buffer);
    const cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if((type->valuestring != NULL))
    {
        if(strcmp(type->valuestring, "List") == 0)
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            int counter = -1;
            if (content != NULL)
            {
                cJSON *tempJson = NULL;
                counter = 0;
                printf("Messages that contains this word:\n");
                cJSON_ArrayForEach(tempJson, content)
                {
                    cJSON *sender = cJSON_GetObjectItemCaseSensitive(tempJson, "sender");
                    cJSON *message = cJSON_GetObjectItemCaseSensitive(tempJson, "content");
                    printf("%s : %s\n", sender->valuestring, message->valuestring);
                    counter++;
                }
            }
            if (counter == 0)
                printf("No results found :(\n");
            printf("Press any key to continue...\n");
            getch();
            cJSON_Delete(content);
            ChatMenu(AuthToken);
        }
        else
        {
            const cJSON *content = cJSON_GetObjectItemCaseSensitive(json, "content");
            printf("%s\n", content->valuestring);
            printf("Press any key to continue...\n");
            getch();
            ChatMenu(AuthToken);
        }
    }
    free(buffer);
	cJSON_Delete(json);
}
