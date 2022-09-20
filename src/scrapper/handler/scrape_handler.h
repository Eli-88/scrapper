#pragma once

#include <core/interface/net/response_handler_interface.h>
#include <core/interface/regex_interface.h>
#include <core/interface/threadpool_interface.h>
#include <memory>
#include <string>
#include <vector>

namespace Scrapper {
namespace Handler {

class ScrapeHandler final : public Core::Net::IHttpResponseHandler {
 public:
  using RegexCollection = std::vector<std::string>;
  virtual ~ScrapeHandler();
  explicit ScrapeHandler(Core::IThreadPool& threadPool,
                         const RegexCollection& regexPatterns);
  virtual void OnHttpResponse(std::string&& body) override;

 private:
  using RegexSearchCollection =
      std::vector<std::unique_ptr<Core::IRegexSearch>>;
  Core::IThreadPool& threadPool_;
  RegexSearchCollection regexSearchers_;
};
}  // namespace Handler
}  // namespace Scrapper