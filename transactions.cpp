#include "blockchain.h"
#include "hash.h"
#include <cstdlib>

string transactionToString(const Transaction &transaction)
{
    return transaction.sender + "|" +
           transaction.receiver + "|" +
           to_string(transaction.amount) + "|" +
           to_string(transaction.sequence);
}

string calculateTransactionId(const Transaction &transaction)
{
    return hashText(transactionToString(transaction));
}

void generateTransactions(Blockchain &blockchain, int count)
{
    blockchain.pool.clear();
    blockchain.acceptedTransactions = 0;
    blockchain.rejectedTransactions = 0;

    if (blockchain.users.size() < 2)
    {
        cout << "Nepakanka vartotoju transakcijoms kurti!" << endl;
        return;
    }

    for (int i = 0; i < count; i++)
    {
        Transaction transaction;

        int senderNumber = rand() % (int)blockchain.users.size();
        int receiverNumber = rand() % (int)blockchain.users.size();

        while (senderNumber == receiverNumber)
        {
            receiverNumber = rand() % (int)blockchain.users.size();
        }

        transaction.sender = blockchain.users[senderNumber].publicKey;
        transaction.receiver = blockchain.users[receiverNumber].publicKey;
        transaction.amount = 1 + rand() % 10000;
        transaction.sequence = i + 1;
        transaction.transactionId = calculateTransactionId(transaction);

        blockchain.pool.push_back(transaction);
    }
}

bool transactionExistsInChain(const Blockchain &blockchain, string id)
{
    for (int i = 0; i < (int)blockchain.chain.size(); i++)
    {
        for (int j = 0; j < (int)blockchain.chain[i].transactions.size(); j++)
        {
            if (blockchain.chain[i].transactions[j].transactionId == id)
            {
                return true;
            }
        }
    }

    return false;
}

bool transactionExistsInList(const vector<Transaction> &transactions, string id)
{
    for (int i = 0; i < (int)transactions.size(); i++)
    {
        if (transactions[i].transactionId == id)
        {
            return true;
        }
    }

    return false;
}
