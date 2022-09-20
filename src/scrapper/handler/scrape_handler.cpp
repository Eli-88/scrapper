#include "scrape_handler.h"
#include <core/logger.h>

#include <iostream>

namespace Scrapper {

namespace Handler {

ScrapeHandler::~ScrapeHandler() {}

ScrapeHandler::ScrapeHandler(Core::IThreadPool& threadPool,
                             const RegexCollection& regexPatterns)
    : threadPool_(threadPool) {
  for (const auto& pattern : regexPatterns) {
    regexSearchers_.emplace_back(Core::CreateRegexSearch(pattern));
  }
}

void ScrapeHandler::OnHttpResponse(std::string&& body) {
  auto response = std::make_shared<std::string>(std::move(body));
  for (auto& searcher : regexSearchers_) {
    Core::IThreadPool::Work work = [&searcher, response] {
      try {
        const auto result = searcher->Search(*response);
        if (result.empty()) {
          std::cout << "no result" << std::endl;
        } else {
          for (const auto& r : result) {
            std::cout << r << std::endl;
          }
        }
      } catch (const std::exception& ex) {
        LOG_DEBUG("%s", ex.what());
      }
    };
    threadPool_.Post(work);
  }
}
}  // namespace Handler
}  // namespace Scrapper