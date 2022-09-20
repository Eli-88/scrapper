add_executable(scrapper main.cpp)

target_sources(scrapper
    PRIVATE
        config/config.cpp
        engine/engine.cpp
        engine/scrape_manager.cpp
        handler/scrape_handler.cpp
        handler/healthcheck_handler.cpp
        handler/registration_handler.cpp
        protocol/healthcheck.cpp
        protocol/registration.cpp
        protocol/json_decoder.cpp)

target_include_directories(scrapper
    PRIVATE
        .)

target_link_libraries(scrapper
    PRIVATE
        core
        Boost::program_options)