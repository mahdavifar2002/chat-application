#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include "cJSON.h"
#include "cJSON.c"
#define MAX 8000

int main() {
    cJSON* a = cJSON_CreateObject();
    cJSON_AddStringToObject(a, "type", "Successful");
    cJSON_AddStringToObject(a, "name", "my channel");
    cJSON* list = cJSON_AddArrayToObject(a, "messages");
    cJSON* item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "sender", "me");
    cJSON_AddStringToObject(item, "content", "Hello!");
    cJSON_AddItemToArray(list, item);
    item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "sender", "me");
    cJSON_AddItemToArray(list, item);
    cJSON_AddStringToObject(a, "contes", "my channel");

    cJSON* b = cJSON_Parse("{\n\"username\":\"a\"\n,\n\"password\":\"a\"\n}\n");

    printf("________________________________\n");
        printf("%s\n", cJSON_Print(a));
    printf("________________________________\n");
        printf("%s\n", cJSON_Print(b));

}
