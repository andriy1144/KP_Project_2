#include "LibrarySystem.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

#include "ConsoleUtils.hpp"

LibrarySystem::~LibrarySystem() = default;

LibrarySystem::LibrarySystem() {
	_libraryManager = std::make_unique<LibraryManager>();
	_readerManager = std::make_unique<ReaderManager>();
	_literatureManager = std::make_unique<LiteratureManager>();
	_librarianManager = std::make_unique<LibrarianManager>();
	_issuanceManager = std::make_unique<IssuanceManager>();

	_readerCategoryManager = std::make_unique<ReaderCategoryManager>();
	_subscriptionManager = std::make_unique<SubscriptionManager>();

	_userManager = std::make_unique<UserManager>();
	authorizeUser();
}

void LibrarySystem::authorizeUser(){
	if(_userManager->getUsers().empty()){
		ConsoleUtils::printInfo("Користувачів немає. Створіть нового користувача.");
		std::string username = ConsoleUtils::getInputString("Введіть логін: ");
		std::string password = ConsoleUtils::getInputString("Введіть пароль: ");
		_userManager->addUser(username, password, UserRole::ADMIN, -1);
		ConsoleUtils::printSuccess("Користувача було створено успішно! Автоматично присвоєно роль \x1B[34m (Admin) \033[0m!");
	}
	else {
		std::string username = ConsoleUtils::getInputString("Введіть логін: ");
		std::string password = ConsoleUtils::getInputString("Введіть пароль: ");
		if (_userManager->login(username, password)) {
			ConsoleUtils::printSuccess("Авторизація пройшла успішно!");
		}
		else {
			ConsoleUtils::printError("Невірний логін або пароль.");
			throw std::runtime_error("Authorization failed.");
		}
		system("cls||clear");
	}
}

/* ЗАПИТИ */
std::vector<Reader*> LibrarySystem::getReadersByAttributes(const std::string& category, const std::string& attribute, const std::string& value) {
	std::vector<Reader*> result;
	const auto& readers = _readerManager->getReaders();
	for (const auto& reader : readers) {
		if (reader->getCategory() == category) {
			const auto& attributes = reader->getAttributes();
			for (const auto& attr : attributes) {
				if (attr.first == attribute && attr.second == value) {
					result.push_back(reader.get());
					break;
				}
			}
		}
	}
	return result;
}

std::vector<Reader*> LibrarySystem::getReadersByLiterature(int literatureId) {
	std::vector<Reader*> result;
	const auto& readers = _readerManager->getReaders();
	const auto& issues = _issuanceManager->getIssues();
	for (const auto& issue : issues) {
		if (issue->getLiteratureId() == literatureId) {
			for (const auto& reader : readers) {
				if (reader->getId() == issue->getReaderId()) {
					result.push_back(reader.get());
				}
			}
		}
	}
	return result;
}

std::vector<Reader*> LibrarySystem::getReadersByLiteratureType(const std::string& type) {
	std::vector<Reader*> result;
	const auto& readers = _readerManager->getReaders();
	const auto& literature = _literatureManager->getLiterature();
	const auto& issues = _issuanceManager->getIssues();
	for (const auto& issue : issues) {
		for (const auto& lit : literature) {
			if (issue->getLiteratureId() == lit->getId() && lit->getType() == type) {
				for (const auto& reader : readers) {
					if (reader->getId() == issue->getReaderId()) {
						result.push_back(reader.get());
					}
				}
			}
		}
	}
	return result;
}

std::vector<Reader*> LibrarySystem::getReadersByLiteratureInPeriod(int literatureId, time_t startDate, time_t endDate) {
	std::vector<Reader*> result;
	std::vector<int> readerIds;

	const auto& readers = _readerManager->getReaders();
	const auto& issues = _issuanceManager->getIssues();
	for (const auto& issue : issues) {
		if (issue->getLiteratureId() == literatureId 
			&& issue->getIssueDate().getDate() >= startDate 
			&& issue->getIssueDate().getDate() <= endDate) {
			for (const auto& reader : readers) {
				if (reader->getId() == issue->getReaderId() &&
					std::find(readerIds.begin(), readerIds.end(), reader->getId()) == readerIds.end()) {
					result.push_back(reader.get());
					readerIds.push_back(reader->getId());
				}
			}
		}
	}
	return result;
}

std::vector<std::string> LibrarySystem::getLiteratureTypesByReaderAndReaderLibraryInPeriod(int readerId, time_t startDate, time_t endDate) {
	std::vector<std::string> result;
	const auto& issues = _issuanceManager->getIssues();
	const auto& reader = _readerManager->getReaderById(readerId);
	
	if (!reader) throw std::runtime_error("Читача не знайдено.");

	for (const auto& issue : issues) {
		if (issue->getReaderId() == readerId 
			&& issue->getIssueDate().getDate() >= startDate 
			&& issue->getIssueDate().getDate() <= endDate) {
			const auto& literature = _literatureManager->getLiteratureById(issue->getLiteratureId());
			if (literature->getLibraryId() == reader->getLibId()) {
				result.push_back(literature->getType());
			}
		}
	}
	return result;
}

std::vector<std::string> LibrarySystem::getLiteratureTypesByReaderAndNotReaderLibraryInPeriod(int readerId, time_t startDate, time_t endDate) {
	std::vector<std::string> result;
	const auto& issues = _issuanceManager->getIssues();
	const auto& reader = _readerManager->getReaderById(readerId);
	
	if (!reader) throw std::runtime_error("Читача не знайдено.");
	
	for (const auto& issue : issues) {
		if (issue->getReaderId() == readerId 
			&& issue->getIssueDate().getDate() >= startDate 
			&& issue->getIssueDate().getDate() <= endDate) {
			const auto& literature = _literatureManager->getLiteratureById(issue->getLiteratureId());
			if (literature->getLibraryId() != reader->getLibId()) {
				result.push_back(literature->getType());
			}
		}
	}
	return result;
}

std::vector<Reader*> LibrarySystem::getReadersServedByLibrarianInPeriod(int librarianId, time_t startDate, time_t endDate) {
	std::vector<Reader*> result;
	const auto& issues = _issuanceManager->getIssues();
	const auto& readers = _readerManager->getReaders();
	
	for (const auto& issue : issues) {
		if (issue->getLibrarianId() == librarianId && issue->getIssueDate().getDate() >= startDate && issue->getIssueDate().getDate() <= endDate) {
			for (const auto& reader : readers) {
				if (reader->getId() == issue->getReaderId()) {
					result.push_back(reader.get());
				}
			}
		}
	}
	return result;
}

int LibrarySystem::getAmountOfServedReadersByLibrarianInPeriod(int librarianId, time_t startDate, time_t endDate) {
	int count = 0;
	const auto& issues = _issuanceManager->getIssues();
	
	for (const auto& issue : issues) {
		if (issue->getLibrarianId() == librarianId 
			&& issue->getIssueDate().getDate() >= startDate 
			&& issue->getIssueDate().getDate() <= endDate) {
			count++;
		}
	}
	return count;
}

std::vector<Literature*> LibrarySystem::getIssuedLiteratureByShelf(const std::string& location) {
	std::vector<Literature*> result;
	const auto& literature = _literatureManager->getLiterature();
	const auto& issues = _issuanceManager->getIssues();

	for (const auto& issue : issues) {
		for (const auto& lit : literature) {
			if (issue->getLiteratureId() == lit->getId() && lit->getLocation() == location) {
				result.push_back(lit.get());
			}
		}
	}
	return result;
}

std::vector<Reader*> LibrarySystem::getOverdueReaders() {
	std::vector<Reader*> result;
	std::vector<int> readerIds;
	const auto& readers = _readerManager->getReaders();
	const auto& issues = _issuanceManager->getIssues();

	time_t now = time(nullptr);
	for (const auto& issue : issues) {
		if (issue->getDueDate().getDate() < now && !issue->isReturned()) {
			for (const auto& reader : readers) {
				if (reader->getId() == issue->getReaderId() &&
					std::find(readerIds.begin(), readerIds.end(), reader->getId()) == readerIds.end()) {
					readerIds.push_back(reader->getId());
					result.push_back(reader.get());
				}
			}
		}
	}
	return result;
}

std::vector<Literature*> LibrarySystem::getLiteratureByPeriod(time_t startDate, time_t endDate, bool onlyArchived = false) {
	std::vector<Literature*> result;
	const auto& literature = _literatureManager->getLiterature();
	for (const auto& lit : literature) {
		if (onlyArchived && !lit->isArchived()) continue;
		time_t date = lit->isArchived() ? lit->getArchiveDate().getDate() : lit->getEntryDate().getDate();
		if (date >= startDate && date <= endDate) {
			result.push_back(lit.get());
		}
	
	}
	return result;
}

std::vector<std::string> LibrarySystem::getInvNumsAndLibNameByLiteratureName(const std::string& name) {
	std::vector<std::string> result;
	const auto& literature = _literatureManager->getLiterature();
	for (const auto& lit : literature) {
		if (lit->getTitle() == name) {
			result.push_back(lit->getInventoryNumber() + " - " + lit->getTitle() + " - " + _libraryManager->getLibraryById(lit->getLibraryId())->getName());
		}
	}
	return result;
}

std::vector<Librarian*> LibrarySystem::getLibrariansByHallId(const std::string& hallId, int libId) {
	std::vector<Librarian*> result;
	const auto& librarians = _librarianManager->getLibrarians();
	for (const auto& librarian : librarians) {
		if (librarian->getHallId() == hallId 
			&& librarian->getLibraryId() == libId) {
			result.push_back(librarian.get());
		}
	}
	return result;
}

std::vector<Reader*> LibrarySystem::getReadersWithoutVisitsInPeriod(time_t startDate, time_t endDate) {
	std::vector<Reader*> result;
	const auto& readers = _readerManager->getReaders();
	const auto& issues = _issuanceManager->getIssues();
	for (const auto& reader : readers) {
		bool hasVisited = false;
		for (const auto& issue : issues) {
			if (issue->getReaderId() == reader->getId() 
				&& issue->getIssueDate().getDate() >= startDate 
				&& issue->getIssueDate().getDate() <= endDate) {
				hasVisited = true;
				break;
			}
		}
		if (!hasVisited) {
			result.push_back(reader.get());
		}
	}
	return result;
}

std::vector<Literature*> LibrarySystem::getMostPopularLiterature(int topN) {
	std::vector<Literature*> result;
	const auto& literature = _literatureManager->getLiterature();
	const auto& issues = _issuanceManager->getIssues();
	
	std::map<int, int> literatureCount;
	for (const auto& issue : issues) {
		literatureCount[issue->getLiteratureId()]++;
	}

	std::vector<std::pair<int, int>> sortedLiteratureCount(literatureCount.begin(), literatureCount.end());
	std::sort(sortedLiteratureCount.begin(), sortedLiteratureCount.end(),
		[](const std::pair<int, int>& a, const std::pair<int, int>& b) {
			return a.second > b.second;
		});
	
	for (int i = 0; i < topN && i < sortedLiteratureCount.size(); ++i) {
		int literatureId = sortedLiteratureCount[i].first;
		for (const auto& lit : literature) {
			if (lit->getId() == literatureId) {
				result.push_back(lit.get());
				break;
			}
		}
	}

	return result;
}

std::vector<std::string> LibrarySystem::getInvNumsAndLibNameByLiteratureAuthor(const std::string& author) {
	std::vector<std::string> result;
	const auto& literature = _literatureManager->getLiterature();
	for (const auto& lit : literature) {
		if (lit->getAuthor() == author) {
			result.push_back(lit->getInventoryNumber() + " - " + lit->getTitle() + " - " + _libraryManager->getLibraryById(lit->getLibraryId())->getName());
		}
	}
	return result;
}

/* КОМАНДИ ДЛЯ ЧИТАЧА */
void LibrarySystem::readerCommands() {
	while (true) {
		std::cout << "\n=== Доступні команди для керування читачами ===\n";
		std::cout << " - \t1. Додати читача\n";
		std::cout << " - \t2. Показати всіх читачів\n";
		std::cout << " - \t3. Додати нову категорію читача \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t4. Видалити читача \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t5. Видалити категорію читача \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t0. вихід\n";
		
		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");
		try {
			switch (choice) {
				case 1: {
					std::string firstName = ConsoleUtils::getInputString("Введіть ім'я читача: ");
					std::string lastName = ConsoleUtils::getInputString("Введіть прізвище читача: ");
					int age = ConsoleUtils::getInputInt("Введіть вік читача: ");
					std::string email = ConsoleUtils::getInputString("Введіть e-mail читача: ");
					std::string phone = ConsoleUtils::getInputString("Введіть номер телефону читача (+380 ..., +1 ...): ");
					std::string address = ConsoleUtils::getInputString("Введіть адресу читача: ");
					std::string city = ConsoleUtils::getInputString("Введіть місто читача: ");					

					ConsoleUtils::printInfo("Виберіть бібліотеку для реєстрації:");
					int libraryId = _libraryManager->printLibraryMenu();

					ConsoleUtils::printInfo("Виберіть абонимент читача: ");
					int subscriptionId = _subscriptionManager->printSubscriptionMenuByLibraryId(libraryId);

					ConsoleUtils::printInfo("Виберіть категорію читача:");
					std::pair<std::string, std::vector<std::string>> categoryAttributes
						= _readerCategoryManager->showCategoriesMenuAndGetAttrs();

					std::string category = categoryAttributes.first;
					std::vector<std::pair<std::string, std::string>> attributes = 
						_readerCategoryManager->getInputCategoryAttributes(categoryAttributes.second);

					int newReaderId =
						_readerManager->addReader(firstName,
							lastName,email,phone,address,city,age, 
							category, libraryId,subscriptionId, attributes);
					ConsoleUtils::printSuccess("Читача було додано успішно! Ідентифікатор читача: " 
						+ std::to_string(newReaderId));
					break;
				}
				case 2: {					
					const auto& readers = _readerManager->getReaders();
					
					if (readers.empty()) ConsoleUtils::printInfo("Читачів немає.");
					else {						
						ConsoleUtils::printInfo("Доступні читачі:");
						ConsoleUtils::printArrayFormatted(
							std::vector<std::shared_ptr<EntityObject>>(readers.begin(), readers.end()),
							'-', 150
						);
					}
					break;
				}
				case 3: {
					if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");
					
					std::string categoryName = ConsoleUtils::getInputString("Введіть назву нової категорії читача: ");
					std::string attributes = ConsoleUtils::getInputString("Введіть перелік атрибутів нової категорії читача (через кому!!!): ");

					int newCategoryId = _readerCategoryManager->addReaderCategory(categoryName, attributes);
					ConsoleUtils::printSuccess("Категорію читача було додано успішно! Ідентифікатор категорії: " 
						+ std::to_string(newCategoryId));
					break;
				}
				case 4: {
					if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");
					
					ConsoleUtils::printInfo("Виберіть читача для видалення:");
					int readerId = _readerManager->printReadersMenu();
					
					if (_issuanceManager->checkIfReaderHasCurrentIssues(readerId)) throw std::runtime_error("Видалення читача заборонено. Читач має активні видачі літератури.");
					
					bool confirmation = ConsoleUtils::printConfirmation("Ви впевнені, що хочете видалити читача з ідентифікатором " 
						+ std::to_string(readerId));
					if (!confirmation) {
						ConsoleUtils::printWarning("Видалення користувача скасовано!");
						break;
					}
					
					_readerManager->deleteReaderById(readerId);
					ConsoleUtils::printSuccess("Читача було видалено успішно!");
					break;
				}
				case 5: {
					if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");

					ConsoleUtils::printInfo("Виберіть категорію читача для видалення:");
					int categoryId = _readerCategoryManager->printReaderCategoriesMenu();

					bool confirmation = ConsoleUtils::printConfirmation("Ви впевнені, що хочете видалити категорію читача з ідентифікатором "
						+ std::to_string(categoryId));
					if (!confirmation) {
						ConsoleUtils::printWarning("Видалення категорії читача скасовано!");
						break;
					}

					_readerCategoryManager->deleteReaderCategoryById(categoryId);
					ConsoleUtils::printSuccess("Категорію читача було видалено успішно!");
					break;
				}
				case 0: {
					std::cout << std::endl;
					return;
				}
				default: {
					ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
				}
			}
		}catch(const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

/* КОМАНДИ ДЛЯ ЛІТЕРАТУРИ */
void LibrarySystem::literatureCommands() {
	while (true) {
		std::cout << "\n=== Доступні команди для керування літературою ===\n";
		std::cout << " - \t1. Додати літерутуру\n";
		std::cout << " - \t2. Показати всю літературу\n";
		std::cout << " - \t3. Списати літературу \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");
		try {
			switch (choice) {
			case 1: {
				std::string title = ConsoleUtils::getInputString("Введдіть назву: ");
				std::string author = ConsoleUtils::getInputString("Введіть ПІБ/Ініціали/Псевдонім автора: ");
				std::string type = ConsoleUtils::getInputString("Введіть тип (Книга/Журнал/і тд.): ");
				
				ConsoleUtils::printInfo("Виберіть бібліотеку для реєстрації:");
				int libId = _libraryManager->printLibraryMenu();

				std::string inventoryNumber = ConsoleUtils::getInputString("Введіть інвентарний номер літератури(Приклад: ІН-1001, ІНВ-1051 і тд.): ");
				std::string location = ConsoleUtils::getInputString("Введіть розташування (Наприклад: Зал1-Стелаж1-Полиця1): ");
				int isReadingRoomOnly = ConsoleUtils::getInputInt("Ця книга для читання тільки в читальних залах бібліотеки? (1 - 'Так', 0 - 'Ні'): ");
				int issuePeriodDays = ConsoleUtils::getInputInt("Введіть допустимий період для видачі літератури (днів): ");

				if (issuePeriodDays <= 0) throw std::runtime_error("Недопустимий період видачі літератури. Спробуйте ще раз.");


				int newLitId =
					_literatureManager->addLiterature(title, author, type, inventoryNumber,
						location, isReadingRoomOnly, libId, issuePeriodDays);
				ConsoleUtils::printSuccess("Літературу було додано успішно! Ідентифікатор літератури: " 
					 + std::to_string(newLitId));
				break;
			}
			case 2: {				
				const auto& literature = _literatureManager->getLiterature();
				if (literature.empty()) ConsoleUtils::printInfo("Літератури немає.");
				else {
					ConsoleUtils::printInfo("Доступна література:");
					ConsoleUtils::printArrayFormatted(
						std::vector<std::shared_ptr<EntityObject>>(literature.begin(), literature.end()),
						'-', 150
					);
				}
				break;
			}
			case 3: {
				if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");

				ConsoleUtils::printInfo("Виберіть літературу для списання:");
				int literatureId = _literatureManager->printLiteratureMenu();

				bool confirmation = ConsoleUtils::printConfirmation("Ви впевнені, що хочете списати літературу з ідентифікатором "
					+ std::to_string(literatureId));
				if (!confirmation) {
					ConsoleUtils::printWarning("Списання скасовано!");
					break;
				}

				_literatureManager->archiveLiteratureById(literatureId);
				ConsoleUtils::printSuccess("Літературу було списано успішно!");
				break;
			}
			case 0: {
				std::cout << std::endl;
				return;
			}
			default: {
				ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
			}
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

/* КОМАНДИ ДЛЯ БІБЛІОТЕКИ */
void LibrarySystem::libraryCommands() {
	while (true) {
		std::cout << "\n=== Доступні команди для керування бібліотеками ===\n";
		std::cout << " - \t1. Додати бібліотеку \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t2. Показати всі бібліотеки \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t3. Видалити бібліотеку \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");
		try {
			switch (choice) {
			case 1: {
				if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");

				std::string name = ConsoleUtils::getInputString("Введіть назву бібліотеки: ");
				std::string address = ConsoleUtils::getInputString("Введіть адресу бібліотеки: ");

				std::vector<std::string> halls;
				std::string hallsInput = ConsoleUtils::getInputString("Введіть усі доступні читальні зали бібліотеки (розділені комою!!!):");

				int newId = _libraryManager->addLibrary(name, address, hallsInput);
				ConsoleUtils::printSuccess("Бібліотеку будо додано успішно! Ідентифікатор бібліотеки: " 
					+ std::to_string(newId));
				break;
			}
			case 2: {
				if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");

				const auto& libraries = _libraryManager->getLibraries();
				if (libraries.empty()) ConsoleUtils::printInfo("Бібліотек немає.");
				else {
					ConsoleUtils::printInfo("Доступні бібліотеки:");
					ConsoleUtils::printArrayFormatted(
						std::vector<std::shared_ptr<EntityObject>>(libraries.begin(), libraries.end()),
						'-', 150
					);
				}
				break;
			}
			case 3: {
				if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");
				
				ConsoleUtils::printInfo("Виберіть бібліотеку для видалення:");
				int libraryId = _libraryManager->printLibraryMenu();
				
				if (_readerManager->isActiveReadersByLibraryId(libraryId)) throw std::runtime_error("Видалення бібліотеки заборонено. Бібліотека має активних читачів.");

				bool confirmation = ConsoleUtils::printConfirmation("Ви впевнені, що хочете видалити бібліотеку з ідентифікатором "
					+ std::to_string(libraryId));
				if (!confirmation) {
					ConsoleUtils::printWarning("Видалення бібліотеки скасовано!");
					break;
				}
				
				_libraryManager->deleteLibraryById(libraryId);
				ConsoleUtils::printSuccess("Бібліотеку було видалено успішно!");
				break;
			}
			case 0: {
				std::cout << std::endl;
				return;
			}
			default: {
				ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
			}
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

/* КОМАНДИ ДЛЯ БІБЛІОТЕКАРЯ */
void LibrarySystem::librarianCommands() {
	while (true) {
		std::cout << "\n=== Доступні команди для керування бібліотекарями ===\n";
		std::cout << " - \t1. Додати бібліотекаря \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t2. Показати всіх бібліотекарів \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t3. Видалити бібліотекаря \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");
		try {
			switch (choice) {
			case 1: {
				if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");
				
				std::string firstName = ConsoleUtils::getInputString("Введіть ім'я бібліотекаря: ");
				std::string lastName = ConsoleUtils::getInputString("Введіть прізвище бібліотекаря: ");
				int age = ConsoleUtils::getInputInt("Введіть вік бібліотекаря: ");
				std::string email = ConsoleUtils::getInputString("Введіть e-mail бібліотекаря: ");
				std::string phoneNumber = ConsoleUtils::getInputString("Введіть номер телефону бібліотекаря (+380 ..., +1 ...): ");
				std::string address = ConsoleUtils::getInputString("Введіть адресу бібліотекаря: ");
				std::string city = ConsoleUtils::getInputString("Введіть місто бібліотекаря: ");
				
				ConsoleUtils::printInfo("Виберіть бібліотеку для реєстрації:");
				int libraryId = _libraryManager->printLibraryMenu();

				ConsoleUtils::printInfo("Виберіть залу бібліотеки для реєстрації бібліотекаря:");
				std::string hallId = _libraryManager->printHallsByLibraryId(libraryId);

				int newLibrarianId = _librarianManager->addLibrarian(
					firstName, lastName,email, phoneNumber,address,city,age,
					hallId, libraryId);
				ConsoleUtils::printSuccess("Бібліотекаря було додано успішно! Ідентифікатор бібліотекаря: " 
					+ std::to_string(newLibrarianId));
				break;
			}
			case 2: {
				if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");

				const auto& librarians = _librarianManager->getLibrarians();
				if (librarians.empty()) ConsoleUtils::printInfo("Бібліотекарів немає.");
				else {
					ConsoleUtils::printInfo("Доступні бібліотекарі:");
					ConsoleUtils::printArrayFormatted(
						std::vector<std::shared_ptr<EntityObject>>(librarians.begin(), librarians.end()),
						'-', 150
					);
				}
				break;
			}
			case 3: {
				if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання цієї команди.");

				ConsoleUtils::printInfo("Виберіть бібліотекаря для видалення:");
				int librarianId = _librarianManager->printLibrarianMenu();

				bool confirmation = ConsoleUtils::printConfirmation("Ви впевнені, що хочете видалити бібліотекаря з ідентифікатором "
					+ std::to_string(librarianId));
				if (!confirmation) {
					ConsoleUtils::printWarning("Видалення бібліотекаря скасовано!");
					break;
				}

				_librarianManager->deleteLibrarianById(librarianId);
				ConsoleUtils::printSuccess("Бібліотекаря успішно видалено");
				break;
			}
			case 0: {
				std::cout << std::endl;
				return;
			}
			default: {
				ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
			}
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

/* КОМАНДИ ДЛЯ ВИДАЧІ ЛІТЕРАТУРИ */ 
void LibrarySystem::issuanceCommands() {
	while (true) {
		std::cout << "\n=== Доступні команди для керування видачами літератури ===\n";
		std::cout << " - \t1. Видати літературу\n";
		std::cout << " - \t2. Показати всі видачі\n";
		std::cout << " - \t3. Закрити видачу\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");
		try {
			switch (choice) {
			case 1: {
				ConsoleUtils::printInfo("Виберіть читача для видачі літератури:");
				int readerId = _readerManager->printReadersMenu();

				const auto& readerSubscriptionInfo = 
					_readerManager->getReaderBookCountAndSubscriptionId(readerId);

				if (_subscriptionManager->checkSubscriptionLimitValid(
					readerSubscriptionInfo.first, readerSubscriptionInfo.second)) {
					throw std::logic_error("Читач перевищив ліміт видачі літератури за абонементом.");
				}

				ConsoleUtils::printInfo("Виберіть літературу для видачі:");
				int literatureId = _literatureManager->printLiteratureMenu();

				int librarianId;
				if (_userManager->getCurrentUserRole() == UserRole::LiBRARIAN) {
					librarianId = _userManager->getCurrentLibrarianId();
				}
				else {
					ConsoleUtils::printInfo("Виберіть бібліотекаря, який обслуговує читача:");
					librarianId = _librarianManager->printLibrarianMenu();
				}

				int issuePeriodDays = ConsoleUtils::getInputInt("Введіть період видачі (днів): ");
				if (issuePeriodDays <= 0) throw std::runtime_error("Недопустимий період видачі літератури. Спробуйте ще раз.");
				if(!_literatureManager->checkLiteratureIssueDaysValid(literatureId, issuePeriodDays))
					throw std::runtime_error("Період видачі літератури перевищує її максимальне значення. Спробуйте ще раз.");

				int newIssueId = _issuanceManager->issueLiterature(readerId, literatureId, librarianId, issuePeriodDays);
				ConsoleUtils::printSuccess("Літературу було видана успішно! Ідентифікатор видачі: " 
					+ std::to_string(newIssueId));
				_readerManager->changeReaderBookCount(readerId, 1);
				break;
			}
			case 2: {
				const auto& issues = _issuanceManager->getIssues();
				if (issues.empty()) ConsoleUtils::printInfo("Доступних видач немає.");
				else {
					ConsoleUtils::printInfo("Список доступних видач:");
					ConsoleUtils::printArrayFormatted(
						std::vector<std::shared_ptr<EntityObject>>(issues.begin(), issues.end()),
						'-', 150
					);
				}
				break;
			}
			case 3: {
				ConsoleUtils::printInfo("Виберіть видачу для закриття:");
				int issueId = _issuanceManager->printIssuesMenu();

				bool confirmation = ConsoleUtils::printConfirmation("Ви впевнені, що хочете закрити видачу з ідентифікатором "
					+ std::to_string(issueId));
				if (!confirmation) {
					ConsoleUtils::printWarning("Закриття видачі скасовано!");
					break;
				}
				_issuanceManager->closeIssue(issueId);
				ConsoleUtils::printSuccess("Видачу було закрито успішно!");

				int readerId = _issuanceManager->getIssuanceById(issueId)->getReaderId();
				_readerManager->changeReaderBookCount(readerId, -1);
				break;
			}
			case 0: {
				std::cout << std::endl;
				return;
			}
			default: {
				ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
			}
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

/* КОМАНДИ ДЛЯ БІБЛІОТЕСЬКИХ АБОНИМЕНТІВ */
void LibrarySystem::subscriptionsCommands() {
	while (true) {
		std::cout << "\n=== Доступні команди для керування бібліотекарями ===\n";
		std::cout << " - \t1. Додати новий абонимент \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t2. Показати всі створені абонимент бібліотечного фонду\x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t3. Показати всі абонимент за вказаною бібліотекою\x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t4. Видалити абонимент \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");
		try {
			switch (choice) {
			case 1: {
				ConsoleUtils::printInfo("Виберіть бібліотеку для якої буде присвоєно даний абонимент:");
				int libraryId = _libraryManager->printLibraryMenu();

				std::string name = ConsoleUtils::getInputString("Введіть назву абонименту: ");
				float price = ConsoleUtils::getInputFloat("Введіть ціну абонименту на місяць: ");
				int books_limit = ConsoleUtils::getInputInt("Введіть обмеження кількості книг на цей абонимент: ");

				int newSubId = _subscriptionManager->addSubscription(name, price, books_limit, libraryId);
				ConsoleUtils::printSuccess("Новий абонимент успішно додано! Ідентифікатор: " + std::to_string(newSubId));
				break;
			}
			case 2: {
				const auto& subscriptions = _subscriptionManager->getSubscriptions();
				if (subscriptions.empty()) ConsoleUtils::printInfo("Абониментів немає.");
				else {
					ConsoleUtils::printInfo("Доступні абонименти:");
					ConsoleUtils::printArrayFormatted(
						std::vector<std::shared_ptr<EntityObject>>(subscriptions.begin(), subscriptions.end()),
						'-', 150
					);
				}
				break;
			}
			case 3: {
				ConsoleUtils::printInfo("Виберіть бібліотеку для якої буде показано абонимент:");
				const auto& libraryId = _libraryManager->printLibraryMenu();
				const auto& subscriptions = _subscriptionManager->getSubscriptionsByLibraryId(libraryId);

				if (subscriptions.empty()) ConsoleUtils::printInfo("Абониментів немає.");
				else {
					ConsoleUtils::printInfo("Доступні абонименти:");
					ConsoleUtils::printArrayFormatted(
						std::vector<std::shared_ptr<EntityObject>>(subscriptions.begin(), subscriptions.end()),
						'-', 100
					);
				}
				break;
			}
			case 4: {
				ConsoleUtils::printInfo("Виберіть абонимент для видалення:");
				int subscriptionId = _subscriptionManager->printSubscriptionMenu();
				
				if (_readerManager->isActiveReaderBySubscriptionId(subscriptionId)) 
					throw std::logic_error("Видалення абонименту заборонено. Абонимент має дійсних читачів.");

				bool confirmation = ConsoleUtils::printConfirmation("Ви впевнені, що хочете видалити абонимент з ідентифікатором "
					+ std::to_string(subscriptionId));
				if (!confirmation) {
					ConsoleUtils::printWarning("Видалення абонименту скасовано!");
					break;
				}
				
				_subscriptionManager->deleteSubscriptionById(subscriptionId);
				ConsoleUtils::printSuccess("Абонимент було видалено успішно!");
				break;
			}
			case 0: {
				std::cout << std::endl;
				return;
			}
			default: {
				ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
			}
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

/* МЕНЮШКИ */
void LibrarySystem::proceedEntitieCommands() {
	while (true) {
		std::cout << "\n=== Доступні сутності ===\n";
		std::cout << " - \t1. Читачі\n";
		std::cout << " - \t2. Література\n";
		std::cout << " - \t3. Бібліотеки\n";
		std::cout << " - \t4. Бібліотекарі\n";
		std::cout << " - \t5. Видача літератури\n";
		std::cout << " - \t6. Абонименти\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");

		try {
			switch (choice) {
				case 1: {
					readerCommands();
					break;
				}
				case 2: {
					literatureCommands();
					break;
				}
				case 3: {
					libraryCommands();
					break;
				}
				case 4: {
					librarianCommands();
					break;
				}
				case 5: {
					issuanceCommands();
					break;
				}
				case 6: {
					subscriptionsCommands();
					break;
				}
				case 0: {
					std::cout << std::endl;
					return;
				}
				default: {
					ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
				}
			}
		}catch(const std::exception& e) {
            ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

void LibrarySystem::proceedQueriesCommands() {
	while (true) {
		std::cout << "\n=== Доступні готові запити системи ===\n";
		std::cout << " - \t1. Запит: Читачі із заданими характеристиками\n";
		std::cout << " - \t2. Запит: Читачі із зазначеним твором\n";
		std::cout << " - \t3. Запит: Читачі із зазначеним виданням\n";
		std::cout << " - \t4. Запит: Читачі які отримували літературу в зазначений період\n";
		std::cout << " - \t5. Запит: Список видань, які отримував читач у зазначений період, з бібліотеки де він зареєстрований\n";
		std::cout << " - \t6. Запит: Список видань, які отримував читач у зазначений період, з бібліотеки де він НЕ зареєстрований\n";
		std::cout << " - \t7. Запит: Список літератури виданої з зазначеної полиці\n";
		std::cout << " - \t8. Запит: Читачі, обслужені зазначеним бібліотекари у зазначений період\n";
		std::cout << " - \t9. Запит: Продуктивінсть бібліотекаря(число обслужених читачів у зазначений період)\n";
		std::cout << " - \t10. Запит: Читачі з простроченим терміном виданої літератури\n";
		std::cout << " - \t11. Запит: Список літератури, яка надійшла(була списана) у зазначений період\n";
		std::cout << " -\t12. Запит: Список бібліотекарів, які працюють у вказаному залі бібліотеки\n";
		std::cout << " -\t13. Запит: Читачі, які не відвідували бібліотеку у зазначений період\n";
		std::cout << " -\t14. Запит: Список інвентарних номерів і назв з бібліотечного фонду в яких міститься заданий твір\n";
		std::cout << " -\t15. Запит: Список інвентарних номерів і назв з бібліотечного фонду в яких містяться твори зазначеного автора\n";
		std::cout << " - \t16. Запит: Список перших N найпопулярніших творів\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");

		try {
			switch (choice) {
			case 1: {
				std::pair<std::string, std::vector<std::string>> categoryChoice = 
					_readerCategoryManager->showCategoriesMenuAndGetAttrs();				

				std::string category = categoryChoice.first;
				std::cout << "Доступий список атрибутів для " << category << ":\n";
				for (const auto& attr : categoryChoice.second) {
					std::cout << " - " << attr << "\n";
				}

				std::string attribute = ConsoleUtils::getInputString("Введіть атрибут зі списку вище: ");
				std::string value = ConsoleUtils::getInputString("Введіть значення атрибуту: ");

				auto result = getReadersByAttributes(category, attribute, value);
				if (result.empty()) ConsoleUtils::printInfo("Читачів за такими атрибутами не знайдено.");
				else {
					ConsoleUtils::printInfo("Відповідні читачі за вказаними атрибутами: ");
					for (const auto& reader : result) {
						std::cout << reader->toString();
						ConsoleUtils::printDelimeter('-',150);
					}
				}
				break;
			}
			case 2: {
				ConsoleUtils::printInfo("Виберіть літературу для запиту:");
				int literatureId = _literatureManager->printLiteratureMenu();

				auto result = getReadersByLiterature(literatureId);
				if (result.empty()) ConsoleUtils::printInfo("Читачів із заданою літературою не знайдено.");
				else {
					ConsoleUtils::printInfo("Читачі з літературою з ідентифікатором " + std::to_string(literatureId) + ": ");					
					for (const auto& reader : result) {
						std::cout << reader->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 3: {
				_literatureManager->printLiteratureTypes();
				std::string type = ConsoleUtils::getInputString("Введіть тип літератури зі списку вище: ");
				
				auto result = getReadersByLiteratureType(type);
				if (result.empty()) ConsoleUtils::printInfo("Читачів із вказаним типом літератури не знайдено.");
				else {
					ConsoleUtils::printInfo("Читачі із зазначеним типом літератури: " + type);					
					for (const auto& reader : result) {
						std::cout << reader->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 4: {
				ConsoleUtils::printInfo("Виберіть літературу для запиту:");
				int literatureId = _literatureManager->printLiteratureMenu();

				time_t startDate = Date::getInputDate("Введіть початкову дату");
				time_t endDate = Date::getInputDate("Введіть кінцеву дату");

				if (startDate > endDate) throw std::runtime_error("Недійсні дати. Спробуйте ще раз.");

				auto result = getReadersByLiteratureInPeriod(literatureId, startDate, endDate);
				if(result.empty()) ConsoleUtils::printInfo("Читачів із вказаною літературою у зазначений період не знайдено.");
				else {
					ConsoleUtils::printInfo("Читачі, які отримували зазначену літерутуру з ідентифіктором "
						+ std::to_string(literatureId) + " у зазначений період: ");					
					for (const auto& reader : result) {
						std::cout << reader->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}				
				}
				break;
			}
			case 5: {
				ConsoleUtils::printInfo("Виберіть читача для запиту:");
				int readerId = _readerManager->printReadersMenu();

				time_t startDate = Date::getInputDate("Введіть початкову дату");
				time_t endDate = Date::getInputDate("Введіть кінцеву дату");

				if (startDate > endDate) throw std::runtime_error("Недійсні дати. Спробуйте ще раз.");

				auto result = getLiteratureTypesByReaderAndReaderLibraryInPeriod(readerId, startDate, endDate);
				if (result.empty()) ConsoleUtils::printInfo("Вказаний читач не отримував ніякі видання у бібліотеці, де він зареєстрований у заданий період.");
				else {
					ConsoleUtils::printInfo("Список видань, які отримував читач з ідентифікатором " +
						std::to_string(readerId) + " у бібліотеці, де він зареєстрований та у зазначений період:");					
					for (const auto& type : result) {
						std::cout << type << "\n";
					}					
				}
				break;
			}
			case 6: {
				ConsoleUtils::printInfo("Виберіть читача для запиту:");
				int readerId = _readerManager->printReadersMenu();

				time_t startDate = Date::getInputDate("Введіть початкову дату");
				time_t endDate = Date::getInputDate("Введіть кінцеву дату");

				if (startDate > endDate) throw std::runtime_error("Недійсні дати. Спробуйте ще раз.");

				auto result = getLiteratureTypesByReaderAndNotReaderLibraryInPeriod(readerId, startDate, endDate);
				if (result.empty()) ConsoleUtils::printInfo("Вказаний читач не отримував ніякі видання у бібліотеці, де він не зареєстрований у заданий період.");
				else {
					ConsoleUtils::printInfo("Список видань, які отримував читач з ідентифікатором " +
						std::to_string(readerId) + " у бібліотеці, де він НЕ зареєстрований та у зазначений період:");
					for (const auto& type : result) {
						std::cout << type << "\n";
					}
				}
				break;
			}
			case 7: {
				std::string location = ConsoleUtils::getInputString("Введіть розміщення полиці: ");
				
				auto result = getIssuedLiteratureByShelf(location);
				if (result.empty()) ConsoleUtils::printInfo("Із вказаної полиці ще не було видано літератури.");
				else {
					ConsoleUtils::printInfo("Виданна література з полиці " + location + ": ");					
					for (const auto& lit : result) {
						std::cout << lit->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 8: {
				ConsoleUtils::printInfo("Виберіть бібліотекаря для запиту:");
				int librarianId = _librarianManager->printLibrarianMenu();

				time_t startDate = Date::getInputDate("Введіть початкову дату");
				time_t endDate = Date::getInputDate("Введіть кінцеву дату");

				if (startDate > endDate) throw std::runtime_error("Недійсні дати. Спробуйте ще раз.");

				auto result = getReadersServedByLibrarianInPeriod(librarianId, startDate, endDate);
				if(result.empty()) ConsoleUtils::printInfo("Читачів, обслужених бібліотекарем з ідентифіктором " 
					+ std::to_string(librarianId) + " у зазначений період не знайдено.");
				else {
					ConsoleUtils::printInfo("Читачі обслужені бібліотекарем з ідентифіктором "
						+ std::to_string(librarianId) + " у зазначений період: ");					
					for (const auto& reader : result) {
						std::cout << reader->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 9: {
				ConsoleUtils::printInfo("Виберіть бібліотекаря для запиту:");
				int librarianid = _librarianManager->printLibrarianMenu();

				time_t startDate = Date::getInputDate("Введіть початкову дату");
				time_t endDate = Date::getInputDate("Введіть кінцеву дату");

				if (startDate > endDate) throw std::runtime_error("Недійсні дати. Спробуйте ще раз.");

				int result = getAmountOfServedReadersByLibrarianInPeriod(librarianid, startDate, endDate);
				ConsoleUtils::printInfo("Кількість обслужених читачів бібліотекарем з ідентифікатором " 
					+ std::to_string(librarianid) + " у зазначений період: " + std::to_string(result));
				break;
			}
			case 10: {
				auto result = getOverdueReaders();
				if (result.empty()) ConsoleUtils::printInfo("Читачів з простроченим терміном літератури не знайдено.");
				else {
					ConsoleUtils::printInfo("Читачі з простроченим терміном літератури:");					
					for (const auto& reader : result) {
						std::cout << reader->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 11: {
				time_t startDate = Date::getInputDate("Введіть початкову дату");
				time_t endDate = Date::getInputDate("Введіть кінцеву дату");
				if (startDate > endDate) throw std::runtime_error("Недійсні дати. Спробуйте ще раз.");

				std::string isArchivedInput = ConsoleUtils::getInputString("Вивести тільки списану літературу(1 - 'Так', 0 - 'Ні') ");
				
				auto result = getLiteratureByPeriod(startDate, endDate, isArchivedInput == "1");
				if (result.empty()) ConsoleUtils::printInfo("Літературу, яка надійшла(була списана) у зазначений період не знайдено.");
				else {
					ConsoleUtils::printInfo("Список літератури, яка надійшла(була списана) у зазначений період:");					
					for (const auto& lit : result) {
						std::cout << lit->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 12: {
				ConsoleUtils::printInfo("Виберіть бібліотеку для запиту:");
				int libraryId = _libraryManager->printLibraryMenu();

				ConsoleUtils::printInfo("Виберіть залу бібліотеки для запиту:");
				std::string hallId = _libraryManager->printHallsByLibraryId(libraryId);

				const auto& librarians = getLibrariansByHallId(hallId, libraryId);
				if (librarians.empty()) ConsoleUtils::printInfo("У вказаному залі бібліотекарів не знайдено.");
				else {
					ConsoleUtils::printInfo("Бібліотекарі, які працюють в зазначеному залі: ");					
					for (const auto& librarian : librarians) {
						std::cout << librarian->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 13: {
				time_t startDate = Date::getInputDate("Введіть початкову дату");
				time_t endDate = Date::getInputDate("Введіть кінцеву дату");

				if(startDate > endDate) throw std::runtime_error("Недійсні дати. Спробуйте ще раз.");

				auto result = getReadersWithoutVisitsInPeriod(startDate, endDate);
				if(result.empty()) ConsoleUtils::printInfo("Читачів, які не відвідували бібліотеку у зазначений період не знайдено.");
				else {
					ConsoleUtils::printInfo("Читачі, які не відвідували бібліотеку в період:");					
					for (const auto& reader : result) {
						std::cout << reader->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 14: {
				std::string title = ConsoleUtils::getInputString("Введіть назву твору: ");
				
				auto result = getInvNumsAndLibNameByLiteratureName(title);
				if(result.empty()) ConsoleUtils::printInfo("Література з такою назвою не знайдена.");
				else {
					ConsoleUtils::printInfo("Список інвентарних номерів і назв з бібліотечного фонду в яких міститься заданий твір:");
					for (const auto& res : result) {
						std::cout << res << "\n";
					}
				}
				break;
			}
			case 15: {
				std::string author = ConsoleUtils::getInputString("Введіть ПІБ/Ініціали/Псевдонім автора: ");
				
				auto result = getInvNumsAndLibNameByLiteratureAuthor(author);
				if(result.empty()) ConsoleUtils::printInfo("Література з таким автором не знайдена.");
				else {
					ConsoleUtils::printInfo("Список інвентарних номерів і назв з бібліотечного фонду в яких містяться твори зазначеного автора:");
					for (const auto& res : result) {
						std::cout << res << "\n";
					}
				}
				break;
			}
			case 16: {
				int topN = ConsoleUtils::getInputInt("Введіть номер для виведення перших N найпопулярнішої літератури: ");
				
				auto result = getMostPopularLiterature(topN);
				if(result.empty()) ConsoleUtils::printInfo("Найпопулярніша література не знайдена.");
				else {
					ConsoleUtils::printInfo("Найпопулярніша література: ");					
					for (const auto& lit : result) {
						std::cout << lit->toString();
						ConsoleUtils::printDelimeter('-', 150);
					}
				}
				break;
			}
			case 0: {
				std::cout << std::endl;
				return;
			}
			default:
				ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
			}
		}catch(const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

void LibrarySystem::proceedUserCommands() {
	while (true) {
		std::cout << "\n=== Доступні команди для керування користувачами ===\n";
		std::cout << " - \t1. Додати користувача\n";
		std::cout << " - \t2. Вивести поточного користувача\n";
		std::cout << " - \t3. Вивести всіх користувачів\n";
		std::cout << " - \t4. Вивести користувачів за ролю\n";
		std::cout << " - \t5. Видалити користувача\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");
		try {
			switch (choice)
			{
			case 1: {
				std::string name = ConsoleUtils::getInputString("Введіть унікальний нікнейм користувача: ");
				std::string password = ConsoleUtils::getInputString("Введіть пароль: ");
				std::string role = ConsoleUtils::getInputString("Введіть роль (Admin - 0/Librarian - 1): ");

				UserRole chosenRole = UserRole::ADMIN;
				int librarianId = -1;
				if (role == "1") {
					chosenRole = UserRole::LiBRARIAN;
					librarianId = _librarianManager->printLibrarianMenu();
				}

				_userManager->addUser(name, password, chosenRole, librarianId);
				ConsoleUtils::printSuccess("Користувача було додано успішно!");
				break;
			}
			case 2: {
				std::shared_ptr<User> currentUser = _userManager->getCurrentUser();
				ConsoleUtils::printSuccess("Інформація про поточного користувача");
				std::cout << currentUser->toString() << "\n";
				break;
			}
			case 3: {
				_userManager->printUsers();
				break;
			}
			case 4: {
				std::string role = ConsoleUtils::getInputString("Введіть роль (Admin - 0/Librarian - 1): ");
				UserRole chosenRole = UserRole::ADMIN;
				if (role == "1") {
					chosenRole = UserRole::LiBRARIAN;
				}
				else if (role != "0") {
					throw std::runtime_error("Недійсний варіант. Спробуйте ще раз.");
					break;
				}
				_userManager->printUsers(chosenRole);
				break;
			}
			case 5: {
				std::string username = ConsoleUtils::getInputString("Введіть нікнейм користувача: ");
				_userManager->deleteUser(username);
				break;
			}
			case 0: {
				std::cout << std::endl;
				return;
			}
			default:
				ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
			}
		}catch(const std::exception& e) {
			ConsoleUtils::printError(std::string("Помилка: ") + e.what());
		}
	}
}

/* ГОЛОВНА ФУНКЦІЯ */ 
void LibrarySystem::run() {
	while (true) {
		std::cout << "=== Система керування бібліотечним фондом міста ===\n";
		std::cout << " - \t1. Керування сутностями\n";
		std::cout << " - \t2. Виконання запитів \n";
		std::cout << " - \t3. Керування користувачами \x1B[34m (Admin) \033[0m\n";
		std::cout << " - \t4. очистити екран\n";
		std::cout << " - \t0. вихід\n";

		int choice = ConsoleUtils::getInputInt("Введіть номер команди: ");

		switch (choice) {
		case 1: {
			proceedEntitieCommands();
			break;
		}
		case 2: {
			proceedQueriesCommands();
			break;
		}
		case 3: {
			if (!_userManager->checkAdminsPermissions()) throw std::runtime_error("Недостатньо прав для виконання запитів.");
				
			proceedUserCommands();
			break;
		}
		case 4: {
			system("cls||clear");
			break;
		}
		case 0: {
			std::cout << "Завершення роботи...\n";
			return;
		}
		default:
			ConsoleUtils::printWarning("Недійсний варіант. Спробуйте ще раз.");
		}
	}
}