#ifndef THREADHELPER_H
#define	THREADHELPER_H

#include "../common/common.h"

class threadHelper 
{
public:
    threadHelper();
    threadHelper(const threadHelper& orig);
    virtual ~threadHelper();

    int thread_start();
    
protected:
    virtual void handle_command() = 0;
    
private:
    static void* thread_launcher(void *This);
    
    pthread_t thread_id;
};

#endif	/* THREADHELPER_H */

