#include "ex3_q1.h"

int main(int argc, char *argv[]) {
    char files [10][30];
    pthread_t thread_ids[10];
    int error;

    if (pthread_mutex_init(&all_stud_mutex, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    for(int argIndex=1; argIndex<argc; ++argIndex)
    {
        strcpy(files[files_count++], argv[argIndex]);
    }

    for(int thIndex = 0; thIndex< files_count; thIndex++) {
        error = pthread_create(&(thread_ids[thIndex]), NULL, manageThreadWork, &(files[thIndex]));

        if (error != 0)
            exit(EXIT_FAILURE);
    }

    for(int thIndex = 0; thIndex< files_count; thIndex++) {
        pthread_join(thread_ids[thIndex], NULL);
    }

    pthread_mutex_destroy(&all_stud_mutex);
}

float getAvgForStudent(char *line, char *name) {
    const char s[2] = " ";
    char *token;
    char* rest = line;

    token = strtok_r(rest, s, &rest);
    strcpy(name, token);
    token = strtok_r(rest,s, &rest);

    float sum=0, count=0;
    while( token != NULL ) {
        sum+=atoi(token);
        count++;
        token = strtok_r(rest, s,&rest);
    }

    if (sum==0)
        return 0;

    return sum/count;
}

void writeToAllStud(char *name, float avg) {
    struct student stud;
    strcpy(stud.name,name);
    stud.avg_grade = avg;
    // lock area
    pthread_mutex_lock(&all_stud_mutex);
    all_stud.stud_arr[all_stud.count] = stud;
    all_stud.count++;
    pthread_mutex_unlock(&all_stud_mutex);
}

void printUntilDone() {
    while(true) {
        int indexToPrint=-1;
        pthread_mutex_lock(&all_stud_mutex);

        if (students_printed < all_stud.count){
            indexToPrint = students_printed;
            students_printed++;
        }

        if(indexToPrint==-1) {
            pthread_mutex_unlock(&all_stud_mutex);
            break;
        } else {
            print_student(indexToPrint);
            pthread_mutex_unlock(&all_stud_mutex);
        }
    }
}

void *manageThreadWork(void* filename) {
    sumFileAverageIntoAllStud((char *)filename);

    pthread_mutex_lock(&threads_mutex);
    files_parsed++;
    if (files_parsed == files_count) {
        sort_student_arr();
        pthread_cond_broadcast(&threads_cond);
    } else {
        pthread_cond_wait(&threads_cond, &threads_mutex);
    }
    pthread_mutex_unlock(&threads_mutex);

    printUntilDone();

    return NULL;
}

void sumFileAverageIntoAllStud(char* filename) {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char path [33] = "./";
    FILE *source = fopen(strcat(path, (char*)filename), "r");
    char studentName [30];

    if (source == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, source)) != -1) {
        float avg = getAvgForStudent(line, studentName);
        writeToAllStud(studentName,avg);
    }

    fclose(source);
    if (line)
        free(line);
}
