#include<stdio.h>
#include<string.h>
int main(){
<<<<<<< HEAD
    FILE* fp = fopen("myfile.txt", "w+");
=======
    FILE* fp = fopen("~\\Linux\\IO\\myfile.txt", "w+");
>>>>>>> 1df5dfe4a86bb789f31b98a4721d8e9585665ca9
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
