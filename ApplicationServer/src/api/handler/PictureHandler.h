#ifndef PROJECT_PICTUREHANDLER_H
#define PROJECT_PICTUREHANDLER_H

#include <fstream>
#include "Handler.h"

class PictureHandler : public Handler {
public:
    PictureHandler();
    ~PictureHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage);
    Response* handlePutRequest(http_message* httpMessage);

private:
    Response* buildGetPictureResponse(int id);
};

#endif //PROJECT_PICTUREHANDLER_H
