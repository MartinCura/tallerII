#ifndef PROJECT_RECOMMENDATIONSHANLDER_H
#define PROJECT_RECOMMENDATIONSHANDLER_H

#include "../../person/PersonManager.h"
#include "../../api/handler/Handler.h"

class RecommendationsHandler : public Handler{
public:
    RecommendationsHandler();
    ~RecommendationsHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);

private:
    long getToUserFromQueryParams(string queryParams);
    long getFromUserFromQueryParams(string queryParams);
};

#endif //PROJECT_RECOMMENDATIONSHANDLER_H
