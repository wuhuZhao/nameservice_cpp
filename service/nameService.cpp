#include "nameService.h"
#include "utils/string_utils.h"
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
        std::pair<std::string, std::unordered_set<std::string>> nameToHostPair;
        for (auto it = cntl->http_request().uri().QueryBegin();
             it != cntl->http_request().uri().QueryEnd(); ++it)
        {
            if (it->first == "name")
            {
                nameToHostPair.first = it->second;
            }
            else if (it->first == "host")
            {
                const std::string &hostList = it->second;
                auto hostVec = util::split(hostList, ';');
                std::unordered_set<std::string> hostSet;
                for (auto host : hostVec)
                {
                    hostSet.emplace(host);
                }
                nameToHostPair.second = hostSet;
            }
            else
            {
                continue;
            }
        }
        // auto body = cntl->request_attachment();
        butil::IOBufBuilder os;
        if (nameToHostPair.first.size() == 0 || nameToHostPair.second.size() == 0)
        {
            os << "{\"data\": null, \"msg\": \"Fail\", \"code\": -1}"
               << "\n";
            os.move_to(cntl->response_attachment());
            return;
        }
        int ret = redisImpl.get()->setSet(nameToHostPair.first, nameToHostPair.second);
        if (ret == -1)
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
        std::unordered_set<std::string> value;
        int ret = redisImpl.get()->getSet(key, value);
        if (ret == -1)
        {
            os << "{\"data\": null, \"msg\": \"Fail\", \"code\": -1}"
               << "\n";
        }
        else
        {
            os << "{\"data\": \"[";
            std::string data;
            for (auto it = value.begin(); it != value.end(); it++)
            {
                data.append(*it);
                data.append(",");
                LOG(ERROR) << *it << "\n";
            }
            if (data.size() != 0)
            {
                data = data.substr(0, data.size() - 1);
            }
            os << data << "]\", \"msg\": \"Success\", \"code\": 200}"
               << "\n";
        }
        os.move_to(cntl->response_attachment());
        return;
    };
}
