#include "ReaderCategory.hpp"

ReaderCategory::ReaderCategory(int id,
	const std::string& name,
	const std::vector<std::string> categoryAttributes) : 
	_id(id), _name(name),_categoryAttributes(categoryAttributes) {}

ReaderCategory::~ReaderCategory() = default;

std::string ReaderCategory::getName() const { return _name; }
int ReaderCategory::getId() const { return _id; }
std::vector<std::string> ReaderCategory::getCategoryAttributes() const { return _categoryAttributes; }

void ReaderCategory::setName(const std::string& name) { _name = name; }
void ReaderCategory::setCategoryAttributes(const std::vector<std::string>& attributes) { _categoryAttributes = attributes; }

std::string ReaderCategory::serialize() const {
	std::string res = std::to_string(_id) + "|" + _name + "|";
	for (int i = 0; i < _categoryAttributes.size(); i++) {
		if(i == _categoryAttributes.size() - 1) {
			res += _categoryAttributes[i];
		}
		else res += _categoryAttributes[i] + ";";
	}
	return res;
}