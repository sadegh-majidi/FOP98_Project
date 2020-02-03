#ifndef CJSON_H_INCLUDED
#define CJSON_H_INCLUDED

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

#endif // CJSON_H_INCLUDED
