#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/restful.h>
#include "proto/nameservice.pb.h"




namespace nameService {
    class NameServiceImpl : public nameService::HttpService {
        public:
            NameServiceImpl() {};
            virtual ~NameServiceImpl() {};
            void addName2Ip(google::protobuf::RpcController* cntl_base,
                            const nameService::HttpRequest*,
                            nameService::HttpResponse*,
                            google::protobuf::Closure* done);
    private: 
        std::unordered_map<std::string,std::string> name2Host{};
    };
}