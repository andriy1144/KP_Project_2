#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <ctime>
#include <map>
#include <algorithm>

using namespace std;

// ----- Клас дати -----
struct Date {
    int day, month, year;

    bool operator<=(const Date& other) const {
        if (year != other.year) return year <= other.year;
        if (month != other.month) return month <= other.month;
        return day <= other.day;
    }
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator>=(const Date& other) const {
        if (year != other.year) return year >= other.year;
        if (month != other.month) return month >= other.month;
        return day >= other.day;
    }

    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
};

ostream& operator<<(ostream& os, const Date& d) {
    os << d.day << "." << d.month << "." << d.year;
    return os;
}

// ----- Базові класи творів та читачів -----
class Work {
public:
    string title, author;
    virtual string getType() const = 0;
    virtual ~Work() = default;
};

class Book : public Work {
public:
    string publisher;
    string getType() const override { return "Book"; }
};

class Reader {
public:
    string name;
    string registeredLibrary;
    virtual string getCategory() const = 0;
    virtual ~Reader() = default;
};

class Student : public Reader {
public:
    string university, faculty;
    int course;
    string getCategory() const override { return "Student"; }
};

class Scientist : public Reader {
public:
    string organization, topic;
    string getCategory() const override { return "Scientist"; }
};

// ----- Клас видання -----
struct Publication {
    shared_ptr<Work> work;
    string inventoryNumber;
    string location;
    bool isAvailable = true;
    Date arrivalDate;
    bool writtenOff = false;
};

struct Librarian {
    string name;
    string libraryName;
    string roomName;
};

struct Loan {
    shared_ptr<Reader> reader;
    shared_ptr<Publication> publication;
    Date issueDate;
    Date returnDate;
    bool returned = false;
    Librarian librarian;
};

class Library {
public:
    string name;
    vector<Librarian> librarians;
    vector<shared_ptr<Publication>> publications;
};

class LibrarySystem {
    vector<shared_ptr<Reader>> readers;
    vector<shared_ptr<Work>> works;
    vector<shared_ptr<Publication>> publications;
    vector<Library> libraries;
    vector<Loan> loans;

public:
    void addReader(shared_ptr<Reader> r) { readers.push_back(r); }
    void addWork(shared_ptr<Work> w) { works.push_back(w); }
    void addPublication(shared_ptr<Publication> p) { publications.push_back(p); }
    void addLibrary(const Library& l) { libraries.push_back(l); }

    void issuePublication(shared_ptr<Reader> reader, shared_ptr<Publication> publication, const Date& issueDate, const Date& returnDate, const Librarian& librarian) {
        if (!publication->isAvailable) {
            cout << "Видання недоступне\n";
            return;
        }
        publication->isAvailable = false;
        loans.push_back({ reader, publication, issueDate, returnDate, false, librarian });
    }

    void returnPublication(shared_ptr<Publication> publication) {
        for (auto& loan : loans) {
            if (loan.publication == publication && !loan.returned) {
                loan.returned = true;
                publication->isAvailable = true;
                return;
            }
        }
    }

    void showMenu() {
        cout << "\n===== МЕНЮ ЗАПИТІВ =====\n";
        cout << "1-16. Усі запити згідно з ТЗ\n";
    }

    void executeQuery(int queryNumber) {
        Date d1{ 1, 1, 2023 }, d2{ 31, 12, 2024 };
        switch (queryNumber) {
        case 1:
            for (auto& r : readers) {
                if (r->getCategory() == "Student") {
                    auto s = dynamic_pointer_cast<Student>(r);
                    cout << s->name << " з " << s->university << " факультет: " << s->faculty << endl;
                }
            }
            break;
        case 2:
            for (auto& loan : loans)
                if (!loan.returned)
                    cout << loan.reader->name << " має " << loan.publication->work->title << endl;
            break;
        case 3:
            for (auto& loan : loans)
                if (!loan.returned)
                    cout << loan.reader->name << " має інв. № " << loan.publication->inventoryNumber << endl;
            break;
        case 4:
            for (auto& loan : loans)
                if (loan.issueDate >= d1 && loan.issueDate <= d2)
                    cout << loan.reader->name << " отримав " << loan.publication->work->title << endl;
            break;
        case 5:
            for (auto& loan : loans)
                if (loan.reader->registeredLibrary == loan.librarian.libraryName)
                    cout << loan.reader->name << " отримав " << loan.publication->work->title << endl;
            break;
        case 6:
            for (auto& loan : loans)
                if (loan.reader->registeredLibrary != loan.librarian.libraryName)
                    cout << loan.reader->name << " користувався " << loan.publication->work->title << endl;
            break;
        case 7:
            for (auto& loan : loans)
                if (!loan.returned)
                    cout << loan.publication->inventoryNumber << " з " << loan.publication->location << endl;
            break;
        case 8:
            for (auto& loan : loans)
                if (loan.issueDate >= d1 && loan.issueDate <= d2)
                    cout << loan.librarian.name << " обслужив " << loan.reader->name << endl;
            break;
        case 9: {
            map<string, int> count;
            for (auto& loan : loans)
                if (loan.issueDate >= d1 && loan.issueDate <= d2)
                    count[loan.librarian.name]++;
            for (auto& p : count) cout << p.first << ": " << p.second << endl;
            break;
        }
        case 10: {
            Date today{ 2, 5, 2025 };
            for (auto& loan : loans)
                if (!loan.returned && loan.returnDate < today)
                    cout << loan.reader->name << " має прострочене повернення: " << loan.publication->work->title << endl;
            break;
        }
        case 11:
            for (auto& pub : publications)
                if (pub->arrivalDate >= d1 && pub->arrivalDate <= d2 && !pub->writtenOff)
                    cout << pub->inventoryNumber << " (надходження): " << pub->work->title << endl;
            break;
        case 12:
            for (auto& lib : libraries)
                for (auto& libn : lib.librarians)
                    cout << libn.name << " працює у залі " << libn.roomName << " бібліотеки " << lib.name << endl;
            break;
        case 13:
            for (auto& r : readers) {
                bool visited = false;
                for (auto& loan : loans)
                    if (loan.reader == r && loan.issueDate >= d1 && loan.issueDate <= d2)
                        visited = true;
                if (!visited) cout << r->name << " не відвідував бібліотеку" << endl;
            }
            break;
        case 14:
            for (auto& pub : publications)
                cout << pub->inventoryNumber << ": " << pub->work->title << endl;
            break;
        case 15:
            for (auto& pub : publications)
                cout << pub->inventoryNumber << " автора: " << pub->work->author << endl;
            break;
        case 16: {
            map<string, int> freq;
            for (auto& loan : loans)
                freq[loan.publication->work->title]++;
            vector<pair<string, int>> sorted(freq.begin(), freq.end());
            sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) { return b.second < a.second; });
            for (auto& p : sorted)
                cout << p.first << " (" << p.second << ")" << endl;
            break;
        }
        default:
            cout << "Невірний запит\n";
        }
    }

    void loadSampleData() {
        auto s1 = make_shared<Student>(); s1->name = "Іван Іванов", s1->university = "КПІ", s1->faculty = "ФІОТ", s1->course = 2; s1->registeredLibrary = "Центральна";
        auto sc1 = make_shared<Scientist>(); sc1->name = "Петро Науковець", sc1->organization = "Інститут фізики", sc1->topic = "Квантова механіка"; sc1->registeredLibrary = "Наукова";

        readers.push_back(s1); readers.push_back(sc1);

        auto b1 = make_shared<Book>(); b1->title = "Програмування", b1->author = "Автор 1", b1->publisher = "Наука";
        auto b2 = make_shared<Book>(); b2->title = "Фізика", b2->author = "Автор 2", b2->publisher = "Освіта";

        works.push_back(b1); works.push_back(b2);

        auto p1 = make_shared<Publication>(); p1->work = b1; p1->inventoryNumber = "INV001"; p1->location = "A-1-1"; p1->arrivalDate = { 10,1,2023 };
        auto p2 = make_shared<Publication>(); p2->work = b2; p2->inventoryNumber = "INV002"; p2->location = "B-2-3"; p2->arrivalDate = { 15,2,2024 };

        publications.push_back(p1); publications.push_back(p2);

        Librarian lib1{ "Оксана Бібліотекар", "Центральна", "Зал №1" };
        Librarian lib2{ "Ірина Залова", "Наукова", "Зал №2" };

        Library l1{ "Центральна", {lib1}, {p1} };
        Library l2{ "Наукова", {lib2}, {p2} };
        libraries.push_back(l1); libraries.push_back(l2);

        issuePublication(s1, p1, { 1,3,2024 }, { 1,4,2024 }, lib1);
        issuePublication(sc1, p2, { 10,4,2024 }, { 1,5,2024 }, lib2);
    }
};

/*
int main() {
	setlocale(LC_ALL, "ukrainian");

    LibrarySystem system;
    system.loadSampleData();
    system.showMenu();
    int query;
    cout << "Введіть номер запиту (1-16): ";
    cin >> query;
    system.executeQuery(query);
    return 0;
}
*/