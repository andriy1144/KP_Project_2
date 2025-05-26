#pragma once

#include <string>
#include <vector>

#include "Serializable.hpp"
#include "EntityObject.hpp"

class Library : public Serializable, public EntityObject {
private:
    int _id;
    std::string _name;
    std::string _address;
    std::vector<std::string> _halls;
public:
    Library(int id, const std::string& name, const std::string& address, std::vector<std::string> halls);
    ~Library();

    void addHall(const std::string& hallId);
    void setHalls(const std::vector<std::string>& halls);

    int getId() const;
    std::string getName() const;
    std::vector<std::string> getHalls() const;
    std::string getAddress() const;

	void setName(const std::string& name);
	void setAddress(const std::string& address);

    std::string serialize() const override;
	std::string toString() const override;
};