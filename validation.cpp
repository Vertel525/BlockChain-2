#include "blockchain.h"
#include "hash.h"
#include <climits>
#include <unordered_set>

using namespace std;

bool validateBlock(const Blockchain& blockchain, const Block& block)
{
    if (blockchain.chain.empty())
    {
        return false;
    }

    const Block& previousBlock = blockchain.chain.back();

    if (block.height != previousBlock.height + 1)
    {
        return false;
    }

    if (block.prevHash != previousBlock.hash)
    {
        return false;
    }

    if (block.version != 1 || block.difficulty != 3)
    {
        return false;
    }

    if (block.transactions.empty() || block.transactions.size() > 100)
    {
        return false;
    }

    if (block.txRoot != calculateTransactionRoot(block.transactions, block.version))
    {
        return false;
    }

    if (block.hash != calculateBlockHash(block))
    {
        return false;
    }

    if (!validProof(block))
    {
        return false;
    }

    vector<long long> balances;

    for (int i = 0; i < (int)blockchain.users.size(); i++)
    {
        balances.push_back(blockchain.users[i].balance);
    }

    unordered_set<string> chainIds;

    for (int i = 0; i < (int)blockchain.chain.size(); i++)
    {
        for (int j = 0; j < (int)blockchain.chain[i].transactions.size(); j++)
        {
            chainIds.insert(
                blockchain.chain[i].transactions[j].transactionId);
        }
    }

    unordered_set<string> usedIds;

    for (int i = 0; i < (int)block.transactions.size(); i++)
    {
        const Transaction& transaction = block.transactions[i];

        int sender = findUserIndex(blockchain, transaction.sender);
        int receiver = findUserIndex(blockchain, transaction.receiver);

        if (sender == -1 || receiver == -1 || sender == receiver)
        {
            return false;
        }

        if (transaction.amount <= 0)
        {
            return false;
        }

        if (transaction.transactionId != calculateTransactionId(transaction))
        {
            return false;
        }

        if (chainIds.find(transaction.transactionId) != chainIds.end())
        {
            return false;
        }

        if (!usedIds.insert(transaction.transactionId).second)
        {
            return false;
        }

        if (balances[sender] < transaction.amount)
        {
            return false;
        }

        balances[sender] -= transaction.amount;

        if (balances[receiver] > LLONG_MAX - transaction.amount)
        {
            return false;
        }

        balances[receiver] += transaction.amount;
    }

    return true;
}
