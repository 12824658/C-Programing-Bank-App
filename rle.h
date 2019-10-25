#include <stdio.h>
#include <stdlib.h>

/*******
* Function Prototypes
*******/
int run_length_encode(FILE *input_file, FILE *output_file);
int run_length_decode(FILE *input_file, FILE *output_file);


int run_length_encode(FILE *input_file, FILE *output_file){
    int i, count = 1;
    char *buffer = NULL;
    size_t size = 0;
    /*check files are not NULL*/
    if((input_file == NULL) || (output_file == NULL)){
        /*Return Failure*/
        return -1;
    }

    /*finds the end of the file*/
    fseek(input_file, 0, SEEK_END);
    /*counts the size of the file*/
    size = ftell(input_file);
    /*move back to the start of the file*/
    rewind(input_file);
    /*allocate an appropriate sized buffer to hold data*/
    buffer = malloc((size + 1) * sizeof(*buffer));
    /*read the input data into a buffer*/
    fread(buffer, size, 1, input_file);
    /*adds end to array*/
    buffer[size] = '\0';

    for(i = 0; i<size; ++i){
        /*looks for a run*/
        if(buffer[i] == buffer[i + 1]){
            ++count;
        }else{
            /*will output once the run is complete or non are identified*/
            fputc(buffer[i], output_file);
            fprintf(output_file, "%d", count);
            count = 1;
        }
    }
    return 1;
}

int run_length_decode(FILE *input_file, FILE *output_file)
{
    int i, j, run;
    char *buffer = NULL;
    size_t size = 0;

    /* validate input and output files */
    if ((NULL == input_file) || (NULL == output_file)){
        return -1;
    }

    fseek(input_file, 0, SEEK_END);
    size = ftell(input_file);
    rewind(input_file);
    buffer = malloc((size + 1) * sizeof(*buffer));
    fread(buffer, size, 1, input_file);
    buffer[size] = '\0';

    for(i = 0; i<size;i+=2){
        run = buffer[i+1] - '0';
        for(j=0; j < run;++j){
            fputc(buffer[i], output_file);
        }
    }

    return 1;
}
