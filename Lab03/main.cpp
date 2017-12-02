#include<cstdio>
#include<cstdlib>
#include<time.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<assert.h>
#include<vector>
#include<pthread.h>
#include<semaphore.h>
#include<iostream>

#define threadNum 15
#define pr pair<int,int>

using namespace std;

int pid;
vector<pr> sub;
vector<sem_t> sem;
vector<int> par;
vector<int> arr;

template<typename iter>
void kernel(iter begin, iter end){
	// bubble
	int n = end-begin;
	for(int i=0;i<n;++i)
	for(int j=0;j<n-i-1;++j)
		if(begin[j+1]<begin[j])
			swap(begin[j+1],begin[j]);
}

void* pSort(void* arg){// [l,r)
	int& id = *(int*)arg;
	pid && sem_wait(&sem[id]);
	int begin = sub[id].first, end = sub[id].second;
	int n = end-begin;	
	if(n<=1||id*2+1>=threadNum){
		kernel(arr.begin()+begin, arr.begin()+end);
		pid && sem_post(&sem[(id-1)/2]);
		return NULL;
	}
	unsigned int seed = time(NULL)*id;
	int pvt = rand_r(&seed)%n;
	swap(arr[begin],arr[pvt+begin]);
	int l=begin+1, r=end-1;
	while(l<=r){
		if(arr[l]<arr[begin]) ++l;
		else if(arr[r]>=arr[begin]) --r;
		else swap(arr[l],arr[r]);
	}
	swap(arr[begin],arr[r]);
	sub[id*2+1].first = begin, sub[id*2+1].second = r;
	sub[id*2+2].first = r, sub[id*2+2].second = end;
	if(pid){
		sem_post(&sem[id*2+1]); sem_post(&sem[id*2+2]);
		sem_wait(&sem[id]); sem_wait(&sem[id]);
		sem_post(&sem[(id-1)/2]);
	} else{
		pSort((void*)&par[id*2+1]);
		pSort((void*)&par[id*2+2]);
	}
	return NULL;
}

void dump(FILE* fout){
	for(int i=0;i<arr.size();++i)
		i&&fputc(' ',fout),
		fprintf(fout,"%d",arr[i]);
	fputc('\n',fout);
}

int solve(int n){
	timeval from, to;
	gettimeofday(&from, 0);
	FILE* fout = fopen(pid?"output1.txt":"output2.txt","w");
	sub.resize(threadNum);
	par.resize(threadNum);
	sem.resize(pid?threadNum:0);
	vector<pthread_t> tid(pid?threadNum:0);
	for(int i=threadNum-1;i;--i){
		par[i] = i;
		pid && sem_init(&sem[i],0,0);
		pid && pthread_create(&tid[i],NULL,pSort,(void*)&par[i]);
	}
	pid && sem_init(&sem[0],0,1);
	par[0] = 0;
	sub[0].first = 0, sub[0].second = n;
	pSort((void*)&par[0]);
	dump(fout);
	fclose(fout);
	gettimeofday(&to, 0);
	printf("%s: ",pid?"mt":"st");
	printf("%lf sec\n",(double)to.tv_sec-from.tv_sec+(to.tv_usec-from.tv_usec)*1e-6);
	return 0;
}

int main(){
	if(threadNum&(threadNum+1)){
		fprintf(stderr,"threadNum mismatch: %d\n",threadNum);
		throw exception();
	}
	char in[255];
	printf("Input filename: "); scanf("%s",in);
	puts("");
	freopen(in,"r",stdin);
	int n; scanf("%d",&n);
	arr.resize(n);
	for(int i=0;i<n;++i)
		scanf("%d",&arr[i]);
	pid = fork();
	pid?solve(n),wait(NULL):solve(n);
	return 0;
}
