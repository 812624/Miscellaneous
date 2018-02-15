#include <iostream>
#include <thread>
#include <queue>
#include <stdlib.h> 
#include <mutex>
using namespace std;

// Declaring the lock
std::mutex g_display_mutex;

// Shared queue
queue <int> Q;


// Know the contents of queue
void showq()
{
    queue <int> g = Q;
    while (!g.empty())
    {
        cout << '\t' << g.front();
        g.pop();
    }
    cout << '\n';
}

// Performing addition for thread B
int add_B()
{
	int s = 0;
	g_display_mutex.lock();
	while(Q.size()>=1)
	{
		cout << "Inside sum " << Q.front() << endl;
		s += Q.front();
	    //std::thread::id this_id = std::this_thread::get_id();
		//cout << this_id << endl;
		Q.pop();
	}
	g_display_mutex.unlock();
	return s;
}


// Performing addition for thread C
int add_C()
{
	int s = 0;
	g_display_mutex.lock();
	while(Q.size()>=1)
	{
		cout << "Inside sum " << Q.front() << endl;
		s += Q.front();
	    //std::thread::id this_id = std::this_thread::get_id();
		//cout << this_id << endl;
		Q.pop();
		
	}
	g_display_mutex.unlock();
	return s;
}


int generate_number()
{	
	srand (time(NULL));
	static int count = 0;
	int num;
	while(count <= 10)
	{
		std::thread::id this_id = std::this_thread::get_id();
		cout << this_id << endl;		
		num = rand() % 10 + 1;
		Q.push(num);
		showq();
		count++;
	}
	if(count >= 10)
		return 1;
	else
		return 0;	
}


int main()
{
	int get_A = 0, sum_B = 0, sum_C = 0;
    
    thread th1(([&]{get_A = generate_number();}));
    showq();

    //thread th2(([&]{sum_B = add_B();}));

	//thread th3(([&]{sum_C = add_C();}));

    th1.join();
	while(Q.size()>=1){
		thread th2(([&]{sum_B = add_B();}));

		thread th3(([&]{sum_C = add_C();}));
		
		th2.join();
		th3.join();	
	}
	//	th2.join();
		//th3.join();	
	
	//showq();
	cout << sum_B << " " << sum_C << endl;
	if(sum_B > sum_C)
		cout << "Winner is B "  << endl;
	else
		cout << "Winner is C " << endl;
    return 0;
}
