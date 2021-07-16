#include "widget.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/async_logger.h"
#include "ui_widget.h"
#include <iostream>

namespace spd = spdlog;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 基于文件的简单日志
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
    // 基于滚动文件的日志，每个文件5MB，三个文件
    // auto logger = spdlog::rotating_logger_mt("file_logger", "myfilename", 1024 * 1024 * 5, 3);

    // 定制输出格式
    spdlog::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");

    // 多个日志器共享SINK
    auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logfile", 23, 59);
    // 下面几个同步日志器共享的输出到目标文件
    auto net_logger = std::make_shared<spdlog::logger>("net", daily_sink);
    auto hw_logger = std::make_shared<spdlog::logger>("hw", daily_sink);
    auto db_logger = std::make_shared<spdlog::logger>("db", daily_sink);

    // 一个日志器使用多个SINK
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back( std::make_shared<spdlog::sinks::stdout_sink_st>());
    sinks.push_back( std::make_shared<spdlog::sinks::daily_file_sink_st>( "logfile", 23, 59 ));
    auto combined_logger = std::make_shared<spdlog::logger>( "name", begin( sinks ), end( sinks ));
    spdlog::register_logger( combined_logger );

    // 异步
    // 每个日志器分配8192长度的队列，队列长度必须2的幂
    //s pdlog::set_async_mode(8192);
    // 程序退出前清理
    // spdlog::drop_all();


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    try
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}
