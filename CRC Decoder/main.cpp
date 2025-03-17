//
//  main.cpp
//  CRC Decoder
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

bool checkCRC(string received, string generator) {
    string remainder = mod2div(received, generator);
    return remainder.find('1') == string::npos;
}

int main() {
    string received_bits, generator_bits;
    cout << "Enter received bits: ";
    cin >> received_bits;
    cout << "Enter generator bits: ";
    cin >> generator_bits;
    
    if (checkCRC(received_bits, generator_bits)) {
        cout << "OK" << endl;
    } else {
        cout << "ERROR" << endl;
    }
    
    return 0;
}
