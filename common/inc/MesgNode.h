#ifndef __MESG_NODE_H__
#define __MESG_NODE_H__

#define MESG_DATA_LEN 50

#define GET_MESG_CODE_FROM_BUF(x) (int)(*(int*)x)
#define GET_MESG_FROM_BUF(x) (unsigned char*)((unsigned char*)x + sizeof(int))

class MesgNode {
private:
    int mesgCode;
    unsigned char data[MESG_DATA_LEN];
public:
    MesgNode(int code, unsigned char data[]);
    ~MesgNode();
    int getMesgCode();
    unsigned char* getData();
};

#endif