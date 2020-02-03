///----------------------- In the Name of God --------------------------
///------------------ Student Name: Ali Mahdavifar ---------------------
///------- Chat Application Project - FoP Class - 2020 Winter ----------
/// Department of Computer Engineering | Sharif University of Technology
///---------------------------------------------------------------------
//--------------------------- cJSON Types: -----------------------------
#define cJSON_String (1 << 4)
#define cJSON_Array  (1 << 5)
#define cJSON_Object (1 << 6)
//------------------------- cJSON Structure: ---------------------------
typedef struct cJSON {
    struct cJSON* next;
    struct cJSON* prev;

    struct cJSON* child;

    int type;

    char* valuestring;

    char* name;
} cJSON;
//------------------------- cJSON Functions: ---------------------------
cJSON* cJSON_CreateObject (void);
cJSON* cJSON_CreateArray (void);
cJSON* cJSON_CreateString (char* string);
cJSON* cJSON_AddItemToObject (cJSON* object, char* name, cJSON* item);
cJSON* cJSON_AddItemToArray(cJSON *array, cJSON *item);
cJSON* cJSON_AddStringToObject (cJSON* object, char* name, char* string);
cJSON* cJSON_AddArrayToObject (cJSON* object, char* name);
cJSON* add_child (cJSON* parent, cJSON* item);
cJSON* add_brother (cJSON* prev, cJSON* item);
cJSON* cJSON_GetObjectItem (cJSON* object, char* name);
cJSON* cJSON_GetObjectItemCaseSensitive (cJSON* object, char* name);
cJSON* cJSON_GetArrayItem (cJSON *array, int index);
int cJSON_GetArraySize (cJSON* array);
char* cJSON_PrintUnformatted (cJSON* item);
char* cJSON_Print (cJSON* item);
void addprint (const char * format, ...);
char* print (cJSON* item);
cJSON* cJSON_Parse (char* str);
cJSON* parse (char* str);
