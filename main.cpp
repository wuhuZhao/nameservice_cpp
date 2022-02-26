#include "service/nameService.h"
#include <brpc/redis.h>
#include <brpc/channel.h>
DEFINE_int32(port, 8081, "TCP Port of this server");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
            "read/write operations during the last `idle_timeout_s`");
DEFINE_int32(logoff_ms, 2000, "Maximum duration of server's LOGOFF state "
             "(waiting for client to close connection before server stops)");

DEFINE_string(certificate, "/root/brpc-bazel/cert.pem", "Certificate file path to enable SSL");
DEFINE_string(private_key, "/root/brpc-bazel/key.pem", "Private key file path to enable SSL");
DEFINE_string(ciphers, "", "Cipher suite used for SSL connections");
int main(int argc, char* argv[]) {
    GFLAGS_NS::ParseCommandLineFlags(&argc, &argv, true);
    brpc::Server server;
    nameService::NameServiceImpl http_svc;

    if (server.AddService(&http_svc,
                          brpc::SERVER_DOESNT_OWN_SERVICE,
                          "/v1/nameservice/add => addName2Ip,"
                          "/v1/nameservice/search => getName2Ip,"
                          "/v1/nameservice/update => updateName2Ip,"
                          "/v1/nameservice/delete => deleteName2Ip"
                          ) != 0) {
        LOG(ERROR) << "Fail to add http_svc";
        return -1;
    }
    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    options.mutable_ssl_options()->default_cert.certificate = FLAGS_certificate;
    options.mutable_ssl_options()->default_cert.private_key = FLAGS_private_key;
    options.mutable_ssl_options()->ciphers = FLAGS_ciphers;
    if (server.Start(FLAGS_port, &options) !=0) {
        LOG(ERROR) << "Fail to start HttpServer";
        return -1;
    }
    server.RunUntilAskedToQuit();
    return 0;
}