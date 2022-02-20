#include "datasource/redis_utils.h"

/**
 * @brief 设置redis的key-value对
 *
 * @param key
 * @param value
 * @return int
 */
int nameService::RedisImpl::set(std::string &key, std::string &value)
{
    brpc::RedisRequest set_request;
    brpc::RedisResponse response;
    brpc::Controller cntl;
    set_request.AddCommand("SET %s %s", key.c_str(), value.c_str());
    redis_channel.CallMethod(nullptr, &cntl, &set_request, &response, nullptr);
    if (cntl.Failed())
    {
        LOG(ERROR) << "Fail to access redis-server"
                   << "\n";
        return -1;
    }
    if (response.reply(0).is_error())
    {
        LOG(ERROR) << "Fail to set"
                   << "\n";
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
int nameService::RedisImpl::get(std::string &key, std::string &value)
{
    brpc::RedisRequest set_request;
    brpc::RedisResponse response;
    brpc::Controller cntl;
    set_request.AddCommand("GET %s", key.c_str());
    redis_channel.CallMethod(nullptr, &cntl, &set_request, &response, nullptr);
    if (cntl.Failed())
    {
        LOG(ERROR) << "Fail to access redis-server"
                   << "\n";
        return -1;
    }
    if (response.reply(0).is_error())
    {
        LOG(ERROR) << "Fail to set"
                   << "\n";
        return -1;
    }
    value = response.reply(0).is_string() ? response.reply(0).data().as_string() : "";
    return 0;
}

/**
 * @brief 向redis集合中添加
 *
 * @param key std::string&
 * @param set std::unordered_set<std::string>& set
 * @return int 返回成功插入结果的数量
 */
int nameService::RedisImpl::setSet(std::string &key, std::unordered_set<std::string> &set)
{
    brpc::RedisRequest set_request;
    brpc::RedisResponse response;
    brpc::Controller cntl;
    for (auto it = set.begin(); it != set.end(); it++)
    {
        set_request.AddCommand("SADD %s %s", key.c_str(), it->c_str());
    }
    redis_channel.CallMethod(nullptr, &cntl, &set_request, &response, nullptr);
    if (cntl.Failed())
    {
        LOG(ERROR) << "Fail to access redis-server"
                   << "\n";
        return -1;
    }
    int ret = response.reply_size();
    return ret;
}

/**
 * @brief 查看redis集合
 *
 * @param key std::string& key
 * @param set 需要赋值的引用 std::unordered_set<std::string>& set
 * @return int 返回-1则代表连接失败
 */
int nameService::RedisImpl::getSet(std::string &key, std::unordered_set<std::string> &set)
{
    brpc::Controller cntl;
    brpc::RedisRequest set_request;
    brpc::RedisResponse response;
    set_request.AddCommand("SMEMBERS %s", key.c_str());
    redis_channel.CallMethod(nullptr, &cntl, &set_request, &response, nullptr);
    if (cntl.Failed())
    {
        LOG(ERROR) << "Fail to access redis-server"
                   << "\n";
        return -1;
    }
    int ret = response.reply_size();

    set.clear();
    for (int n = 0; n < ret; n++)
    {
        for (size_t i = 0; i < response.reply(n).size(); i++)
        {
            set.emplace(response.reply(n)[i].data().as_string());
        }
    }
    return ret;
}