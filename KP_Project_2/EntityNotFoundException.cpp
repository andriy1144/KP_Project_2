#include "EntityNotFoundException.hpp"

EntityNotFoundException::EntityNotFoundException(const std::string& entityName, const std::string& message)
	: std::runtime_error("EntityNotFoundException(" + entityName + "): " + message) {}
EntityNotFoundException::~EntityNotFoundException() = default;

const char* EntityNotFoundException::what() const noexcept { return std::runtime_error::what(); }