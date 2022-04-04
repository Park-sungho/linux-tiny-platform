
#include <iostream>
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

int TinyPlatform::initialize()
{
}

int TinyPlatform::start()
{
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
