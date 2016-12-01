#ifndef PROJECT_MAINHELPER_H
#define PROJECT_MAINHELPER_H

#include "../Mongoose/mongoose.h"

class MainHelper {

public:
    MainHelper();
    ~MainHelper();

    bool validBody(struct mbuf body, struct mbuf bodyToSend);
};

#endif //PROJECT_MAINHELPER_H
