//
// Created by Jason Mohoney on 7/2/20.
//

#ifndef MARIUS_LOGGER_H
#define MARIUS_LOGGER_H
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <string>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

using std::string;
using std::shared_ptr;

class MariusLogger {
  private:
    shared_ptr<spdlog::sinks::basic_file_sink_mt> trace_sink_;
    shared_ptr<spdlog::sinks::basic_file_sink_mt> debug_sink_;
    shared_ptr<spdlog::sinks::basic_file_sink_mt> info_sink_;
    shared_ptr<spdlog::sinks::basic_file_sink_mt> warn_sink_;
    shared_ptr<spdlog::sinks::basic_file_sink_mt> error_sink_;
    shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_;
  public:
    shared_ptr<spdlog::logger> main_logger_;
    shared_ptr<spdlog::logger> eval_trace_;
    shared_ptr<spdlog::logger> train_trace_;

    MariusLogger(string filename) {
        spdlog::drop_all();

        console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink_->set_level(spdlog::level::info);
        console_sink_->set_pattern("[%l] [%x %T.%e] %v");

        trace_sink_ = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("logs/{}_{}.log", filename, "trace"), true);
        trace_sink_->set_level(spdlog::level::trace);
        trace_sink_->set_pattern("[%l] [%x %T.%e] [PID:%P TID:%t] [%s:%!:%#] %v");

        debug_sink_ = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("logs/{}_{}.log", filename, "debug"), true);
        debug_sink_->set_level(spdlog::level::debug);
        debug_sink_->set_pattern("[%l] [%x %T.%e] [PID:%P TID:%t] [%s:%!:%#] %v");

        info_sink_ = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("logs/{}_{}.log", filename, "info"), true);
        info_sink_->set_level(spdlog::level::info);
        info_sink_->set_pattern("[%l] [%x %T.%e] %v");

        warn_sink_ = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("logs/{}_{}.log", filename, "warn"), true);
        warn_sink_->set_level(spdlog::level::warn);
        warn_sink_->set_pattern("[%l] [%x %T.%e] [PID:%P TID:%t] [%s:%!:%#] %v");

        error_sink_ = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("logs/{}_{}.log", filename, "error"), true);
        error_sink_->set_level(spdlog::level::err);
        error_sink_->set_pattern("[%l] [%x %T.%e] [PID:%P TID:%t] [%g:%s:%!:%#] %v");

        spdlog::sinks_init_list sink_list = {error_sink_, warn_sink_, info_sink_, debug_sink_, trace_sink_, console_sink_};

        main_logger_ = std::make_shared<spdlog::logger>("MariusLogger", sink_list.begin(), sink_list.end());

        main_logger_->set_level(spdlog::level::trace);

        train_trace_ = std::make_shared<spdlog::logger>("TrainPipeline", std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("logs/{}_{}.trace", filename, "train"), true));
        train_trace_->set_pattern("[%x %T.%e] %v");
        train_trace_->set_level(spdlog::level::trace);

        eval_trace_ = std::make_shared<spdlog::logger>("EvalPipeline", std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("logs/{}_{}.trace", filename, "evaluation"), true));
        eval_trace_->set_pattern("[%x %T.%e] %v");
        eval_trace_->set_level(spdlog::level::trace);

        spdlog::register_logger(main_logger_);
        spdlog::register_logger(train_trace_);
        spdlog::register_logger(eval_trace_);

        spdlog::flush_every(std::chrono::seconds(1));
    }

    void setConsoleLogLevel(spdlog::level::level_enum level) {
        console_sink_->set_level(level);
    }
};

#endif //MARIUS_LOGGER_H