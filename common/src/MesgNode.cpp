#include <cstring>
#include <stdlib.h>
#include "MesgNode.h"

MesgNode::MesgNode(int c, unsigned char d[]) : mesgCode(c) {
    memcpy(data, d, MESG_DATA_LEN);
}

MesgNode::~MesgNode() {
    memset(data, 0, MESG_DATA_LEN);
}

int MesgNode::getMesgCode() {
    return this->mesgCode;
}

unsigned char* MesgNode::getData() {
    return this->data;
}