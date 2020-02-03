///----------------------- In the Name of God --------------------------
///------------------ Student Name: Ali Mahdavifar ---------------------
///------- Chat Application Project - FoP Class - 2020 Winter ----------
/// Department of Computer Engineering | Sharif University of Technology
///---------------------------------------------------------------------
//**********************************************************************
char* print_buffer;
#define p1size 8000
//**********************************************************************
cJSON* cJSON_CreateObject (void) {
    cJSON* node = calloc(1,sizeof(cJSON));
    node->type = cJSON_Object;

    return node;
}
//**********************************************************************
cJSON* cJSON_CreateArray (void) {
    cJSON* node = calloc(1,sizeof(cJSON));
    node->type = cJSON_Array;

    return node;
}
//**********************************************************************
cJSON* cJSON_CreateString(char* string) {
    cJSON* node = calloc(1,sizeof(cJSON));
    node->type = cJSON_String;
    node->valuestring = strdup(string);

    return node;
}
//**********************************************************************
cJSON* cJSON_AddItemToObject (cJSON* object, char* name, cJSON* item) {
    item->name = strdup(name);
    add_child(object, item);

    return item;
}
//**********************************************************************
cJSON* cJSON_AddItemToArray(cJSON *array, cJSON *item) {
    add_child(array, item);
}
//**********************************************************************
cJSON* cJSON_AddStringToObject (cJSON* object, char* name, char* string) {
    cJSON* item = cJSON_CreateString(string);
    return cJSON_AddItemToObject(object, name, item);
}
//**********************************************************************
cJSON* cJSON_AddArrayToObject (cJSON* object, char* name) {
    cJSON* array = cJSON_CreateArray();
    return cJSON_AddItemToObject(object, name, array);
}
//**********************************************************************
cJSON* add_child (cJSON* parent, cJSON* item) {
    cJSON* child = parent->child;
    if (child == NULL) {
        parent->child = item;
    }
    else {
        while (child->next) {
            child = child->next;
        }
        add_brother(child, item);
    }

    return item;
}
//**********************************************************************
cJSON* add_brother (cJSON* prev, cJSON* item) {
    prev->next = item;
    item->prev = prev;

    return item;
}
//**********************************************************************
cJSON* cJSON_GetObjectItem (cJSON* object, char* name) {
    cJSON* child = object->child;

    while (child != NULL && child->name != NULL && strcmp(child->name, name)) {
        child = child->next;
    }

    if (child == NULL || child->name == NULL)
        return NULL;

    return child;
}
//**********************************************************************
cJSON* cJSON_GetObjectItemCaseSensitive (cJSON* object, char* name) {
    return cJSON_GetObjectItem(object, name);
}
//**********************************************************************
cJSON* cJSON_GetArrayItem(cJSON *array, int index) {
    if (index < 0)
        return NULL;

    cJSON* child = array->child;

    for (int i = 0; i < index; i++) {
        if (child == NULL)
            return NULL;
        child = child->next;
    }

    return child;
}
//**********************************************************************
int cJSON_GetArraySize (cJSON* array) {
    int cnt = 0;
    cJSON* child = array->child;
    while (child != NULL) {
        child = child->next;
        cnt++;
    }

    return cnt;
}
//**********************************************************************
void addprint (const char * format, ...) {
    char* s = print_buffer;
    va_list args;
    va_start (args, format);
    vsprintf (s + strlen(s), format, args);
    va_end (args);
}
//**********************************************************************
char* cJSON_Print (cJSON* item) {
    print_buffer = calloc(p1size, 1);
    print(item);
    return print_buffer;
}
//**********************************************************************
char* cJSON_PrintUnformatted (cJSON* item) {
    return cJSON_Print(item);
}
//**********************************************************************
char* print (cJSON* item) {
    //------------------------------------------------------------------
    if (item->type == cJSON_Object)
    {
        if (item->name != NULL) {
            addprint("\"%s\":", item->name);
        }

        addprint("{\n");

        cJSON* child = item->child;

        while (child != NULL) {
            print(child);
            if (child->next != NULL) {
                addprint(",\n");
            }
            child = child->next;
        }

        addprint("}\n");
    }
    //------------------------------------------------------------------
    if (item->type == cJSON_Array)
    {
        if (item->name != NULL) {
            addprint("\"%s\":", item->name);
        }

        addprint("[\n");

        cJSON* child = item->child;

        while (child != NULL) {
            print(child);
            if (child->next != NULL) {
                addprint("\n,\n");
            }
            child = child->next;
        }

        addprint("]\n");
    }
    //------------------------------------------------------------------
    if (item->type == cJSON_String)
    {
        if (item->name != NULL) {
            addprint("\"%s\":", item->name);
        }
        if (item->valuestring != NULL) {
            addprint("\"%s\"\n", item->valuestring);
        }
    }



    return print_buffer;
}
//**********************************************************************
int depth = 0;
cJSON* obj[10];
bool first = true;
//----------------------------------------------------------------------
cJSON* parse (char* str) {
    cJSON* item = cJSON_CreateObject();

    char* tok;

    if (first) {
        first = false;
        tok = strtok(str, "\n\0\r");
    }   else {
        tok = strtok(0, "\n\0\r");
    }
    ///if (tok != NULL)    printf("%s\n", tok);

    while (tok != NULL)
    {
        if (strlen(tok) == 0) {
        }
        //--------------------------------------------------------------
        if (tok[0] == '{') {
            obj[depth] = item;
            depth++;

            parse(str);

        }
        //--------------------------------------------------------------
        else if (tok[0] == '}') {
            break;
        }
        //--------------------------------------------------------------
        else if (tok[0] == ']') {
            break;
        }
        //--------------------------------------------------------------
        else if (tok[0] == ',') {
            add_child(obj[depth-1], item);
            item = cJSON_CreateObject();
        }
        //--------------------------------------------------------------
        else if (tok[0] == '"') {
            if (tok[strlen(tok)-1] == '[') {
                int namelen = strchr(tok, ':') - tok;
                tok[namelen-1] = 0;
                item->name = strdup(tok+1);
                obj[depth] = item;
                depth++;
                item->type = cJSON_Array;

                parse(str);
            }

            else {
                if (strchr(tok,':') != NULL)
                {
                    int namelen = strchr(tok, ':') - tok;
                    tok[namelen-1] = 0;
                    item->name = strdup(tok+1);
                    item->type = cJSON_String;
                    tok += namelen + 2;
                    tok[strlen(tok)-1] = 0;
                    item->valuestring = strdup(tok);
                }
                else
                {
                    tok[strlen(tok)-1] = 0;
                    item->valuestring = strdup(tok+1);
                    item->type = cJSON_String;
                }
            }
        }
        //--------------------------------------------------------------
        else if (tok[0] == '\n') {
        }
        //--------------------------------------------------------------
        else {
            ///printf("---  %c  ---\n", tok[0]);
        }

        tok = strtok(NULL, "\n\0\r");
        ///if (tok != NULL) printf("%s\n", tok);
    }

    if (depth > 0) {
        add_child(obj[depth-1], item);
    }
    depth--;

    return item;

}
//**********************************************************************
cJSON* cJSON_Parse (char* str) {
    char* buffer = calloc(p1size, 1);
    strcpy(buffer, str);
    if (buffer[0] != '{') {
        printf("Error type 1\n");
        return NULL;
    }

    //printf("%s\n", buffer);

    cJSON* ans = parse(buffer);

    depth = 0;
    first = true;
    memset(obj, 0, sizeof(obj));

    free(buffer);
    return ans;
}
