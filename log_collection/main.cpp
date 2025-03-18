#include "httplib.h"
#include <iostream>
#include <nlohmann/json.hpp>

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

    svr.Get("/api/alerts",[](const Request& req, Response& res) {
        std::cout << "进入"<<std::endl;
        set_cors(res);
        auto status = req.get_param_value("status");
        auto module = req.get_param_value("module");
        auto start_time = req.get_param_value("start_time");
        auto end_time = req.get_param_value("end_time");

        json alerts = 
        {
            {{"id",1}, {"type", "Database"}, {"module","database"}, {"timestamp", "2025-03-18 12:00:00"},{"status","unprocessed"},{"handler","小鸡"},{"remark","未完成，小鸡待处理"}},
            {{"id",2}, {"type", "Network"}, {"module","firewall"}, {"timestamp", "2025-03-18 12:05:00"},{"status","unprocessed"},{"handler","大牛"},{"remark","防火墙异常，大牛待处理"}},
            {{"id",3}, {"type", "System"}, {"module","server"}, {"timestamp", "2025-03-18 12:10:00"},{"status","processed"},{"handler","小明"},{"remark","服务器重启完成"}},
            {{"id",4}, {"type", "Security"}, {"module","antivirus"}, {"timestamp", "2025-03-18 12:15:00"},{"status","unprocessed"},{"handler","小红"},{"remark","病毒库更新失败，小红待处理"}},
            {{"id",5}, {"type", "Application"}, {"module","web"}, {"timestamp", "2025-03-18 12:20:00"},{"status","processed"},{"handler","阿强"},{"remark","Web服务已恢复"}},
            {{"id",6}, {"type", "Hardware"}, {"module","disk"}, {"timestamp", "2025-03-18 12:25:00"},{"status","unprocessed"},{"handler","老王"},{"remark","磁盘空间不足，老王待处理"}}
        };

        res.set_header("Content-Type", "application/json");
        res.status = 200;
        res.body = alerts.dump();
        std::cout << "返回成功"<<std::endl;
    });

    std::cout << "Server listening on port 8089..." << std::endl;
    svr.listen("0.0.0.0", 8089);

    return 0;
}