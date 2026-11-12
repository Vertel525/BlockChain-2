#include "blockchain.h"
#include "hash.h"
#include <cstdlib>

long long randomNumber(long long min, long long max)
{
    unsigned long long number =
        (unsigned long long)rand() * ((unsigned long long)RAND_MAX + 1) + rand();

    return min + (long long)(number % (unsigned long long)(max - min + 1));
}

void generateUsers(Blockchain& blockchain, int count)
{
    string names[10] = {
        "Jonas", "Petras", "Tomas", "Lukas", "Matas",
        "Ieva", "Ona", "Greta", "Laura", "Aiste"
    };

    string surnames[10] = {
        "Jonaitis", "Petraitis", "Kazlauskas", "Jankauskas",
        "Stankevicius", "Jonaitiene", "Petrauskiene",
        "Kazlauskaite", "Jankauskaite", "Stankeviciute"
    };

    blockchain.users.clear();
    blockchain.initialTotalMoney = 0;

    for (int i = 0; i < count; i++)
    {
        User user;

        user.name = names[rand() % 10] + " " + surnames[rand() % 10];

        user.publicKey = hashText("USER|" + to_string(i + 1))
            + to_string(i + 1);

        user.initialBalance = randomNumber(100, 1000000);
        user.balance = user.initialBalance;

        blockchain.initialTotalMoney += user.initialBalance;

        blockchain.users.push_back(user);
    }
}

int findUserIndex(const Blockchain& blockchain, string key)
{
    for (int i = 0; i < (int)blockchain.users.size(); i++)
    {
        if (blockchain.users[i].publicKey == key)
        {
            return i;
        }
    }

    return -1;
}
