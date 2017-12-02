#include<bits/stdc++.h>

using namespace std;

int main(){
	vector<int> arr;
	int temp;
	while(cin>>temp) arr.push_back(temp);
	bool ac = true;
	vector<int> falsePt;
	for(int i=1;i<arr.size();++i)
		if(arr[i-1]>arr[i]) ac = false, falsePt.push_back(i);
	printf("size: %d... ",(int)arr.size());	
	if(ac){
		puts("AC");
	} else{
		printf("WA, %d flase points:\n",(int)falsePt.size());
		for(int i=0;i<falsePt.size();++i)
			i&&printf(" "), printf("%d",falsePt[i]);
		puts("");
	}
	return 0;
}
