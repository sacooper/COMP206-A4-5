#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMBERS "data/Members.csv"
#define LOGGEDIN "data/LoggedIn.csv"
#define CATALOGUE "public/views/catalogue.html"

int valid(char *name, char *password, char *line){
    char *tok;
    char **fields;
    fields = malloc(3*sizeof(char*));
    int i = 0;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
        if (i== 3) return 0;
        fields[i++] = strdup(tok);
    }
    if (!strcmp(name, fields[1]) && !strcmp(password, fields[2])) return 1;
    else return 0;
}

char** parsePostRequest(){
    char *len_ = getenv("CONTENT_LENGTH");
    if (!len_){
        // invalid post request
        exit(1);
    }



    long len = strtol(len_, NULL, 10);
    char *postdata = malloc(len + 1);
    if (!postdata) {
        // TODO: error page...
    }
    fgets(postdata, len + 1, stdin);

    char ** ret = malloc(2*sizeof(char*));

    char *sep1 = strchr(postdata, '&');
    if (!sep1){
        // invalid input - not enough args
    }
    *sep1 = '\0';
    sep1 += 1;
    char *username = strchr(postdata, '=');
    if (!username){
        // invalid format for post args
    }
    *username = 0;
    username += 1;  // Put username string to

    char *password = strchr(sep1, '=');
    if (!password){
        // invalid format for post args
    }
    *password = 0;
    password+=1;
    ret[0] = strdup(username);
    ret[1] = strdup(password);
    free(postdata);
    return ret;
}

int main(void){
    printf("Content-Type:text/html%c%c\n", 13, 10);

    char **userAndPass = parsePostRequest();
    char *user = userAndPass[0];
    char *password = userAndPass[1];

    FILE *members = fopen(MEMBERS, "r");

    if (!members){
        // TODO: print error, members file not found
        printf("ERROR!");
        return 0;
    }

    char line[1024];
    while (fgets(line, 1024, members)){
        char* tmp = strdup(line);
        if (valid(user, password, tmp)){
            FILE *loggedin = fopen(LOGGEDIN, "a+");
            FILE *cat = fopen(CATALOGUE, "r");

            fprintf(loggedin, "%s\n", user);
            char out[512];
            while (fgets(out, 512, cat)){
                if (!strncmp(out, "{user}", 6))
                    printf("<input type=\"hidden\" name=\"user\" value=\"%s\"/>", user);
                else
                    printf("%s", out);
            }

            exit(0);
        }
        free(tmp);
    }

    // If we get here, it means incorrect login info
}
