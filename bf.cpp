#include <fstream>
#include <iostream>
#include <stack>
#include <string>

#define ARRAY_SIZE 30000
#define PRINT_SIZE 20

int main(int argc, char** argv) {

    if (argc == 1) {
        std::cerr << "No file given";
    }

    // Read file to f_buf
    std::ifstream fin (argv[1], std::ifstream::in);
    std::filebuf* pbuf = fin.rdbuf();
    std::size_t size = pbuf->pubseekoff(0, fin.end, fin.in);
    pbuf->pubseekpos(0, fin.in);
    char* f_buf = new char[size];
    pbuf->sgetn(f_buf, size);
    fin.close();

    unsigned char* tape = new unsigned char[ARRAY_SIZE];
    unsigned char* t_ptr = tape;

    std::stack<char*> brackets;

    for (char* f_ptr = f_buf; *f_ptr != EOF; f_ptr++) {
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
                    f_ptr++;
                    for (int n = 1; n > 0; f_ptr++) {
                        if (*f_ptr == '[') n++;
                        else if (*f_ptr == ']') n--;
                        else if (*f_ptr == EOF) {
                            std::cerr << "Syntax error: unmatched [\n";
                            goto QUIT;
                        }
                    }
                } else {
                    brackets.push(f_ptr);
                }
                break;
            case ']':
                if (*t_ptr != 0) {
                    if (brackets.size() == 0) {
                        std::cerr << "Syntax error: unmatched ]\n";
                        goto QUIT;
                    }
                    f_ptr = brackets.top();
                } else {
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
                for (int i = 0; i < PRINT_SIZE; i++) {
                    std::cout << "[" << (int) tape[i] << "]";
                    if (t_ptr - tape == i) {
                        std::cout << "\t<---";
                    }
                    std::cout << "\n";
                }
                std::cout << "\n";
                break;
            case '\0':
                goto QUIT;
        }
    }

QUIT:
    delete[] f_buf;
    delete[] tape;
}
