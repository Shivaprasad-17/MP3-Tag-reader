#include <stdio.h>
#include <string.h>
#include "mp3.h"

//when CLA is less than 2
Status less_arguments(){
    printf("\n------------------------------------------------------------------\n");
    printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
    printf("USAGE :\nTo view please pass like: ./a.out -v mp3filename\n");
    printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    printf("To get help pass like : ./a.out --help\n");
    printf("------------------------------------------------------------------\n\n");
}

//help menu
void help_menu(){
    printf("\n-----------------HELP MENU---------------\n");
    printf("1. -v -> to view mp3 file contents\n2. -e -> to edit mp3 file contents\n");
    printf("\t 2.1. -t -> to edit song title\n\t 2.2. -a -> to edit artist name\n\t 2.3. -A -> to edit album name\n");
    printf("\t 2.4. -y -> to edit year\n\t 2.5. -m -> to edit content\n\t 2.6. -c -> to edit comment\n");
    printf("------------------------------------------\n\n");
}

Status validate(mp3Info *MP3INFO){
    char* str = strchr(MP3INFO->file_name,'.');
    if(str == NULL){
        printf("ERROR: %s is not a .mp3 file\n",MP3INFO->file_name);
        return e_failure;
    }
    else if(strcmp(str,".mp3")!=0){
        printf("ERROR: %s is not a .mp3 file\n",MP3INFO->file_name);
        return e_failure;
    }
}

//in mp3 file the data is stored in big endian formate, but our system is little endian so converting from big to little
int conv_little(int num){
    //typecasting num to char pointer to acess each byte
    char* cptr = (char*)&num;
    //swapping first and last byte
    int temp = cptr[0];
    cptr[0] = cptr[3];
    cptr[3]=temp;

    //swapping second and third byte
    temp=cptr[1];
    cptr[1]=cptr[2];
    cptr[2]=temp;

    return num;
}

// copying tag name based on the argument
Status select_tag_to_edit(mp3Info *MP3INFO){
    char option = MP3INFO->edit_type;
    switch(option){
        case 't': MP3INFO->tag_name = "TIT2";
        break;

        case 'a': MP3INFO->tag_name = "TPE1";
        break;
        
        case 'A': MP3INFO->tag_name = "TALB";
        break;

        case 'y': MP3INFO->tag_name = "TYER";
        break;

        case 'm': MP3INFO->tag_name = "TCON";
        break;

        case 'c': MP3INFO->tag_name = "COMM";
        break;

        default : printf("ERROR: Invalid Arguments!\n");
        return e_failure;
    }
}

//opening the src mp3 file in read mode
Status open_mp3_file(mp3Info *MP3INFO){
    MP3INFO->mp3_pointer = fopen(MP3INFO->file_name,"r");
    if(MP3INFO->mp3_pointer == NULL){
        printf("%s file not found!\n",MP3INFO->file_name);
        return e_failure;
    }
}

//checking ID3 exist or not
Status check_ID3(FILE* fptr){
    char buffer[4];
    fread(buffer,1,3,fptr);
    if(strcmp(buffer,"ID3")){
        printf("ERROR: ID3 tag does not exist\n");
        return e_failure;
    }
}

//checking version is v2 or not
Status check_version(FILE* fptr){
    short ver=0;
    fread(&ver,2,1,fptr);
    if(ver!=3){
        printf("ERROR: Not a file of version v2\n");
        return e_failure;
    }
}

//skipping 10 bytes header part
Status skip_header(FILE* fptr){
    rewind (fptr);
    fseek(fptr,10,SEEK_SET);
}

//view details of song
Status tag_read(mp3Info *MP3INFO){
    printf("\n------SELECTED VIEW DETAILS-------\n");
    printf("--------------------------------------------------------------------\n");
    printf("\t\tMP3 TAG READER AND EDITOR FOR ID3v2\t\n");
    printf("--------------------------------------------------------------------\n");
    //loop for 6 times, since tags we are viewing is 6
    for(int i=0;i<6;i++){
    char tag[4];
    int size=0;
    //reading tag name
    fread(tag,1,4,MP3INFO->mp3_pointer);
    //reading the size of tag
    fread(&size,4,1,MP3INFO->mp3_pointer);
    //converting big to little endianess
    size = conv_little(size);
    //taking buffer of size
    char tag_data[size];
    memset(tag_data,0,size);
    //decrementing the size
    size--;
    //skipping 3 bytes (2 flags and one null char);
    fseek(MP3INFO->mp3_pointer,3,SEEK_CUR);

    //copying tag data
    fread(tag_data,1,size,MP3INFO->mp3_pointer);

    //Checking tag name and displaying respectively the data
    if(strcmp(tag,"TIT2")==0){
        printf("TITLE\t :\t%s\n",tag_data);
    }
    else if(strcmp(tag,"TPE1")==0){
        printf("ARTIST\t :\t%s\n",tag_data);
    }
    else if(strcmp(tag,"TALB")==0){
        printf("ALBUM\t :\t%s\n",tag_data);
    }
    else if(strcmp(tag,"TYER")==0){
        printf("YEAR\t :\t%s\n",tag_data);
    }
    else if(strcmp(tag,"TCON")==0){
        printf("MUSIC\t :\t%s\n",tag_data);
    }
    else if(strcmp(tag,"COMM")==0){
        printf("COMMENT\t :\t%s\n",tag_data);
    }
  }
  printf("---------------------------------------------------------------------\n");
}

//while editing
Status changes_to_new_existing(mp3Info *MP3INFO){
    int choice;
    //whether to create new file or not
    printf("Enter where to make changes\n1.Save in another file\n2.Save in same file\nEnter choice: ");
    scanf(" %d",&choice);
    if(choice == 1){
        //creating new mp3 file
        printf("Enter the new file name: ");
        scanf(" %s",MP3INFO->new_file_name);
        //get address of '.'
        char* ch = strchr(MP3INFO->new_file_name,'.');
        //storing name to array
        MP3INFO->new_or_not = 1;
        //file with no extension
        if(ch == NULL){
            printf("ERROR: %s is not an .mp3 file\n",MP3INFO->new_file_name);
            return e_failure;
        }
        //checking whether the file is mp3 or not
        else if(strcmp(ch,".mp3")!=0){
            printf("ERROR: %s is not an .mp3 file\n",MP3INFO->new_file_name);
            return e_failure;
        }
        printf("Created file %s\n",MP3INFO->new_file_name);
    }
    //to make changes in same file
    else if(choice==2){
        strcpy(MP3INFO->new_file_name,"copy.mp3");
    }
    else{
        printf("Error: invalid choice\n");
        return e_failure;
    }
}


//opening copy file and src mp3 file
Status open_copy_file(mp3Info *MP3INFO){
    MP3INFO->copy_pointer = fopen(MP3INFO->new_file_name,"w+");
    MP3INFO->mp3_pointer = fopen(MP3INFO->file_name,"r");
    if(MP3INFO->mp3_pointer == NULL){
        printf("%s file not found!\n",MP3INFO->file_name);
    }
}

//writing new size to copy pointer
int write_size(mp3Info *MP3INFO){
    //size based on string length
    int len = strlen(MP3INFO->edit_name);
    MP3INFO->len = len;
    //incrementing len
    len++;
    //converting little to big;
    len = conv_little(len);
    //storing to copy pointer
    fwrite(&len,sizeof(int),1,MP3INFO->copy_pointer);
    return len;
}

//writing new data of the tag
Status write_new_data(mp3Info *MP3INFO){
    //copy flag
    char flag[3];
    fread(flag,1,3,MP3INFO->mp3_pointer);
    fwrite(flag,1,3,MP3INFO->copy_pointer);

    //writing new data passed in CLA
    fwrite(MP3INFO->edit_name,1,MP3INFO->len,MP3INFO->copy_pointer);
    printf("Edited successfully\n");
}

//finding tag details
Status tag_edit(mp3Info *MP3INFO){
    char tag[4];
    //running loop till finding the tag to be edited
        while(1){
            fread(tag,1,4,MP3INFO->mp3_pointer);
            //comparing every 4 bytes
            //if tag is matched
            if(strcmp(tag,MP3INFO->tag_name)==0){
                //writing the tag name
                fwrite(tag,1,4,MP3INFO->copy_pointer);
                //writing new size
                write_size(MP3INFO);
                //writing new data
                write_new_data(MP3INFO);
                break;
            }
            //if tag is not found
            else{
                //taking the offset back 3 bytes, because tag name can be shuffled
                fseek(MP3INFO->mp3_pointer,-3,SEEK_CUR);
                //writing the same byte to copy
                fwrite(tag,1,1,MP3INFO->copy_pointer);
            }
    }
}

//copying remaining data
Status copy_remaining(mp3Info *MP3INFO){

    //skipping the old details of tag in src file
    int size;
    fread(&size,4,1,MP3INFO->mp3_pointer);
    size--;
    fseek(MP3INFO->mp3_pointer,size,SEEK_CUR);

    //copying all data till end of file
    int ch;
    while((ch = getc(MP3INFO->mp3_pointer))!=EOF){
        putc(ch,MP3INFO->copy_pointer);
    }
}

//to make the same changes in the src file
Status copy_to_src(mp3Info *MP3INFO){
    //rewinding the copy pointer
    rewind(MP3INFO->copy_pointer);
    //opening the src file in write mode
    FILE* fptr = fopen(MP3INFO->file_name,"w");
    int ch;
    //writing all data to src file
    while((ch = getc(MP3INFO->copy_pointer))!=EOF){
        putc(ch,fptr);
    }
    //closing both the pointers
    fclose(MP3INFO->copy_pointer);
    fclose(fptr);
    //removing the copy file
    remove("copy.mp3");
}


Status view(mp3Info *MP3INFO){
    if(validate(MP3INFO)==e_failure){
        return e_failure;
    }
    if(open_mp3_file(MP3INFO)==e_failure){
        return e_failure;
    }
    if(check_ID3(MP3INFO->mp3_pointer)==e_failure){
        return e_failure;
    }
    if(check_version(MP3INFO->mp3_pointer)==e_failure){
        return e_failure;
    }
    if(skip_header(MP3INFO->mp3_pointer)==e_failure){
        return e_failure;
    }
    if(tag_read(MP3INFO)==e_failure){
        return e_failure;
    }
}

Status edit(mp3Info *MP3INFO){
    if(validate(MP3INFO)==e_failure){
        return e_failure;
    }
    if(select_tag_to_edit(MP3INFO)==e_failure){
        return e_failure;
    }
    if(changes_to_new_existing(MP3INFO)==e_failure){
        return e_failure;
    }
    if(open_copy_file(MP3INFO)==e_failure){
        return e_failure;
    }
    if(tag_edit(MP3INFO)==e_failure){
        return e_failure;
    }
    if(copy_remaining(MP3INFO)==e_failure){
        return e_failure;
    }
    if(MP3INFO->new_or_not!=1){
        if(copy_to_src(MP3INFO)==e_failure){
            return e_failure;
        }
    }
}