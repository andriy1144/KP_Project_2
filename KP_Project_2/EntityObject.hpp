#pragma once

#include <string>

class EntityObject {
public:
	EntityObject() = default;
	virtual ~EntityObject() = default;

	virtual std::string toString() const = 0;
};