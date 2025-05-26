#include "FileException.hpp"

FileException::FileException(const std::string& fileName, const std::string& message)
    : std::runtime_error("FileException (" + fileName + "): " + message) {}

FileException::~FileException() = default;

const char* FileException::what() const noexcept { return std::runtime_error::what(); }