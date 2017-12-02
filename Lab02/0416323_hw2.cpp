#include<cstdio>
#include<cstdlib>
#include<time.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<assert.h>

#define uint unsigned int

int forkNum;

using namespace std;

uint* in[2];

void solve(uint* shmPtr, int tpid, int n){
	int seg = n/forkNum+(n%forkNum>0);
	int base = tpid*seg;
	int last = base+seg<=n?base+seg:n;
	for(uint i=base;i<last;++i)
	for(uint j=0;j<n;++j){
		shmPtr[i*n+j]=0;
		for(uint k=0;k<n;++k)
			shmPtr[i*n+j]+=in[0][i*n+k]*in[1][k*n+j];
	}
}

int main(){
	int n;
	printf("Input the matrix dimension: ");
	scanf("%d",&n); puts("");
	int shmID = shmget(IPC_PRIVATE, 3*n*n*sizeof(uint), IPC_CREAT | 0666);
	assert(shmID>=0);
	uint* shmPtr = (uint*) shmat(shmID,NULL,0);
	in[0] = shmPtr+n*n, in[1]=shmPtr+n*n*2;
	for(int i=0;i<n;++i)
	for(int j=0;j<n;++j)
		in[0][i*n+j] = i*n+j, in[1][i*n+j] = i*n+j;
	assert(shmPtr>=0);
	for(forkNum=1;forkNum<=16;++forkNum){
		printf("Multiplying matrices using %d process%s\n",forkNum,forkNum>1?"es":"");
		clock_t begin = clock();
		for(int i=forkNum-1;i;--i)
			if(!fork()){
				solve(shmPtr,i,n);
				exit(0);
			}
		solve(shmPtr,0,n);
		int status;
		uint sum=0;
		while(wait(&status)!=-1);
		for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			sum+=shmPtr[i*n+j];
		printf("Elapsed time: %lf sec, Checksum: %u\n",
				(double)(clock()-begin)/CLOCKS_PER_SEC,sum);
	}
	shmdt((void*)shmPtr);
	shmctl(shmID, IPC_RMID, NULL);
	exit(0);
	return 0;
}
