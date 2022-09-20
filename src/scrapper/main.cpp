#include "config/config.h"
#include "engine/engine.h"

#include <core/logger.h>

using namespace Scrapper;

int main(int argc, char const** argv) try {
  auto config = Config::New(argc, argv);
  Engine scrapper(config->Host(), config->Port());
  scrapper.Run();
  scrapper.Join();
} catch (const std::exception& ex) {
  LOG_ERROR("%s", ex.what());
}
