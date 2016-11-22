#include "MainHelper.h"

MainHelper::MainHelper() {}

MainHelper::~MainHelper() {}

bool MainHelper::validBody(struct mbuf body, struct mbuf bodyToSend) {
    bool validBody = true;
    int offset = 7;
    const char* bodyPointer = body.buf + body.len - 1;
    const char* bodyToSendPointer = bodyToSend.buf + bodyToSend.len - 1;
    bodyToSendPointer = bodyToSendPointer - offset;
    for(int i = 0; i < body.len; i++) {
        if (*bodyPointer != *bodyToSendPointer) {
            validBody = false;
            break;
        }
        bodyPointer--;
        bodyToSendPointer--;
    }
    return validBody;
}
