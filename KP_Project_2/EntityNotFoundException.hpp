#pragma once

#include <stdexcept>
#include <string>

class EntityNotFoundException : public std::runtime_error {
public:
	EntityNotFoundException(const std::string& entityType, const std::string& message);
	~EntityNotFoundException();

	const char* what() const noexcept override;
};
