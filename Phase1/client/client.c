///----------------------- In the Name of God --------------------------
///------------------ Student Name: Ali Mahdavifar ---------------------
///------- Chat Application Project - FoP Class - 2020 Winter ----------
/// Department of Computer Engineering | Sharif University of Technology
///---------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "header.c"
#include "cJSON.c"
#include "cJSON.h"
//********************* Global Variables ***********************
char name[MAX], pass[MAX], channel[MAX], token[MAX];
//********************* List of Functions **********************
int account_menu();     // 1
int register_menu();    // 1.1
int login_menu();       // 1.2
int main_menu();        // 2
int creat_channel();    // 2.1
int join_channel();     // 2.2
int logout_menu();      // 2.3
int chat_menu();        // 3
int send_message();     // 3.1
int refresh();          // 3.2
int members();          // 3.3
int leave_channel();    // 3.4
//************************ int main() **************************
int main() {
    int state = 1;

    while (true)
    {
        system("cls");

        switch (state)
        {
        case 0:
            return 0;
        case 1:
            state = account_menu();
            break;
        case 11:
            state = register_menu();
            break;
        case 12:
            state = login_menu();
            break;
        case 2:
            state = main_menu();
            break;
        case 21:
            state = creat_channel();
            break;
        case 22:
            state = join_channel();
            break;
        case 23:
            state = logout_menu();
            break;
        case 3:
            state = chat_menu();
            break;
        case 31:
            state = send_message();
            break;
        case 32:
            state = refresh();
            break;
        case 33:
            state = members();
            break;
        case 34:
            state = leave_channel();
            break;
        }
    }
}
//********************* 1. Account Menu ************************
int account_menu () {
    cprintf(11, "Account Menu:\n\n");
    cprintf(11, "1: Register\n");
    cprintf(11, "2: Login\n");
    cprintf(11, "3: Exit\n");
    char inp;
    while (true) {
        inp = getch();
        if (inp == '1')
            return 11;
        if (inp == '2')
            return 12;
        if (inp == '3')
            return 0;
    }
}
//******************** 1.1. Register Menu **********************
int register_menu () {
    // User Interface
    cprintf(11, "Enter Username:\n");
    char name[MAX];
    cscanf(240, " %s", name);
    cprintf(11, "Enter Password:\n");
    char pass[MAX];
    cscanf(240, " %s", pass);


    // Send the message to server
    _send ("register %s, %s\n", name, pass);

    // Read the message from server and copy it to buffer
    char* buffer = _recv();

    // Print the whole JSON
    //printf("server: %s\n", buffer);

    // Parsing JSON
    cJSON* json = cJSON_Parse(buffer);
    cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON* content = cJSON_GetObjectItemCaseSensitive(json, "content");

    // Check if some Error happened
    if (strcmp(type->valuestring, "Error") == 0)
    {
        cprintf(4, "Error! %s\n", content->valuestring);
    }

    else // == Successful
    {
        // User created successfully
        cprintf(10, "User \"%s\" successfully registered :)\n", name);

    }

    // Wait and return
    system("pause");
    return 1;
}
//********************* 1.2. Login Menu ************************
int login_menu () {
    // User Interface
    cprintf(11, "Enter Username:\n");
    cscanf(240, " %s", name);
    cprintf(11, "Enter Password:\n");
    cscanf(240, " %s", pass);


    // Send the message to server
    _send ("login %s, %s\n", name, pass);

    // Read the message from server and copy it to buffer
    char* buffer = _recv();

    // Print the whole JSON
    //printf("server: %s\n", buffer);

    // Parsing JSON
    cJSON* json = cJSON_Parse(buffer);
    cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON* content = cJSON_GetObjectItemCaseSensitive(json, "content");

    int state = 0;

    // Check if some Error happened
    if (strcmp(type->valuestring, "Error") == 0)
    {
        cprintf(4, "Error! %s\n", content->valuestring);

        state = 1;
    }

    else // == Successful
    {
        // User logged in successfully
        strcpy(token, content->valuestring);
        cprintf(10, "User \"%s\" successfully logged in :)\n", name);

        state = 2;
    }

    // Wait and return
    system("pause");
    return state;
}
//*********************** 2. Main Menu *************************
int main_menu () {
    cprintf(11, "Main Menu:\n\n");
    cprintf(11, "1: Create Channel\n");
    cprintf(11, "2: Join Channel\n");
    cprintf(11, "3: Logout\n");

    char inp;
    while (true) {
        inp = getch();
        if (inp == '1')
            return 21;
        if (inp == '2')
            return 22;
        if (inp == '3')
            return 23;
    }
}
//******************* 2.1. Create Channel **********************
int creat_channel() {
    // User interface
    cprintf(11, "Enter New Channel Name:\n");
    cscanf(240, " %s", channel);
    _send("create channel %s, %s\n", channel, token);

    // Read the message from server and copy it to buffer
    char* buffer = _recv();

    // Print the whole JSON
    //printf("server: %s\n", buffer);

    // Parsing JSON
    cJSON* json = cJSON_Parse(buffer);
    cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON* content = cJSON_GetObjectItemCaseSensitive(json, "content");

    int state = 0;

    // Check if some Error happened
    if (strcmp(type->valuestring, "Error") == 0)
    {
        cprintf(4, "Error! %s\n", content->valuestring);
        state = 2;
    }

    else // == Successful
    {
        // User created successfully
        cprintf(10, "Channel \"%s\" successfully created :)\n", channel);

        state = 3;
    }

    // Wait and return
    system("pause");
    return state;
}
//******************** 2.2. Join Channel ***********************
int join_channel () {
    //User interface
    cprintf(11, "Enter Existed Channel Name:\n");
    cscanf(240, " %s", channel);
    _send("join channel %s, %s\n", channel, token);

    // Read the message from server and copy it to buffer
    char* buffer = _recv();

    // Print the whole JSON
    //printf("server: %s\n", buffer);

    // Parsing JSON
    cJSON* json = cJSON_Parse(buffer);
    cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON* content = cJSON_GetObjectItemCaseSensitive(json, "content");

    int state = 0;

    // Check if some Error happened
    if (strcmp(type->valuestring, "Error") == 0)
    {
        cprintf(4, "Error! %s\n", content->valuestring);
        state = 2;
    }

    else // == Successful
    {
        // User created successfully
        cprintf(10, "You successfully joined \"%s\" channel :)\n", channel);

        state = 3;
    }

    // Wait and return
    system("pause");
    return state;
}
//********************* 2.3. Logout Menu ***********************
int logout_menu () {
    // Send request to server
    _send("logout %s\n", token);
    char* buffer = _recv();

    // User interface
    cprintf(10, "User \"%s\" successfully logged out :)\n", name);

    // Wait and return
    system("pause");
    return 1;
}
//*********************** 3. Chat Menu *************************
int chat_menu () {
    cprintf(11, "Chat Menu:\n\n");
    cprintf(11, "1: Send Message\n");
    cprintf(11, "2: Refresh\n");
    cprintf(11, "3: Channel Members\n");
    cprintf(11, "4: Leave Channel\n");

    char inp;
    while (true) {
        inp = getch();
        if (inp == '1')
            return 31;
        if (inp == '2')
            return 32;
        if (inp == '3')
            return 33;
        if (inp == '4')
            return 34;
    }
}
//******************** 3.1. Send Message ***********************
int send_message () {
    // User interface: Get message
    char text[MAX];
    cprintf(11, "Enter Your Message:\n");
    cscanf(240, " %[^\n]", text);

    // Send message to server
    _send("send %s, %s\n", text, token);

    // User interface: Response to action
    cprintf(10, "Message sent to channel :)\n");

    // Wait and return
    system("pause");
    return 3;
}
//******************** 3.2. Refresh Menu ***********************
int refresh () {
    // Send request to server
    _send ("refresh %s\n", token);

    // Read the message from server and copy it to buffer
    char* buffer = _recv();

    // Print the whole JSON
    //printf("server: %s\n", buffer);

    // Parsing JSON
    cJSON* json = cJSON_Parse(buffer);
    cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON* content = cJSON_GetObjectItemCaseSensitive(json, "content");

    int num = cJSON_GetArraySize(content);

    // Check if new massage exits
    if (num == 0) {
        cprintf(4, "No New Message!\n\n");
    }
    else {
        cprintf(11, "New Messages:\n\n");
    }

    for (int i = 0; i < cJSON_GetArraySize(content); i++)
    {
        // Parsing new message
        cJSON* item = cJSON_GetArrayItem(content, i);
        cJSON* sender = cJSON_GetObjectItemCaseSensitive(item, "sender");
        cJSON* text = cJSON_GetObjectItemCaseSensitive(item, "content");

        // Set message color based on sender
        int col1, col2;
        if (strcmp(sender->valuestring, "server") == 0) {
            col1 = 128;
            col2 = 8;
        }
        else if (strcmp(sender->valuestring, name) == 0) {
            col1 = 10;
            col2 = 32;
        }
        else {
            col1 = 15;
            col2 = 240;
        }

        // Print new message
        cprintf(col1, "%s:", sender->valuestring);
        printf(" ");
        cprintf(col2, "%s\n\n", text->valuestring);
    }

    // Wait and return
    system("pause");
    return 3;
}
//******************** 3.3. Members Menu ***********************
int members () {
    // Send request to server
    _send ("channel members %s\n", token);

    // Read the message from server and copy it to buffer
    char* buffer = _recv();

    // Print the whole JSON
    //printf("server: %s\n", buffer);

    // Parsing JSON
    cJSON* json = cJSON_Parse(buffer);
    cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON* content = cJSON_GetObjectItemCaseSensitive(json, "content");

    int num = cJSON_GetArraySize(content);

    cprintf(11, "List of %d Members:\n\n", num);

    for (int i = 0; i < cJSON_GetArraySize(content); i++)
    {
        // Parsing new member
        cJSON* member = cJSON_GetArrayItem(content, i);

        cprintf(240, "%3d)", i+1);
        cprintf(15, " %s\n\n", member->valuestring);
    }

    // Wait and return
    system("pause");
    return 3;
}
//******************** 3.4. Leave Channel **********************
int leave_channel() {
    // Send request to server
    _send("leave %s\n", token);
    char* buffer = _recv();

    // User interface
    cprintf(10, "You successfully leaved \"%s\" channel :)\n", channel);

    // Wait and return
    system("pause");
    return 2;
}
