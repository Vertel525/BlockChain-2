#include "hash.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

uint32_t rotateLeft(uint32_t value, int bits)
{
    return (value << bits) | (value >> (32 - bits));
}

string hashText(string text)
{
    uint32_t h[8] = {
        0x12345678, 0x23456789,
        0x34567890, 0x45678901,
        0x56789012, 0x67890123,
        0x78901234, 0x89012345
    };

    for (int i = 0; i < (int)text.length(); i++)
    {
        unsigned char c = (unsigned char)text[i];

        for (int j = 0; j < 8; j++)
        {
            h[j] = h[j] ^ (c + i + j * 17);
            h[j] = h[j] * 2654435761u;
            h[j] = rotateLeft(h[j], (j + 5) % 31 + 1);
            h[j] = h[j] ^ h[(j + 1) % 8];
        }
    }

    for (int j = 0; j < 8; j++)
    {
        h[j] = h[j] ^ (uint32_t)text.length();
        h[j] = h[j] * 2246822519u;
        h[j] = rotateLeft(h[j], 13);
    }

    stringstream result;

    for (int i = 0; i < 8; i++)
    {
        result << hex << setw(8) << setfill('0') << h[i];
    }

    return result.str();
}

string hashFile(string fileName)
{
    ifstream file(fileName, ios::binary);

    if (!file.is_open())
    {
        cout << "Nepavyko atidaryti failo!" << endl;
        return "";
    }

    string content;
    char c;

    while (file.get(c))
    {
        content += c;
    }

    file.close();

    return hashText(content);
}
