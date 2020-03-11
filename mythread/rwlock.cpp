#include<iostream>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
using namespace std;

volatile int data = 1000;
pthread_rwlock_t rwlock;

void * reader(void * arg) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        if (data == 0) {
            pthread_rwlock_unlock(&rwlock);
            break;
        }
        printf("reader thread%d: %d\n", arg, data);
        pthread_rwlock_unlock(&rwlock);
        usleep(1);
    }
    return nullptr;
}
void * writer(void * arg) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        if (data == 0) {
            pthread_rwlock_unlock(&rwlock);
            break;
        }
        printf("writer thread%d: %d\n", arg, --data);
        pthread_rwlock_unlock(&rwlock);
        usleep(1);

    }
    return nullptr;
}
void init_readers(vector<pthread_t>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        pthread_create(&vec[i], nullptr, reader, (void *)i);
    }
}
void init_writers(vector<pthread_t>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        pthread_create(&vec[i], nullptr, writer, (void *)i);
    }

}
void join_threads(vector<pthread_t> const& vec){
    for (vector<pthread_t>::const_reverse_iterator it = vec.rbegin(); it !=
            vec.rend(); ++it) {
        pthread_join(*it, nullptr);
    }
}
void init_rwlock() {
#if 1
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlock_init(&rwlock, &attr);
    pthread_rwlockattr_destroy(&attr);
#else // 读优先，会造成写饥饿
    pthread_rwlock_init(&rwlock, nullptr);
#endif
}

int main() {
    //测试效果不明显的情况下，可以加大 reader_nr
    //但也不能太大，超过一定阈值后系统就调度不了主线程了
    const size_t reader_nr = 500;
    const size_t writer_nr = 2;
    vector<pthread_t> readers(reader_nr);
    vector<pthread_t> writers(writer_nr);
    init_rwlock();
    init_readers(readers);
    init_writers(writers);
    join_threads(writers);
    join_threads(readers);
    pthread_rwlock_destroy(&rwlock);
}
