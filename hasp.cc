#include "hasp.h"

static const char* _PATH_PARTITIONS = "/proc/partitions";

int checkHaspDisk(FILE* diskfp){
    fseek(diskfp, 0x100, SEEK_SET);
    char mark[9];
    fread(mark, 9, 1, diskfp);
    if(memcmp(mark, "curacloud", 9) == 0)
        return 1;
    return 0;
}

int findHasp(){
    FILE* fp = fopen(_PATH_PARTITIONS, "r");
    char line[0xff];
    while(fgets(line,0xff,fp)){
        int major = -1, minor = -1, blocks = -1;
        char name[0xff];
        sscanf(line, "%d %d %d %s", &major, &minor, &blocks, name);
        if(-1 == major || -1 == minor || -1 == blocks || memcmp(name, "ram", 3) == 0)
            continue;
        char dev[0xff];
        sprintf(dev, "/dev/%s", name);
        FILE *diskfp = fopen(dev, "r+b");
        if(diskfp == NULL)
            continue;
        int nR = checkHaspDisk(diskfp);
        fclose(diskfp);
        if(!nR){
            continue;
        } else {
            fclose(fp);
            return nR;
        }         
    }
    fclose(fp);
    return 0;
}   

