// Copyright (c) 2016-2021 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2021 The Dash Core Developers
// Copyright (c) 2009-2021 The Bitcoin Developers
// Copyright (c) 2009-2021 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CASH_MINER_UTIL_H
#define CASH_MINER_UTIL_H

#include <atomic>
#include <memory>

// #include "chain/chain.h"
#include "primitives/block.h"

class CBlockIndex;
class CChainParams;
class CConnman;
class CReserveKey;
class CScript;
class CWallet;

namespace Consensus
{
struct Params;
};

static const bool DEFAULT_GENERATE = false;
static const uint8_t DEFAULT_GENERATE_THREADS_CPU = 0;
static const uint8_t DEFAULT_GENERATE_THREADS_GPU = 0;

static const bool DEFAULT_PRINTPRIORITY = false;

struct CBlockTemplate {
    CBlock block;
    std::vector<CAmount> vTxFees;
    std::vector<int64_t> vTxSigOps;
    CTxOut txoutMasternode;                 // masternode payment
    std::vector<CTxOut> voutSuperblock; // masternode payment
};

/** Set pubkey script in generated block */
void SetBlockPubkeyScript(CBlock& block, const CScript& scriptPubKeyIn);
/** Generate a new block, without valid proof-of-work */
std::unique_ptr<CBlockTemplate> CreateNewBlock(const CChainParams& chainparams, const CScript* scriptPubKeyIn = nullptr);
std::unique_ptr<CBlockTemplate> CreateNewBlock(const CChainParams& chainparams, const CScript& scriptPubKeyIn);
/** Called by a miner when new block was found. */
bool ProcessBlockFound(const CBlock& block, const CChainParams& chainparams);

/** Modify the extranonce in a block */
void IncrementExtraNonce(CBlock& pblock, const CBlockIndex* pindexPrev, unsigned int& nExtraNonce);
int64_t UpdateTime(CBlockHeader& pblock, const Consensus::Params& consensusParams, const CBlockIndex* pindexPrev);

#endif // CASH_MINER_UTIL_H
