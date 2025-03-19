#include "httplib.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <librdkafka/rdkafkacpp.h>
#include <kafka.h>
using json = nlohmann::json;
using namespace httplib;



void set_cors(httplib::Response& res)
{
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
}

int main() {

    Server svr;
    
    svr.Get("/api/logs", [](const Request& req, Response& res) {
        // 解析查询参数
        set_cors(res);

        auto keyword = req.get_param_value("keyword");
        auto field = req.get_param_value("field");
        auto start_time = req.get_param_value("start_time");
        auto end_time = req.get_param_value("end_time");


        // 模拟日志数据
        json logs = {
            { {"id", 1}, {"level", "INFO"}, {"message", "系统启动成功"}, {"timestamp", "2025-03-18 12:00:00"}, {"details", "详细日志内容..."} },
            { {"id", 2}, {"level", "ERROR"}, {"message", "数据库连接失败"}, {"timestamp", "2025-03-18 12:10:00"}, {"details", "错误详情..."} }
        };

        // 设置响应头，返回 JSON
        res.set_header("Content-Type", "application/json");
        res.status = 200;
        res.body = logs.dump();
    });

    std::cout << "Server listening on port 8089..." << std::endl;
    svr.listen("0.0.0.0", 8089);

    return 0;
}