#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "ex3_q1_given.h"

#ifndef Q1_EX3_Q1_H
#define Q1_EX3_Q1_H

struct all_students all_stud;
pthread_mutex_t all_stud_mutex;
pthread_cond_t threads_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t threads_mutex = PTHREAD_MUTEX_INITIALIZER;
int files_count=0;
int files_parsed=0;
int students_printed=0;

void sumFileAverageIntoAllStud(char *filename);
void *manageThreadWork(void* filename);
float getAvgForStudent(char *line, char *name);
void writeToAllStud(char *name, float avg);
void printUntilDone();
void *manageThreadWork(void* filename);
void sumFileAverageIntoAllStud(char* filename);

#endif //Q1_EX3_Q1_H
