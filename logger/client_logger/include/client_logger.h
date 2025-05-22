#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"
#include <fstream>
#include <nlohmann/json.hpp>

class client_logger final:
    public logger
{

private:
    std::map<std::string, unsigned char> _files;
    std::string _output_format;
    static std::map<std::string, std::pair<std::ofstream*, int>> all_streams;

    client_logger(std::map<std::string, unsigned char> const &files, std::string const& output_format);
    friend class client_logger_builder;

public:

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;


private:
    void copy(client_logger const &other);
    void move(client_logger &&other);
    void clear();
    std::string formating_string(std::string const &text, logger::severity severity) const;
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H