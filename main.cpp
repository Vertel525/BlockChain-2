#include <iostream>
#include <cstdlib>
#include "blockchain.h"
#include "hash.h"

using namespace std;

int main()
{
    srand(12345);

    Blockchain blockchain;

    cout << "Generuojami vartotojai..." << endl;

    generateUsers(blockchain, 1000);

    cout << "Sugeneruota vartotoju: "
        << blockchain.users.size() << endl;

    cout << "\nGeneruojamos transakcijos..." << endl;

    generateTransactions(blockchain, 10000);

    cout << "Sugeneruota transakciju: "
        << blockchain.pool.size() << endl;

    cout << "\nPradine bendra pinigu suma: "
        << blockchain.initialTotalMoney << endl;

    createGenesisBlock(blockchain);

    processTransactions(blockchain);

    printSummary(blockchain);

    return 0;
}
