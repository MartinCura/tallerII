#ifndef PROJECT_PICTUREHANDLER_H
#define PROJECT_PICTUREHANDLER_H

#include <fstream>
#include "Handler.h"
#include "../../Exceptions/ErrorDeletingFileException.h"

class PictureHandler : public Handler {
public:
    PictureHandler();
    ~PictureHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);

private:
    Response* buildGetPictureResponse(long id);
    string getFilePath(long id);
    bool existsPictureForId(long id);
    void deletePicture(long id);
    void savePicture(long id, const char* body, size_t size);
};

#endif //PROJECT_PICTUREHANDLER_H
