#pragma once

#include <string>
#include <vector>

#include "Serializable.hpp"

class ReaderCategory : public Serializable {
private:
	int _id;
	std::string _name;
	std::vector<std::string> _categoryAttributes;
public:
	ReaderCategory(int id, 
		const std::string& name,
		const std::vector<std::string> categoryAttributes);
	~ReaderCategory();

	std::string getName() const;
	int getId() const;
	std::vector<std::string> getCategoryAttributes() const;
	
	void setName(const std::string& name);
	void setCategoryAttributes(const std::vector<std::string>& attributes);

	std::string serialize() const override;
};