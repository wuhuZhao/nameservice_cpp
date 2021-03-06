#include "nameService.h"
#include "utils/string_utils.h"
#include "utils/response_utils.h"
#include "proxy/NameServiceStoreProxy.h"
#include <json2pb/rapidjson.h>
#include <json2pb/pb_to_json.h>
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
        auto body = cntl->request_attachment().to_string();
        butil::rapidjson::Document document;
        document.Parse(body.c_str());
        std::string name = document["name"].GetString();
        auto hostVec = util::split(document["host"].GetString(), ';');
        // std::unordered_set<std::string> hostSet;
        // for (auto host : hostVec)
        // {
        //     hostSet.emplace(host);
        // }
        butil::IOBufBuilder os;
        RpcResponse response;
        std::string res;
        std::string error;
        if (name.size() == 0 || hostVec.size() == 0)
        {
            util::generateFailResponse(&res);
            os << res << "\n";
            os.move_to(cntl->response_attachment());
            return;
        }
        // int ret = redisImpl.get()->setSet(name, hostSet);
        std::vector<bool> healthys(hostVec.size(), true);
        std::vector<int> weights(hostVec.size(), 100);
        int ret = proxy::insertAnyLineNameServiceTopMysql(name, hostVec, healthys, weights);
        if (ret == -1)
            util::generateFailResponse(&res);
        else
            util::generateSuccessResponse(&res);
        os << res << "\n";
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
        std::string res;
        if (ret == -1)
            util::generateFailResponse(&res);
        else
        {
            std::string data;
            data.push_back('[');
            for (auto it = value.begin(); it != value.end(); it++)
            {
                data.append(*it);
                data.append(",");
            }
            if (data.size() != 0)
            {
                data = data.substr(0, data.size() - 1);
            }
            data.push_back(']');
            util::generateSuccessResponse(&res, data);
        }
        os << res << "\n";
        os.move_to(cntl->response_attachment());
        return;
    };

    void NameServiceImpl::updateName2Ip(google::protobuf::RpcController *cntl_base,
                                        const nameService::HttpRequest *,
                                        const nameService::HttpResponse *,
                                        google::protobuf::Closure *done)
    {
        brpc::ClosureGuard done_guard(done);
        brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);
        cntl->http_response().set_content_type("application_json");
        auto body = cntl->request_attachment().to_string();
        butil::rapidjson::Document document;
        document.Parse(body.c_str());
        std::string name = document["name"].GetString();
        auto hostVec = util::split(document["host"].GetString(), ';');
        std::unordered_set<std::string> hostSet;
        for (auto host : hostVec)
        {
            hostSet.emplace(host);
        }
        butil::IOBufBuilder os;
        std::string res;
        if (name.size() == 0 || hostSet.size() == 0)
        {
            util::generateFailResponse(&res);
            os << res << "\n";
            os.move_to(cntl->response_attachment());
            return;
        }
        int ret = redisImpl.get()->updateSet(name, hostSet);
        if (ret == -1)
            util::generateFailResponse(&res);
        else
            util::generateSuccessResponse(&res);
        os << res << '\n';
        os.move_to(cntl->response_attachment());
        return;
    };
    void NameServiceImpl::deletename2Ip(google::protobuf::RpcController *cntl_base,
                                        const nameService::HttpRequest *,
                                        const nameService::HttpResponse *,
                                        google::protobuf::Closure *done)
    {
        brpc::ClosureGuard done_guard(done);
        brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);
        cntl->http_response().set_content_type("application_json");
        auto body = cntl->request_attachment().to_string();
        butil::rapidjson::Document document;
        document.Parse(body.c_str());
        std::string name = document["name"].GetString();
        auto hostVec = util::split(document["host"].GetString(), ';');
        std::unordered_set<std::string> hostSet;
        for (auto host : hostVec)
        {
            hostSet.emplace(host);
        }
        butil::IOBufBuilder os;
        std::string res;
        if (name.size() == 0 || hostSet.size() == 0)
        {
            util::generateFailResponse(&res);
            os << res << "\n";
            os.move_to(cntl->response_attachment());
            return;
        }
        int ret = redisImpl.get()->deleteSet(name, hostSet);
        if (ret == -1)
            util::generateFailResponse(&res);
        else
            util::generateSuccessResponse(&res);

        os << res << '\n';
        os.move_to(cntl->response_attachment());
        return;
    };
}
