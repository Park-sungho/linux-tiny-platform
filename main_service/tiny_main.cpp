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

struct job
{
  thread jobThread;
  mutex jobMutex;
}

job jobs[JOB_NUM];

int count = 0;

template<typename T>
struct MyStruct
{
  MyStruct(T* param) : data(param) {}
  ~MyStruct() { delete data; }
  void setData(T* param) {data=param;}
  T* getData() {return data;}

  private:
    T* data;
};

//MyStruct<char*> td2(nullptr);
MyStruct<void*> td2(nullptr);

void runMainWorkerTask1(int worker)
{
  while(1)
  {
    jobs[worker].jobMutext.lock();
    // do some job
    cout << "[Main Task 1]\t working... COUNT:" << count++ <<endl;
    usleep(1000);
  }
}

void runSubWorkerTask1(int worker)
{
  while(1)
  {
    jobs[worker].jobMutext.lock();
    // do some job
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
    jobs[worker].jobMutext.lock();
    // do some job
    cout << "[Sub Task 2]\t working... COUNT:" << count++ <<endl;

    auto output = td2.getData();
    if (output != nullptr)
    {
      out << "[Sub Task 2]\t working... GET:" << (char*)output[0] <<endl;
    }

    usleep(1000);
  }
}

int TinyPlatform::initialize()
{
  int i;
  for (i=0;i<BASE_TASK_NUM;i++)
  {
    jobs[i].jobThread = thread(runMainWorkerTask1, i);
  }

  jobs[i].jobThread = thread(runSubWorkerTask1, i++);
  jobs[i].jobThread = thread(runSubWorkerTask2, i++);

  cout << "i=" << i << endl;

  return 1;
}

int TinyPlatform::start()
{
  int bufferSize = 0;

  while(1)
  {
    bufferSize = JOB_NUM;
    if (bufferSize==JOB_NUM)
    {
      for(int i=0;i<JOB_NUM;i++)
      {
        jobs[i].jobMutex.unlock();
      }
    }
    //break;
  }
  return 1;
}

int main()
{
    cout << "################################" << endl;
    cout << "Start Tiny Platform            ." << endl;
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
