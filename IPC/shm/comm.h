#ifndef _COMM_H_
#define _COMM_H_
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#define PATHNAME "."
#define PROJ_ID 0x6666

int createShm(int size);
int destroyShm(int shmid);
int getShm(int size);

#endif
