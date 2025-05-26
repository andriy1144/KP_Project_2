#pragma once

#include <string>

class Serializable {
public:
	Serializable() = default;
	virtual ~Serializable() = default;

	virtual std::string serialize() const = 0;
};