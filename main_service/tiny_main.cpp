#include <iostream>
#include <thread>
#include <mutex>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

//#include "tiny_main.h"

using namespace std;

#define BASE_TASK_NUM 2
#define EXTRA_TASK_NUM 3

#define JOB_NUM BASE_TASK_NUM + EXTRA_TASK_NUM

/**
 * @brief Main Service
 * 
 */
class TinyPlatform
{
public:
    int initialize();
    int start();

private:

};

struct WorkerThread
{
  thread worker;
  mutex lock_key;
};

WorkerThread worker_thread_list[JOB_NUM];

int count = 0;

template<typename T>
struct SharedData
{
  SharedData(T* param) : data(param) {}
  ~SharedData() { delete data; }
  void setData(T* param) {data=param;}
  T* getData() {return data;}

  private:
    T* data;
};

//SharedData<char*> td2(nullptr);
SharedData<void*> td2(nullptr);

void runMainWorkerTask1(int worker)
{
  while(1)
  {
    worker_thread_list[worker].lock_key.lock();
    // TODO
    cout << "[Main Task 1]\t working... COUNT:" << count++ <<endl;
    usleep(1000);
  }
}

void runSubWorkerTask1(int worker)
{
  while(1)
  {
    worker_thread_list[worker].lock_key.lock();

    // TODO
    cout << "[Sub Task 1]\t working... COUNT:" << count++ <<endl;

    char data_org[64] = "<Sensor>Turn On</Sensor>";
    void* data = &data_org[0];
    td2.setData(&data);
    cout << "[Sub Task 1]\t working... SEND:" << (char*)data <<endl;

    usleep(1000);
  }
}

void runSubWorkerTask2(int worker)
{
  while(1)
  {
    worker_thread_list[worker].lock_key.lock();

    // TODO
    cout << "[Sub Task 2]\t working... COUNT:" << count++ <<endl;

    auto output = td2.getData();
    if (output != nullptr)
    {
      cout << "[Sub Task 2]\t working... GET:" << (char*)output[0] <<endl;
    }

    usleep(1000);
  }
}

int TinyPlatform::initialize()
{
  int i;
  for (i=0; i < BASE_TASK_NUM; i++)
  {
    worker_thread_list[i].worker = thread(runMainWorkerTask1, i);
  }

  worker_thread_list[i].worker = thread(runSubWorkerTask1, i++);
  worker_thread_list[i].worker = thread(runSubWorkerTask2, i++);

  cout << "i=" << i << endl;

  return 1;
}

int TinyPlatform::start()
{
  int buffer_size = 0;

  while(1)
  {
    buffer_size = JOB_NUM;
    if (buffer_size == JOB_NUM)
    {
      for(int i=0; i < JOB_NUM; i++)
      {
        worker_thread_list[i].lock_key.unlock();
      }
    }
  }
  return 1;
}

int main()
{
    cout << "################################" << endl;
    cout << "Start Tiny Platform             " << endl;
    cout << "################################" << endl;

    TinyPlatform tiny;
  
    tiny.initialize();
    tiny.start();
  
    while(1)
    {
      usleep(1000000);
    };
  
    cout << "Platform stoped." << endl;

    return 0;
}
