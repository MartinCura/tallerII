#ifndef APPLICATIONSERVER_RESPONSE_H
#define APPLICATIONSERVER_RESPONSE_H


class Response {
public:
    Response();
    ~Response();

    const char* getHeader();
    const char* getBody();
    void setHeader(const char* header);
    void setBody(const char* body);

    void setSuccessfulHeader();
    void setNotFoundHeader();
    void setNotImplementedHeader();
private:
    const char* header;
    const char* body;
};

#endif //APPLICATIONSERVER_RESPONSE_H
