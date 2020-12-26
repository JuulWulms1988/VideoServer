#include "JW_88_vlcServer.h"

__workerTh_ mainWork;

const char* mainBasString[] = { "shutdown", "clear", "fd://0", "loop", "get_time" };

int main() {
	//load
		//Prep
			prepBegin();
			
		
		//VLC:
			rcBegin();
			

		//Timer
			__timerTsk_begin();

			
		//Executor:
			__dynHeapP_prep(&execDynHeap, nosHeapSize());
			__dynHeapP_prep(&execDynHeap, 0);
			execBegin();
			
			
		//Netclient
			netCBegin();
			

	return 0;
}