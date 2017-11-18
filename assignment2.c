#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// Some definitions
#define MAXBUFSIZE  1000
#define Max_Token_Size 100

typedef enum {
    KEYWORD, COMMA, SEMICOLON, ASSIGN,
    PERIOD, NUM, PLUS, MINUS,
    MUL, DIV, LBRACE, RBRACE, ID, NON
} TokenType;

// The Global variables
// File descriptor to the input file
int fd;

// The pointer pointing to the starting position
// for the current token recognizing
char *startPoint;

// The distance from starting_point
// for the current token recognizing
int forward;

// The token lexeme value
char tokenValue[Max_Token_Size];

bool isAlphabet(const char c) {
    char temp = c;
    if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z')) {
        return true;
    }
    return false;
}

bool isNumber(const char c) {
    char temp = c;
    if (temp >= '0' && temp <= '9') {
        return true;
    }
    return false;
}

bool isKeyword() {
    const char keywords[3][5] = {"var", "begin", "end"};
    // i should below the length of keywords
    for (int i = 0; i < sizeof (keywords) - 1; ++i) {
        if (strcmp(tokenValue, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// print_token will print the token based on its token type
//
// @param TokenType token can be
// KEYWORD*, COMMA, SEMICOLON, ASSIGN,
// PERIOD, NUM*, PLUS, MINUS,
// MUL, DIV, LBRACE, RBRACE, ID*, NON

void printToken(TokenType token) {
    switch (token) {
        case KEYWORD:
            printf("KEYWORD\t\t\t\t| %s\n", tokenValue);
            break;
        case COMMA:
            printf("COMMA\t\t\t\t|\n");
            break;
        case SEMICOLON:
            printf("SEMICOLON\t\t\t|\n");
            break;
        case ASSIGN:
            printf("ASSIGN\t\t\t\t|\n");
            break;
        case PERIOD:
            printf("PERIOD\t\t\t\t|\n");
            break;
        case NUM:
            printf("NUM\t\t\t\t| %s\n", tokenValue);
            break;
        case PLUS:
            printf("PLUS\t\t\t\t|\n");
            break;
        case MINUS:
            printf("MINUS\t\t\t\t|\n");
            break;
        case MUL:
            printf("MUL\t\t\t\t|\n");
            break;
        case DIV:
            printf("DIV\t\t\t\t|\n");
            break;
        case LBRACE:
            printf("LBRACE\t\t\t\t|\n");
            break;
        case RBRACE:
            printf("RBRACE\t\t\t\t|\n");
            break;
        case ID:
            printf("ID\t\t\t\t| %s\n", tokenValue);
            break;
        case NON:
            break;
    }
}

// getNextSyntax will move the token value
// @param int forward
// @return int nf

int getNextSyntax(const int f) {
    int i;
    for (i = 0; i < f; i++) {
        tokenValue[i] = startPoint[i];
    }
    tokenValue[i] = '\0';
    int nf = f - 1;
    return nf;
}

// get_next_token will iterate the char
//
// @return -1 if normal finish
// @return -2 if the char is illegal

int getNextToken() {
    char c;
    int currentState = 0;
    forward = 0;

    while (1) {
        c = startPoint[forward];
        switch (currentState) {
            case 0:
                if (isAlphabet(c)) {
                    forward++;
                    currentState = 1;
                } else if (isNumber(c)) {
                    forward++;
                    currentState = 3;
                } else {
                    currentState = 7;
                }
                break;

            case 1:
                if (isAlphabet(c) || isNumber(c)) {
                    forward++;
                    currentState = 1;
                } else {
                    currentState = 2;
                }
                break;

            case 2:
                forward = getNextSyntax(forward);
                return isKeyword() ? KEYWORD : ID;
                break;

            case 3:
                if (c == '.') {
                    forward++;
                    currentState = 4;
                } else if (isNumber(c)) {
                    forward++;
                    currentState = 3;
                } else {
                    forward = getNextSyntax(forward);
                    return NUM;
                }
                break;

            case 4:
            case 5:
                if (isNumber(c)) {
                    forward++;
                    currentState = 4;
                } else {
                    forward = getNextSyntax(forward);
                    return NUM;
                }
                break;

            case 7:
                if (
                        c == ' '
                        || c == '\t'
                        || c == '\n'
                        || c == '\v'
                        || c == '\f'
                        || c == '\r'
                        ) {
                    return NON;
                }

                if (c == ',') {
                    currentState = 8;
                    break;
                }

                if (c == ';') {
                    currentState = 9;
                    break;
                }

                if (c == '=') {
                    currentState = 10;
                    break;
                }

                if (c == '.') {
                    currentState = 11;
                    break;
                }

                if (c == '+') {
                    currentState = 12;
                    break;
                }

                if (c == '-') {
                    currentState = 13;
                    break;
                }

                if (c == '*') {
                    currentState = 14;
                    break;
                }

                if (c == '/') {
                    currentState = 15;
                    break;
                }

                if (c == '(') {
                    currentState = 16;
                    break;
                }

                if (c == ')') {
                    currentState = 17;
                    break;
                }

                if (c == '\0') {
                    currentState = 18;
                    break;
                } else {
                    currentState = 19;
                }
                break;

            case 8:
                return COMMA;
            case 9:
                return SEMICOLON;
            case 10:
                return ASSIGN;
            case 11:
                return PERIOD;
            case 12:
                return PLUS;
            case 13:
                return MINUS;
            case 14:
                return MUL;
            case 15:
                return DIV;
            case 16:
                return LBRACE;
            case 17:
                return RBRACE;
            case 18:
                return -1;
            case 19:
                return -2;
            default:
                printf("Wrong state (should not occur)\n");
                return -1;
        }
    }
}

int openFile(const char *filePath) {
    int fd;
    // Open the source file
    if ((fd = open(filePath, O_RDONLY)) < 0) {
        printf("Can not open the file (File_Name:%s).\n", filePath);
        return false;
    }
    return fd;
}

// parseFile will read the file
// @return 1 if file can read
// @return -1 if file cannot read

bool parseFile(const char *arg) {
    int fd, tokenName, rbytes;

    fd = openFile(arg);
    // Maximum buffer size
    char inputBuffer[MAXBUFSIZE];

    startPoint = inputBuffer;

    // Read the input file into a buffer
    // Inputed file cannot bigger than MAXBUFSIZE
    rbytes = read(fd, inputBuffer, MAXBUFSIZE);

    if (rbytes < 0) {
        printf("Could not read from the file\n");
        close(fd);
        return false;
    } else if (rbytes == 0) {
        printf("The input file is empty.\n");
        close(fd);
        return false;
    } else {
        // File ends with \0
        inputBuffer[rbytes - 1] = '\0';
    }

    printf("Token Name\t\t\t|Token Value \n");
    printf("--------------------------------------------\n");
    while ((tokenName = getNextToken()) >= 0) {
        printToken(tokenName);
        startPoint = startPoint + forward + 1;
    }

    if (tokenName == -1) printf("\nSuccessfully finish scan.\n");
    else printf("\nError: Non-recognizable character in the input file.\n");

    close(fd);
    return false;
}

bool main(int argc, char *argv[]) {
    // Check the number of input
    if (argc != 2) {
        printf("Usage: scan input_file \n");
        return false;
    }

    // Process lexical analysis
    return parseFile(argv[1]) > 0;
}
