#include "DummyServer.h"

DummyServer::DummyServer() {}

DummyServer::~DummyServer() {}

string DummyServer::getFirebaseDummyResponse() {
    return "";
}

string DummyServer::getFacebookDummyResponse() {
    string response = "";
    response += "HTTP/1.1 200 OK\n";
    response += "x-app-usage: {\"call_count\":2,\"total_cputime\":1,\"total_time\":2}\n";
    response += "x-fb-rev: 2702404\n";
    response += "x-fb-trace-id: Cf9CGhVLlFm\n";
    response += "Expires: Sat, 01 Jan 2000 00:00:00 GMT\n";
    response += "Content-Type: application/json; charset=UTF-8\n";
    response += "facebook-api-version: v2.8\n";
    response += "Cache-Control: private, no-cache, no-store, must-revalidate\n";
    response += "Pragma: no-cache\n";
    response += "ETag: \"883a40b13cf35bc6f98afbe90d124efee9798267\"\n";
    response += "Access-Control-Allow-Origin: *\n";
    response += "Vary: Accept-Encoding\n";
    response += "X-FB-Debug: wb85HnWzGrROPDsHluYraPDNVCgVSkS2adiz0PxseiMJHf3S8wqXRNOUicXszztkyGSyVaB80TpYq+TpjrgE4g==\n";
    response += "Date: Thu, 24 Nov 2016 20:01:35 GMT\n";
    response += "Transfer-Encoding: chunked\n";
    response += "Connection: keep-alive\r\n";
    response += "\r\n";
    response += "{\"first_name\":\"John\",\"last_name\":\"Doe\",\"birthday\":\"01/01/1990\",\"email\":\"john\u0040doe.com\",\"id\":\"1000000\"}";
    return response;
}