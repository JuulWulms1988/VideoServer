#ifndef JW_88_workerThreads_H_   
#define JW_88_workerThreads_H_

typedef void(*basUniF)(void*);
typedef short __workerTh_;
typedef short __workerTh_t;

extern __workerTh_ __workerTh_begin(int count, char trace);
extern void __workerTh_open (__workerTh_ point, basUniF func, void* obj, __workerTh_t flagHeapOrWaitWT_pId);
extern void __workerTh_wait (__workerTh_ point, __workerTh_t WT_pId);
extern void __workerTh_end(__workerTh_ point);

#endif