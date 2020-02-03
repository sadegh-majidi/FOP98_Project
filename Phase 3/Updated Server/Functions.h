#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include "cJson.c"

cJSON* Register(char command[1000]);
cJSON* Login(char command[1000]);
cJSON* CreateChannel(char command[1000]);
