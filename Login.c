#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMBERS "data/Members.csv"
#define LOGGEDIN "data/LoggedIn.csv"
#define CATALOGUE "public/views/catalogue.html"

int valid(char *name, char *password, char*line){
    char *tok;
    char **fields;
    fields = malloc(3*sizeof(char*));
    int i = 0;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
        if (i== 3) return 0;
        fields[i++] = strdup(tok);
    }

    if (!strcmp(name, fields[1]) && !strcmp(password, fields[2]))
        return 1;
    else return 0;
}

int main(void){
    FILE *members = fopen(MEMBERS, "r");
    char *name, *password;
    if (!members){
        // TODO: print error, members file not found
    }

    char line[1024];
    while (fgets(line, 1024, members)){
        char* tmp = strdup(line);
        if (valid(name, password, tmp)){
            FILE *loggedin = fopen(LOGGEDIN, "a+");
            FILE *cat = fopen(CATALOGUE, "r");
            fprintf(loggedin, "%s\n", name);
            char out[512];
            while (fgets(out, 512, cat)){
                if (!strncmp(out, "{user}", 6))
                    printf("<input type=\"hidden\" name=\"user\" value=\"%s\"/>", name);
                else
                    printf("%s", out);
            }

            exit(0);
        }
        free(tmp);
    }

    // If we get here, it means incorrect login info
}
