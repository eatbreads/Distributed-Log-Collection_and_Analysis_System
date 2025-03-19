#include <iostream>
#include <csignal>
#include <cstring>
#include <librdkafka/rdkafkacpp.h>

bool run = true;

void sigterm(int sig) {
    run = false;
}

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb(RdKafka::Message &message) override {
        if (message.err()) {
            std::cerr << "Message delivery failed: " << message.errstr() << std::endl;
        } else {
            std::cout << "Message delivered to topic " << message.topic_name() <<
                         " [" << message.partition() << "] at offset " <<
                         message.offset() << std::endl;
        }
    }
};

class ExampleRebalanceCb : public RdKafka::RebalanceCb {
public:
    void rebalance_cb(RdKafka::KafkaConsumer *consumer,
                      RdKafka::ErrorCode err,
                      std::vector<RdKafka::TopicPartition *> &partitions) override {
        std::cerr << "Rebalance event: " << RdKafka::err2str(err) << std::endl;
        if (err == RdKafka::ERR__ASSIGN_PARTITIONS) {
            consumer->assign(partitions);
        } else {
            consumer->unassign();
        }
    }
};