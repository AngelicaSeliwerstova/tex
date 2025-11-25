#include <fstream>
#include <iostream>
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Use: \n\t" << argv[0] << " source destination\n";
        return 0;
    }
    const char* inputfilename = argv[1];
    const char* outputfilename = argv[2];

    std::ifstream inFile;
    inFile.open(inputfilename, std::ios_base::binary);
    if (!inFile) {
        std::cout << "Can not open file '" << inputfilename << "'\n";
        return 1;
    }

    std::ofstream outFile; 
    outFile.open(outputfilename, std::ios_base::binary);
    if (!outFile) {
        std::cout << "Can not open file '" << outputfilename << "'\n";
        inFile.close();
        return 2;
    }
    enum class States { outmath, inmath, possiblestartmath, possiblestopmath };
    States state = States::outmath;
    const char* math="<math>";
    const char* closemath="</math>";
    int ch;
    int tmp;
    while ( (ch = inFile.get()) != EOF ) {
        if (ch == '$') {
            switch (state) {
            case States::outmath:
                state = States::possiblestartmath;
                break;
            case States::inmath:
                state = States::possiblestopmath;
                break;
            case States::possiblestartmath:
                outFile << math;
                state = States::inmath;
                break;
            case States::possiblestopmath:
                outFile << closemath;
                state = States::outmath;
                break;
            }
        }
        else {
            switch (state) {
            case States::possiblestartmath:
                outFile << math;
                state = States::inmath;
                break;
            case States::possiblestopmath:
                outFile << closemath;
                state = States::outmath;
                break;
            }
            outFile.put(ch);
            if (ch == '\\') {
                if (EOF != (ch = inFile.get())) {
                    outFile.put(ch);
                }
            }
        }
    }
    if (state==States::possiblestopmath || state==States::inmath){
        outFile<<closemath;
    }
    inFile.close();
    outFile.close();
    return 0;
 }
