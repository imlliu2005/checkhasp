#include "hasp.h"
#include <iostream>
#include <stdio.h>         //printf()
#include <unistd.h>        //pause()
#include <signal.h>        //signal()
#include <string.h>        //memset()
#include <sys/time.h>      //struct itimerval, setitimer()

static int count = 0;

void check(int signo)
{
    int ret = findHasp();
    std::string hasp = "nohasp";
    if (ret == 1) hasp = "hashasp";
    std::cout<<hasp<<std::endl;
}

int main()
{
    struct itimerval tick;
    
    signal(SIGALRM, check);
    memset(&tick, 0, sizeof(tick));

    //Timeout to run first time
    tick.it_value.tv_sec = 1;
    tick.it_value.tv_usec = 0;

    //After first, the Interval time for clock
    tick.it_interval.tv_sec = 3;
    tick.it_interval.tv_usec = 0;

    if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
    {
        printf("Set timer failed!\n");
    }
    
    //When get a SIGALRM, the main process will enter another loop for pause()
    int n;
    while(std::cin >> n)
    {
        pause();
    }
    return 0;
}

