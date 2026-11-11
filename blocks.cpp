#include "blockchain.h"
#include "hash.h"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <climits>

string calculateTransactionRoot(const vector<Transaction> &transactions, int version)
{
    // v0.1: paprastas transakciju ID sujungimas.
    // Tikras Merkle medis bus idiegtas atskirame v0.2 etape.
    string data = "";

    for (int i = 0; i < (int)transactions.size(); i++)
    {
        if (i > 0)
        {
            data += "|";
        }

        data += transactions[i].transactionId;
    }

    return hashText(data);
}

string calculateBlockHash(const Block &block)
{
    string data =
        to_string(block.height) + "|" +
        block.prevHash + "|" +
        to_string(block.timestamp) + "|" +
        to_string(block.version) + "|" +
        block.txRoot + "|" +
        to_string(block.nonce) + "|" +
        to_string(block.difficulty);

    return hashText(data);
}

void createGenesisBlock(Blockchain &blockchain)
{
    Block genesis;

    genesis.height = 0;
    genesis.prevHash = string(64, '0');
    genesis.timestamp = 0;
    genesis.version = 1;
    genesis.txRoot = hashText("");
    genesis.nonce = 0;
    genesis.difficulty = 3;
    genesis.hash = calculateBlockHash(genesis);

    blockchain.chain.push_back(genesis);

    cout << "\\nSukurtas Genesis blokas!" << endl;
    cout << "Genesis hash: " << genesis.hash << endl;
}

bool mineBlock(Block &block, unsigned long long maxAttempts)
{
    if (block.difficulty < 0 || block.difficulty > 64)
    {
        return false;
    }

    string requiredZeros(block.difficulty, '0');

    for (unsigned long long i = 0; i < maxAttempts; i++)
    {
        block.nonce = i;
        block.hash = calculateBlockHash(block);

        if (block.hash.substr(0, block.difficulty) == requiredZeros)
        {
            return true;
        }
    }

    return false;
}

void processTransactions(Blockchain &blockchain)
{
    const unsigned long long maxAttempts = 1000000;

    while (!blockchain.pool.empty())
    {
        vector<Transaction> selectedTransactions;
        vector<long long> tempBalances;

        for (int i = 0; i < (int)blockchain.users.size(); i++)
        {
            tempBalances.push_back(blockchain.users[i].balance);
        }

        vector<int> order;

        for (int i = 0; i < (int)blockchain.pool.size(); i++)
        {
            order.push_back(i);
        }

        for (int i = 0; i < (int)order.size(); i++)
        {
            int j = rand() % (int)order.size();
            int temp = order[i];
            order[i] = order[j];
            order[j] = temp;
        }

        vector<bool> removeFromPool(blockchain.pool.size(), false);

        for (int i = 0; i < (int)order.size(); i++)
        {
            int index = order[i];
            Transaction transaction = blockchain.pool[index];

            int sender = findUserIndex(blockchain, transaction.sender);
            int receiver = findUserIndex(blockchain, transaction.receiver);

            if (sender == -1 || receiver == -1 || sender == receiver ||
                transaction.amount <= 0)
            {
                removeFromPool[index] = true;
                blockchain.rejectedTransactions++;
                continue;
            }

            if (transaction.transactionId != calculateTransactionId(transaction) ||
                transactionExistsInChain(blockchain, transaction.transactionId) ||
                transactionExistsInList(selectedTransactions, transaction.transactionId))
            {
                removeFromPool[index] = true;
                blockchain.rejectedTransactions++;
                continue;
            }

            if (tempBalances[sender] < transaction.amount)
            {
                // Siuntejui siuo metu truksta lesu. Paliekame ja fonde,
                // nes velesniame bloke balansas dar gali pasikeisti.
                continue;
            }

            if (tempBalances[receiver] > LLONG_MAX - transaction.amount)
            {
                removeFromPool[index] = true;
                blockchain.rejectedTransactions++;
                continue;
            }

            tempBalances[sender] -= transaction.amount;
            tempBalances[receiver] += transaction.amount;
            selectedTransactions.push_back(transaction);
            removeFromPool[index] = true;

            if (selectedTransactions.size() == 100)
            {
                break;
            }
        }

        if (selectedTransactions.empty())
        {
            cout << "\\nNebuvo daugiau tinkamu transakciju. Likusios transakcijos atmestos." << endl;
            blockchain.rejectedTransactions += (int)blockchain.pool.size();
            blockchain.pool.clear();
            break;
        }

        Block block;

        block.height = (int)blockchain.chain.size();
        block.prevHash = blockchain.chain.back().hash;
        block.timestamp = time(nullptr);
        block.version = 1;
        block.transactions = selectedTransactions;
        block.txRoot = calculateTransactionRoot(block.transactions, 1);
        block.nonce = 0;
        block.difficulty = 3;

        cout << "\\nKasamas blokas #" << block.height << "..." << endl;

        auto start = chrono::steady_clock::now();
        bool mined = mineBlock(block, maxAttempts);
        auto end = chrono::steady_clock::now();

        long long miningTime =
            chrono::duration_cast<chrono::milliseconds>(end - start).count();

        if (!mined)
        {
            cout << "Nepavyko iskasti bloko per nustatyta bandymu skaiciu." << endl;
            break;
        }

        if (!validateBlock(blockchain, block))
        {
            cout << "Blokas atmestas!" << endl;
            break;
        }

        // Balansus pakeiciame tik po sekmingo bloko patikrinimo.
        for (int i = 0; i < (int)block.transactions.size(); i++)
        {
            const Transaction &transaction = block.transactions[i];
            int sender = findUserIndex(blockchain, transaction.sender);
            int receiver = findUserIndex(blockchain, transaction.receiver);

            blockchain.users[sender].balance -= transaction.amount;
            blockchain.users[receiver].balance += transaction.amount;
        }

        blockchain.chain.push_back(block);
        blockchain.acceptedTransactions += (int)block.transactions.size();

        vector<Transaction> remainingTransactions;

        for (int i = 0; i < (int)blockchain.pool.size(); i++)
        {
            if (!removeFromPool[i])
            {
                remainingTransactions.push_back(blockchain.pool[i]);
            }
        }

        blockchain.pool = remainingTransactions;

        cout << "Blokas sekmingai priimtas!" << endl;
        cout << "Aukstis: " << block.height << endl;
        cout << "Transakciju kiekis: " << block.transactions.size() << endl;
        cout << "Block hash: " << block.hash << endl;
        cout << "Nonce: " << block.nonce << endl;
        cout << "Bandymu skaicius: " << block.nonce + 1 << endl;
        cout << "Kasimo laikas: " << miningTime << " ms" << endl;
    }
}

void printSummary(const Blockchain &blockchain)
{
    long long finalTotalMoney = 0;

    for (int i = 0; i < (int)blockchain.users.size(); i++)
    {
        finalTotalMoney += blockchain.users[i].balance;
    }

    cout << "\\n========== GALUTINE SUVESTINE ==========" << endl;
    cout << "Vartotoju skaicius: " << blockchain.users.size() << endl;
    cout << "Grandines bloku skaicius (be Genesis): "
         << blockchain.chain.size() - 1 << endl;
    cout << "Patvirtintu transakciju: " << blockchain.acceptedTransactions << endl;
    cout << "Atmestu transakciju: " << blockchain.rejectedTransactions << endl;
    cout << "Likusiu transakciju fonde: " << blockchain.pool.size() << endl;
    cout << "Pradine pinigu suma: " << blockchain.initialTotalMoney << endl;
    cout << "Galutine pinigu suma: " << finalTotalMoney << endl;

    if (blockchain.initialTotalMoney == finalTotalMoney)
    {
        cout << "Pinigu suma nepasikeite!" << endl;
    }
    else
    {
        cout << "KLAIDA: pinigu suma pasikeite!" << endl;
    }
}
