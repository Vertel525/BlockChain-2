#include "blockchain.h"
#include "hash.h"
#include <climits>

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

    vector<string> usedIds;

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

        if (transactionExistsInChain(blockchain, transaction.transactionId))
        {
            return false;
        }

        for (int j = 0; j < (int)usedIds.size(); j++)
        {
            if (usedIds[j] == transaction.transactionId)
            {
                return false;
            }
        }

        usedIds.push_back(transaction.transactionId);

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
