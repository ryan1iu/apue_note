#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'a'
#define NAMESIZE 64

struct transfer_st{
    long mtype;
    char name[NAMESIZE];
    int math;
    int english;
};


#endif
