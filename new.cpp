#include <iostream>
#include <fstream>
#include <pthread.h>
#include <queue>
#include <stdlib.h> 

#define MAX 10

using namespace std;

int dataCount = 10, sum_B = 0, sum_C = 0;
int  consumerCount1 = 0; int  consumerCount2 = 0;

// Shared queue
queue <int> Q;

void *producerFun(void *);
void *add_B(void *);
void *add_C(void *);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t dataNotProduced = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotConsumed = PTHREAD_COND_INITIALIZER;

int main()
{

  pthread_t producerThread, consumerThread1, consumerThread2;

  int retProducer = pthread_create(&producerThread, NULL, producerFun, NULL);
  int retConsumer1 = pthread_create(&consumerThread1, NULL, *add_B, NULL);
  int retConsumer2 = pthread_create(&consumerThread2, NULL, *add_C, NULL);
  
 
  pthread_join(producerThread, NULL);
  pthread_join(consumerThread1, NULL);
  pthread_join(consumerThread2, NULL);
  if(sum_C  > sum_B)
	cout << "Winner is  Thread C" << endl;
  else if(sum_C  < sum_B)	
    cout << "Winner is  Thread B" << endl;
  else
    cout << "Both has same score" << endl; 
  return 0;

}

void *producerFun(void *)
{
    int producerCount = 0;
	srand (time(NULL));
    while(1)
    {
      pthread_mutex_lock(&mutex);

      if(Q.size() < 10)
      {
		int num = rand() % 10 + 1;
        cout << "Produced:  " << num << endl;

		Q.push(num);

        producerCount ++;

        pthread_cond_broadcast(&dataNotProduced);
      }
      else
      {
        cout << ">> Producer is in wait.." << endl;
        pthread_cond_wait(&dataNotConsumed, &mutex);
      }
      pthread_mutex_unlock(&mutex);

      if(producerCount == dataCount)
      {
        return NULL;
      }
    }
}

void *add_B(void *)
{

  while(1)
  {
    pthread_mutex_lock(&mutex);

    if(Q.size()>0)
    {
      int data = Q.front();

      cout << "B thread consumed: " << data << endl;
      sum_B += data;
	  Q.pop();

      consumerCount1 ++;

      pthread_cond_signal(&dataNotConsumed);
    }
    
    else if(consumerCount2 + consumerCount1  == dataCount)
    {
	  pthread_mutex_unlock(&mutex);
      return NULL;
    }
    else
    {
      cout << "B is in wait.." << endl;
      pthread_cond_wait(&dataNotProduced, &mutex);
    }
      pthread_mutex_unlock(&mutex);

  }
}

void *add_C(void *)
{

  while(1)
  {
    pthread_mutex_lock(&mutex);


    if(Q.size()>0)
    {


      int data = Q.front();
      cout << "C thread consumed: " << data << endl;
      sum_C += data;
	  Q.pop();
      consumerCount2 ++;

      pthread_cond_signal(&dataNotConsumed);
    }
    else if(consumerCount2 + consumerCount1  == dataCount)
    {
	  pthread_mutex_unlock(&mutex);
      return NULL;
    }    
    else
    {
      cout << ">> C is in wait.." << endl;
      pthread_cond_wait(&dataNotProduced, &mutex);
    }
    pthread_mutex_unlock(&mutex);
  }
} 
