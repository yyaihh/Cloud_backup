#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
int main(){
    while(1){
        printf("[username@localhost]$");
        fflush(stdout);
        char cmd_buf[1024] = { 0 };
        fgets(cmd_buf, 1023, stdin);
        cmd_buf[strlen(cmd_buf) - 1] = 0;
        printf("cmd:[%s]\n", cmd_buf);
        int argc = 0;
        char* argv[32] = { NULL };
        char* p;
        for(p = cmd_buf; *p != '\0'; ++p){
            if(isspace(*p)){//不是空白字符
                argv[argc] = p;
                ++argc;
                while(!isspace(*p) && *p != '\0'){
                    ++p;
                }
               // *p = '\0';
            }
        }
        //argv[argc] = NULL;
    }
    return 0;
}
