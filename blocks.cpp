
#include "blockchain.h"
#include "hash.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <climits>

using namespace std;

int hexValue(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return -1;
}

string hexToBytes(const string& hex)
{
    if (hex.length() % 2 != 0)
    {
        return "";
    }

    string bytes;

    for (int i = 0; i < (int)hex.length(); i += 2)
    {
        int high = hexValue(hex[i]);
        int low = hexValue(hex[i + 1]);

        if (high == -1 || low == -1)
        {
            return "";
        }

        char byte = (char)(high * 16 + low);
        bytes.push_back(byte);
    }

    return bytes;
}

string calculateTransactionRoot(
    const vector<Transaction>& transactions,
    int version)
{
    if (transactions.empty())
    {
        return hashText("");
    }

    vector<string> hashes;

    for (int i = 0; i < (int)transactions.size(); i++)
    {
        hashes.push_back(transactions[i].transactionId);
    }

    while (hashes.size() > 1)
    {
        vector<string> newHashes;

        for (int i = 0; i < (int)hashes.size(); i += 2)
        {
            string left = hexToBytes(hashes[i]);
            string right;

            if (i + 1 < (int)hashes.size())
            {
                right = hexToBytes(hashes[i + 1]);
            }
            else
            {
                right = left;
            }

            if (left.empty() || right.empty())
            {
                return "";
            }

            newHashes.push_back(hashText(left + right));
        }

        hashes = newHashes;
    }

    return hashes[0];
}

string calculateBlockHash(const Block& block)
{
    string data;

    data = to_string(block.height) + "|" +
        block.prevHash + "|" +
        to_string(block.timestamp) + "|" +
        to_string(block.version) + "|" +
        block.txRoot + "|" +
        to_string(block.nonce) + "|" +
        to_string(block.difficulty);

    return hashText(data);
}

bool validProof(const Block& block)
{
    string required(block.difficulty, '0');

    return block.hash.substr(0, block.difficulty) == required;
}

bool mineBlock(Block& block, unsigned long long maxAttempts)
{
    for (unsigned long long i = 0; i < maxAttempts; i++)
    {
        block.hash = calculateBlockHash(block);

        if (validProof(block))
        {
            return true;
        }

        block.nonce++;
    }

    return false;
}

void createGenesisBlock(Blockchain& blockchain)
{
    Block genesis;

    genesis.height = 0;
    genesis.prevHash = "0";
    genesis.timestamp = chrono::duration_cast<chrono::seconds>(
        chrono::system_clock::now().time_since_epoch()).count();

    genesis.version = 1;
    genesis.txRoot = hashText("GENESIS");
    genesis.nonce = 0;
    genesis.difficulty = 3;

    mineBlock(genesis, 1000000);

    blockchain.chain.push_back(genesis);
}

void processTransactions(Blockchain& blockchain)
{
    const int candidateCount = 5;
    const int maxTransactions = 100;
    const unsigned long long initialMiningAttempts = 100000;
    const unsigned long long maxMiningAttempts = 1000000;

    blockchain.acceptedTransactions = 0;
    int rejectedBeforeMining = blockchain.rejectedTransactions;

    while (!blockchain.pool.empty())
    {
        vector<Block> candidates;

        int poolSize = (int)blockchain.pool.size();

        for (int candidateNumber = 0;
            candidateNumber < candidateCount;
            candidateNumber++)
        {
            Block block;

            block.height = (int)blockchain.chain.size();
            block.prevHash = blockchain.chain.back().hash;

            block.timestamp = chrono::duration_cast<chrono::seconds>(
                chrono::system_clock::now().time_since_epoch()).count();

            block.version = 1;
            block.nonce = 0;
            block.difficulty = 3;

            vector<long long> balances;

            for (int i = 0; i < (int)blockchain.users.size(); i++)
            {
                balances.push_back(blockchain.users[i].balance);
            }

            vector<Transaction> selectedTransactions;

            int startIndex = (candidateNumber * 100) % poolSize;

            for (int i = 0; i < poolSize; i++)
            {
                if ((int)selectedTransactions.size() >= maxTransactions)
                {
                    break;
                }

                int transactionIndex = (startIndex + i) % poolSize;

                Transaction transaction =
                    blockchain.pool[transactionIndex];

                int sender = findUserIndex(
                    blockchain, transaction.sender);

                int receiver = findUserIndex(
                    blockchain, transaction.receiver);

                if (sender == -1 || receiver == -1 || sender == receiver)
                {
                    continue;
                }

                if (transaction.amount <= 0 ||
                    transaction.sequence <= 0)
                {
                    continue;
                }

                if (transaction.transactionId !=
                    calculateTransactionId(transaction))
                {
                    continue;
                }

                if (transactionExistsInChain(
                    blockchain, transaction.transactionId))
                {
                    continue;
                }

                if (transactionExistsInList(
                    selectedTransactions, transaction.transactionId))
                {
                    continue;
                }

                if (balances[sender] < transaction.amount)
                {
                    continue;
                }

                if (balances[receiver] > LLONG_MAX - transaction.amount)
                {
                    continue;
                }

                balances[sender] -= transaction.amount;
                balances[receiver] += transaction.amount;

                selectedTransactions.push_back(transaction);
            }

            if (selectedTransactions.empty())
            {
                continue;
            }

            block.transactions = selectedTransactions;

            block.txRoot = calculateTransactionRoot(
                block.transactions, block.version);

            unsigned long long attemptsUsed = 0;
            unsigned long long currentBudget = initialMiningAttempts;

            bool mined = false;

            while (attemptsUsed < maxMiningAttempts)
            {
                unsigned long long remaining =
                    maxMiningAttempts - attemptsUsed;

                unsigned long long attemptsThisRound =
                    currentBudget < remaining ? currentBudget : remaining;

                if (mineBlock(block, attemptsThisRound))
                {
                    mined = true;
                    break;
                }

                attemptsUsed += attemptsThisRound;

                currentBudget *= 2;
            }

            if (mined)
            {
                candidates.push_back(block);
            }
        }

        if (candidates.empty())
        {
            cout << "Nepavyko iskasti nei vieno kandidatinio bloko!"
                << endl;
            break;
        }

        int bestIndex = 0;

        for (int i = 1; i < (int)candidates.size(); i++)
        {
            if (candidates[i].transactions.size() >
                candidates[bestIndex].transactions.size())
            {
                bestIndex = i;
            }
            else if (
                candidates[i].transactions.size() ==
                candidates[bestIndex].transactions.size() &&
                candidates[i].hash < candidates[bestIndex].hash)
            {
                bestIndex = i;
            }
        }

        Block bestBlock = candidates[bestIndex];

        if (!validateBlock(blockchain, bestBlock))
        {
            cout << "Laimetojo bloko patikrinimas nepavyko!"
                << endl;
            break;
        }

        for (int i = 0; i < (int)bestBlock.transactions.size(); i++)
        {
            const Transaction& transaction = bestBlock.transactions[i];

            int sender = findUserIndex(
                blockchain, transaction.sender);

            int receiver = findUserIndex(
                blockchain, transaction.receiver);

            blockchain.users[sender].balance -= transaction.amount;
            blockchain.users[receiver].balance += transaction.amount;

            blockchain.acceptedTransactions++;
        }

        blockchain.chain.push_back(bestBlock);

        for (int i = 0; i < (int)bestBlock.transactions.size(); i++)
        {
            string id = bestBlock.transactions[i].transactionId;

            for (int j = 0; j < (int)blockchain.pool.size(); j++)
            {
                if (blockchain.pool[j].transactionId == id)
                {
                    blockchain.pool.erase(
                        blockchain.pool.begin() + j);
                    break;
                }
            }
        }

        cout << "\nSukurtas naujas blokas" << endl;
        cout << "Aukstis: " << bestBlock.height << endl;
        cout << "Patvirtintos transakcijos: "
            << bestBlock.transactions.size() << endl;
    }

    blockchain.rejectedTransactions = rejectedBeforeMining;
    blockchain.remainingTransactions = (int)blockchain.pool.size();
}

void printSummary(const Blockchain& blockchain)
{
    cout << "\n========== BLOCKCHAIN REZULTATAI ==========" << endl;

    cout << "Bloku skaicius: "
        << blockchain.chain.size() << endl;

    cout << "Patvirtintos transakcijos: "
        << blockchain.acceptedTransactions << endl;

    cout << "Atmestos transakcijos: "
        << blockchain.rejectedTransactions << endl;

    cout << "Likusios nepatvirtintos transakcijos: "
        << blockchain.remainingTransactions << endl;

    long long totalMoney = 0;

    for (int i = 0; i < (int)blockchain.users.size(); i++)
    {
        totalMoney += blockchain.users[i].balance;
    }

    cout << "Pradine pinigu suma: "
        << blockchain.initialTotalMoney << endl;

    cout << "Dabartine pinigu suma: "
        << totalMoney << endl;

}
