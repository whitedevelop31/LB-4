#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;


string addByteToString(string decimal, int byte) {
    int carry = byte;
    for (int i = (int)decimal.size() - 1; i >= 0; i--) {
        int current = (decimal[i] - '0') * 256 + carry;
        decimal[i] = (current % 10) + '0';
        carry = current / 10;
    }
    while (carry) {
        decimal.insert(0, 1, (carry % 10) + '0');
        carry /= 10;
    }
    return decimal.empty() ? "0" : decimal;
}


int divideBy256(string& decimal) {
    long long remainder = 0;
    string result = "";
    bool leadingZeros = true;
    for (char c : decimal) {
        long long current = remainder * 10 + (c - '0');
        int digit = (int)(current / 256);
        if (!(leadingZeros && digit == 0)) {
            result += to_string(digit);
            leadingZeros = false;
        }
        remainder = current % 256;
    }
    decimal = result.empty() ? "0" : result;
    return (int)remainder;
}

class EndianLibrary {
public:
    static string hexToDec(string hex, bool isLittleEndian) {
        if (hex.substr(0, 2) == "0x" || hex.substr(0, 2) == "0X") hex = hex.substr(2);
        if (hex.length() % 2 != 0) hex = "0" + hex;

        vector<uint8_t> bytes;
        for (unsigned int i = 0; i < hex.length(); i += 2) {
            bytes.push_back((uint8_t)strtol(hex.substr(i, 2).c_str(), NULL, 16));
        }

        if (isLittleEndian) reverse(bytes.begin(), bytes.end());

        string decimal = "0";
        for (uint8_t b : bytes) decimal = addByteToString(decimal, b);
        return decimal;
    }

    
    static string decToHex(string decimal, bool isLittleEndian, int expectedBytes) {
        vector<uint8_t> bytes;
        string tempDec = decimal;
        while (tempDec != "0") {
            bytes.push_back((uint8_t)divideBy256(tempDec));
        }

        
        while (bytes.size() < (size_t)expectedBytes) {
            bytes.push_back(0);
        }

        
        if (!isLittleEndian) reverse(bytes.begin(), bytes.end());

        stringstream ss;
        for (uint8_t b : bytes) {
            ss << hex << setfill('0') << setw(2) << (int)b;
        }
        return ss.str();
    }
};

int main() {
    setlocale(LC_ALL, "Ukrainian");
    int choice, nBytes;
    string input;

    while (true) {
        cout << "\n=== ÌÅÍÞ ËÀÁÎÐÀÒÎÐÍÎ¯ ÐÎÁÎÒÈ ¹4 ===" << endl;
        cout << "1. HEX -> Ö³ë³ ÷èñëà (Big & Little)" << endl;
        cout << "2. ×èñëî -> HEX (ÿê Little Endian)" << endl;
        cout << "3. ×èñëî -> HEX (ÿê Big Endian)" << endl;
        cout << "0. Âèõ³ä" << endl;
        cout << "Âèáåð³òü ä³þ: ";
        cin >> choice;

        if (choice == 0) break;

        cout << "Ââåä³òü çíà÷åííÿ: ";
        cin >> input;

        if (choice == 1) {
            cout << "Big-endian:    " << EndianLibrary::hexToDec(input, false) << endl;
            cout << "Little-endian: " << EndianLibrary::hexToDec(input, true) << endl;
        }
        else {
            cout << "Ââåä³òü ê³ëüê³ñòü áàéò (íàïðèêëàä, 32 àáî 4): ";
            cin >> nBytes;
            if (choice == 2)
                cout << "Ðåçóëüòàò HEX (LE): " << EndianLibrary::decToHex(input, true, nBytes) << endl;
            else
                cout << "Ðåçóëüòàò HEX (BE): " << EndianLibrary::decToHex(input, false, nBytes) << endl;
        }
    }
    return 0;
}