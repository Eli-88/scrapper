#pragma once

#include "scrape_manager_interface.h"

#include <core/interface/net/response_handler_interface.h>
#include <core/interface/threadpool_interface.h>
#include <core/interface/timer_interface.h>
#include <core/interface/net/context_manager_interface.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace Scrapper {

class ScrapeManager final : public IScrapeManager {
 public:
  ScrapeManager();
  ScrapeManager(const ScrapeManager&) = delete;

  virtual void Run() override;

  virtual void Dispatch(const std::string& host,
                        const std::string& port,
                        const std::string& target,
                        int version,
                        const RegexCollection& regexCollection) override;

 private:
  using ResponseHandlerMapping =
      std::unordered_map<std::string,
                         std::shared_ptr<Core::Net::IHttpResponseHandler>>;

  std::unique_ptr<Core::ITimerFactory> timerFactory_;
  ResponseHandlerMapping responseHandlerMapping_{};
  std::unique_ptr<Core::IThreadPool> taskPool_;
  std::unique_ptr<Core::Net::IContextManager> contextManager_;

  void Send(Core::Net::HttpVerb httpVerb,
            const std::string& body,
            const std::string& host,
            const std::string& port,
            const std::string& target,
            int version);
};
}  // namespace Scrapper