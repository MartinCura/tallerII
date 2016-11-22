#include <string>
#include <iostream>
#include <thread>
#include "gtest/gtest.h"
#include "../Mongoose/mongoose.h"
#include "../api/WebHandler.h"
#include "../logger/Logger.h"
#include "../tools/DbBuilder.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static int s_sig_num = 0;
static string configFile = "../ApplicationServer/src/tests/config.js";

bool validBody(struct mbuf body, struct mbuf bodyToSend) {
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

struct mbuf processMessage(struct mg_connection *nc, struct http_message *httpMessage, WebHandler *webHandler, Response* response) {
    response = webHandler->handleRequest(httpMessage);
    struct mbuf body;
    body.buf = (char*) response->getBody();
    body.len = response->getBodyLength();
    mg_printf(nc, response->getHeader());
    if (response->hasBinaryContent) {
        mg_send_http_chunk(nc, body.buf, body.len);
    } else {
        mg_printf_http_chunk(nc, body.buf, body.len);
    }
    mg_send_http_chunk(nc, "", 0);
    return body;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_REQUEST) {
        WebHandler *webHandler = new WebHandler();
        Response* response = new Response();
        struct http_message *httpMessage = (struct http_message *) ev_data;
        struct mbuf body = processMessage(nc, httpMessage, webHandler, response);
        if (!validBody(body, nc->send_mbuf)) {
            Logger::getInstance()->info("Rearmando body de la respuesta");
            mbuf_remove(&nc->send_mbuf, nc->send_mbuf.len);
            processMessage(nc, httpMessage, webHandler, response);
        }
        delete response;
        delete webHandler;
    }
}

void runAppServer() {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    struct mg_bind_opts bind_opts;
    const char *err_str;
    mg_mgr_init(&mgr, NULL);
    memset(&bind_opts, 0, sizeof(bind_opts));
    bind_opts.error_string = &err_str;
    nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.enable_directory_listing = "yes";
    while (s_sig_num == 0) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}

void loadDB() {
    DbBuilder* dbb = new DbBuilder();
    dbb->loadUsers();
    delete dbb;
}

void deleteDB(string dbName) {
    string removeDataBaseCommand = "rm -rf " + dbName;
    system(removeDataBaseCommand.c_str());
}

void runTest(string test, string dbName) {
    loadDB();
    int result = system(test.c_str());
    if (result != 0) {
        ASSERT_TRUE(false) << "Fallo el test: " + test;
    }
    deleteDB(dbName);
}

TEST(Testing, Api) {
    if (FILE *file = fopen(configFile.c_str(), "r")) {
        fclose(file);
    } else {
        ASSERT_TRUE(false) << "No se pudo cargar el archivo de configuracion";
        return;
    }
    Config::getInstance()->load(configFile);
    string dbName = Config::getInstance()->get(Config::NAME_DB);
    if (dbName == "") {
        ASSERT_TRUE(false) << "Nombre de base de datos invalido";
        return;
    }
    std::thread t1(runAppServer);
    runTest("resttest.py http://127.0.0.1:8000 ../ApplicationServer/src/tests/apitests/testing_pictures.yaml", dbName);
    runTest("resttest.py http://127.0.0.1:8000 ../ApplicationServer/src/tests/apitests/testing_allusers.yaml", dbName);
    runTest("resttest.py http://127.0.0.1:8000 ../ApplicationServer/src/tests/apitests/testing_contacts.yaml", dbName);
    runTest("resttest.py http://127.0.0.1:8000 ../ApplicationServer/src/tests/apitests/testing_messages.yaml", dbName);
    runTest("resttest.py http://127.0.0.1:8000 ../ApplicationServer/src/tests/apitests/testing_notificationtoken.yaml", dbName);
    runTest("resttest.py http://127.0.0.1:8000 ../ApplicationServer/src/tests/apitests/testing_recommendations.yaml", dbName);
    runTest("resttest.py http://127.0.0.1:8000 ../ApplicationServer/src/tests/apitests/testing_users.yaml", dbName);
    s_sig_num = 1;
    t1.join();
    delete Config::getInstance();
}
