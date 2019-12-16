#include<stdio.h>
#include<string.h>
int main(){
    const char* str = "hello world\n";
    int len = strlen(str);
    printf(str);
    puts("hello world");
    fwrite(str, 1, len, stdout);
    fprintf(stdout, str);
    fputs(str, stdout);
    for(int i = 0; i < len; ++i) {
        putchar(*(str + i));
    }
    for(int i = 0; i < len; ++i) {
        fputc(*(str + i), stdout);
    }
    for(int i = 0; i < len; ++i) {
        putc(*(str + i), stdout);
    }
    return 0;
}
