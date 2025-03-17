//
//  main.cpp
//  Bit Permeter
//
//  Created by Lance Cole on 3/10/25.
//

#include <iostream>
#include <cstdlib>
using namespace std;

string xorOperation(string a, string b) {
    string result = "";
    for (size_t i = 1; i < b.length(); i++) {
        result += (a[i] == b[i]) ? '0' : '1';
    }
    return result;
}

string mod2div(string dividend, string divisor) {
    int pick = divisor.length();
    string tmp = dividend.substr(0, pick);
    
    while (pick < dividend.length()) {
        if (tmp[0] == '1') {
            tmp = xorOperation(divisor, tmp) + dividend[pick];
        } else {
            tmp = xorOperation(string(pick, '0'), tmp) + dividend[pick];
        }
        pick++;
    }
    
    if (tmp[0] == '1') {
        tmp = xorOperation(divisor, tmp);
    } else {
        tmp = xorOperation(string(pick, '0'), tmp);
    }
    
    return tmp;
}

pair<string, string> encodeData(string data, string generator) {
    string appended_data = data + string(generator.length() - 1, '0');
    string remainder = mod2div(appended_data, generator);
    return {data + remainder, generator};
}

void flipBit(string &data, int bitPos) {
    if (bitPos > 0 && bitPos <= data.length()) {
        data[bitPos - 1] = (data[bitPos - 1] == '0') ? '1' : '0';
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./flipbit <bit_position>" << endl;
        return 1;
    }
    
    int bitPosition = atoi(argv[1]);
    string data_bits, generator_bits;
    cout << "Enter data bits: ";
    cin >> data_bits;
    cout << "Enter generator bits: ";
    cin >> generator_bits;
    
    if (bitPosition != 0) {
        flipBit(data_bits, bitPosition);
    }
    
    pair<string, string> encoded = encodeData(data_bits, generator_bits);
    cout << encoded.first << endl; // Print modified data bits followed by CRC bits
    cout << endl; // Print a newline
    cout << encoded.second << endl; // Print generator bits
    
    return 0;
}
