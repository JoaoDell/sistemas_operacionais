/*João Victor Dell Agli Floriano - 10799783
Sistemas Operacionais - 2022.1*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_LINE_SIZE 500
#define MAX_WORD_SIZE 100


enum Error {SUCESS, FAILURE, BAD_ARGUMENT, BAD_MALLOC};


// Struct para guardar a quantidade de palavras em uma string linha e sua localização na mesma 
typedef struct word_buffer{

    // buffer para guardar a linha
    char buffer[MAX_LINE_SIZE];

    // número de palavras
    unsigned int numb_words;

    // Array de tamanho numb_words com cada índice guardando o inicio de cada palavra na linha
    unsigned int *words_offset;
} word_buffer;



word_buffer *buffer_init(unsigned int n_words){
    // Função de inicialização de um word_buffer

    word_buffer *w = (word_buffer *) malloc(sizeof(word_buffer));
    if(w == NULL) {
        fprintf(stderr, "word_buffer could not be allocated!\n");
        return NULL;
    }

    w->numb_words = n_words;
    w->words_offset = (unsigned int *) malloc(n_words*sizeof(unsigned int));
    if(w == NULL) {
        fprintf(stderr, "word_buffer->words_offset could not be allocated!\n");
        return NULL;
    }

    // A localização da primeira palavra é sempre a primeira casa da linha
    w->words_offset[0] = 0;

    return w;
}


void buffer_destroy(word_buffer * buffer){
    // Função para liberar o espaço ocupado por um word_buffer
    free(buffer->buffer);
    free(buffer->words_offset);
    free(buffer);
}


void lower_converter(char *line, unsigned int line_length){
    //Conversor de letras maiúsculas para minúsculas
    for(int i = 0; i < line_length; i++){
        if(line[i] > 64 && line[i] < 91)
            line[i] += 32;
    }
}


int word_getter(char *origin, word_buffer **buffer){
    //Função para coletar a quantidade de palavras em uma linha e suas respectivas localizações 

    char getter = '\0';
    unsigned int i = 0;
    unsigned int numb_words = 0;
    unsigned int str_counter = 0;
    unsigned int len = strlen(origin);

  

    while (i < len){
        getter = origin[i];
        if(getter == ' ' || getter == '\n') numb_words++;
        i++;
    }

    *buffer = buffer_init(numb_words);
    if(buffer ==  NULL){
        fprintf(stderr, "Buffer could not be allocated!\n");
        return BAD_MALLOC;
    }


    getter = '\0';
    i = 0;

    while(i < len){
        getter = origin[i];
        (*buffer)->buffer[i] = getter;
        i++;
        if(getter == ' ') {
            str_counter++;
            (*buffer)->words_offset[str_counter] = i;
        }
    }

    return SUCESS;

}









int main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(stderr, "Number of arguments must be exactly three!\n");
        return BAD_ARGUMENT;
    }

    FILE *f = fopen(argv[2], "r");

    char line_buffer[MAX_LINE_SIZE];
    char string_buffer[MAX_WORD_SIZE];

    unsigned int linecount = 0;
    unsigned int line_length = 0;
    unsigned int str_length = 0;


    word_buffer *buffer;

    while(!feof(f)){

        //Zerando o buffer de linha para evitar qualquer char indesejado vindo da memória
        for(int j = 0; j < MAX_LINE_SIZE; j++) 
            line_buffer[j] = '\0';

        fgets(line_buffer, MAX_WORD_SIZE, f);
        linecount++;

        line_length = strlen(line_buffer);

        /* Lidando com um problema de leitura causado pelo EOF, 
        fazendo com que a última palavra da última linha não fosse lida*/
        if(line_buffer[line_length - 1] != '\n'){
            line_buffer[line_length] = '\n';
            line_length = strlen(line_buffer);
        }
    

        // Coletando as palavras
        if( word_getter(line_buffer, &buffer) != SUCESS ){
            fprintf(stderr, "Words could not get read!\n");
            return FAILURE;
        };



        for(int i = 0; i < buffer->numb_words; i++){
            // PARTE DE ANÁLISE DAS PALAVRAS

            //Zerando o buffer de string para evitar qualquer char indesejado vindo da memória
            for(int j = 0; j < MAX_WORD_SIZE; j++) 
                string_buffer[j] = '\0';

            // Caso chegue na última palavra, a posição limite para leitura será o tamanho da linha
            int jend = (i == (buffer->numb_words - 1) ? line_length : buffer->words_offset[i + 1]);

            for(int j  = buffer->words_offset[i]; j < jend; j++)
                string_buffer[j - buffer->words_offset[i]] = buffer->buffer[j];

            // Forçando a finalização da string para apagar \n que podem ter vindo junto
            string_buffer[ jend - 1 - buffer->words_offset[i]] = '\0';


            // Comparação da palavra lida com a requisitada pelo usuário
            if(strcmp(string_buffer, argv[1]) == 0){
                printf("%d: %s", linecount, line_buffer);
            }

        }

        buffer_destroy(buffer);

    }

    fclose(f);


    return SUCESS;
}