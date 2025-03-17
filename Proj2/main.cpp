//
//  main.cpp
//  Proj2
//
//  Created by Lance Cole on 3/10/25.
//

#include <iostream>
using namespace std;

//xor operations
string xorOperation(string a, string b) {
    string result = "";
    for (size_t i = 1; i < b.length(); i++) {
        result += (a[i] == b[i]) ? '0' : '1';
    }
    return result;
}

//Bit division
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

int main() {
    string data_bits, generator_bits;
    cout << "Enter data bits: ";
    cin >> data_bits;
    cout << "Enter generator bits: ";
    cin >> generator_bits;
    
    pair<string, string> encoded = encodeData(data_bits, generator_bits);
    cout << encoded.first << endl; // Print data bits followed by CRC bits
    cout << endl; // Print a newline
    cout << encoded.second << endl; // Print generator bits
    
    return 0;
}
