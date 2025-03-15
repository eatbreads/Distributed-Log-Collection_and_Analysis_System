#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <map>
#include <sstream>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace fs = std::filesystem;

// 日志目录
const std::string LOG_DIR = "log_output";

// 生成随机整数
int random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// 生成随机浮点数
double random_double(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(min, max);
    return distrib(gen);
}

// 生成时间戳
std::string get_timestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// 日志管理类
class LogManager {
public:
    LogManager() {
        fs::create_directories(LOG_DIR);  // 确保日志目录存在

        // 初始化日志文件映射
        log_files = {
            {"app", LOG_DIR + "/app.log"},
            {"system", LOG_DIR + "/system.log"},
            {"network", LOG_DIR + "/network.log"},
            {"database", LOG_DIR + "/database.log"},
            {"container", LOG_DIR + "/container.log"}
        };

        // 配置 loggers
        for (const auto& [type, file] : log_files) {
            auto logger = spdlog::rotating_logger_mt(type, file, 1024 * 1024 * 5, 3); // 5MB 分割日志，最多保留 3 份
            logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
            spdlog::flush_every(std::chrono::seconds(1)); // 每 1 秒刷新日志
            loggers[type] = logger;
        }
    }

    void write_log(const std::string& log_type, const std::string& message) {
        if (loggers.find(log_type) != loggers.end()) {
            loggers[log_type]->info(message);
        }
    }

private:
    std::map<std::string, std::string> log_files;
    std::map<std::string, std::shared_ptr<spdlog::logger>> loggers;
};

// 日志生成函数
void generate_app_log(LogManager& log_manager) {
    std::vector<std::string> messages = {
        "User login success, user_id=" + std::to_string(random_int(1000, 9999)),
        "Order created, order_id=" + std::to_string(random_int(10000, 99999)),
        "Error: Database connection failed",
        "Debug: Processing payment for order_id=" + std::to_string(random_int(10000, 99999))
    };
    log_manager.write_log("app", messages[random_int(0, messages.size() - 1)]);
}

void generate_system_log(LogManager& log_manager) {
    std::string timestamp = get_timestamp();
    std::vector<std::string> messages = {
        "System rebooted at " + timestamp,
        "High CPU usage detected: " + std::to_string(random_int(80, 100)) + "%",
        "Disk space low: " + std::to_string(random_int(1, 10)) + "% remaining",
        "Process crashed: process_id=" + std::to_string(random_int(100, 999))
    };
    log_manager.write_log("system", messages[random_int(0, messages.size() - 1)]);
}

void generate_network_log(LogManager& log_manager) {
    std::vector<std::string> messages = {
        "HTTP request: 200 GET /api/data from IP " +
        std::to_string(random_int(1, 255)) + "." + std::to_string(random_int(1, 255)) + "." +
        std::to_string(random_int(1, 255)) + "." + std::to_string(random_int(1, 255)),

        "Firewall blocked IP " +
        std::to_string(random_int(1, 255)) + "." + std::to_string(random_int(1, 255)) + "." +
        std::to_string(random_int(1, 255)) + "." + std::to_string(random_int(1, 255)),

        "DDoS attack detected, blocking traffic from " +
        std::to_string(random_int(1, 255)) + "." + std::to_string(random_int(1, 255)) + "." +
        std::to_string(random_int(1, 255)) + "." + std::to_string(random_int(1, 255))
    };
    log_manager.write_log("network", messages[random_int(0, messages.size() - 1)]);
}

void generate_database_log(LogManager& log_manager) {
    std::vector<std::string> messages = {
        "MySQL slow query detected, execution time=" + std::to_string(random_double(1.5, 10.0)) + "s",
        "User " + std::to_string(random_int(1000, 9999)) + " updated table 'orders'",
        "Redis cache miss for key 'session:" + std::to_string(random_int(1000, 9999)) + "'"
    };
    log_manager.write_log("database", messages[random_int(0, messages.size() - 1)]);
}

void generate_container_log(LogManager& log_manager) {
    std::vector<std::string> messages = {
        "Kubernetes Pod restarted: pod_id=pod-" + std::to_string(random_int(100, 999)),
        "Container killed due to OOM: container_id=cont-" + std::to_string(random_int(1000, 9999)),
        "Auto-scaling triggered: scaled up to " + std::to_string(random_int(5, 20)) + " replicas"
    };
    log_manager.write_log("container", messages[random_int(0, messages.size() - 1)]);
}

// 统一日志生成控制
void generate_logs() {
    LogManager log_manager;

    // 定义日志生成函数列表
    std::vector<void (*)(LogManager&)> log_functions = {
        generate_app_log,
        generate_system_log,
        generate_network_log,
        generate_database_log,
        generate_container_log
    };

    while (true) {
        int index = random_int(0, log_functions.size() - 1);
        log_functions[index](log_manager);

        std::this_thread::sleep_for(std::chrono::milliseconds(random_int(500, 2000)));
    }
}

int main() {
    generate_logs();
    return 0;
}
