#pragma once

#include <functional>

namespace Core {

class IThreadPool {
 public:
  using Work = std::function<void()>;

  virtual ~IThreadPool() = default;
  virtual void Post(Work work) = 0;
  virtual void Stop() = 0;
};
}  // namespace Core