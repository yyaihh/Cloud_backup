#include<stdio.h>
#include<string.h>
int main(){
    FILE* fp = fopen("myfile.txt", "w+");
    if(fp == NULL){
        printf("打开失败\n");
        return -1;
    }
    const char* str = "hello world!\n";
    int count = 5;
    int len = strlen(str);
    while(count--){
        fwrite(str, len, 1, fp);
    }
    fseek(fp, 0 , 0);
    char buf[1024] = { 0 };
    count = 5;
    size_t n;
    while(count--){
        n = fread(buf, 1, len, fp);
        if(n > 0){
            printf("%s", buf);
        }
        if(feof(fp)){
            break;
        }
    }
    fclose(fp);
    return 0;
}
