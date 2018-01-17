#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/time.h>

using namespace std;

char buf[2] = {};

int main(){
	timeval from, to;
	gettimeofday(&from,0);
	vector<string> fNameList;
	for(int i=0;i<1<<12;++i){
		static char nameBuf[512];
		snprintf(nameBuf,sizeof(nameBuf),"%d.block",i);
		FILE* fp = fopen(nameBuf,"w");
		fNameList.push_back(nameBuf);
		if(!fp) break;
		fwrite(buf,sizeof(char),sizeof(buf)/sizeof(char),fp);
		fclose(fp);
	}
	sync();
	for(int i=0;i<fNameList.size();i+=2){
		remove(fNameList[i].c_str());
		fNameList[i]="";
	}
	sync();
	
	FILE* lfp = fopen("largefile.txt","w");
	while(fwrite(buf,sizeof(char),sizeof(buf),lfp));
	fclose(lfp);
	sync();
	system("filefrag -v largefile.txt");
	gettimeofday(&to,0);
	printf("%f seconds\n",
		(double)to.tv_sec-from.tv_sec+(to.tv_usec-from.tv_usec)*1e-6);
	return 0;
}
