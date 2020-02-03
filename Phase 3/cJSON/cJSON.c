#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"



/* JSON Struct */
typedef struct cJsonBaseObject
{
    char *type;
    struct cJsonBaseObject *nextObject;
    struct cJsonBaseObject *previousObject;
    char *valuestring;
    struct cJsonBaseObject *ItemObject;
    struct cJsonBaseObject *ItemArray;
}cJSON;

#define cJSON_ArrayForEach(item, array) for(struct cJsonBaseObject *temp = (array)->ItemArray->ItemObject , *item = ((array != NULL) ? cJSON_CloneObject((array)->ItemArray->ItemObject) : NULL); (temp != NULL); (temp = temp->nextObject),(item = (array != NULL) ? cJSON_CloneObject(temp) : NULL))


//Define necessary functions
char* cJSON_PrintUnformatted(cJSON* json);
cJSON* cJSON_CreateObject();
cJSON* cJSON_CreateString(const char *string);
void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
cJSON* cJSON_Parse(const char *value);
cJSON* cJSON_ParseProcess(const char* str);
cJSON* cJSON_GetObjectItemCaseSensitive(const cJSON *const object, const char* const string);
cJSON* cJSON_CreateArray();
void cJSON_AddItemToArray(cJSON *array, cJSON *item);
void cJSON_Delete(cJSON *item);
void cJSON_Print(cJSON *item, char *string);
cJSON_CloneObject(cJSON *element);
//

char* cJSON_PrintUnformatted(cJSON* json)
{
    char *JsonToString = (char *)calloc(1000, sizeof(char));
    char *temp = (char *)calloc(1000, sizeof(char));
    int flag = 0;
    while(json != NULL)
    {
        if(flag)
            strcat(JsonToString,",");
        flag = 1;
        cJSON_Print(json, temp);
        strcat(JsonToString, temp);
        json = json->nextObject;
    }
    free(temp);
    return JsonToString;
}

cJSON* cJSON_CreateObject()
{
    cJSON *output = (cJSON*)calloc(1,sizeof(cJSON));
    output->ItemArray = NULL;
    output->ItemObject = NULL;
    output->nextObject = NULL;
    output->previousObject = NULL;
    output->valuestring = NULL;
    output->type = NULL;
    return output;
}

cJSON* cJSON_CreateString(const char *string)
{
    cJSON *output = (cJSON*)calloc(1, sizeof(cJSON));
    output->ItemArray = NULL;
    output->ItemObject = NULL;
    output->nextObject = NULL;
    output->previousObject = NULL;
    output->type = NULL;
    output->valuestring = (char*)malloc(1000 * sizeof(char));
    strcpy(output->valuestring, string);
    output->valuestring[strlen(string)] = '\0';
    return output;
}

void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item)
{
    if(object->ItemObject == NULL)
    {
        object->ItemObject = item;
        item->type = string;
    }
    else
    {
        cJSON* iterator = object->ItemObject;
        while(iterator->nextObject != NULL)
            iterator = iterator->nextObject;
        iterator->nextObject = item;
        iterator->nextObject->nextObject = NULL;
        iterator->nextObject->previousObject = iterator;
        item->type = string;
    }
}

cJSON* cJSON_Parse(const char *value)
{
    int size = strlen(value);
    cJSON *tempJson, *pointer;
    int counter = 0, start = 0, s = 1;
    char *tempstr = (char*)calloc(size + 1, sizeof(char));
    for(int i = 0; i < size+1; i++)
    {
        switch(value[i])
        {
            case '{':
            case '[':
                counter++;
                break;
            case '}':
            case ']':
                counter--;
                break;
        }
        if(!counter && (value[i] == ',' || value[i] == '\0'))
        {
            strncpy(tempstr, value + start, i-start);
            tempstr[i-start]= '\0';
            if(s)
            {
                tempJson = pointer = cJSON_ParseProcess(tempstr);
                s = 0;
            }
            else
            {
                pointer->nextObject = cJSON_ParseProcess(tempstr);
                pointer = pointer->nextObject;
            }
            start = i +1;
        }
    }
    free(tempstr);
    return tempJson;
}

cJSON* cJSON_ParseProcess(const char* str)
{
    int size = strlen(str);
    char *tempString = (char*)calloc(size+1, sizeof(char));
    cJSON *tempJSON;
    if(str[0] == '{' && str[size-1] == '}')
    {
        tempJSON = cJSON_CreateObject();
        strncpy(tempString, str+1, size-2);
        tempJSON->ItemObject = cJSON_Parse(tempString);
    }
    else if(str[0] == '[' && str[size-1] == ']')
    {
        tempJSON = cJSON_CreateArray();
        strncpy(tempString, str+1, size-2);
        tempJSON->ItemArray->ItemObject = cJSON_Parse(tempString);
    }
    else
    {
        if(strstr(str, ":") != NULL)
        {
            char type[100] = {0}, content[10000] = {0};
            sscanf(str, "\"%[^\"]\":%[^\0]", type, content);
            if(!(content[0] == '"' && content[strlen(content)-1] == '"'))
            {
                tempJSON = cJSON_Parse(content);
            }
            else
            {
                strncpy(content, content+1, strlen(content)- 2);
                content[strlen(content)-2] = '\0';
                tempJSON = cJSON_CreateString(content);
            }
            tempJSON->type = (char*)calloc(strlen(type)+1, sizeof(char));
            strcpy(tempJSON->type, type);
        }
        else
        {
            char content[10000];
            sscanf(str, "%[^\0]", content);
            if(!(content[0] == '"' && content[strlen(content)-1] == '"'))
            {
                tempJSON = cJSON_Parse(content);
            }
            else
            {
                strncpy(content, content+1, strlen(content)-2);
                content[strlen(content)-2] = '\0';
                tempJSON = cJSON_CreateString(content);
            }
        }
    }
    free(tempString);
    return tempJSON;
}

cJSON* cJSON_GetObjectItemCaseSensitive(const cJSON *const object, const char* const string)
{
    cJSON *iterator = object->ItemObject;
    while(iterator != NULL)
    {
        if(iterator->type != NULL && strcmp(iterator->type, string) == 0)
        {
            cJSON *temp = cJSON_CreateObject();
            if(iterator->ItemObject != NULL)
                temp->ItemObject = iterator->ItemObject;
            else if(iterator->ItemArray != NULL)
                temp->ItemArray = iterator->ItemArray;
            else if(iterator->valuestring != NULL)
                temp->valuestring = iterator->valuestring;
            return temp;
        }
        iterator = iterator->nextObject;
    }
    cJSON *nullJson = NULL;
    return nullJson;
}

cJSON* cJSON_CreateArray()
{
    cJSON *output = (cJSON*)calloc(1, sizeof(cJSON));
    output->ItemArray = (cJSON*)calloc(1, sizeof(cJSON));
    output->ItemObject = NULL;
    output->nextObject = NULL;
    output->previousObject = NULL;
    output->type = NULL;
    output->valuestring = NULL;
}

void cJSON_AddItemToArray(cJSON *array, cJSON *item)
{
    if(array->ItemArray->ItemObject == NULL)
    {
        array->ItemArray->ItemObject = item;

    }
    else
    {
        cJSON *iterator = array->ItemArray->ItemObject;
        while(iterator->nextObject != NULL)
            iterator = iterator->nextObject;
        iterator->nextObject = item;
        iterator->nextObject->nextObject = NULL;
        iterator->nextObject->previousObject = iterator;
    }
}

void cJSON_Print(cJSON *item, char *string)
{
    char temp1[1000] = {0};
    char temp2[1000] = {0};
    if(item->ItemObject != NULL)
    {
        strcpy(temp1, cJSON_PrintUnformatted(item->ItemObject));
        if(item->type != NULL)
        {
            strcpy(temp2, item->type);
            sprintf(string, "\"%s\":{%s}", temp2,temp1);
        }
        else
            sprintf(string, "{%s}", temp1);
    }
    else if(item->ItemArray != NULL)
    {
        strcpy(temp1, cJSON_PrintUnformatted(item->ItemArray->ItemObject));
        if(item->type != NULL)
        {
            strcpy(temp2, item->type);
            sprintf(string, "\"%s\":[%s]", temp2,temp1);
        }
        else
            sprintf(string, "[%s]", temp1);
    }
    else if(item->valuestring != NULL)
    {
        strcpy(temp1, item->valuestring);
        if(item->type != NULL)
        {
            strcpy(temp2, item->type);
            sprintf(string, "\"%s\":\"%s\"", temp2,temp1);
        }
        else
            sprintf(string, "\"%s\"", temp1);
    }
}

void cJSON_Delete(cJSON *item)
{
    while(item != NULL)
    {
        cJSON *temp = item->nextObject;
        if(item->ItemObject != NULL)
        {
            cJSON_Delete(item->ItemObject);
        }
        if(item->ItemArray != NULL)
        {
            cJSON_Delete(item->ItemArray);
        }
        if(item->type != NULL)
        {
            free(item->type);
        }
        if(item->valuestring != NULL)
        {
            free(item->valuestring);
        }
        free(item);
        item = temp;
    }
}

cJSON_CloneObject(cJSON *element)
{
    if (element != NULL)
    {
        cJSON *temp = cJSON_CreateObject();
        if(element->valuestring != NULL)
            temp->valuestring = element->valuestring;
        else if(element->ItemObject != NULL)
            temp->ItemObject = element->ItemObject;
        else if(element->ItemArray != NULL)
            temp->ItemArray = element->ItemArray;
        return temp;
    }
    else
        return NULL;
}
