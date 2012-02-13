#ifndef TASK_H
#define	TASK_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef USE_PTHREADS	
#include <pthread.h>
#else
#include <sys/types.h>	
#endif	
typedef enum {
    car, tram, pedestriant
} task_type;
typedef struct {
    int id;
    int posX;
    int posY;
    int alive;
    task_type type;
#ifdef USE_PTHREADS	
	pthread_t thread;
#else
	pid_t pid;
#endif	
} task_context_t;
void task_start(int i);

#ifdef	__cplusplus
}
#endif

#endif	/* TASK_H */

