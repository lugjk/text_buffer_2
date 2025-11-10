#include "RopeTextBuffer.h"
#include <iostream>
using namespace std;

int main() {
    RopeTextBuffer buf;

    cout << "=== TEST INSERT ===" << endl;
    buf.insert("Hello");
    buf.insert(" World");
    cout << "Content: " << buf.getContent() << endl; // Expect: Hello World
    cout << "Cursor: " << buf.getCursorPos() << endl;

    cout << "\n=== TEST DELETE ===" << endl;
    buf.moveCursorTo(5);   // Move after "Hello"
    buf.deleteRange(1);    // Delete space
    cout << "Content: " << buf.getContent() << endl; // Expect: HelloWorld

    cout << "\n=== TEST REPLACE ===" << endl;
    buf.moveCursorTo(5);
    buf.replace(5, " Everyone");
    cout << "Content: " << buf.getContent() << endl; // Expect: Hello Everyone

    cout << "\n=== TEST MOVE CURSOR ===" << endl;
    buf.moveCursorTo(0);
    cout << "Cursor: " << buf.getCursorPos() << " (expect 0)" << endl;
    buf.moveCursorRight();
    cout << "Cursor: " << buf.getCursorPos() << " (expect 1)" << endl;
    buf.moveCursorLeft();
    cout << "Cursor: " << buf.getCursorPos() << " (expect 0)" << endl;

    cout << "\n=== TEST FIND ===" << endl;
    int pos = buf.findFirst('o');
    cout << "First 'o': " << pos << endl;
    int* allO = buf.findAll('o');
    cout << "All 'o': ";
    for (int i = 0; allO[i] != 0; i++) cout << allO[i] << " ";
    cout << endl;
    delete[] allO;

    cout << "\n=== TEST HISTORY (Undo/Redo) ===" << endl;
    buf.printHistory();
    cout << endl;

    cout << "Undo 1" << endl;
    buf.undo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Undo 2" << endl;
    buf.undo();
        cout << "Content: " << buf.getContent() << endl;

    cout << "Undo 3" << endl;
    buf.undo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Undo 4" << endl;
    buf.undo();
        cout << "Content: " << buf.getContent() << endl;

    cout << "Undo 5" << endl;
    buf.undo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Undo 6" << endl;
    buf.undo();
        cout << "Content: " << buf.getContent() << endl;

    cout << "Undo 7" << endl;
    buf.undo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Undo 8" << endl;
    buf.undo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Redo 1" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Redo 2" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;

 cout << "Redo 3" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Redo 4" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;


 cout << "Redo 5" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Redo 6" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;

 cout << "Redo 7" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "Redo 8" << endl;
    buf.redo();
    cout << "Content: " << buf.getContent() << endl;

    cout << "\n=== TEST MANY INSERTS (chunk size) ===" << endl;
    buf.moveCursorTo(buf.getContent().size());
    buf.insert(" 123456789ABCDEFG"); // > chunk size
    cout << "Content: " << buf.getContent() << endl;

    cout << "\n=== DONE ===" << endl;
    return 0;
}
