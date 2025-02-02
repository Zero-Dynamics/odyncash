// Copyright (c) 2016-2021 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2017 The Dash Core Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "psnotificationinterface.h"

#include "chainparams.h"
#include "masternode-payments.h"
#include "masternode-sync.h"
#include "masternodeman.h"
#include "governance.h"
#include "instantsend.h"
#include "privatesend.h"
#ifdef ENABLE_WALLET
#include "privatesend-client.h"
#endif // ENABLE_WALLET

void CPSNotificationInterface::InitializeCurrentBlockTip()
{
    LOCK(cs_main);
    UpdatedBlockTip(chainActive.Tip(), NULL, IsInitialBlockDownload());
}

void CPSNotificationInterface::AcceptedBlockHeader(const CBlockIndex* pindexNew)
{
    masternodeSync.AcceptedBlockHeader(pindexNew);
}

void CPSNotificationInterface::NotifyHeaderTip(const CBlockIndex* pindexNew, bool fInitialDownload)
{
    masternodeSync.NotifyHeaderTip(pindexNew, fInitialDownload, connman);
}

void CPSNotificationInterface::UpdatedBlockTip(const CBlockIndex* pindexNew, const CBlockIndex* pindexFork, bool fInitialDownload)
{
    if (pindexNew == pindexFork) // blocks were disconnected without any new ones
        return;

    masternodeSync.UpdatedBlockTip(pindexNew, fInitialDownload, connman);

    // update instantsend autolock activation flag
    instantsend.isAutoLockBip9Active =
            (VersionBitsState(pindexNew->pprev, Params().GetConsensus(), Consensus::DEPLOYMENT_ISAUTOLOCKS, versionbitscache) == THRESHOLD_ACTIVE);

    if (fInitialDownload)
        return;

    if (fLiteMode)
        return;

    mnodeman.UpdatedBlockTip(pindexNew);
    CPrivateSend::UpdatedBlockTip(pindexNew);
#ifdef ENABLE_WALLET
    privateSendClient.UpdatedBlockTip(pindexNew);
#endif // ENABLE_WALLET
    instantsend.UpdatedBlockTip(pindexNew);
    mnpayments.UpdatedBlockTip(pindexNew, connman);
    governance.UpdatedBlockTip(pindexNew, connman);
}

void CPSNotificationInterface::SyncTransaction(const CTransaction& tx, const CBlockIndex* pindex, int posInBlock)
{
    instantsend.SyncTransaction(tx, pindex, posInBlock);
    CPrivateSend::SyncTransaction(tx, pindex, posInBlock);
}
