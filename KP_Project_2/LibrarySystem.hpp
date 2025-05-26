#pragma once

#include <vector>
#include <memory>
#include "Reader.hpp"
#include "Literature.hpp"
#include "Librarian.hpp"
#include "Issuance.hpp"
#include "Library.hpp"

#include "LibraryManager.hpp"
#include "ReaderManager.hpp"
#include "LiteratureManager.hpp"
#include "LibrarianManager.hpp"
#include "IssuanceManager.hpp"

#include "ReaderCategoryManager.hpp"
#include "SubscriptionManager.hpp"

#include "UserManager.hpp"

class LibrarySystem {
private:
    std::unique_ptr<LibraryManager> _libraryManager;     
    std::unique_ptr<ReaderManager> _readerManager;
	std::unique_ptr<LiteratureManager> _literatureManager;
	std::unique_ptr<LibrarianManager> _librarianManager;
	std::unique_ptr<IssuanceManager> _issuanceManager;
    
	std::unique_ptr<ReaderCategoryManager> _readerCategoryManager;
	std::unique_ptr<SubscriptionManager> _subscriptionManager;

	std::unique_ptr<UserManager> _userManager;
	void authorizeUser();
public:
    LibrarySystem();
	~LibrarySystem();

    std::vector<Reader*> getReadersByAttributes(const std::string& category, const std::string& attribute, const std::string& value);
    std::vector<Reader*> getReadersByLiterature(int literatureId);
    std::vector<Reader*> getReadersByLiteratureType(const std::string& type);
    std::vector<Reader*> getReadersByLiteratureInPeriod(int literatureId, time_t startDate, time_t endDate);
	std::vector<std::string> getLiteratureTypesByReaderAndReaderLibraryInPeriod(int readerId, time_t startDate, time_t endDate);
    std::vector<std::string> getLiteratureTypesByReaderAndNotReaderLibraryInPeriod(int readerId, time_t startDate, time_t endDate);
	std::vector<Reader*> getReadersServedByLibrarianInPeriod(int librarianId, time_t startDate, time_t endDate);
	int getAmountOfServedReadersByLibrarianInPeriod(int librarianId, time_t startDate, time_t endDate);
	std::vector<Librarian*> getLibrariansByHallId(const std::string& hallId, int libId);
    std::vector<Reader*> getReadersWithoutVisitsInPeriod(time_t startDate, time_t endDate);
    std::vector<Literature*> getMostPopularLiterature(int topN);
    std::vector<Literature*> getIssuedLiteratureByShelf(const std::string& location);
    std::vector<Reader*> getOverdueReaders();
	std::vector<Literature*> getLiteratureByPeriod(time_t startDate, time_t endDate, bool onlyArchived);
	std::vector<std::string> getInvNumsAndLibNameByLiteratureName(const std::string& name);
	std::vector<std::string> getInvNumsAndLibNameByLiteratureAuthor(const std::string& author);

	void libraryCommands();
    void readerCommands();
	void literatureCommands();
	void librarianCommands();
	void issuanceCommands();
	void subscriptionsCommands();

    void proceedEntitieCommands();
    void proceedQueriesCommands();
    void proceedUserCommands();
    void run();
};