#include <iostream>
#include <memory>
#include <string>
// Step 1: Define the Command Interface
class Command {
public:
    virtual ~Command() {}
    virtual void Execute() const = 0;
};
//Some commands can implement simple operations on their own.
class SimpleCommand : public Command {
private:
    std::string pay_load_;

public:
    explicit SimpleCommand(const std::string& pay_load) : pay_load_(pay_load) {}
    void Execute() const override {
        std::cout << "SimpleCommand: See, I can do simple things like printing ("
            << this->pay_load_ << ")\n";
    }
};
 //The Receiver classes contain some important business logic.
class Receiver {
public:
    void DoSomething(const std::string& a) {
        std::cout << "Receiver: Working on (" << a << ".)\n";
    }
    void DoSomethingElse(const std::string& b) {
        std::cout << "Receiver: Also working on (" << b << ".)\n";
    }
};
//Complex commands can delegate more complex operations to other objects.
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
        std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
        receiver_->DoSomething(a_);
        receiver_->DoSomethingElse(b_);
    }
};
//The Invoker is associated with one or several commands.
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
        std::cout << "Invoker: Does anybody want something done before I begin?\n";
        if (on_start_) {
            on_start_->Execute();
        }
        std::cout << "Invoker: ...doing something really important...\n";
        std::cout << "Invoker: Does anybody want something done after I finish?\n";
        if (on_finish_) {
            on_finish_->Execute();
        }
    }
};
int main() {
    Invoker invoker;
    // Obiekty poleceń są tworzone dynamicznie za pomocą std::make_unique,
    // co gwarantuje automatyczne zarządzanie pamięcią.
    invoker.SetOnStart(std::make_unique<SimpleCommand>("Say Hi!"));
    // Tworzymy odbiorcę na stosie.
    Receiver receiver;
    invoker.SetOnFinish(std::make_unique<ComplexCommand>(&receiver, "Send email", "Save report"));

    invoker.DoSomethingImportant();

    return 0;
}
