#include <string>
#include <json2pb/pb_to_json.h>
#include "proto/nameservice.pb.h"

namespace util
{

    void generateSuccessResponse(std::string *res, std::string &&data)
    {
        nameService::RpcResponse response;
        response.set_msg("Success");
        response.set_data(data);
        response.set_code(200);
        json2pb::ProtoMessageToJson(response, res, nullptr);
        return;
    }

    void generateSuccessResponse(std::string *res)
    {
        generateSuccessResponse(res, "null");
    }

    void generateSuccessResponse(std::string *res, std::string &data)
    {
        nameService::RpcResponse response;
        response.set_msg("Success");
        response.set_data(data);
        response.set_code(200);
        json2pb::ProtoMessageToJson(response, res, nullptr);
        return;
    }

    void generateFailResponse(std::string *res, std::string &&data)
    {
        nameService::RpcResponse response;
        response.set_msg("Fail");
        response.set_data(data);
        response.set_code(-1);
        json2pb::ProtoMessageToJson(response, res, nullptr);
        return;
    }

    void generateFailResponse(std::string *res, std::string &data)
    {
        nameService::RpcResponse response;
        response.set_msg("Fail");
        response.set_data(data);
        response.set_code(-1);
        json2pb::ProtoMessageToJson(response, res, nullptr);
        return;
    }
    void generateFailResponse(std::string *res)
    {
        generateFailResponse(res, "null");
    }
}