#pragma once

#include <vector>
#include <memory>

#include "Reader.hpp"
#include "FileManager.hpp"

class ReaderManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<Reader>> _readers;
	int _nextReaderId;

	FileManager _fileManager;
	void loadReaders();
public:
	ReaderManager();
	~ReaderManager();

	int addReader(const std::string firstName,
		const std::string lastName, const std::string email,
		const std::string phoneNumber, const std::string address,
		const std::string city, int age,
		const std::string& category, int libraryId, int subscriptionId,
		const std::vector<std::pair<std::string, std::string>>& attributes);
	
	void changeReaderBookCount(int reader, int count);

	void deleteReaderById(int id);
	
	int printReadersMenu();
	
	bool isActiveReadersByLibraryId(int libraryId) const;
	bool isActiveReaderBySubscriptionId(int subscriptionId) const;

	std::shared_ptr<Reader> getReaderById(int id) const;
	std::pair<int, int> getReaderBookCountAndSubscriptionId(int readerId) const;
	std::vector<std::shared_ptr<Reader>> getReaders() const;
};