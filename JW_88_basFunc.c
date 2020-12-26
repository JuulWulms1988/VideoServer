#include "JW_88_basFunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char bsBreakChar(char x) { for (char t = 0, ar[] = { ' ', '\r', '\n', '\0' }; t < 3; ) if (x == ar[t++]) return 1; return 0; }
char vergCharP(char* x, char* y) { for (;; x++, y++) if (*x == '\0') return *y == '\0'; else if (*y == '\0' || *x != *y) return 0; }
int charCp(char* x, char* y) { int t = 0; while ((*x++ = *y++) != '\0') t++; return t; }
char isNoF(char x) { return x >= '0' && x <= '9'; }
void __dynHeapP_prep(__dynHeapP_ptr* obj, unsigned long long dynOrstatSize) { if (!dynOrstatSize) *((char*)(*obj = malloc((unsigned long long)*obj))) = '\0'; else if ((unsigned long long)*obj < dynOrstatSize) *obj = (void*)dynOrstatSize; }
void __dynHeapP_chckOpenF(__dynHeapP_ptr obj, void(*ptr)(__dynHeapP_ptr)) { if (!*((char*)obj)) ptr(obj); }

char vergCharPH(char* x, char* y) {
	for (;;) {
		if (*x == '\0') return *y == '\0';
		else if (*y == '\0') return 0;
		if (*x != *y)
			if (*x >= 'a' && *x <= 'z') { if (*x - ('a' - 'A') != *y) return 0; }
			else if (*x >= 'A' && *x <= 'Z') { if (*x + ('a' - 'A') != *y) return 0; }
			else return 0;
		x++, y++;
	}
}