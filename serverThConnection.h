#ifndef THCONNECTION_H_
#define THCONNECTION_H_

#include "commonThread.h"
#include "serverConnection.h"


class ThConnection : public Thread {
    private:
        Connection connection;
        bool dead;

    public:
        bool is_dead();
        ThConnection(Socket peer, Index & index);
        virtual void run();
        void stop();
};
#endif
