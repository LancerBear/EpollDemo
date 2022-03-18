#include <iostream>
#include <pthread.h>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include "MyEpoll.h"
#include "MesgNode.h"

using namespace std;

void dealMesg(MesgNode& node) {
    cout << "get mesg " << node.getMesgCode() << endl;

    return;
}

int main()
{
    unsigned short port = 2222;
    queue<MesgNode> q;
    MyEpoll ep(port, &q);

    while (1) {
        if (!q.empty()) {
            MesgNode node = q.front();
            q.pop();
            dealMesg(node);
        }
        usleep(100);
    }

    return 0;
}
