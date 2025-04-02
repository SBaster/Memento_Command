from abc import ABC, abstractmethod

# Step 1: Define the Command Interface
class Command(ABC):
    @abstractmethod
    def execute(self):
        pass

# Some commands can implement simple operations on their own.
class SimpleCommand(Command):
    def __init__(self, payload):
        self._payload = payload

    def execute(self):
        print("SimpleCommand: See, I can do simple things like printing ({})".format(self._payload))

# The Receiver classes contain some important business logic.
class Receiver:
    def do_something(self, a):
        print("Receiver: Working on ({}).".format(a))

    def do_something_else(self, b):
        print("Receiver: Also working on ({}).".format(b))

# Complex commands can delegate more complex operations to other objects.
class ComplexCommand(Command):
    def __init__(self, receiver, a, b):
        self._receiver = receiver
        self._a = a
        self._b = b

    def execute(self):
        print("ComplexCommand: Complex stuff should be done by a receiver object.")
        self._receiver.do_something(self._a)
        self._receiver.do_something_else(self._b)

# The Invoker is associated with one or several commands.
class Invoker:
    def __init__(self):
        self._on_start = None
        self._on_finish = None

    def set_on_start(self, command):
        self._on_start = command

    def set_on_finish(self, command):
        self._on_finish = command

    def do_something_important(self):
        print("Invoker: Does anybody want something done before I begin?")
        if self._on_start:
            self._on_start.execute()
        print("Invoker: ...doing something really important...")
        print("Invoker: Does anybody want something done after I finish?")
        if self._on_finish:
            self._on_finish.execute()

if __name__ == "__main__":
    invoker = Invoker()
    invoker.set_on_start(SimpleCommand("Say Hi!"))
    
    # Creating the receiver on the stack
    receiver = Receiver()
    invoker.set_on_finish(ComplexCommand(receiver, "Send email", "Save report"))

    invoker.do_something_important()
