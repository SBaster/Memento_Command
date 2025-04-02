#include <iostream>
#include <memory>
#include <string>

// Krok 1: Definicja interfejsu polecenia
class Command {
public:
    virtual ~Command() {}
    virtual void Execute() const = 0;
};

// Proste polecenie, które wykonuje prostą operację
class SimpleCommand : public Command {
private:
    std::string pay_load_;

public:
    explicit SimpleCommand(const std::string& pay_load) : pay_load_(pay_load) {}
    void Execute() const override {
        std::cout << "ProstePolecenie: Mogę wykonać proste operacje, np. wypisać tekst ("
                  << this->pay_load_ << ")\n";
    }
};

// Klasa odbiorcy zawierająca istotną logikę biznesową.
class Receiver {
public:
    void DoSomething(const std::string& a) {
        std::cout << "Odbiorca: Pracuję nad (" << a << ").\n";
    }
    void DoSomethingElse(const std::string& b) {
        std::cout << "Odbiorca: Pracuję również nad (" << b << ").\n";
    }
};

// Złożone polecenie, które deleguje operacje do obiektu odbiorcy.
class ComplexCommand : public Command {
private:
    Receiver* receiver_;
    std::string a_;
    std::string b_;
public:
    ComplexCommand(Receiver* receiver, const std::string& a, const std::string& b)
        : receiver_(receiver), a_(a), b_(b) {
    }
    void Execute() const override {
        std::cout << "ZłożonePolecenie: Złożone operacje powinien wykonać obiekt odbiorcy.\n";
        receiver_->DoSomething(a_);
        receiver_->DoSomethingElse(b_);
    }
};

// Inwoker, który jest powiązany z jednym lub kilkoma poleceniami.
class Invoker {
private:
    std::unique_ptr<Command> on_start_;
    std::unique_ptr<Command> on_finish_;
public:
    void SetOnStart(std::unique_ptr<Command> command) {
        on_start_ = std::move(command);
    }
    void SetOnFinish(std::unique_ptr<Command> command) {
        on_finish_ = std::move(command);
    }
    void DoSomethingImportant() {
        std::cout << "Inwoker: Czy ktoś potrzebuje, żebym coś zrobił przed rozpoczęciem?\n";
        if (on_start_) {
            on_start_->Execute();
        }
        std::cout << "Inwoker: ...wykonuję bardzo ważne zadanie...\n";
        std::cout << "Inwoker: Czy ktoś potrzebuje, żebym coś zrobił po zakończeniu?\n";
        if (on_finish_) {
            on_finish_->Execute();
        }
    }
};

int main() {
    Invoker invoker;
    // Tworzenie obiektów poleceń przy użyciu std::make_unique, co gwarantuje automatyczne zarządzanie pamięcią.
    invoker.SetOnStart(std::make_unique<SimpleCommand>("Powiedz Cześć!"));
    // Tworzymy obiekt odbiorcy na stosie.
    Receiver receiver;
    invoker.SetOnFinish(std::make_unique<ComplexCommand>(&receiver, "Wyślij email", "Zapisz raport"));

    invoker.DoSomethingImportant();

    return 0;
}
