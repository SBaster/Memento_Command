import time
import random

class Memento:
    def get_name(self):
        raise NotImplementedError

    def date(self):
        raise NotImplementedError

    def state(self):
        raise NotImplementedError

class ConcreteMemento(Memento):
    def __init__(self, state):
        self._state = state
        # time.ctime() returns a string representation of the current time.
        self._date = time.ctime()

    # Method used by the Originator to restore its state.
    def state(self):
        return self._state

    # Methods used by the Caretaker to display metadata.
    def get_name(self):
        return f"{self._date} / ({self._state[:9]}...)"

    def date(self):
        return self._date

class Originator:
    def __init__(self, state):
        self._state = state
        print("Originator: My initial state is: " + self._state)

    def do_something(self):
        print("Originator: I'm doing something important.")
        # Generate a random string of 30 characters.
        self._state = ''.join(random.choice("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") for _ in range(30))
        print("Originator: and my state has changed to: " + self._state)

    def save(self):
        return ConcreteMemento(self._state)

    def restore(self, memento):
        self._state = memento.state()
        print("Originator: My state has changed to: " + self._state)

class Caretaker:
    def __init__(self, originator):
        self._mementos = []
        self._originator = originator

    def backup(self):
        print("\nCaretaker: Saving Originator's state...")
        self._mementos.append(self._originator.save())

    def undo(self):
        if not self._mementos:
            return
        memento = self._mementos.pop()
        print("Caretaker: Restoring state to: " + memento.get_name())
        try:
            self._originator.restore(memento)
        except Exception:
            self.undo()

    def show_history(self):
        print("Caretaker: Here's the list of mementos:")
        for memento in self._mementos:
            print(memento.get_name())

def client_code():
    originator = Originator("Super-duper-super-puper-super.")
    caretaker = Caretaker(originator)

    caretaker.backup()
    originator.do_something()

    caretaker.backup()
    originator.do_something()

    caretaker.backup()
    originator.do_something()

    print()
    caretaker.show_history()

    print("\nClient: Now, let's rollback!\n")
    caretaker.undo()

    print("\nClient: Once more!\n")
    caretaker.undo()

if __name__ == "__main__":
    random.seed(time.time())
    client_code()
