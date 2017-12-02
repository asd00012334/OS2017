#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/resource.h>
#include<signal.h>
using namespace std;

int flag;
#define pipeSig (1<<0)
#define redirSig (1<<1)
#define rushSig (1<<2)
char* readline(char* buf, int n){
	for(;n;--n,++buf){
		int ch = getchar();
		if(ch==EOF || ch=='\n') break;
		*buf = ch;
	}
	return buf;
}

int tokenize(char* buf, char* argv[]){
	int argc=0;	
	for(buf=strtok(buf," ");buf;buf=strtok(NULL," "))
		argv[argc++] = buf;
	return argc;
}

int redirect(char* argv[], int argc){
	for(int i=0;i<argc;++i)
		if(argv[i][0]=='>'){
			flag |= redirSig;
			return i;
		} else if(argv[i][0]=='|'){
			flag |= pipeSig;
			return i;
		}
	return -1;
}

#define bufSize 200
int main(){
	while(1){
		flag=0;
		char buf[bufSize]={}, *argv[bufSize]={}, *ebuf;
		printf("> "), ebuf=readline(buf,bufSize-1);
		if(buf==ebuf) continue;
		if(ebuf[-1]=='&') flag |= rushSig, *--ebuf=0;
		int argc=tokenize(buf,argv);
		int pvt=redirect(argv,argc);
		int stat[0], pid = fork();
		if(!pid){
			if(flag&pipeSig){
				int fd[2];
				pipe(fd);
				int pipeId = fork();
				if(!pipeId){
					close(1);
					dup(fd[1]);
					close(fd[0]);
					argv[pvt]=NULL;
				} else{
					close(0);
					dup(fd[0]);
					close(fd[1]);
					char** exPvt = argv+pvt+1;
					execvp(exPvt[0],exPvt);
					waitpid(pipeId,stat+1,0);
					puts("exec failed");
					exit(0);
				}
			} else if(flag&redirSig){
				freopen(argv[pvt+1],"w",stdout);
				argv[pvt]=NULL;
			}
			execvp(buf,argv);
			puts("exec failed");
			exit(0);
		} else if(flag&rushSig) signal(SIGCHLD,SIG_IGN);
		else waitpid(pid,stat,0);
	}
	return 0;
}
