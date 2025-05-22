#include <not_implemented.h>

#include "../include/client_logger_builder.h"

#include <client_logger.h>

client_logger_builder::client_logger_builder(): _output_format("%d %t %s %m") {

}

client_logger_builder::client_logger_builder(
    client_logger_builder const &other)
{
    copy(other);
}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder const &other)
{
    if(this != &other) {
        files.clear();
        copy(other);
    }
    return *this;
}

client_logger_builder::client_logger_builder(
    client_logger_builder &&other) noexcept
{
    move(std::move(other));
}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder &&other) noexcept
{
    if(this != &other) {
        files.clear();
        move(std::move(other));
    }
    return *this;
}

client_logger_builder::~client_logger_builder() noexcept
{
    files.clear();
    _output_format = " ";
}

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    if(stream_file_path.empty())
        throw std::logic_error("Path cant be empty");
    std::string absolute_file_path = get_absolute_path(stream_file_path);
    std::map<std::string, unsigned char>::iterator i = files.find(absolute_file_path);

    if (i != files.end()) {
        if((i->second >> static_cast<int>(severity)) & 1 )
            return this;
        i->second ^= (1 << static_cast<int>(severity));
        return this;
    }
    files.emplace(absolute_file_path, 1 << static_cast<int>(severity));
    return this;
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    std::map<std::string, unsigned char>::iterator i = files.find("cerr");
    if(i != files.end()) {
        //if((i->second >> static_cast<int>(severity)) & 1)
        //    return this;
        i->second |= (1 << static_cast<int>(severity));
        return this;
    }
    files.emplace("cerr", (1 << static_cast<int>(severity)));
    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream configuration_file(configuration_file_path);
    if(!configuration_file)
        throw std::runtime_error("File does not exist");

    nlohmann::json data;
    configuration_file >> data;

    for(const auto &file_path : data[configuration_path]) {
        std::string path = file_path["path"];
        auto severity = file_path["severity"];

        for(const std::string &s : severity)
            this->add_file_stream(path, string_to_severity(s));
    }

    this->_output_format = data["format"];

    configuration_file.close();

    return this;
}

logger_builder *client_logger_builder::clear()
{
    files.clear();
    _output_format = " ";
    return this;
}

logger *client_logger_builder::build() const
{
    return new client_logger(files, _output_format);
}

void client_logger_builder::copy(client_logger_builder const &other) {
    files = other.files;
    _output_format = other._output_format;
}

void client_logger_builder::move(client_logger_builder &&other) {
    files = std::move(other.files);
    _output_format = std::move(other._output_format);
}

inline std::string const client_logger_builder::get_absolute_path(std::string const &file_path) {
    return std::filesystem::absolute(file_path).string();
}