#include "Mongoose/mongoose.h"
#include "api/WebHandler.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_REQUEST) {
        struct http_message *httpMessage = (struct http_message *) ev_data;
        WebHandler *webHandler = new WebHandler();
        Response* response = webHandler->handleRequest(httpMessage, ev);
        mg_printf(nc, "%s", response->getHeader());
        mg_printf_http_chunk(nc, response->getBody());
        mg_send_http_chunk(nc, "", 0);
        delete webHandler;
        delete response;
    }
}

int main(int argc, char *argv[]) {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    struct mg_bind_opts bind_opts;
    int i;
    char *cp;
    const char *err_str;
#ifdef MG_ENABLE_SSL
    const char *ssl_cert = NULL;
#endif
    mg_mgr_init(&mgr, NULL);
    if (argc > 0 && ((cp = strrchr(argv[0], DIRSEP)) != NULL)) {
        *cp = '\0';
        s_http_server_opts.document_root = argv[0];
    }
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0 && i + 1 < argc) {
            mgr.hexdump_file = argv[++i];
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            s_http_server_opts.document_root = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            s_http_port = argv[++i];
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            s_http_server_opts.auth_domain = argv[++i];
#ifdef MG_ENABLE_JAVASCRIPT
            } else if (strcmp(argv[i], "-j") == 0 && i + 1 < argc) {
      const char *init_file = argv[++i];
      mg_enable_javascript(&mgr, v7_create(), init_file);
#endif
        } else if (strcmp(argv[i], "-P") == 0 && i + 1 < argc) {
            s_http_server_opts.global_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-A") == 0 && i + 1 < argc) {
            s_http_server_opts.per_directory_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            s_http_server_opts.url_rewrites = argv[++i];
#ifndef MG_DISABLE_CGI
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            s_http_server_opts.cgi_interpreter = argv[++i];
#endif
#ifdef MG_ENABLE_SSL
            } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      ssl_cert = argv[++i];
#endif
        } else {
            fprintf(stderr, "Unknown option: [%s]\n", argv[i]);
            exit(1);
        }
    }

    memset(&bind_opts, 0, sizeof(bind_opts));
    bind_opts.error_string = &err_str;
#ifdef MG_ENABLE_SSL
    if (ssl_cert != NULL) {
    bind_opts.ssl_cert = ssl_cert;
  }
#endif
    nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s: %s\n", s_http_port,
                *bind_opts.error_string);
        exit(1);
    }

    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.enable_directory_listing = "yes";

    printf("Starting RESTful server on port %s, serving %s\n", s_http_port,
           s_http_server_opts.document_root);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
    return 0;
}
