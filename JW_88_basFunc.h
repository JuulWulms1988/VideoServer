#ifndef JW_88_basFunc_H_   
#define JW_88_basFunc_H_

typedef void(*basUniF)(void*);
typedef void* __dynHeapP_ptr;

extern char bsBreakChar(char x);
extern char vergCharPH(char* x, char* y);
extern char vergCharP(char* x, char* y);
extern int charCp(char* x, char* y);
extern char isNoF(char x);
extern void __dynHeapP_prep(__dynHeapP_ptr* obj, unsigned long long dynOrstatSize);
extern void __dynHeapP_chckOpenF(__dynHeapP_ptr obj, void(*ptr)(__dynHeapP_ptr));


#endif