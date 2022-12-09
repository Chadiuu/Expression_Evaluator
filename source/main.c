
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "virtualMachine.h"
#define LINE_LIMIT 1000
static void eval() {
    char line[1000];
    for (;;)
    {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n"); break;
        }
        run_code_vm(line);
    }
}
static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file = NULL) exit(2);

    fseek(file, 0l, SEEK_END);
    size_t size = ftell(file); // fileSize
    rewind(file);
    char* buff = (char*)malloc(size + 1);
    if (buff == NULL) exit(2);
    size_t bytes_read = fread(buff, sizeof(char), size, file); buff[bytes_read] = '\0';
    fclose(file);
    return buff;
    
}
static void execute_file(const char* path) {
    char* code = read_file(path);
    result_vm state = run_code_vm(code); //result
    free(code);
    if (state == COMPILE_TIME_ERROR || state == RUNTIME_ERROR) exit(2);
}
int main(int argc, const char* argv[])
{

    initializeVM();
    printf("%s","Test 1 \n");
    if (argc == 1) eval();
    else if (argc == 2) execute_file(argv[1]);
    else exit(2);
    deleteVM();
    return 0;

}

