#include "scrape_manager.h"
#include "handler/scrape_handler.h"

#include <core/interface/net/client_interface.h>
#include <core/net/context_manager.h>
#include <core/threadpool.h>
#include <core/timer_factory.h>

#include <iostream>

namespace Net = Core::Net;

namespace Scrapper {

ScrapeManager::ScrapeManager()
    : timerFactory_(std::make_unique<Core::TimerFactory>()),
      taskPool_(std::make_unique<Core::ThreadPool>(4)),
      contextManager_(std::make_unique<Core::Net::ContextManager>()) {}

void ScrapeManager::Dispatch(const std::string& host,
                             const std::string& port,
                             const std::string& target,
                             int version,
                             const RegexCollection& regexCollection) {
  auto responseHandler =
      std::make_shared<Handler::ScrapeHandler>(*taskPool_, regexCollection);
  const auto key = host + target;
  auto handlerIter = responseHandlerMapping_.find(key);

  if (handlerIter == responseHandlerMapping_.end()) {
    responseHandlerMapping_.insert({key, responseHandler});
    Core::ITimer::Work work = [this, host, port, target, version]() {
      this->Send(Net::HttpGet, "", host.c_str(), port.c_str(), target.c_str(),
                 version);
    };

    auto timer = timerFactory_->CreateTimer(
        *contextManager_, work,
        5);  // TODO: find out better timeout interval
             // or maybe even add them in the request
    timer->Run();
  } else {
    handlerIter->second = responseHandler;
  }
}

void ScrapeManager::Run() {
  contextManager_->Run();
}

void ScrapeManager::Send(Net::HttpVerb httpVerb,
                         const std::string& body,
                         const std::string& host,
                         const std::string& port,
                         const std::string& target,
                         int version) {
  auto handler = responseHandlerMapping_[host + target];
  assert(handler);  // should not fail as they should be set already before
                    // sending

  Net::CreateSecureHttpClient(*contextManager_, *handler)
      ->Send(httpVerb, body, host.c_str(), port.c_str(), target.c_str(),
             version);
}
}  // namespace Scrapper