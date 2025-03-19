#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>


class KafkaConsumer
{
public:/**
     * @brief KafkaConsumer
     * @param brokers
     * @param groupID
     * @param topics
     * @param partition
     */
    explicit KafkaConsumer(const std::string& brokers, const std::string& groupID,
                           const std::vector<std::string>& topics, int partition);
    void pullMessage();
    ~KafkaConsumer();
protected:
    std::string m_brokers;
    std::string m_groupID;
    std::vector<std::string> m_topicVector;
    int m_partition;
    RdKafka::Conf* m_config;
    RdKafka::Conf* m_topicConfig;
    RdKafka::KafkaConsumer* m_consumer;
    RdKafka::EventCb* m_event_cb;
    RdKafka::RebalanceCb* m_rebalance_cb;
};


class ConsumerEventCb : public RdKafka::EventCb
{
public:
    void event_cb (RdKafka::Event &event)
    {
        switch (event.type())
        {
        case RdKafka::Event::EVENT_ERROR:
            if (event.fatal())
            {
                std::cerr << "FATAL ";
            }
            std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<
                      event.str() << std::endl;
            break;

        case RdKafka::Event::EVENT_STATS:
            std::cerr << "\"STATS\": " << event.str() << std::endl;
            break;

        case RdKafka::Event::EVENT_LOG:
            fprintf(stderr, "LOG-%i-%s: %s\n",
                    event.severity(), event.fac().c_str(), event.str().c_str());
            break;

        case RdKafka::Event::EVENT_THROTTLE:
            std::cerr << "THROTTLED: " << event.throttle_time() << "ms by " <<
                      event.broker_name() << " id " << (int)event.broker_id() << std::endl;
            break;

        default:
            std::cerr << "EVENT " << event.type() <<
                      " (" << RdKafka::err2str(event.err()) << "): " <<
                      event.str() << std::endl;
            break;
        }
    }
};

class ConsumerRebalanceCb : public RdKafka::RebalanceCb
{
private:
    static void printTopicPartition (const std::vector<RdKafka::TopicPartition*>&partitions)        // 打印当前获取的分区
    {
        for (unsigned int i = 0 ; i < partitions.size() ; i++)
            std::cerr << partitions[i]->topic() <<
                      "[" << partitions[i]->partition() << "], ";
        std::cerr << "\n";
    }

public:
    void rebalance_cb (RdKafka::KafkaConsumer *consumer,
                       RdKafka::ErrorCode err,
                       std::vector<RdKafka::TopicPartition*> &partitions)
    {
        std::cerr << "RebalanceCb: " << RdKafka::err2str(err) << ": ";
        printTopicPartition(partitions);
        if (err == RdKafka::ERR__ASSIGN_PARTITIONS)
        {
            consumer->assign(partitions);
            partition_count = (int)partitions.size();
        }
        else
        {
            consumer->unassign();
            partition_count = 0;
        }
    }
private:
    int partition_count;
};

KafkaConsumer::KafkaConsumer(const std::string& brokers, const std::string& groupID,
                             const std::vector<std::string>& topics, int partition)
{
    m_brokers = brokers;
    m_groupID = groupID;
    m_topicVector = topics;
    m_partition = partition;

    std::string errorStr;
    RdKafka::Conf::ConfResult errorCode;
    m_config = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    m_event_cb = new ConsumerEventCb;
    errorCode = m_config->set("event_cb", m_event_cb, errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Conf set failed: " << errorStr << std::endl;
    }

    m_rebalance_cb = new ConsumerRebalanceCb;
    errorCode = m_config->set("rebalance_cb", m_rebalance_cb, errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Conf set failed: " << errorStr << std::endl;
    }

    errorCode = m_config->set("enable.partition.eof", "false", errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Conf set failed: " << errorStr << std::endl;
    }

    errorCode = m_config->set("group.id", m_groupID, errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Conf set failed: " << errorStr << std::endl;
    }
    errorCode = m_config->set("bootstrap.servers", m_brokers, errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Conf set failed: " << errorStr << std::endl;
    }
    errorCode = m_config->set("max.partition.fetch.bytes", "1024000", errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Conf set failed: " << errorStr << std::endl;
    }

    // partition.assignment.strategy  range,roundrobin

    m_topicConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    // 获取最新的消息数据
    errorCode = m_topicConfig->set("auto.offset.reset", "latest", errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Topic Conf set failed: " << errorStr << std::endl;
    }
    errorCode = m_config->set("default_topic_conf", m_topicConfig, errorStr);
    if(errorCode != RdKafka::Conf::CONF_OK)
    {
        std::cout << "Conf set failed: " << errorStr << std::endl;
    }
    m_consumer = RdKafka::KafkaConsumer::create(m_config, errorStr);
    if(m_consumer == NULL)
    {
        std::cout << "Create KafkaConsumer failed: " << errorStr << std::endl;
    }
    std::cout << "Created consumer " << m_consumer->name() << std::endl;
}

void msg_consume(RdKafka::Message* msg, void* opaque)
{
    switch (msg->err())
    {
    case RdKafka::ERR__TIMED_OUT:
        std::cerr << "消费者错误: " << msg->errstr() << std::endl;  // 超时
        break;
    case RdKafka::ERR_NO_ERROR:     // 有消息进来
        std::cout << " 消息进入-> 主题:" << msg->topic_name() << "partition:["
                  << msg->partition() << "] 位于偏移量 " << msg->offset()
                  << " 键: " << msg->key() << " 负载: "
                  << (char*)msg->payload() << std::endl;
        break;
    default:
        std::cerr << "消费者错误: " << msg->errstr() << std::endl;
        break;
    }
}

void KafkaConsumer::pullMessage()
{
    // 订阅Topic
    RdKafka::ErrorCode errorCode = m_consumer->subscribe(m_topicVector);
    if (errorCode != RdKafka::ERR_NO_ERROR)
    {
        std::cout << "订阅失败: " << RdKafka::err2str(errorCode) << std::endl;
    }
    // 消费消息
    while(true)
    {
        RdKafka::Message *msg = m_consumer->consume(1000);
        msg_consume(msg, NULL);
        delete msg;
    }
}

KafkaConsumer::~KafkaConsumer()
{
    m_consumer->close();
    delete m_config;
    delete m_topicConfig;
    delete m_consumer;
    delete m_event_cb;
    delete m_rebalance_cb;

}



class KafkaProducer
{
public:
	/**
	* @brief KafkaProducer
	* @param brokers
	* @param topic
	* @param partition
	*/
	explicit KafkaProducer(const std::string& brokers, const std::string& topic, int partition);
	/**
	* @brief push Message to Kafka
	* @param str, message data
	*/
	void pushMessage(const std::string& str, const std::string& key);
	~KafkaProducer();

private:
	std::string m_brokers;			// Broker列表，多个使用逗号分隔
	std::string m_topicStr;			// Topic名称
	int m_partition;				// 分区

	RdKafka::Conf* m_config;        // Kafka Conf对象
	RdKafka::Conf* m_topicConfig;   // Topic Conf对象
	RdKafka::Topic* m_topic;		// Topic对象
	RdKafka::Producer* m_producer;	// Producer对象

	/*只要看到Cb 结尾的类，要继承它然后实现对应的回调函数*/
	RdKafka::DeliveryReportCb* m_dr_cb;
	RdKafka::EventCb* m_event_cb;
	RdKafka::PartitionerCb* m_partitioner_cb;
};


// call back
class ProducerDeliveryReportCb : public RdKafka::DeliveryReportCb
{
public:
	void dr_cb(RdKafka::Message &message)
	{
		if(message.err())
			std::cerr << "Message delivery failed: " << message.errstr() << std::endl;
		else
		{
			// Message delivered to topic test [0] at offset 135000
			std::cerr << "Message delivered to topic " << message.topic_name()
				<< " [" << message.partition() << "] at offset "
				<< message.offset() << std::endl;
		}
	}
};

class ProducerEventCb : public RdKafka::EventCb
{
public:
	void event_cb(RdKafka::Event &event)
	{
		switch (event.type())
		{
		case RdKafka::Event::EVENT_ERROR:
			std::cout << "RdKafka::Event::EVENT_ERROR: " << RdKafka::err2str(event.err()) << std::endl;
			break;
		case RdKafka::Event::EVENT_STATS:
			std::cout << "RdKafka::Event::EVENT_STATS: " << event.str() << std::endl;
			break;
		case RdKafka::Event::EVENT_LOG:
			std::cout << "RdKafka::Event::EVENT_LOG " << event.fac() << std::endl;
			break;
		case RdKafka::Event::EVENT_THROTTLE:
			std::cout << "RdKafka::Event::EVENT_THROTTLE " << event.broker_name() << std::endl;
			break;
		}
	}
};

class HashPartitionerCb : public RdKafka::PartitionerCb
{
public:
	int32_t partitioner_cb(const RdKafka::Topic *topic, const std::string *key,
		int32_t partition_cnt, void *msg_opaque)
	{
		char msg[128] = { 0 };
		int32_t partition_id = generate_hash(key->c_str(), key->size()) % partition_cnt;
		//                          [topic][key][partition_cnt][partition_id] 
		//                          :[test][6419][2][1]
		sprintf(msg, "HashPartitionerCb:topic:[%s], key:[%s]partition_cnt:[%d], partition_id:[%d]", topic->name().c_str(),
			key->c_str(), partition_cnt, partition_id);
		std::cout << msg << std::endl;
		return partition_id;
	}
private:

	static inline unsigned int generate_hash(const char *str, size_t len)
	{
		unsigned int hash = 5381;
		for (size_t i = 0; i < len; i++)
			hash = ((hash << 5) + hash) + str[i];
		return hash;
	}
};


KafkaProducer::KafkaProducer(const std::string& brokers, const std::string& topic, int partition)
{
	m_brokers = brokers;
	m_topicStr = topic;
	m_partition = partition;

	/* 创建Kafka Conf对象 */
	m_config = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	if(m_config==NULL)
		std::cout << "Create RdKafka Conf failed." << std::endl;

	/* 创建Topic Conf对象 */
	m_topicConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	if (m_topicConfig == NULL)
		std::cout << "Create RdKafka Topic Conf failed." << std::endl;

	/* 设置Broker属性 */
	RdKafka::Conf::ConfResult errCode;
	std::string errorStr;
	m_dr_cb = new ProducerDeliveryReportCb;
	// 设置dr_cb属性值
	errCode = m_config->set("dr_cb", m_dr_cb, errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		std::cout << "Conf set failed:" << errorStr << std::endl;
	}
	// 设置event_cb属性值
	m_event_cb = new ProducerEventCb;
	errCode = m_config->set("event_cb", m_event_cb, errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		std::cout << "Conf set failed:" << errorStr << std::endl;
	}
	// 自定义分区策略
	m_partitioner_cb = new HashPartitionerCb;
	errCode = m_topicConfig->set("partitioner_cb", m_partitioner_cb, errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		std::cout << "Conf set failed:" << errorStr << std::endl;
	}
	// 设置配置对象的属性值
	errCode = m_config->set("statistics.interval.ms", "10000", errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		std::cout << "Conf set failed:" << errorStr << std::endl;
	}
	errCode = m_config->set("message.max.bytes", "10240000", errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		std::cout << "Conf set failed:" << errorStr << std::endl;
	}
	errCode = m_config->set("bootstrap.servers", m_brokers, errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		std::cout << "Conf set failed:" << errorStr << std::endl;
	}

	/* 创建Producer */
	m_producer = RdKafka::Producer::create(m_config, errorStr);
	if (m_producer == NULL)
	{
		std::cout << "Create Producer failed:" << errorStr << std::endl;
	}

	/* 创建Topic对象 */
	m_topic = RdKafka::Topic::create(m_producer, m_topicStr, m_topicConfig, errorStr);
	if (m_topic == NULL)
	{
		std::cout << "Create Topic failed:" << errorStr << std::endl;
	}
}

KafkaProducer::~KafkaProducer()
{
	while (m_producer->outq_len() > 0)
	{
		std::cerr << "Waiting for " << m_producer->outq_len() << std::endl;
		m_producer->flush(5000);
	}
	delete m_config;
	delete m_topicConfig;
	delete m_topic;
	delete m_producer;
	delete m_dr_cb;
	delete m_event_cb;
	delete m_partitioner_cb;
}

void KafkaProducer::pushMessage(const std::string& str, const std::string& key)
{
	int32_t len = str.length();
	void* payload = const_cast<void*>(static_cast<const void*>(str.data()));
	RdKafka::ErrorCode errorCode = m_producer->produce(
		m_topic,
		RdKafka::Topic::PARTITION_UA,
		RdKafka::Producer::RK_MSG_COPY,
		payload,
		len,
		&key,
		NULL);
	m_producer->poll(0);
	if (errorCode != RdKafka::ERR_NO_ERROR)
	{
		std::cerr << "Produce failed: " << RdKafka::err2str(errorCode) << std::endl;
		if (errorCode == RdKafka::ERR__QUEUE_FULL)
		{
			m_producer->poll(100);
		}
	}
}
