#include <iostream>
#include <cstring>
#include <string>
using namespace std;

#define MAX_LENGTH 256


// Abstract Command Class
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Print buffer string.
class PrintCommand : public Command{

    char* buffer;

public:
    PrintCommand(char* buffer){
        this->buffer = buffer;
    }
    void execute() {
        if (strlen(buffer) == 0)
        {
            cout << ">>[ Buffer is empty. ]" << endl;
        }
        else {
            cout << ">>" << buffer << endl;
        }
    }
    void undo(){}
};

// Append string to buffer.
class AddCommand : public Command{

    char* buffer;
    int* size;
    int* length;
    int maxSize;
    int inputLength;
public:
    AddCommand(char* buffer, int* _size, int* _length, int _maxSize){
        this->buffer = buffer;
        size = _size;
        length = _length;
        maxSize = _maxSize;
        inputLength = 0;
    }
    void execute() {
        cin.ignore(1, ' ');
        cin.get(&buffer[*length], *size, '\n');
        inputLength = strlen(buffer);
        *length += inputLength;
        *size = maxSize - *length;
        cout << ">>[ Added ]" << endl;
    }
    void undo(){
        buffer[*length - inputLength] = '\0';
        cout << ">> [ Undid add.]" << endl;
    }
};

// Remove 'n' characters from beginning of buffer.
class RemoveCommand : public Command{

    char* buffer;
    int* size;
    int* length;
    int maxSize;
    int remove;
    //char* removed;
public:
    RemoveCommand(char* buffer, int* _size, int* _length, int _maxSize){
        this->buffer = buffer;
        size = _size;
        length = _length;
        maxSize = _maxSize;
        remove = 0;
        //removed = NULL;
    }
    void execute() {
        if (*length == 0){
            cout << ">>[ Buffer is already empty. ]" << endl;
            cin.ignore();
        }
        else {
            cin.ignore(1, ' ');
            cin >> remove;
            //removed = new char[remove];
            if (remove < *length) {
                //memcpy(removed, buffer, remove);
                memmove(buffer, buffer+remove, (*length + remove));
                *length = strlen(buffer);
                *size = maxSize - *length;
            }
            else {
                //memcpy(removed, buffer, *length);
                buffer[0] = '\0';
                *size = maxSize;
                *length = 0;
            }
            cout << ">>[ Removing " << remove << " characters. ]" << endl;
        }
    }
    void undo(){}
};

// Empty buffer.
class EmptyCommand : public Command{

    char* buffer;
    int* size;
    int* length;
    int maxSize;

public:
    EmptyCommand(char* buffer, int* _size, int* _length, int _maxSize) {
        this->buffer = buffer;
        size = _size;
        length = _length;
        maxSize = _maxSize;
    }
    void execute() {
        cout << ">>[ Buffer emptied. ]" << endl;
        buffer[0] = '\0';
        *size = maxSize;
        *length = 0;
    }
    void undo(){}
};

// Return length of buffer.
class LengthCommand : public Command{

int* length;

public:
    LengthCommand(int* length){
        this->length = length;
    }
    void execute() {
        cout << ">>" << *length << endl;
    }
    void undo(){}
};

class NoCommand : public Command{
public:
    void execute(){}
    void undo(){}
};

class Shell {
    Command** commandList;
public:
    Shell(Command* cmdList[]){
        this->commandList = cmdList;
    }
    void prompt(){
        // Command object to hold instances of commands.
        Command* commandSlot = new NoCommand();
        char operation = '\0';
        bool exit = false;
        bool undo = false;
        // Title Screen
        cout
        << "\tCommand Pattern"
        << endl;

        // User input loop.
        while(!exit){
            cout << "Enter command ('?' for help): ";
            // Clear input buffer.
            cin.clear();
            operation = '\0';

            // Retrieve operation from buffer.
            cin.get(operation);
            switch(operation){
                case '?':
                    cout << ">>\n\tp - Print Buffer"
                    << "\n\ta <str> - Add string to buffer"
                    << "\n\tr <n> - Remove 'n' chars from start of buffer"
                    << "\n\te - Empty buffer"
                    << "\n\tl- Return length of buffer"
                    << "\n\tu - Undo last command"
                    << "\n\tq - Quit\n" << endl;
                    commandSlot = new NoCommand();
                    break;
                case 'p':
                    commandSlot = commandList[0];
                    break;
                case 'a':
                    commandSlot = commandList[1];
                    break;
                case 'r':
                    commandSlot = commandList[2];
                    break;
                case 'l':
                    commandSlot = commandList[3];
                    break;
                case 'e':
                    commandSlot = commandList[4];
                    break;
                case 'q':
                    exit = true;
                    commandSlot = new NoCommand();
                    break;
                case 'u':
                    undo = true;
                    break;
                default:
                        cout << ">>[ Unknown command, please try again ]" << endl;
                        commandSlot = new NoCommand();
            }
            // Run selected operation.
            if (undo)
            {
                commandSlot->undo();
                undo = false;
            }
            else{
                commandSlot->execute();
            }
            // Clear buffer.
            cin.ignore();
        }
    }
};

int main(){

    // --Properties
    int length = 0;
    int size = MAX_LENGTH;
    char buffer[size];

    // Specific commands.
    Command* printCmd = new PrintCommand(&buffer[0]);
    Command* addCmd = new AddCommand(&buffer[0], &size, &length, MAX_LENGTH);
    Command* removeCmd = new RemoveCommand(&buffer[0], &size, &length, MAX_LENGTH);
    Command* emptyCmd = new EmptyCommand(&buffer[0], &size, &length, MAX_LENGTH);
    Command* lengthCmd = new LengthCommand(&length);

    // List of existing commands.
    Command* commandList[] = { printCmd, addCmd, removeCmd, lengthCmd, emptyCmd};

    Shell* shell = new Shell(commandList);

    shell->prompt();

    return 0;
}
