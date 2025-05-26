#pragma once
#include <stdexcept>
#include <string>

class FileException : public std::runtime_error {
public:
	FileException(const std::string& fileName,const std::string& message);
	~FileException();

	const char* what() const noexcept override;
};
