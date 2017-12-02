#include<bits/stdc++.h>
#define pr pair<int,int>
using namespace std;



int main(){
	int n;
	cin>>n;
	cout<<n<<"\n";
	srand(time(NULL));
	while(--n){
		cout<<rand();
		if(n) cout<<" ";
	}
	cout<<"\n";

}
