#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
    printf("test \n");
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    stream = fopen("file.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);
    
    while ((read = getline(&line, &len, stream)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%zd", read);
        printf("%s", line);
    }
    printf("test \n");
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
    
    
    
}
