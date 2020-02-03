///----------------------- In the Name of God --------------------------
///------------------ Student Name: Ali Mahdavifar ---------------------
///------- Chat Application Project - FoP Class - 2020 Winter ----------
/// Department of Computer Engineering | Sharif University of Technology
///---------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include "server_header.c"
#include "cJSON.h"
#include "cJSON.c"
//********************* Global Variables ***********************
typedef struct {
    char name[100];
    char pass[100];
    bool on;
    char token[100];
    cJSON* chnl;
    int seen;
} USER;
//--------------------------------------------------------------
USER   users[MAX];
cJSON* chnls[MAX];
int    uMAX = 0  ,  cMAX = 0;
char*  req;
char   token[100];
cJSON* ans;
//********************* List of Functions **********************
void read_data ();      // 0
void read_user ();      // 0.1
void read_chnl ();      // 0.2
void register_user ();  // 1.1
void login_user ();     // 1.2
void create_channel (); // 2.1
void join_channel ();   // 2.2
void logout_user ();    // 2.3
void send_message ();   // 3.1
void refresh ();        // 3.2
void members ();        // 3.3
void search_user ();    // 3.4.1
void search_message (); // 3.4.2
void leave_channel ();  // 3.5
//--------------------- Auxiliary Functions ---------------------
void make_token (int);
USER* find_token ();
void make_ans (char*, char*);
void update_channel (cJSON*);
void add_to_channel (cJSON*, char*, char*);
bool find (char* str, char* word);
//************************ int main() **************************
int main() {
    // reading data...
    read_data();

    while (true)
    {
        req = _recv();
        ans = cJSON_CreateObject();
        printf("\nFrom Client:\t%s\n",req);

        if (strncmp("register ", req, 9) == 0)
        {
            register_user();
        }
        //------------------------------------------------------
        else if (strncmp("login ", req, 6) == 0)
        {
            login_user();
        }
        //------------------------------------------------------
        else if (strncmp("logout ", req, 7) == 0)
        {
            logout_user();
        }
        //------------------------------------------------------
        else if (strncmp("create channel ", req, 15) == 0) {
            create_channel();
        }
        //------------------------------------------------------
        else if (strncmp("join channel ", req, 13) == 0) {
            join_channel();
        }
        //------------------------------------------------------
        else if (strncmp("send ", req, 5) == 0) {
            send_message();
        }
        //------------------------------------------------------
        else if (strncmp("refresh ", req, 8) == 0) {
            refresh();
        }
        //------------------------------------------------------
        else if (strncmp("channel members ", req, 16) == 0) {
            members();
        }
        //------------------------------------------------------
        else if (strncmp("leave ", req, 6) == 0) {
            leave_channel();
        }
        //------------------------------------------------------
        else if (strncmp("search user ", req, 12) == 0) {
            search_user();
        }
        //------------------------------------------------------
        else if (strncmp("search message ", req, 15) == 0) {
            search_message();
        }
        //------------------------------------------------------
        else
        {
            make_ans("Error", "not ready yet.");
        }
        //------------------------------------------------------
        char* str = cJSON_PrintUnformatted(ans);
        _send("%s", str);

        printf("To Client:\t");
        for (int i = 0; i < strlen(str); i++)
            if (str[i] != '\n' && str[i] != '\t')
                printf("%c", str[i]);
        printf("\n\n");
    }

}
//*********************** Reading Data *************************
void read_data() {
    // Create folders at the first time
    CreateDirectory("Resources",NULL);
    CreateDirectory("Resources\\Users",NULL);
    CreateDirectory("Resources\\Channels",NULL);

    read_user();
    read_chnl();
}
//********************** Reading  Users ************************
void read_user () {
    // Open user directory
    DIR *dir;
    struct dirent *ent;
    dir = opendir ("Resources\\Users");

    while ((ent = readdir (dir)) != NULL)
    {
        // Ignoring . and ..
        if (*(ent->d_name) != '.')
        {
            char user_path[MAX] = "Resources\\Users\\";

            FILE* fptr = fopen(strcat(user_path, ent->d_name), "r");

            fseek(fptr, 0, SEEK_END);
            int fs = ftell(fptr);
            fseek(fptr, 0, SEEK_SET);

            char* buffer = malloc(fs);
            fread(buffer, 1, fs, fptr);


            cJSON* ptr = cJSON_Parse(buffer);
            strcpy(users[uMAX].name, cJSON_GetObjectItemCaseSensitive(ptr, "username")->valuestring);
            strcpy(users[uMAX].pass, cJSON_GetObjectItemCaseSensitive(ptr, "password")->valuestring);
            users[uMAX].on = false;
            users[uMAX].chnl = NULL;
            users[uMAX].seen = 0;

            //printf ("%s\n", ent->d_name);
            //printf ("%s\n", cJSON_PrintUnformatted(users[uMAX-1]));
            uMAX++;
            free(buffer);
        }
    }
    closedir (dir);
}
//********************* Reading Channels ***********************
void read_chnl () {
    // Open channel directory
    DIR *dir;
    struct dirent *ent;
    dir = opendir ("Resources\\Channels");

    while ((ent = readdir (dir)) != NULL)
    {
        // Ignoring . and ..
        if (*(ent->d_name) != '.')
        {
            char chnl_path[MAX] = "Resources\\Channels\\";

            FILE* fptr = fopen(strcat(chnl_path, ent->d_name), "r");

            fseek(fptr, 0, SEEK_END);
            int fs = ftell(fptr);
            fseek(fptr, 0, SEEK_SET);

            char* buffer = malloc(fs);
            fread(buffer, 1, fs, fptr);


            chnls[cMAX] = cJSON_Parse(buffer);

            //printf ("%s\n", ent->d_name);
            //printf ("%s\n", cJSON_PrintUnformatted(chnls[cMAX]));
            cMAX++;
            free(buffer);
        }
    }
    closedir (dir);
}
//******************** Register  Function **********************
void register_user () {
    // Parse request from client
    char name[MAX] = "";
    char pass[MAX] = "";
    sscanf(req, "register %[^,], %[^\n]", name, pass);

    // Send Error if user already created
    for (int i = 0; i < uMAX; i++) {
        if (strcmp(name,users[i].name) == 0) {
            make_ans("Error", "this username is not available.");
            return;
        }
    }

    // Find path where user file should be saved
    char path[MAX];
    sprintf(path, "%s%s%s", "Resources\\Users\\", name, ".user.json");
    FILE* fptr = fopen(path, "w");

    if (fptr == NULL) {
        make_ans("Error", "this username is not available.");
        return;
    }
    // Saving new user into file
    cJSON* ptr = cJSON_CreateObject();
    cJSON_AddStringToObject(ptr, "username", name);
    cJSON_AddStringToObject(ptr, "password", pass);
    fprintf(fptr, "%s", cJSON_PrintUnformatted(ptr));
    fclose(fptr);

    // Add new user structure into array
    strcpy(users[uMAX].name, name);
    strcpy(users[uMAX].pass, pass);
    users[uMAX].on = false;
    users[uMAX].chnl = NULL;
    users[uMAX].seen = 0;

    uMAX++;

    make_ans("Successful","");
}
//******************* Make JSON For Answer *********************
void make_ans (char* s1, char* s2) {
    // Generate 'ans' JSON for response to client
    cJSON* type = cJSON_CreateString(s1);
    cJSON* content = cJSON_CreateString(s2);
    cJSON_AddItemToObject(ans, "type", type);
    cJSON_AddItemToObject(ans, "content", content);
}
//************************** Log in ****************************
void login_user () {
    // Parse request from client
    char name[MAX] = "";
    char pass[MAX] = "";
    sscanf(req, "login %[^,], %[^\n]", name, pass);
    // Searching for user...
     for (int i = 0; i < uMAX; i++)
    {
        if (strcmp(name,users[i].name) == 0)
        {
            if (users[i].on) {
                make_ans("Error", "The user is already logged in.");
                return;
            }
            if (strcmp(pass,users[i].pass) != 0) {
                make_ans("Error", "Wrong password.");
                return;
            }

            users[i].on = true;
            make_token(i);
            make_ans("AuthToken", users[i].token);
            return;
        }
    }
    // Send Error if user not found
    make_ans("Error","Username is not valid.");
}
//******************** Generate AuthToken **********************
void make_token (int i) {
    // Generating unique random token
    strcpy(users[i].token, users[i].name);
    for (int j = strlen(users[i].name); j < 99; j++) {
        users[i].token[j] = rand()%26 + 'a';
    }
    users[i].token[99] = '\0';
}
//************************* Log  Out ***************************
void logout_user() {
    // Parse request from client
    sscanf(req, "logout %[^\n]", token);
    USER* user = find_token();
    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }
    // Logging out
    user->on = false;
    memset(user->token, 0, 100);
    make_ans("Successful", "");

}
//******************** Find User By Token **********************
USER* find_token () {

    for (int i = 0; i < uMAX; i++) {
        if (users[i].on) {
            if (strcmp(users[i].token, token) == 0)
                return users + i;
        }
    }

    return NULL;
}
//********************** Create Channel ************************
void create_channel () {
    // Parse request from client
    char name[100];
    sscanf(req, "create channel %[^,], %[^\n]", name, token);
    USER* user = find_token();

    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }

    cJSON* chnl;
    for (int i = 0; i < cMAX; i++) {
        // Send Error if channel is already created
        chnl = chnls[i];

        if (strcmp(name, cJSON_GetObjectItemCaseSensitive(chnl, "name")->valuestring) == 0) {
            make_ans("Error", "Channel name is not available.");
            return;
        }
    }

    chnl = cJSON_CreateObject();
    cJSON_AddStringToObject(chnl, "name", name);
    cJSON_AddArrayToObject(chnl, "messages");

    char text[MAX];
    sprintf(text, "%s created %s.", user->name, name);

    add_to_channel(chnl, "server", text);

    chnls[cMAX] = chnl;
    user->chnl = chnl;
    cMAX++;

    make_ans("Successful","");
    return;
}
//********************** Update Channel ************************
void update_channel (cJSON* chnl) {
    // Find path where channel file should be saved
    char* name = cJSON_GetObjectItemCaseSensitive(chnl,"name")->valuestring;
    char path[MAX];
    sprintf(path, "%s%s%s", "Resources\\Channels\\", name, ".channel.json");

    // Save channel into file
    FILE* fptr = fopen(path, "w");

    fprintf(fptr, "%s", cJSON_PrintUnformatted(chnl));
    fclose(fptr);
}
//****************** Add Message To Channel ********************
void add_to_channel (cJSON* chnl, char* sender, char* content) {

    cJSON* messages = cJSON_GetObjectItemCaseSensitive(chnl,"messages");
    cJSON* text = cJSON_CreateObject();

    cJSON_AddStringToObject(text, "sender", sender);
    cJSON_AddStringToObject(text, "content", content);

    cJSON_AddItemToArray(messages, text);

    update_channel(chnl);
}
//********************* Join In Channel  ***********************
void join_channel () {
    // Parse request from client
    char name[100];
    sscanf(req, "join channel %[^,], %[^\n]", name, token);
    USER* user = find_token();

    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }

    // Searching for channel...
    cJSON* chnl = NULL;

    for (int i = 0; i < cMAX; i++) {
        if (strcmp(name, cJSON_GetObjectItemCaseSensitive(chnls[i], "name")->valuestring) == 0) {
            chnl = chnls[i];
            break;
        }
    }
    // Send Error if channel not found
    if (chnl == NULL) {
        make_ans("Error", "Channel not found.");
        return;
    }

    // Join channel
    char text[MAX];
    sprintf(text, "%s joined.", user->name);

    add_to_channel(chnl, "server", text);
    user->chnl = chnl;

    make_ans("Successful","");
}
//******************* Send Message Request *********************
void send_message () {
    char message[MAX];
    int ind = -1, _size = strlen(req);
    for (int i = 0; i < _size; i++)
        if (req[i] == ',')
            ind = i;

    if (ind == -1) {
        make_ans("Error", "Wrong template.");
        return;
    }

    //sscanf(req, "send %[^,], %[^\n]", message, token);
    req[ind] = '\n';
    sscanf(req + 5, "%[^\n]", message);
    sscanf(req + ind + 2, "%[^\n]", token);

    USER* user = find_token();

    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }

    if (user->chnl == NULL) {
        make_ans("Error", "User is not a member of a channel.");
        return;
    }

    add_to_channel(user->chnl, user->name, message);

    make_ans("Successful","");
    return;
}
//********************* Refresh Messages ***********************
void refresh () {
    // Parse request from client
    sscanf(req, "refresh %[^\n]", token);
    USER* user = find_token();
    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }
    // Send Error if user don't have any channel
    if (user->chnl == NULL) {
        make_ans("Error", "User is not a member of a channel.");
        return;
    }

    cJSON_AddStringToObject(ans, "type", "List");
    cJSON* list = cJSON_AddArrayToObject(ans, "content");

    cJSON* messages = cJSON_GetObjectItemCaseSensitive(user->chnl, "messages");
    int num = cJSON_GetArraySize(messages);

    printf("sending from %d to %d\n\n", user->seen, num);
    for (int i = user->seen; i < num; i++)
    {
        cJSON* item = cJSON_GetArrayItem(messages, i);
        char* sender = cJSON_GetObjectItem(item, "sender")->valuestring;
        char* content = cJSON_GetObjectItem(item, "content")->valuestring;

        item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "sender", sender);
        cJSON_AddStringToObject(item, "content", content);
        cJSON_AddItemToArray(list, item);
    }

    user->seen = num;
}
//********************* Channel Members ************************
void members () {
    // Parse request from client
    sscanf(req, "channel members %[^\n]", token);
    USER* user = find_token();
    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }
    // Send Error if user don't have any channel
    if (user->chnl == NULL) {
        make_ans("Error", "User is not a member of a channel.");
        return;
    }

    cJSON_AddStringToObject(ans, "type", "List");
    cJSON* list = cJSON_AddArrayToObject(ans, "content");

    for (int i = 0; i < uMAX; i++) {
        if (users[i].chnl == user->chnl) {
            cJSON_AddItemToArray(list, cJSON_CreateString(users[i].name));
        }
    }
}
//********************** Leave Channel *************************
void leave_channel () {
    // Parse request from client
    sscanf(req, "leave %[^\n]", token);
    USER* user = find_token();
    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }
    // Send Error if user don't have any channel
    if (user->chnl == NULL) {
        make_ans("Error", "User is not a member of a channel.");
        return;
    }

    // Leave channel
    char text[MAX];
    sprintf(text, "%s leaved the channel.", user->name);

    add_to_channel(user->chnl, "server", text);
    user->chnl = NULL;
    user->seen = 0;

    make_ans("Successful","");
}
//*********************** Search User **************************
void search_user() {
    // Parse request from client
    char name[100] = {};
    sscanf(req, "search user %[^,], %[^\n]", name, token);
    USER* user = find_token();
    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }
    // Send Error if user don't have any channel
    if (user->chnl == NULL) {
        make_ans("Error", "You are not a member of a channel.");
        return;
    }

    // Search among channel users to find given username
    for (int i = 0; i < uMAX; i++) {
        if (strcmp(name, users[i].name) == 0)
        {
            if (users[i].chnl == user->chnl) {
                make_ans("Successful","");
                return;
            }
            else {
                make_ans("Error","is not a member of this channel :(");
                return;
            }
        }
    }

    make_ans("Error","is not a valid username.");
}
//******************* Find word in string **********************
bool find (char* source, char* word) {
    char str[MAX];
    strcpy(str, source);

    char* tok = strtok(str, " \n\0\r\t");

    while (tok != NULL) {

        if (strcmp(word, tok) == 0)
            return true;

        tok = strtok(0, " \n\0\r\t");
    }

    return false;
}
//********************** Search Message ************************
void search_message() {
    // Parse request from client
    char word[100] = {};
    sscanf(req, "search message %[^,], %[^\n]", word, token);
    USER* user = find_token();
    // Send Error if user not found
    if (user == NULL) {
        make_ans("Error", "Authentication failed!");
        return;
    }
    // Send Error if user don't have any channel
    if (user->chnl == NULL) {
        make_ans("Error", "You are not a member of a channel.");
        return;
    }

    //
    cJSON_AddStringToObject(ans, "type", "List");
    cJSON* list = cJSON_AddArrayToObject(ans, "content");

    cJSON* messages = cJSON_GetObjectItemCaseSensitive(user->chnl, "messages");
    int num = cJSON_GetArraySize(messages);

    for (int i = 0; i < num; i++)
    {
        cJSON* item = cJSON_GetArrayItem(messages, i);
        char* sender = cJSON_GetObjectItem(item, "sender")->valuestring;
        char* content = cJSON_GetObjectItem(item, "content")->valuestring;

        if (find(content, word)) {
            item = cJSON_CreateObject();
            cJSON_AddStringToObject(item, "sender", sender);
            cJSON_AddStringToObject(item, "content", content);
            cJSON_AddItemToArray(list, item);
        }
    }
}
