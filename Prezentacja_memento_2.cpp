#include <iostream>
#include <vector>
#include <ctime>
#include <memory>
#include <string>
#include <cstdlib>

// Interfejs Memento
class Memento {
public:
    virtual ~Memento() {}
    virtual std::string GetName() const = 0;
    virtual std::string date() const = 0;
    virtual std::string state() const = 0;
};

// Konkretny Memento przechowujący stan Originatora
class ConcreteMemento : public Memento {
private:
    std::string state_;
    std::string date_;

public:
    ConcreteMemento(const std::string& state) : state_(state) {
        std::time_t now = std::time(nullptr);
        char buffer[26]; // Bufor na wynik, rozmiar standardowy dla ctime
        if (ctime_s(buffer, sizeof(buffer), &now) == 0) {
            this->date_ = buffer;
        }
        else {
            this->date_ = "Błąd konwersji czasu";
        }
    }
    // Metoda służąca Originatorowi do przywrócenia stanu.
    std::string state() const override {
        return this->state_;
    }
    // Metody wykorzystywane przez Caretakera do prezentacji metadanych.
    std::string GetName() const override {
        return this->date_ + " / (" + this->state_.substr(0, 9) + "...)";
    }
    std::string date() const override {
        return this->date_;
    }
};

// Originator, który przechowuje stan i potrafi go zapisać oraz przywrócić
class Originator {
private:
    std::string state_;

    std::string GenerateRandomString(int length = 10) {
        const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        int stringLength = sizeof(alphanum) - 1;

        std::string random_string;
        for (int i = 0; i < length; i++) {
            random_string += alphanum[std::rand() % stringLength];
        }
        return random_string;
    }

public:
    Originator(const std::string& state) : state_(state) {
        std::cout << "Originator: My initial state is: " << this->state_ << "\n";
    }
    // Metoda modyfikująca stan Originatora
    void DoSomething() {
        std::cout << "Originator: I'm doing something important.\n";
        this->state_ = this->GenerateRandomString(30);
        std::cout << "Originator: and my state has changed to: " << this->state_ << "\n";
    }
    // Zapisuje aktualny stan i zwraca go jako unikalny wskaźnik do Memento.
    std::unique_ptr<Memento> Save() {
        return std::make_unique<ConcreteMemento>(this->state_);
    }
    // Przywraca stan z przekazanego memento.
    void Restore(const Memento* memento) {
        this->state_ = memento->state();
        std::cout << "Originator: My state has changed to: " << this->state_ << "\n";
    }
};
// Caretaker przechowujący historię stanów, ale nie mający dostępu do ich wewnętrznej struktury
class Caretaker {
private:
    std::vector<std::unique_ptr<Memento>> mementos_;
    Originator* originator_;

public:
    Caretaker(Originator* originator) : originator_(originator) {}

    void Backup() {
        std::cout << "\nCaretaker: Saving Originator's state...\n";
        mementos_.push_back(originator_->Save());
    }
    void Undo() {
        if (mementos_.empty()) {
            return;
        }
        // Pobieramy ostatni zapisany stan
        auto memento = std::move(mementos_.back());
        mementos_.pop_back();
        std::cout << "Caretaker: Restoring state to: " << memento->GetName() << "\n";
        try {
            originator_->Restore(memento.get());
        }
        catch (...) {
            Undo();
        }
    }
    void ShowHistory() const {
        std::cout << "Caretaker: Here's the list of mementos:\n";
        for (const auto& memento : mementos_) {
            std::cout << memento->GetName() << "\n";
        }
    }
};
// Klient korzystający z wzorca Memento
void ClientCode() {
    // Tworzymy obiekty na stosie
    Originator originator("Super-duper-super-puper-super.");
    Caretaker caretaker(&originator);

    caretaker.Backup();
    originator.DoSomething();

    caretaker.Backup();
    originator.DoSomething();

    caretaker.Backup();
    originator.DoSomething();

    std::cout << "\n";
    caretaker.ShowHistory();

    std::cout << "\nClient: Now, let's rollback!\n\n";
    caretaker.Undo();

    std::cout << "\nClient: Once more!\n\n";
    caretaker.Undo();
}
int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    ClientCode();
    return 0;
}
