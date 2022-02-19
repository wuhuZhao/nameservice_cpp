#include "nameService.h"

namespace nameService
{
    void NameServiceImpl::addName2Ip(google::protobuf::RpcController *cntl_base,
                                     const nameService::HttpRequest *,
                                     nameService::HttpResponse *,
                                     google::protobuf::Closure *done)
    {
        brpc::ClosureGuard done_guard(done);
        brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);
        cntl->http_response().set_content_type("application_json");
        std::vector<std::string> res(2, "");
        for (auto it = cntl->http_request().uri().QueryBegin();
             it != cntl->http_request().uri().QueryEnd(); ++it)
        {
            if (it->first == "name")
            {
                res[0] = it->second;
            }
            else if (it->first == "host")
            {
                res[1] = it->second;
            }
            else
            {
                continue;
            }
        }
        butil::IOBufBuilder os;
        if (res[0] == "" || res[1] == "")
        {
            os << "query error" << '\n';
            os.move_to(cntl->response_attachment());
            return;
        }
        int ret = redisImpl.get()->set(res[0], res[1]);
        if (ret != 0)
        {
            os << "{\"data\": null, \"msg\": \"Fail\", \"code\": -1}"
               << "\n";
        }
        else
        {
            os << "{ \"data\": null, \"msg\": \"Success\", \"code\": 200 }" << '\n';
        }
        os.move_to(cntl->response_attachment());
        return;
    };
    void NameServiceImpl::getName2Ip(google::protobuf::RpcController *cntl_base,
                    const nameService::HttpRequest *,
                    nameService::HttpResponse *,
                    google::protobuf::Closure *done)
    {
        brpc::ClosureGuard done_guard(done);
        brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);
        cntl->http_response().set_content_type("application_json");
        std::string key{};
        for (auto it = cntl->http_request().uri().QueryBegin();
             it != cntl->http_request().uri().QueryEnd(); it++)
        {
            if (it->first == "name")
            {
                key = it->second;
            }
        }
        butil::IOBufBuilder os;
        std::string value;
        int ret = redisImpl.get()->get(key, value);
        if (ret == -1)
        {
            os << "{\"data\": null, \"msg\": \"Fail\", \"code\": -1}"
               << "\n";
        }
        else
        {
            os << "{\"data\": \"" << value << "\", \"msg\": \"Success\", \"code\": 200}"
               << "\n";
        }
        os.move_to(cntl->response_attachment());
        return;
    };
}
