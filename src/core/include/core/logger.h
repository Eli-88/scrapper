#pragma once

#include "core/interface/logger_interface.h"

#include <atomic>
#include <mutex>

#define LOG(lvl, ...)                                                         \
  Core::LoggerSingleton::GetLogger()->Log(lvl, __FILE__, __PRETTY_FUNCTION__, \
                                          __LINE__, __VA_ARGS__)

#define LOG_TRACE()                                                      \
  Core::LoggerSingleton::GetLogger()->Log(Core::ILogger::Level::Trace,   \
                                          __FILE__, __PRETTY_FUNCTION__, \
                                          __LINE__, "")

#define LOG_DEBUG(...) LOG(Core::ILogger::Level::Debug, __VA_ARGS__)
#define LOG_INFO(...) LOG(Core::ILogger::Level::Info, __VA_ARGS__)
#define LOG_WARN(...) LOG(Core::ILogger::Level::Warn, __VA_ARGS__)
#define LOG_ERROR(...) LOG(Core::ILogger::Level::Error, __VA_ARGS__)
#define LOG_FATAL(...) LOG(Core::ILogger::Level::Fatal, __VA_ARGS__)

#define SET_LOG_LEVEL(lvl) Core::LoggerSingleton::GetLogger()->SetLevel(lvl);

namespace Core {
class ConsoleLogger : public ILogger {
 public:
  virtual ~ConsoleLogger() = default;

 protected:
  virtual void DoLog(Level lvl,
                     const char* file,
                     const char* function,
                     int line,
                     const char* fmt,
                     va_list args) override;
};

class ThreadsafeConsoleLogger : public ConsoleLogger {
 private:
  std::mutex mux_;

  virtual void DoLog(Level lvl,
                     const char* file,
                     const char* function,
                     int line,
                     const char* fmt,
                     va_list args) override;
};

struct LoggerSingleton {
  static std::shared_ptr<ILogger> GetLogger();
  static void SetLogger(std::shared_ptr<ILogger> logger);
  static void SetLevel(ILogger::Level lvl);
};

}  // namespace Core