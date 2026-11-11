#include "blockchain.h"
#include "hash.h"
#include <climits>

bool validProof(const Block &block)
{
    if (block.difficulty < 0 || block.difficulty > 64)
    {
        return false;
    }

    string calculatedHash = calculateBlockHash(block);

    if (calculatedHash != block.hash)
    {
        return false;
    }

    string requiredZeros(block.difficulty, '0');
    return block.hash.substr(0, block.difficulty) == requiredZeros;
}

bool validateBlock(const Blockchain &blockchain, const Block &block)
{
    if (blockchain.chain.empty())
    {
        cout << "Grandineje nera Genesis bloko!" << endl;
        return false;
    }

    if (block.height != blockchain.chain.back().height + 1)
    {
        cout << "Neteisingas bloko aukstis!" << endl;
        return false;
    }

    if (block.prevHash != blockchain.chain.back().hash)
    {
        cout << "Neteisingas prevHash!" << endl;
        return false;
    }

    if (block.version != 1 || block.difficulty != 3)
    {
        cout << "Neteisinga bloko versija arba sudetingumas!" << endl;
        return false;
    }

    if (block.transactions.empty() || block.transactions.size() > 100)
    {
        cout << "Neteisingas transakciju kiekis!" << endl;
        return false;
    }

    if (block.txRoot != calculateTransactionRoot(block.transactions, 1))
    {
        cout << "Neteisinga transakciju saknis!" << endl;
        return false;
    }

    if (!validProof(block))
    {
        cout << "Neteisingas darbo irodymas!" << endl;
        return false;
    }

    vector<long long> tempBalances;

    for (int i = 0; i < (int)blockchain.users.size(); i++)
    {
        tempBalances.push_back(blockchain.users[i].balance);
    }

    vector<string> usedIds;

    for (int i = 0; i < (int)block.transactions.size(); i++)
    {
        const Transaction &transaction = block.transactions[i];

        int sender = findUserIndex(blockchain, transaction.sender);
        int receiver = findUserIndex(blockchain, transaction.receiver);

        if (sender == -1 || receiver == -1)
        {
            cout << "Nezinomas transakcijos dalyvis!" << endl;
            return false;
        }

        if (sender == receiver || transaction.amount <= 0)
        {
            cout << "Netinkama transakcija!" << endl;
            return false;
        }

        if (transaction.transactionId != calculateTransactionId(transaction))
        {
            cout << "Neteisingas transakcijos ID!" << endl;
            return false;
        }

        if (transactionExistsInChain(blockchain, transaction.transactionId))
        {
            cout << "Transakcija jau patvirtinta!" << endl;
            return false;
        }

        for (int j = 0; j < (int)usedIds.size(); j++)
        {
            if (usedIds[j] == transaction.transactionId)
            {
                cout << "Pasikartojusi transakcija bloke!" << endl;
                return false;
            }
        }

        if (tempBalances[sender] < transaction.amount)
        {
            cout << "Nepakanka lesu!" << endl;
            return false;
        }

        if (tempBalances[receiver] > LLONG_MAX - transaction.amount)
        {
            cout << "Balanso perpildymas!" << endl;
            return false;
        }

        tempBalances[sender] -= transaction.amount;
        tempBalances[receiver] += transaction.amount;
        usedIds.push_back(transaction.transactionId);
    }

    return true;
}
