#include <brpc/redis.h>
#include <brpc/channel.h>
#include <string>
namespace nameService
{
    class RedisImpl
    {
    public:
        int set(std::string &key, std::string &value);
        int get(std::string &key, std::string &value);
        int setSet(std::string &key, std::string &value);
        int setSet(std::string &key, std::unordered_set<std::string> set);
        void getSet(std::string &key, std::unordered_set<std::string> &set);
        RedisImpl()
        {
            brpc::ChannelOptions redis_options;
            redis_options.protocol = brpc::PROTOCOL_REDIS;
            if (redis_channel.Init(redis_addr.c_str(), &redis_options) != 0)
            {
                LOG(ERROR) << "Fail to init channel to redis-server"
                           << "\n";
            }
        };

    private:
        brpc::Channel redis_channel;
        const std::string redis_addr = "42.194.133.175:6379";
    };
}