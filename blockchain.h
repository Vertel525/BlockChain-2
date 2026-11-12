
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct User
{
    string name;
    string publicKey;
    long long initialBalance;
    long long balance;
};

struct Transaction
{
    string sender;
    string receiver;
    long long amount;
    int sequence;
    string transactionId;
};

struct Block
{
    int height;
    string prevHash;
    long long timestamp;
    int version;
    string txRoot;
    unsigned long long nonce;
    int difficulty;
    string hash;
    unsigned long long miningAttempts;
    double miningTime;
    vector<Transaction> transactions;
};

struct Blockchain
{
    vector<User> users;
    vector<Transaction> pool;
    vector<Block> chain;
    unordered_map<string, int> userIndexes;

    long long initialTotalMoney = 0;
    int acceptedTransactions = 0;
    int rejectedTransactions = 0;
    int remainingTransactions = 0;
};

void generateUsers(Blockchain& blockchain, int count);
int findUserIndex(const Blockchain& blockchain, string key);

void generateTransactions(Blockchain& blockchain, int count);
bool validateTransaction(const Blockchain& blockchain,
    const Transaction& transaction);
string transactionToString(const Transaction& transaction);
string calculateTransactionId(const Transaction& transaction);

bool transactionExistsInChain(const Blockchain& blockchain, string id);
bool transactionExistsInList(const vector<Transaction>& transactions, string id);

string calculateTransactionRoot(const vector<Transaction>& transactions, int version);
string calculateBlockHash(const Block& block);

void createGenesisBlock(Blockchain& blockchain);
bool mineBlock(Block& block, unsigned long long maxAttempts);
void processTransactions(Blockchain& blockchain);
void printSummary(const Blockchain& blockchain);

bool validProof(const Block& block);
bool validateBlock(const Blockchain& blockchain, const Block& block);

#endif
