#include <gflags/gflags.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <brpc/channel.h>
#include "proto/nameservice_store.pb.h"

DEFINE_string(attachment, "", "Carry this along with requests");
DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_string(server, "127.0.0.1:8002", "IP Address of server");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)");
DEFINE_int32(interval_ms, 1000, "Milliseconds between consecutive requests");

namespace proxy
{

    int insertAnyLineNameServiceTopMysql(std::string &serviceName, std::vector<std::string> &hosts, std::vector<bool> &healthys, std::vector<int> &weights)
    {
        brpc::Channel channel;
        brpc::ChannelOptions options;
        options.protocol = FLAGS_protocol;
        options.connection_type = FLAGS_connection_type;
        // 2秒超时
        options.timeout_ms = 2000;
        options.max_retry = FLAGS_max_retry;
        if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0)
        {
            LOG(ERROR) << "Fail to initialize channel"
                       << "\n";
            return -1;
        }
        nameService::InsertAnyLineRequest request;
        nameService::nameServiceStore_Stub stub(&channel);
        nameService::InsertAnyLineResponse response;
        brpc::Controller cntl;
        request.set_service_name(serviceName);
        assert(hosts.size() == weights.size());
        assert(hosts.size() == healthys.size());
        for (size_t i = 0; i < hosts.size(); i++)
        {
            auto *ptr = request.add_tuples();
            ptr->set_host(hosts[i]);
            ptr->set_weight(weights[i]);
            ptr->set_healthy(healthys[i]);
        }
        stub.insertAnyNameService(&cntl, &request, &response, nullptr);
        if (cntl.Failed())
        {
            LOG(WARNING) << cntl.ErrorText();
            return -1;
        }

        if (response.mutable_response()->code() != 200)
        {
            LOG(ERROR) << "insert error from server"
                       << "\n";
            return -1;
        }
        return 0;
    }

    int insertOneLineNameServiceToMysql(std::string &serviceName, std::string &host, bool healthy, int weight)
    {

        brpc::Channel channel;

        // Initialize the channel, NULL means using default options.
        brpc::ChannelOptions options;
        options.protocol = FLAGS_protocol;
        options.connection_type = FLAGS_connection_type;
        options.timeout_ms = FLAGS_timeout_ms /*milliseconds*/;
        options.max_retry = FLAGS_max_retry;
        if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0)
        {
            LOG(ERROR) << "Fail to initialize channel"
                       << "\n";
            return -1;
        }

        nameService::nameServiceStore_Stub stub(&channel);

        nameService::InsertOneLineRequest request;
        nameService::NameServiceTuple tuple;
        nameService::InsertOneLineResponse response;
        brpc::Controller cntl;
        tuple.set_host(host);
        tuple.set_weight(weight);
        tuple.set_healthy(healthy);
        request.set_service_name(serviceName);
        request.set_allocated_tuple(&tuple);
        stub.insertOneNameService(&cntl, &request, &response, nullptr);

        if (cntl.Failed())
        {
            LOG(WARNING) << cntl.ErrorText();
            return -1;
        }

        if (response.mutable_response()->code() != 200)
        {
            LOG(ERROR) << "insert error from server"
                       << "\n";
            return -1;
        }
        return 0;
    }
}