#include "JW_88_vlcServer.h"

#define DVB_CHAN_CNT 11

static struct dvbTvChanS {
	unsigned int freq;
	unsigned long chan;
}dvbTvChan[] = {
	{ 642, 14010 },	/* 0 Nederland 1			*/ { 306, 33020 },	// 1 Nederland 2
	{ 306, 33030 },	/* 2 Nederland 3			*/ { 290, 34020 },	// 3 Rtl 4
	{ 290, 34030 },	/* 4 Rtl 5					*/ { 554, 38030 },	// 5 SBS 6
	{ 298, 36010 },	/* 6 Rtl 7					*/ { 298, 36020 },	// 7 Rtl 8
	{ 738, 4050 },	/* 8 Net 5					*/ { 554, 38020 },	// 9 Veronica
	{ 746, 27000 }	// 10 National Geografic
};