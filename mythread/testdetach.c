#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
void *thread_run( void * arg  )
{
    pthread_detach(pthread_self());
    printf("%s\n", (char*)arg);
    return NULL;

}
int main() {
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread_run, "thread1 run...")!=0)      {   
        printf("create thread error\n");
        return 1;
    }
    int ret = 0;
    sleep(1);//很重要，要让线程先分离，再等待
    //线程被创建, 但创建的线程中的分离语句和下面的join语句
    //哪个先执行还真不一定, 所以说要先睡一下
    if ( pthread_join(tid, NULL ) == 0  ) {
        printf("pthread wait success\n");
        ret = 0;

    } else {
        printf("pthread wait failed\n");
        ret = 1;
    }
    return ret;
}
