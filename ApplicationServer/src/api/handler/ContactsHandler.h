#ifndef PROJECT_CONTACTSHANDLER_H
#define PROJECT_CONTACTSHANDLER_H

#include "../../person/PersonManager.h"
#include "../../person/relation/Contact.h"
#include "../../api/handler/Handler.h"

class ContactsHandler : public Handler {
public:
    ContactsHandler();
    ~ContactsHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);
};

#endif //PROJECT_CONTACTSHANDLER_H
