#pragma once

#include <atomic>
#include <memory>

namespace Core {
class ILogger {
 public:
  enum class Level { Trace = 0, Debug, Info, Warn, Error, Fatal };

  virtual ~ILogger() = default;
  void Log(Level lvl,
           const char* file,
           const char* function,
           int line,
           const char* fmt,
           ...) __attribute__((format(printf, 6, 7)));

  void SetLevel(Level lvl);

 protected:
  std::string LevelToString(Level lvl) const;

 private:
  std::atomic<Level> lvl_{Level::Info};

  virtual void DoLog(Level lvl,
                     const char* file,
                     const char* function,
                     int line,
                     const char* fmt,
                     va_list args) = 0;
};

}  // namespace Core

const Core::ILogger::Level TRACE = Core::ILogger::Level::Trace;
const Core::ILogger::Level DEBUG = Core::ILogger::Level::Debug;
const Core::ILogger::Level INFO = Core::ILogger::Level::Info;
const Core::ILogger::Level WARN = Core::ILogger::Level::Warn;
const Core::ILogger::Level ERROR = Core::ILogger::Level::Error;
const Core::ILogger::Level FATAL = Core::ILogger::Level::Fatal;