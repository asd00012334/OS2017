#include<cstdio>
#include<cstdlib>
#include<string>
#include<assert.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#define ull unsigned long long

using namespace std;

bool indValid=false;
ull ind;

bool fnameValid=false;
string fname;

bool szMinValid;
ull szMin;

bool szMaxValid;
ull szMax;

void listAll(string const& path){
	DIR* dir = opendir(path.c_str());
	if(!dir) return;
	for(dirent* ptr=readdir(dir);ptr;ptr=readdir(dir)){
		string tmp = path+"/"+ptr->d_name;
		if(string(".")==ptr->d_name ||string("..")==ptr->d_name)
			continue;
		listAll(tmp);
		if(indValid && ind!=ptr->d_ino) continue;
		if(fnameValid && fname!=ptr->d_name) continue;
		struct stat buf;
		stat(tmp.c_str(),&buf);
		if(szMinValid && buf.st_size<szMin) continue;
		if(szMaxValid && buf.st_size>szMax) continue;
		printf("%s %lu %lf MB\n",tmp.c_str(),ptr->d_ino,(double)buf.st_size/(1<<20));
	}
}

int main(int argc, char* argv[]){
	assert(argc>2 && argc%2==0);
	string path = argv[1];
	for(int i=2;i<argc;i+=2){
		string k = argv[i], v = argv[i+1];
		if(k=="-inode"){
			indValid = true;
			ind = atoi(v.c_str());
		} else if(k=="-name"){
			fnameValid = true;
			fname = v;
		} else if(k=="-size_min"){
			szMinValid = true;
			szMin = ((ull)atoi(v.c_str()))<<20;
		} else if(k=="-size_max"){
			szMaxValid = true;
			szMax = ((ull)atoi(v.c_str()))<<20;
		} else{
			printf("Invalid argument list");
			exit(-1);
		}
	}
	listAll(path);
}
