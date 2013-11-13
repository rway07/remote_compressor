#include "threadHelper.h"

threadHelper::threadHelper() 
{
}

threadHelper::threadHelper(const threadHelper& orig) 
{
}

threadHelper::~threadHelper() 
{
}

int threadHelper::thread_start()
{
    return pthread_create(&thread_id, NULL, thread_launcher, this);
}

void* threadHelper::thread_launcher(void* This)
{
    ((threadHelper*)This)->handle_command();
    return 0;
}
