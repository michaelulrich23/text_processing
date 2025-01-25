#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]){
    int opt, i, newTextIndex = 0, wordInc=0, sepInc=0, wordInc2, sepInc2;
    char text[1001];
    short int keepDigits=0, keepPuncts=0, toLowercase=0, toUppercase=0, sensitive=0, insensitive=0, wordsFirst=0,
    separatorsFirst=0, nonOptionBool=0, strCompare=0;
    char* optstring = ":dpulr:R:";
    char nonOption[100][100] = {{'\0'}};
    char replace_string[100];
    char words[100][100] = {{'\0'}};
    char separators[100][100] = {{'\0'}};
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case ':':
                return 2;
            case 'R':
                insensitive = 1;
                strcpy(replace_string, optarg);
                break;
            case 'r':
                sensitive = 1;
                strcpy(replace_string, optarg);
                break;
            case 'u':
                toUppercase = 1;
                break;
            case 'l':
                toLowercase = 1;
                break;
            case 'd':
                keepDigits = 1;
                break;
            case 'p':
                keepPuncts = 1;
                break;
            default:
                return 1;
        }
    }

    if(optind < argc){
        nonOptionBool = 1;
        for(int j=0;optind<argc;optind++, j++){
            strcpy(nonOption[j], argv[optind]);
        }
    }
    i=0;

    while (fgets(text, 1001, stdin) != NULL && text[0] != '\n' && text[0] != '\r'){ // && text[0] != '\r'
        char newText[strlen(text)+1];
        while(text[i]){
            if(isalpha(text[i]) || isspace(text[i]) || (keepPuncts == 1 && ispunct(text[i])) || (keepDigits == 1 && isdigit(text[i])) ){
                if(toLowercase)
                    newText[newTextIndex] = tolower(text[i]);
                else if(toUppercase)
                    newText[newTextIndex] = toupper(text[i]);
                else
                    newText[newTextIndex] = text[i];
                newTextIndex++;
            }
            i++;
        }

        newText[newTextIndex] = '\0';
        newTextIndex = 0;
        if(sensitive || insensitive){
            for (i = 0; newText[i] != '\0';) { // && newText[i] != '\n' ------------------
                if(isalnum(newText[i])){
                    if(i==0)
                        wordsFirst = 1;
                    for (wordInc2 = 0; isalnum(newText[i]) ; wordInc2++) {
                        words[wordInc][wordInc2] = newText[i];
                        i++;
                    }
                    wordInc++;
                }
                else{
                    if(i==0)
                        separatorsFirst = 1;
                    for (sepInc2 = 0; !isalnum(newText[i]) && newText[i] != '\0'; sepInc2++) { //&& newText[i] != '\n'
                        separators[sepInc][sepInc2] = newText[i];
                        i++;
                    }
                    sepInc++;
                }
            }
        }

        if(sensitive){
            if(nonOptionBool == 0 || nonOption[0][0] == '\0'){ //nonOptionBool == 0
                for (i = 0; words[i][0] != '\0' ; i++) {
                    if((strlen(words[i]) > strlen(replace_string)) || (strlen(words[i]) == strlen(replace_string))){ //ak je slovo dlhsie alebo rovne ako replace
                        for (int j = 0; replace_string[j] != '\0'; j++) {
                            words[i][j] = replace_string[j];
                        }
                    }
                    else if(strlen(words[i]) < strlen(replace_string)){ //kratsie ako replace
                        for (int j = 0; words[i][j] != '\0' ; j++) {
                            words[i][j] = replace_string[j];
                        }
                    }
                }
            }
            else{
                for (i = 0; words[i][0] != '\0' ; i++) {
                    for (int j = 0; nonOption[j][0] != '\0' ; j++) {
                        if(strncmp(words[i], nonOption[j], strlen(nonOption[j])) == 0 ){
                            strCompare = 1;
                            break;
                        }
                    }
                    if(strCompare){
                        if((strlen(words[i]) > strlen(replace_string)) || (strlen(words[i]) == strlen(replace_string))){ //ak je slovo dlhsie alebo rovne ako replace
                            for (int j = 0; replace_string[j] != '\0'; j++) {
                                words[i][j] = replace_string[j];
                            }
                        }
                        else if(strlen(words[i]) < strlen(replace_string)){ //kratsie ako replace
                            for (int j = 0; words[i][j] != '\0' ; j++) {
                                words[i][j] = replace_string[j];
                            }
                        }
                        strCompare = 0;
                    }
                }
            }
        }
        else if(insensitive) {
            if (nonOptionBool == 0 || nonOption[0][0] == '\0') {
                for (i = 0; words[i][0] != '\0'; i++) {
                    if ((strlen(words[i]) > strlen(replace_string)) || (strlen(words[i]) == strlen(replace_string))) {
                        for (int j = 0; replace_string[j] != '\0'; j++) {
                            if (isalpha(words[i][j])) {
                                if (islower(words[i][j]))
                                    words[i][j] = tolower(replace_string[j]);
                                else
                                    words[i][j] = toupper(replace_string[j]);
                            } else
                                words[i][j] = replace_string[j];
                        }
                    } else if (strlen(words[i]) < strlen(replace_string)) {
                        for (int j = 0; words[i][j] != '\0'; j++) {
                            if (isalpha(words[i][j])) {
                                if (islower(words[i][j]))
                                    words[i][j] = tolower(replace_string[j]);
                                else
                                    words[i][j] = toupper(replace_string[j]);
                            } else
                                words[i][j] = replace_string[j];
                        }
                    }
                }
            } else {
                for (i = 0; words[i][0] != '\0'; i++) {
                    for (int j = 0; nonOption[j][0] != '\0'; j++) {
                        if (strncasecmp(words[i], nonOption[j], strlen(nonOption[j])) == 0) {
                            strCompare = 1;
                            break;
                        }
                    }
                    if (strCompare) {
                        if ((strlen(words[i]) > strlen(replace_string)) ||
                            (strlen(words[i]) == strlen(replace_string))) {
                            for (int j = 0; replace_string[j] != '\0'; j++) {
                                if (isalpha(words[i][j])) {
                                    if (islower(words[i][j]))
                                        words[i][j] = tolower(replace_string[j]);
                                    else
                                        words[i][j] = toupper(replace_string[j]);
                                } else
                                    words[i][j] = replace_string[j];
                            }
                        } else if (strlen(words[i]) < strlen(replace_string)) {
                            for (int j = 0; words[i][j] != '\0'; j++) {
                                if (isalpha(words[i][j])) {
                                    if (islower(words[i][j]))
                                        words[i][j] = tolower(replace_string[j]);
                                    else
                                        words[i][j] = toupper(replace_string[j]);
                                } else
                                    words[i][j] = replace_string[j];
                            }
                        }
                        strCompare = 0;
                    }
                }
            }
        }


        wordInc=0;
        sepInc=0;
        if(sensitive || insensitive) {
            if (wordsFirst) {
                printf("%s", words[0]);
                wordInc=1;
            } else if (separatorsFirst) {
                printf("%s", separators[0]);
                sepInc=1;
            }
            for (i=0;; i++) {
                if(words[i][0] == '\0')
                    break;
                else{
                    if(wordsFirst && i==0){

                    }
                    else{
                        printf("%s", words[wordInc]);
                        wordInc++;
                    }
                }
                if(separators[i][0] == '\0')
                    break;
                else{
                    printf("%s", separators[sepInc]);
                    sepInc++;
                }
            }
            wordInc=0;
            sepInc=0;
            wordsFirst=0;
            separatorsFirst=0;
            //printf("\n");
            for (int j = 0; j < strlen(newText) ; j++) {
                for (int k = 0; k < strlen(newText); k++) {
                    //newText[j] = '\0';
                    words[j][k] = '\0';
                    separators[j][k] = '\0';
                }
            }
            //printf("\n");
        }
        else{
            printf("%s", newText);
        }
        i=0;
    }
    /*
    for (int j = 0; j < *nonOption[j] != '\0'; j++) {
        printf("\n%s", nonOption[j]);
    }
    printf("\nreplace_string = %s\n", replace_string);
    */
    return 0;
}