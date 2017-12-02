#include<bits/stdc++.h>

using namespace std;

int main(){
	vector<int> arr;
	int temp;
	while(cin>>temp) arr.push_back(temp);
	bool ac = true;
	for(int i=1;i<arr.size();++i)
		if(arr[i-1]>arr[i]) ac = false;
	printf("size: %d... ",(int)arr.size());	
	if(ac) puts("AC");
	else puts("WA");
	return 0;
}
