#ifndef JW_88_timerTasks_H_   
#define JW_88_timerTasks_H_
#include <pthread.h>

typedef void(*basUniF)(void*);
typedef char __timerTsk_obj[2 * sizeof(void*) + sizeof(basUniF) + sizeof(char) + sizeof(unsigned int) + sizeof(volatile unsigned int) + sizeof(pthread_cond_t)];

extern unsigned int __timerTsk_timeVal;
extern unsigned int __timerTsk_timNow();

extern void __timerTsk_begin();
extern void __timerTsk_setTimer(__timerTsk_obj obj, char flagTim0IntForcedRun1ForcedWait2InmeditNEG, unsigned int value, basUniF pFunc, void* pObj);
extern void __timerTsk_changeRunSet(__timerTsk_obj obj, short set);
extern void __timerTsk_killTimer(__timerTsk_obj obj, char join2run3fromTim);
extern int __timerTsk_timeDiff(unsigned int x, unsigned int y);

#endif