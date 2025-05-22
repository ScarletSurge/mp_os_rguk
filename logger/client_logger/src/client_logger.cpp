#include <not_implemented.h>

#include "../include/client_logger.h"
std::map<std::string, std::pair<std::ofstream*, int>> client_logger::all_streams;


client_logger::client_logger(std::map<std::string, unsigned char> const &files, std::string const &format)
    :_files(files)
{
    for(auto i = 0; i < format.size(); i++) {
        if(format[i] == '%') {
            if(i == format.size() - 1)
                throw std::logic_error("Format is invalid");

            if(!isalpha(format[i+1]))
                throw std::logic_error("Format is invalid");
        }
    }
    _output_format = format;

    for(const auto& pair : _files) {
        if(pair.first != "cerr") {
            auto i = all_streams.find(pair.first); //std::unordered_map<std::string, std::pair<std::ofstream*, int>>::iterator

            if(i != all_streams.end())
                i->second.second += 1;

            else {
                std::ofstream* file = new std::ofstream(pair.first);

                if(!file) {
                    delete file;

                    for(auto f : all_streams){
                        f.second.first->close();
                        delete f.second.first;
                    }


                    throw std::runtime_error("File does not exist");
                }

                all_streams[pair.first] = {file, 1};
            }
        }
    }
}

client_logger::client_logger(
    client_logger const &other)
{
    copy(other);
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
    if(this != &other) {
        clear();
        copy(other);
    }

    return *this;
}

client_logger::client_logger(
    client_logger &&other) noexcept
{
    move(std::move(other));
}

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    if(this != &other) {
        clear();
        move(std::move(other));
    }

    return *this;
}

client_logger::~client_logger() noexcept
{
    for(const auto &pair : _files) {
        if(pair.first != "cerr") {
            auto i = all_streams.find(pair.first);

            if(i != all_streams.end()) {
                i->second.second -= 1;

                if(i->second.second == 0) {
                    i->second.first->close();
                    delete i->second.first;
                    all_streams.erase(i);
                }
            }
        }
    }
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    for(const auto &pair : _files) {
        if((pair.second & (1 << static_cast<int>(severity))) != 0) {
            std::string str = formating_string(text, severity);

            if(pair.first != "cerr")
                *all_streams[pair.first].first << str << std::endl;
            else
                std::cerr << str << std::endl;
        }
    }

    return this;
}

void client_logger::copy(client_logger const &other) {
    _files = other._files;
    _output_format = other._output_format;
}

void client_logger::move(client_logger &&other) {
    _files = std::move(other._files);
    _output_format = std::move(other._output_format);
}

void client_logger::clear() {
    _files.clear();
    _output_format = " ";
}

std::string client_logger::formating_string(std::string const &text, logger::severity severity) const {
    std::string str;

    for(auto i = 0; i < _output_format.size(); i++) {
        if(_output_format[i] == '%' && (i + 1) != _output_format.size()) {
            switch(_output_format[i+1]) {
                case 'd':
                    str += current_datetime_to_string().substr(0, 10);
                    i++;
                    break;
                case 't':
                    str += current_datetime_to_string().substr(10, 9);
                    i++;
                    break;
                case 's':
                    str += severity_to_string(severity);
                    i++;
                    break;
                case 'm':
                    str += text;
                    i++;
                    break;
                default:
                    str += _output_format[i];
            }
        }
    }

    return str;
}