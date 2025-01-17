
typedef enum{
    e_success,
    e_failure
} Status;


typedef struct _mp3Info{
    char* file_name;
    FILE* mp3_pointer;
    char* edit_name;
    char edit_type;
    char* tag_name;
    int len;
    char new_file_name[20];
    int new_or_not;
    FILE* copy_pointer;
} mp3Info;


//checking type of operation
Status Type_Operation(char* argv,mp3Info *MP3INFO);


//view
//for less arguments passed
Status less_arguments();
//for help menu
void help_menu();
//validating the file
Status validate(mp3Info *MP3INFO);
//to convert big to little endianess
int conv_little(int num);
//opening the mp3 file
Status open_mp3_file(mp3Info *MP3INFO);
//checking for id3 tag
Status check_ID3(FILE* fptr);
//checking version
Status check_version(FILE* fptr);
//skip header of 10 bytes
Status skip_header(FILE* fptr);
//reading the tags
Status tag_read(mp3Info *MP3INFO);
//to display tags
Status view(mp3Info *MP3INFO);


//edit
//opening both src and copy file
Status open_copy_file(mp3Info *MP3INFO);
//selecting the tag to edit based on input
Status select_tag_to_edit(mp3Info *MP3INFO);
//confirming whether to make changes in same file or new file
Status changes_to_new_existing(mp3Info *MP3INFO);
//writing new size
int write_size(mp3Info *MP3INFO);
//writing new data
Status write_new_data(mp3Info *MP3INFO);
//finding the tag to be edited
Status tag_edit(mp3Info *MP3INFO);
//copying remaining data
Status copy_remaining(mp3Info *MP3INFO);
//copying all back to src file
Status copy_to_src(mp3Info *MP3INFO);
//to edit tag
Status edit(mp3Info *MP3INFO);
