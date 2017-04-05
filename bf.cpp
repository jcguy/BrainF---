#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>

#define ARRAY_SIZE 30000
#define PRINT_SIZE 20

int main(int argc, char** argv) {

    if (argc == 1) {
        std::cerr << "No file given";
    }

    // Determine the size of the file
    std::ifstream fin (argv[1], std::ifstream::in);
    std::filebuf* pbuf = fin.rdbuf();
    std::size_t size = pbuf->pubseekoff(0, fin.end, fin.in);
    pbuf->pubseekpos(0, fin.in);

    // Copy the file to an array
    char* f_buf = new char[size];
    pbuf->sgetn(f_buf, size);
    fin.close();

    // Set up the tape
    unsigned char* tape = new unsigned char[ARRAY_SIZE];
    unsigned char* t_ptr = tape;

    std::fill_n(tape, ARRAY_SIZE, 0);

    std::stack<char*> brackets;

    for (char* f_ptr = f_buf; *f_ptr; f_ptr++) {
        switch (*f_ptr) {
            case '+':
                (*t_ptr)++;
                break;

            case '-':
                (*t_ptr)--;
                break;

            case '>':
                t_ptr++;
                break;

            case '<':
                t_ptr--;
                break;

            case '[':
                if (*t_ptr == 0) {
                    /* Skip to the matching ']' by keeping
                     * track of how many '['s we've seen so
                     * far, and stopping when we've seen
                     * just as many ']'s
                     */
                    
                    // We've seen one so far
                    f_ptr++;
                    for (int n = 1; n > 0; f_ptr++) {
                        if (*f_ptr == '[') n++;
                        else if (*f_ptr == ']') n--;
                        else if (!(*f_ptr)) {
                            // We've reached the end of the file
                            std::cerr << "Syntax error: unmatched [\n";
                            goto QUIT;
                        }
                    }
                } else {
                    /* Remember this location so we can jump
                     * back to it when we reach the matching ']'
                     */
                    brackets.push(f_ptr);
                }
                break;

            case ']':
                if (*t_ptr != 0) {
                    // Attempt to jump back to the matching '['
                    if (brackets.size() == 0) {
                        std::cerr << "Syntax error: unmatched ]\n";
                        goto QUIT;
                    }
                    f_ptr = brackets.top();
                } else {
                    // Forget about the matching '['
                    brackets.pop();
                }
                break;

            case '.':
                std::cout << *t_ptr;
                break;

            case ',':
                std::cin >> *t_ptr;
                break;

            case '#':
                /* Prints out the first PRINT_SIZE cells in the
                 * tape, vertically, with an arrow pointing to
                 * the current location of the pointer.
                 */
                for (int i = 0; i < PRINT_SIZE; i++) {
                    std::cout << "[" << (int) tape[i] << "]";
                    if (t_ptr - tape == i) {
                        std::cout << "\t<---";
                    }
                    std::cout << "\n";
                }
                std::cout << "\n";
                break;
        }
    }

QUIT:
    delete[] f_buf;
    delete[] tape;
}
