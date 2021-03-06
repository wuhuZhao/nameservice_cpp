#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/restful.h>
#include "proto/nameservice.pb.h"
#include "datasource/redis_utils.h"

namespace nameService
{
    class NameServiceImpl : public nameService::HttpService
    {
    public:
        NameServiceImpl() : redisImpl(new RedisImpl()){};
        virtual ~NameServiceImpl(){};
        void addName2Ip(google::protobuf::RpcController *cntl_base,
                        const nameService::HttpRequest *,
                        nameService::HttpResponse *,
                        google::protobuf::Closure *done);
        void getName2Ip(google::protobuf::RpcController *cntl_base,
                        const nameService::HttpRequest *,
                        nameService::HttpResponse *,
                        google::protobuf::Closure *done);
        void updateName2Ip(google::protobuf::RpcController *cntl_base,
                           const nameService::HttpRequest *,
                           const nameService::HttpResponse *,
                           google::protobuf::Closure *done);
        void deletename2Ip(google::protobuf::RpcController *cntl_base,
                           const nameService::HttpRequest *,
                           const nameService::HttpResponse *,
                           google::protobuf::Closure *done);
        void addName2IpRpc(google::protobuf::RpcController *cntl_base,
                           const nameService::RpcRequest *req,
                           const nameService::RpcResponse *resp,
                           google::protobuf::Closure *done);

    private:
        std::unique_ptr<nameService::RedisImpl> redisImpl;
    };
}