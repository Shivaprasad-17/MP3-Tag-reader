#include <stdio.h>
#include <string.h>
#include "mp3.h"

mp3Info MP3INFO;

int main(int argc,char* argv[]){

    if(argc<3){
        less_arguments();
        return e_failure;

        if(argc != 1)
        Type_Operation(argv[1],&MP3INFO);
        
        return e_failure;
    }
    
    //for CLA == 3
   else if(argc == 3){
    //copying file name to structure
    MP3INFO.file_name = argv[2];
    //checking operation type
    Type_Operation(argv[1],&MP3INFO);
    }
    //if CLA == 5
    else if(argc == 5){
        //copying file name to structure
        MP3INFO.file_name = argv[4];
        //copying type of tag to add
        MP3INFO.edit_type = argv[2][1];
        //new data for the tag
        MP3INFO.edit_name = argv[3];
        Type_Operation(argv[1],&MP3INFO);
    }
    else{
        printf("ERROR: Invalid arguments\n");
    }
}

//checking type of operation
Status Type_Operation(char* argv,mp3Info *MP3INFO){
    //for displaying help menu
    if(strcmp(argv,"--help")==0){
        help_menu();
    }
    //for viewing details
    else if(strcmp(argv,"-v")==0){
        if(view(MP3INFO)==e_failure){
            return e_failure;
        }
    }
    //for editing details
    else if(strcmp(argv,"-e")==0){
        if(edit(MP3INFO)==e_failure){
            return e_failure;
        }
    }
    //for invalid arguments
    else{
        printf("ERROR: Invalid arguments\n");
    }
}
