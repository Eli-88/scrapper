#include "core/logger.h"
#include "core/interface/logger_interface.h"

#include <stdarg.h>
#include <stdio.h>
#include <sstream>
#include <thread>

namespace Core {

void ILogger::Log(Level lvl,
                  const char* file,
                  const char* function,
                  int line,
                  const char* fmt,
                  ...) {
  auto currentLvl = lvl_.load();
  if (currentLvl <= lvl) {
    va_list args;
    va_start(args, fmt);

    DoLog(lvl, file, function, line, fmt, args);

    va_end(args);
  }
}

void ILogger::SetLevel(Level lvl) {
  lvl_.store(lvl);
}

std::string ILogger::LevelToString(Level lvl) const {
  switch (lvl) {
    case Level::Trace:
      return "trace";
    case Level::Debug:
      return "debug";
    case Level::Info:
      return "info";
    case Level::Warn:
      return "warn";
    case Level::Error:
      return "error";
    default:
      return "fatal";
  }
}

void ConsoleLogger::DoLog(Level lvl,
                          const char* file,
                          const char* function,
                          int line,
                          const char* fmt,
                          va_list args) {
  std::ostringstream os;
  os << "[" << LevelToString(lvl) << "] " << function << " " << file << ":"
     << line << " [tid:" << std::this_thread::get_id() << "]: " << fmt << "\n";
  vprintf(os.str().c_str(), args);
}

void ThreadsafeConsoleLogger::DoLog(Level lvl,
                                    const char* file,
                                    const char* function,
                                    int line,
                                    const char* fmt,
                                    va_list args) {
  std::lock_guard<std::mutex> lg(mux_);
  ConsoleLogger::DoLog(lvl, file, function, line, fmt, args);
}

std::shared_ptr<ILogger> Logger{std::make_shared<ThreadsafeConsoleLogger>()};

std::shared_ptr<ILogger> LoggerSingleton::GetLogger() {
  return Logger;
}

void LoggerSingleton::SetLogger(std::shared_ptr<ILogger> logger) {
  Logger = logger;
}

void LoggerSingleton::SetLevel(ILogger::Level lvl) {
  Logger->SetLevel(lvl);
}

}  // namespace Core