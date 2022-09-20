#pragma once

#include <memory>
#include "core/interface/threadpool_interface.h"

namespace Core {
class ThreadPool final : public IThreadPool {
 public:
  explicit ThreadPool(std::size_t threadCount);
  virtual ~ThreadPool();
  virtual void Post(Work work) override;
  virtual void Stop() override;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace Core