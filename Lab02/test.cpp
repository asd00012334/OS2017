#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<assert.h>

#define forkNum 5

using namespace std;


int main(){
	int pid = fork();
	if(!pid){
		for(int i=0;i<3e8;++i);
		exit(0);
	}
	int stat;
	cout<<WNOHANG<<"\n";
	cout<<pid<<"\n";
	cout<<wait(&stat)<<"\n";
	cout<<wait(&stat)<<"\n";
}
