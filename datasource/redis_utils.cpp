#include "datasource/redis_utils.h"

/**
 * @brief 设置redis的key-value对
 * 
 * @param key 
 * @param value 
 * @return int 
 */
int nameService::RedisImpl::set(std::string& key,std::string& value) {
    brpc::RedisRequest set_request;
    brpc::RedisResponse response;
    brpc::Controller cntl;
    set_request.AddCommand("SET %s %s", key.c_str(),value.c_str());
    redis_channel.CallMethod(nullptr, &cntl, &set_request, &response, nullptr);
    if (cntl.Failed()) {
        LOG(ERROR) << "Fail to access redis-server" << "\n";
        return -1;
    }
    if (response.reply(0).is_error()) {
        LOG(ERROR) << "Fail to set" << "\n";
        return -1;
    }
    return 0;
}

/**
 * @brief 通过key获取redis的value
 * 
 * @param key 
 * @param value 
 * @return int 
 */
int nameService::RedisImpl::get(std::string& key,std::string& value) {
    brpc::RedisRequest set_request;
    brpc::RedisResponse response;
    brpc::Controller cntl;
    set_request.AddCommand("GET %s", key.c_str());
    redis_channel.CallMethod(nullptr, &cntl, &set_request, &response, nullptr);
    if (cntl.Failed()) {
        LOG(ERROR) << "Fail to access redis-server" << "\n";
        return -1;
    }
    if (response.reply(0).is_error()) {
        LOG(ERROR) << "Fail to set" << "\n";
        return -1;
    }
    value = response.reply(0).is_string() ? response.reply(0).data().as_string() : "";
    return 0;
}

