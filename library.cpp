#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

/**
 * @brief Представляє автора публікації.
 */
class Author {
    string name;
public:
    /**
     * @brief Конструктор автора.
     * @param n Ім'я автора.
     */
    explicit Author(string n) : name(move(n)) {}

    /**
     * @brief Повертає ім'я автора.
     * @return Ім'я автора.
     */
    string getName() const { return name; }

    /**
     * @brief Друкує інформацію про автора.
     */
    void print() const { cout << "Author: " << name << endl; }
};

/**
 * @brief Базовий клас для публікацій (книги, журнали, електронні книги).
 *
 * Містить загальні поля: заголовок, рік та автора.
 */
class Publication {
protected:
    string title;
    int year;
    Author author;
public:
    /**
     * @brief Конструктор публікації.
     * @param t Заголовок.
     * @param y Рік видання.
     * @param a Автор (об'єкт Author).
     */
    Publication(string t, int y, Author a)
        : title(move(t)), year(y), author(move(a)) {}

    virtual ~Publication() = default;

    /**
     * @brief Виводить інформацію про публікацію (віртуальний метод).
     */
    virtual void displayInfo() const = 0;

    /**
     * @brief Повертає заголовок публікації.
     * @return Заголовок.
     */
    string getTitle() const { return title; }

    /**
     * @brief Повертає рік видання.
     * @return Рік видання.
     */
    int getYear() const { return year; }

    /**
     * @brief Повертає ім'я автора.
     * @return Ім'я автора.
     */
    string getAuthorName() const { return author.getName(); }
};

/**
 * @brief Клас, що представляє друковану книгу.
 *
 * Містить жанр та стан доступності.
 */
class Book : public Publication {
    string genre;
    bool available;
public:
    /**
     * @brief Конструктор книги.
     * @param t Заголовок.
     * @param y Рік видання.
     * @param a Автор.
     * @param g Жанр.
     */
    Book(string t, int y, Author a, string g)
        : Publication(move(t), y, move(a)), genre(move(g)), available(true) {}

    /**
     * @brief Позичає книгу (робить її недоступною).
     */
    void borrow() { available = false; }

    /**
     * @brief Повертає книгу (робить її доступною).
     */
    void ret() { available = true; }

    /**
     * @brief Чи доступна книга для позички.
     * @return true якщо доступна, false якщо позичена.
     */
    bool isAvailable() const { return available; }

    /**
     * @brief Виводить інформацію про книгу.
     */
    void displayInfo() const override {
        cout << "Book: " << title << " (" << year << "), " << genre
             << " - " << author.getName() << (available ? " [Available]" : " [Taken]") << endl;
    }
};

/**
 * @brief Клас для журналу.
 */
class Magazine : public Publication {
    int issue;
public:
    /**
     * @brief Конструктор журналу.
     * @param t Заголовок.
     * @param y Рік виходу.
     * @param a Автор/редактор.
     * @param i Номер випуску.
     */
    Magazine(string t, int y, Author a, int i)
        : Publication(move(t), y, move(a)), issue(i) {}

    /**
     * @brief Виводить інформацію про журнал.
     */
    void displayInfo() const override {
        cout << "Magazine: " << title << " #" << issue
             << " (" << year << ") - " << author.getName() << endl;
    }
};

/**
 * @brief Електронна книга (уснавлює поведінку Book).
 */
class EBook : public Book {
    double fileSize;
public:
    /**
     * @brief Конструктор електронної книги.
     * @param t Заголовок.
     * @param y Рік видання.
     * @param a Автор.
     * @param g Жанр.
     * @param s Розмір файлу в MB.
     */
    EBook(string t, int y, Author a, string g, double s)
        : Book(move(t), y, move(a), move(g)), fileSize(s) {}

    /**
     * @brief Виводить інформацію про електронну книгу.
     */
    void displayInfo() const override {
        cout << "E-Book: " << getTitle() << " (" << getYear() << "), size: "
             << fileSize << "MB - " << getAuthorName() << endl;
    }
};

/**
 * @brief Користувач (читач), який може позичати книги.
 */
class Reader {
    string name;
    vector<shared_ptr<Book>> borrowed;
public:
    /**
     * @brief Конструктор читача.
     * @param n Ім'я читача.
     */
    explicit Reader(string n) : name(move(n)) {}

    /**
     * @brief Спроба позичити книгу.
     * @param b Смарт-указівник на книгу.
     * @example
     * Reader r("Ivan");
     * r.borrowBook(bookPtr);
     */
    void borrowBook(shared_ptr<Book> b) {
        if (!b) {
            cout << "Invalid book pointer\n";
            return;
        }
        if (b->isAvailable()) {
            borrowed.push_back(b);
            b->borrow();
            cout << name << " borrowed \"" << b->getTitle() << "\"" << endl;
        } else cout << "Book not available!\n";
    }

    /**
     * @brief Повернути всі позичені книги.
     */
    void returnAll() {
        for (auto& b : borrowed) if (b) b->ret();
        borrowed.clear();
    }

    /**
     * @brief Показати всі книги, які позичив читач.
     */
    void listBooks() const {
        cout << name << " borrowed books:\n";
        for (auto& b : borrowed) if (b) b->displayInfo();
    }
};

/**
 * @brief Бібліотека — колекція публікацій.
 */
class Library {
    vector<shared_ptr<Publication>> publications;
public:
    /**
     * @brief Додає публікацію до колекції.
     * @param p Смарт-указівник на Publication.
     */
    void addPublication(shared_ptr<Publication> p) { publications.push_back(p); }

    /**
     * @brief Виводить усю колекцію бібліотеки.
     */
    void showAll() const {
        cout << "\n=== Library Collection ===\n";
        for (auto& p : publications) if (p) p->displayInfo();
    }
};

/**
 * @brief Універсальний шаблон для виводу списку публікацій.
 * @tparam T Тип елементів (мають метод displayInfo()).
 * @param list Вектор смарт-указівників на елементи.
 * @param title Заголовок списку.
 */
template<typename T>
void printList(const vector<T>& list, const string& title) {
    cout << "\n=== " << title << " ===\n";
    for (const auto& item : list) if (item) item->displayInfo();
}

/**
 * @brief Базовий тип користувача системи.
 */
class User {
protected:
    string name;
public:
    /**
     * @brief Конструктор.
     * @param n Ім'я користувача.
     */
    explicit User(string n) : name(move(n)) {}

    /**
     * @brief Показати роль користувача (віртуально).
     */
    virtual void showRole() const = 0;
};

/**
 * @brief Адмін бібліотеки.
 */
class Admin : public User {
public:
    /**
     * @brief Конструктор.
     * @param n Ім'я адміна.
     */
    explicit Admin(string n) : User(move(n)) {}

    /**
     * @brief Показати роль.
     */
    void showRole() const override { cout << name << " is Admin\n"; }

    /**
     * @brief Симульовано видалити книгу.
     * @param lib Посилання на бібліотеку.
     */
    void removeBook(Library& lib) { cout << name << " removed a book (simulated)\n"; }
};

int main() {
    Author a1("Herbert Schildt"), a2("Bjarne Stroustrup");
    Library lib;

    auto b1 = make_shared<Book>("C++ for Beginners", 2020, a1, "Education");
    auto b2 = make_shared<EBook>("The C++ Programming Language", 2013, a2, "Programming", 5.6);
    auto m1 = make_shared<Magazine>("TechWorld", 2025, a1, 12);

    lib.addPublication(b1);
    lib.addPublication(b2);
    lib.addPublication(m1);

    Reader r("Ivan Petrov");
    r.borrowBook(b1);
    r.listBooks();

    lib.showAll();

    Admin admin("Olena");
    admin.showRole();

    vector<shared_ptr<Publication>> all = {b1, b2, m1};
    printList(all, "All Publications (via template)");

    return 0;
}
