#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMBERS "~/public_html/data/Members.csv"
#define LOGGEDIN "~/public_html/data/LoggedIn.csv"
#define CATALOGUE "~/public_html/catalogue.html"
#define ERROR "~/public_html/error.html"

void error(char *message){
    FILE *errorpage = fopen(ERROR, "r");
    if (!errorpage){
        printf("ERROR\n");
        exit (0);
    }
    char out[512];
    while (fgets(out, 512, errorpage)){ // Change lnks because of different path
        if (strstr(out, "{errormessage}"))
            printf("<center><h3 style=\"color:red\">%s</h3></center>\n", message);
        else
            printf("%s", out);
    }
    exit(0);
}

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
        error("Invalid Post Request");
    }

    long len = strtol(len_, NULL, 10);
    char *postdata = malloc(len + 1);
    if (!postdata) {
        error("Invalid Post Request");
    }
    fgets(postdata, len + 1, stdin);

    if (!strstr(postdata, "username") || !strstr(postdata, "password")){
        error("Invalid Form Data");
    }
    char ** ret = malloc(2*sizeof(char*));

    char *sep1 = strchr(postdata, '&');
    if (!sep1){
	exit(0);
        error("Invalid Form Data");
    }
    *sep1 = '\0';
    sep1 += 1;
    char *username = strchr(postdata, '=');
    if (!username){
        error("Invalid Form Data");
    }
    *username = 0;
    username += 1;

    char *password = strchr(sep1, '=');
    if (!password){
	       error("Invalid Form Data");
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
        error("Error: file not found");
    }

    char line[1024];
    while (fgets(line, 1024, members)){
        char* tmp = strdup(line);
        if (valid(user, password, tmp)){
            FILE *loggedin = fopen(LOGGEDIN, "a+");
            FILE *cat = fopen(CATALOGUE, "r");
            if (!loggedin || !cat){
                error("File Not Found");
            }
            fprintf(loggedin, "%s\n", user);
            char out[512];
            while (fgets(out, 512, cat)){
                if (strstr(out, "{user}"))
                    printf("<input type=\"hidden\" name=\"user\" value=\"%s\"/>", user);
                else
                    printf("%s", out);
            }

            exit(0);
        }
        free(tmp);
    }


    error("Invalid login info");
    return 0;
}
