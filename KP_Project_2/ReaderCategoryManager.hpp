#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>

#include "FileManager.hpp"
#include "ReaderCategory.hpp"

class ReaderCategoryManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<ReaderCategory>> _readerCategories;
	int _nextReaderCategoryId;

	FileManager _fileManager;
	void loadReaderCategories();
public:
	ReaderCategoryManager();
	~ReaderCategoryManager();

	std::vector<std::shared_ptr<ReaderCategory>> getReaderCategories() const;
	std::shared_ptr<ReaderCategory> getReaderCategoryById(int id) const;

	std::vector<std::pair<std::string, std::string>> getInputCategoryAttributes(std::vector<std::string> attributesToGet) const;
	std::pair<std::string, std::vector<std::string>> showCategoriesMenuAndGetAttrs() const;
	int printReaderCategoriesMenu() const;


	int addReaderCategory(const std::string& name, const std::string attributes);
	void deleteReaderCategoryById(int id);
};