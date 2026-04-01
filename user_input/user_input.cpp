#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc !=2){
        std::cout << "USAGE: ./user_input.exe <name>\n";
        return 1;
    }

    std::string name = argv[1];
    std::string answer;

    std::cout << "Hello " << name << ". Do you want to play a game?\n";
    std::cin >> answer;

    if (answer != "yes" && answer != "Yes" && answer != "no" && answer != "No") {
        std::cout << "Invalid input\n";
        return 2;
    }

    if (answer == "no" || answer == "No") {
        return 0;
    }

    std::cout << "Your Fleet\n";

    std::cout << "0 1 2 3 4 5 6 7 8 9 10\n";

    std::cout << "A \n";
    std::cout << "B \n";
    std::cout << "C \n";
    std::cout << "D \n";
    std::cout << "E \n";
    std::cout << "F \n";
    std::cout << "G \n";
    std::cout << "H \n";
    std::cout << "I \n";
    std::cout << "J \n";

    std::cout << "Enemy Waters\n";

    std::cout << "0 1 2 3 4 5 6 7 8 9 10\n";
    std::cout << "A \n";
    std::cout << "B \n";
    std::cout << "C \n";
    std::cout << "D \n";
    std::cout << "E \n";
    std::cout << "F \n";
    std::cout << "G \n";
    std::cout << "H \n";
    std::cout << "I \n";
    std::cout << "J \n";

    return 0;
}