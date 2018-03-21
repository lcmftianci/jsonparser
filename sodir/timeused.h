#ifndef _TIME_USED_H_
#define _TIME_USED_H_

#include <stdio.h>
#include <sys/time.h>

//extern FILE* g_fp;
class timeused{
public:
    timeused()
    {
         gettimeofday(&tpstart,NULL);
    }

    ~timeused()
    {
         gettimeofday(&tpend,NULL);
         //timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;//注意，秒的读数和微秒的读数都应计算在内
         timeuse = (1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;
         printf("used time:%fms\n",timeuse);
    }
private:
     struct timeval tpstart,tpend;
     double timeuse;
};

#endif
