#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <omp.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class Queue
{
 public:
 
	  T pop()
	  {
	    std::unique_lock<std::mutex> mlock(mutex_);
	    while (queue_.empty())
	    {
	      cond_.wait(mlock);
	    }
	    auto item = queue_.front();
	    queue_.pop();
	    return item;
	  }
	 
	  void pop(T& item)
	  {
	    std::unique_lock<std::mutex> mlock(mutex_);
	    while (queue_.empty())
	    {
	      cond_.wait(mlock);
	    }
	    item = queue_.front();
	    queue_.pop();
	  }
 
  	void push(const T& item)
 		{
    		std::unique_lock<std::mutex> mlock(mutex_);
    		queue_.push(item);
    		mlock.unlock();
    		cond_.notify_one();
  	}
 
  	void push(T&& item)
  	{
    		std::unique_lock<std::mutex> mlock(mutex_);
    		queue_.push(std::move(item));
    		mlock.unlock();
    		cond_.notify_one();
  	}
	bool empty(){
		return queue_.empty();
	}
 
 private:
  	std::queue<T> queue_;
  	std::mutex mutex_;
  	std::condition_variable cond_;
};

vector<string> tokenize(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

omp_lock_t writelock;

int main(int argc, char** argv){
	Queue<string>q;
	int th = atoi(argv[1]);
	omp_set_num_threads(th);
	omp_init_lock(&writelock);
#pragma omp parallel
{
	int rank = omp_get_thread_num();
	if(rank%2 == 1){
		while(true){
		string ans;
		ans = q.pop();
		if (ans == "closethread")break;
		//Tokenize
		vector<string> ans2 = tokenize(ans, ' ');
		for(auto i:ans2){
		omp_set_lock(&writelock);
		cout << i << endl;
		omp_unset_lock(&writelock);
		}
		}
	}
	else{
		stringstream ss;
		ss << rank;
		string rank_str = ss.str();
		string fname = "file" + rank_str + ".in";
			ifstream is(fname);
		while(!is.eof()){
			string str;
			getline(is, str);
			q.push(str);
		}
		q.push("closethread");
	}
}
	
}
