#include "hasp.h"
#include <iostream>
#include <signal.h>        //signal()
#include <sys/time.h>      //struct itimerval, setitimer()
#include <unistd.h> 

int alive = 1;

void isDvAlive() {
    FILE *fstream = NULL;
    char buff[1024]; 
    memset(buff, 0, sizeof(buff)); 
    fstream = popen("pgrep deepvesselffr","r");

    while(NULL != fgets(buff, sizeof(buff), fstream)) {} 

    if (strlen(buff) == 0) {
        alive = 0;
    }    
    pclose(fstream); 
};

void check(int signo)
{
    isDvAlive();
    if (!alive) return;
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
    while(alive){
        sleep(2000);
    }
    return 0;
}

