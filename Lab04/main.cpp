#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<queue>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>
#include<algorithm>
#define nodeNum 15

using namespace std;

template<class type>
struct Get{
	type& obj;
	Get(type& obj):obj(obj){ sem_wait(&obj.lock);}
	~Get(){ sem_post(&obj.lock);}
	type* operator->(){ return &obj;}
};
template<class type>
Get<type> get(type& obj){ return Get<type>(obj);}

template<typename type>
struct ThreadQueue{
	sem_t size;
	sem_t lock;
	queue<type> dataList;
	ThreadQueue(){
		sem_init(&size, 0, 0);
		sem_init(&lock,0,1);
	}
	~ThreadQueue(){
		sem_destroy(&size);
		sem_destroy(&lock);
	}
	void push(type const& obj){
		get(*this)->dataList.push(obj);
		sem_post(&size);
	}
	type pop(bool sizeProtect=true){
		if(sizeProtect) sem_wait(&size);
		sem_wait(&lock);
			type out = dataList.front();
			dataList.pop();
		sem_post(&lock);
		return out;
	}
	inline type front(){ return get(*this)->dataList.front();}
};

template<class Task>
class ThreadPool{
	const int threadNum;
	volatile bool activate;
	ThreadQueue<Task> taskAwait;
	vector<pthread_t> tid;
	static void* exec(void * arg){
		ThreadPool& tp = *(ThreadPool*)arg;
		while(1){
			sem_wait(&tp.taskAwait.size);
			if(!tp.activate) break;
			tp.taskAwait.pop(false)();
		}
		return NULL;
	}
public:
	ThreadPool(int n): threadNum(n), tid(n){
		activate = true;
		for(int i=0;i<n;++i)
			pthread_create(&tid[i],NULL,exec,(void*)this);
	}
	~ThreadPool(){
		activate = false;
		for(int i=0;i<tid.size();++i) sem_post(&taskAwait.size);
		for(int i=0;i<tid.size();++i) pthread_join(tid[i],NULL);
	}
	void push(Task func){
		taskAwait.push(func);
	}
};

class Solver{
	sem_t& finished;
	ThreadPool<Solver>& tp;
	vector<int>::iterator begin, end;
	int idx;
public:
	template<typename iter, typename Pool>
	Solver(iter begin, iter end, sem_t& finished, Pool& tp,int idx=0):
		begin(begin), end(end), idx(idx), finished(finished),tp(tp){}
	void operator()(){
		if(idx*2+1<nodeNum) quick();
		else bubble();
		sem_post(&finished);
	}
	void quick(){
		int n = end-begin;
		if(n<2){
			tp.push(Solver(begin,end,finished, tp,idx*2+1));
			tp.push(Solver(begin,end,finished, tp,idx*2+2));
			return;
		}
		nth_element(begin,begin+n/2,end);
		tp.push(Solver(begin,begin+n/2, finished, tp,idx*2+1));
		tp.push(Solver(begin+n/2+1,end, finished, tp,idx*2+2));
	}
	void bubble(){
		int n = end-begin;
		for(int i=0;i<n;++i)
		for(int j=0;j<n-i-1;++j)
			if(begin[j+1]<begin[j]) swap(begin[j],begin[j+1]);
	}
};

int main(){
	freopen("input.txt","r",stdin);
	int n; cin>>n;
	vector<int> arrBase(n);
	for(int i=0;i<n;++i) scanf("%d",&arrBase[i]);
	srand(time(NULL));
	for(int threadNum=1;threadNum<=8;++threadNum){
		vector<int> arr = arrBase;
		char fileName[]="output_n.txt"; // pos 7
		fileName[7] = '0'+threadNum;
		FILE* fout = fopen(fileName,"w");
		sem_t finished; sem_init(&finished,0,0);
		timeval from, to;
		gettimeofday(&from, 0);
		ThreadPool<Solver> tp(threadNum);
		tp.push(Solver(arr.begin(),arr.end(), finished, tp));
		for(int i=0;i<nodeNum;++i) sem_wait(&finished);
		gettimeofday(&to, 0);
		printf("%d Thread: %lf sec\n", threadNum,
				(double)to.tv_sec-from.tv_sec+(to.tv_usec-from.tv_usec)*1e-6);
		for(int i=0;i<n;++i) i&&fprintf(fout," "), fprintf(fout,"%d", arr[i]);
		fputs("",fout);
		fclose(fout);
		sem_destroy(&finished);
	}
}
