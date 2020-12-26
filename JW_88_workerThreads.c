#include "JW_88_workerThreads--priv.h"

struct pssS {
	wTrS*p;
	short id;
	char ready;
};

__workerTh_ __workerTh_begin(int count, char trace) {
	wTrS* p = pointF(1, count);
	short point = (short)(unsigned long long)(p - pointF(3,0));
	p = *((wTrS**)p) = (wTrS*)malloc(sizeof(wTrS));
	short cat = p->cat = (count % SIZE__TH_AR > 0) + count / SIZE__TH_AR;
	p->trace = trace;
	p->cnt = count;
	p->now = 0;
	p->pssOb = (struct pssObS*) malloc(cat * sizeof(struct pssObS));
	p->ob = NULL;
		
	pthread_mutex_init(&p->mut, NULL);
	pthread_cond_init(&p->retCon, NULL);
	pthread_cond_init(&p->openCon, NULL);
	
	{
		struct pssObS *x = p->pssOb; 
		if (p->trace) p->ob = (struct obS*) malloc(count * sizeof(struct obS)), pthread_cond_init(&p->ob->con, NULL), p->ob->ready = 1;
		else p->ob = (struct obS*)malloc(sizeof(obS2)), pthread_mutex_init(&((obS2*)(p->ob))->mut, NULL), pthread_cond_init(&((obS2*)(p->ob))->con, NULL), ((obS2*)(p->ob))->p = NULL;
		for (unsigned short t = 0, t3 = 1, m, t2 = (((cat > 1 || !(m = cat % SIZE__TH_AR))) && (m = SIZE__TH_AR), 0); (t2 < m || (++x, t2 = (++t >= cat && count % SIZE__TH_AR && (m = count % SIZE__TH_AR), 0)), t) < cat; t2++, t3++) {
			if (!t2) {
				x->cnt = x->ready = 0;
				pthread_mutex_init(&x->mut, NULL);
				pthread_cond_init(&x->con, NULL);
				pthread_cond_init(&x->con0, NULL);
		
			}
			struct pssS psO = { p,  t, 0 };
			{
				pthread_t y;
				pthread_create(&y, NULL, open, (void*)(&psO));
				pthread_detach(y);
			}
			pthread_mutex_lock(&p->mut);
			while (!psO.ready) pthread_cond_wait(&p->retCon, &p->mut);
			pthread_mutex_unlock(&p->mut);
		}
	}
	return point;
}

void __workerTh_open(__workerTh_ point, basUniF func, void* obj, __workerTh_t flagHeapOrWaitElseWT_pId) {
	wTrS* p = pointF(0, point);
	char x = 0;
	if (p->trace) flagHeapOrWaitElseWT_pId /= (x = flagHeapOrWaitElseWT_pId + 1, 700);
	if (x) __workerTh_waitF(p, x - 1);
	pthread_mutex_lock(&p->mut);
	if (!p->cnt) 
		if (x || !flagHeapOrWaitElseWT_pId)
			while (!p->cnt) 
				pthread_cond_wait(&p->openCon, &p->mut);
		else {
			pthread_mutex_unlock(&p->mut);
			obS2* P = ((obS2*)(p->ob));
			pthread_mutex_lock(&P->mut);
			while (P->p) pthread_cond_wait(&P->con, &P->mut);
			P->p = func;
			P->o = obj;
			pthread_mutex_unlock(&P->mut);
			pthread_t t;
			pthread_create(&t, NULL, run, (void*)P);
			pthread_detach(t);
			return;
		}
	struct pssObS* P = p->pssOb;
	while (!P[p->now %= (++p->now, p->cat)].cnt);
	p->cnt--, (P += p->now)->cnt--;
	pthread_mutex_unlock(&p->mut);
	pthread_mutex_lock(&P->mut);
	while (P->ready) pthread_cond_wait(&P->con0, &P->mut);
	P->ready = (P->p = func, P->ob = obj, x && (p->ob[x - 1].ready = 0), x + !x);
	pthread_mutex_unlock(&P->mut);
	pthread_cond_signal(&P->con);
}

void __workerTh_end(__workerTh_ point) {
	wTrS* p = pointF(2, point);
	pthread_mutex_lock(&p->mut);
	while (p->cnt != p->tot) pthread_cond_wait(&p->retCon, &p->mut);
	for (short t = 0, m = p->cat; t < m; t++) pthread_cond_destroy(&p->pssOb[t].con), pthread_cond_destroy(&p->pssOb[t].con0), pthread_mutex_destroy(&p->pssOb[t].mut);
	free(p->pssOb);
	if (p->trace) for (short t = 0, m = p->tot; t < m; t++) pthread_cond_destroy(&p->ob[t].con);
	else pthread_cond_destroy(&((obS2*)(p->ob))->con);
	free(p->ob);
	pthread_mutex_destroy(&p->mut);
	pthread_cond_destroy(&p->openCon);
	pthread_cond_destroy(&p->retCon);
	free(p);
}

static void __workerTh_waitF(wTrS* p, __workerTh_t WT_pId) {
	struct obS* P = p->ob + WT_pId;
	pthread_mutex_t* m = &p->pssOb[WT_pId / SIZE__TH_AR].mut;
	pthread_mutex_lock(m);
	if (!P->ready) {
		P->ready = 1;
		while (P->ready) pthread_cond_wait(&P->con, m);
		P->ready = 1;
	}
	pthread_mutex_unlock(m);
}

static void* open(void* arg) {
	short id = openPrep(&arg);
	void* o;
	for (;;) {
		{
			wTrS* P = (wTrS*)arg;
			pthread_mutex_lock(&P->mut);
			struct pssObS* p = P->pssOb + id;
			char x = (o = (void*)p, ++P->pssOb[id].cnt, (++P->cnt == P->tot) + (-2) * (P->cnt == 1));
			pthread_mutex_unlock(&P->mut);
			pthread_mutex_lock(&p->mut);
			if (x > 0 || x == -1) pthread_cond_signal(&P->openCon);
			if (x < 1) pthread_cond_signal(&P->retCon);
		}
		while (!((struct pssObS*)o)->ready) pthread_cond_wait(&((struct pssObS*)o)->con, &((struct pssObS*)o)->mut);
		{
			basUniF p = ((struct pssObS*)o)->p;
			{
				struct pssObS*P = (struct pssObS*)o;
				P->ready = (o = P->ob, ((wTrS*)arg)->trace && (id = P->ready - 1), 0);
				pthread_mutex_unlock(&P->mut);
				pthread_cond_signal(&P->con0);
			}
			p(o);
		}
		if (!((wTrS*)arg)->trace) continue;
		struct obS* p = ((wTrS*)arg)->ob + id;
		pthread_mutex_lock(&((wTrS*)arg)->pssOb[id /= SIZE__TH_AR].mut);
		char x = p->ready = !p->ready;
		pthread_mutex_unlock(&((wTrS*)arg)->pssOb[id].mut);
		if (!x) pthread_cond_signal(&p->con);
	}
}

static void* run(void* arg) {
	basUniF f; {
		obS2* p = (obS2*)arg;
		arg = p->o, f = p->p;
		pthread_mutex_lock(&p->mut);
		p->p = NULL;
		pthread_mutex_unlock(&p->mut);
		pthread_cond_signal(&p->con);
	}
	f(arg);
}

static wTrS* pointF(char act, short var) {
	static wTrS** ar = NULL;
	if (act == 3) return (wTrS*)ar;
	static short cnt = 0, *adr;
	static pthread_sharedMutex_t mut;
	wTrS* p;
	if (!act) {
		pthread_sharedMutex_lock_shared(mut);
		short t = -1;
		while (adr[++t] != var);
		wTrS* p = ar[t];
		pthread_sharedMutex_unlock_shared(mut);
		return p;
	}
	if (!ar) pthread_sharedMutex_init(mut); pthread_sharedMutex_lock(mut); 
	switch (act) {
	case 1:
	{
		if (!ar) ar = (wTrS**)malloc(sizeof(wTrS*)), adr = (short*)malloc(sizeof(short*)), cnt = var = 0;
		else {
			ar = (wTrS**)realloc(ar, (cnt + 1) * sizeof(wTrS*));
			adr = (short*)realloc(adr, (cnt + 1) * sizeof(short));
			short a[cnt]; for (; (var < cnt) || (act && !(act = var = 0)); var++) a[var] = 0;
			for (var = 0; var < cnt; var++) if (adr[var] < cnt) a[var] = 1;
			for (var = 0; var < cnt; var++) if (!a[var]) break;
		}
		adr[cnt] = var; ar[cnt] = (wTrS*)(unsigned long long)var; p = (wTrS*)(ar + cnt++);
		break;
	}
	case 2: {
		short t = -1;
		while (adr[++t] != var);
		for (cnt--, p = ar[t]; t < cnt; t++) ar[t] = ar[t + 1], adr[t] = adr[t + 1];
		ar = (wTrS**)realloc(ar, (cnt) * sizeof(wTrS*));
		break;
	}
	}
	pthread_sharedMutex_unlock(mut);
	return p;
}

static short openPrep(void ** arg) {
	struct pssS* p = (struct pssS*)*arg;
	wTrS* P = p->p;
	*arg = (void*)P, P = p->p;
	short id = id = p->id;
	pthread_mutex_lock(&P->mut);
	p->ready = 1;
	pthread_mutex_unlock(&P->mut);
	pthread_cond_signal(&P->retCon);
}