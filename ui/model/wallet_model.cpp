// Copyright 2018 The Beam Team / Copyright 2019 The Grimm Team
// Copyright 2025 MWG Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "wallet_model.h"
#include "app_model.h"
#include "utility/logger.h"
#include "utility/bridge.h"
#include "utility/io/asyncevent.h"
#include "utility/helpers.h"

using namespace MWG;
using namespace MWG::wallet;
using namespace MWG::io;
using namespace std;

WalletModel::WalletModel(IWalletDB::Ptr walletDB, const std::string& nodeAddr, MWG::io::Reactor::Ptr reactor)
    : WalletClient(walletDB, nodeAddr, reactor)
{
    qRegisterMetaType<MWG::wallet::WalletStatus>("MWG::wallet::WalletStatus");
    qRegisterMetaType<MWG::wallet::ChangeAction>("MWG::wallet::ChangeAction");
    qRegisterMetaType<vector<MWG::wallet::TxDescription>>("std::vector<MWG::wallet::TxDescription>");
    qRegisterMetaType<MWG::Amount>("MWG::Amount");
    qRegisterMetaType<vector<MWG::wallet::Coin>>("std::vector<MWG::wallet::Coin>");
    qRegisterMetaType<vector<MWG::wallet::WalletAddress>>("std::vector<MWG::wallet::WalletAddress>");
    qRegisterMetaType<MWG::wallet::WalletID>("MWG::wallet::WalletID");
    qRegisterMetaType<MWG::wallet::WalletAddress>("MWG::wallet::WalletAddress");
    qRegisterMetaType<MWG::wallet::ErrorType>("MWG::wallet::ErrorType");
    qRegisterMetaType<MWG::wallet::TxID>("MWG::wallet::TxID");
}

WalletModel::~WalletModel()
{

}

QString WalletModel::GetErrorString(MWG::wallet::ErrorType type)
{
    // TODO: add more detailed error description
    switch (type)
    {
    case wallet::ErrorType::NodeProtocolBase:
        //% "Node protocol error!"
        return qtTrId("wallet-model-node-protocol-error");
    case wallet::ErrorType::NodeProtocolIncompatible:
        //% "You are trying to connect to incompatible peer."
        return qtTrId("wallet-model-incompatible-peer-error");
    case wallet::ErrorType::ConnectionBase:
        //% "Wait for connection"
        return qtTrId("wallet-model-connection-base-error");
    case wallet::ErrorType::ConnectionTimedOut:
        //% "Connection timed out."
        return qtTrId("wallet-model-connection-time-out-error");
    case wallet::ErrorType::ConnectionRefused:
        //% "Reconnect to node:"
        return qtTrId("wallet-model-connection-refused-error") + " " +  getNodeAddress().c_str();
    case wallet::ErrorType::ConnectionHostUnreach:
        //% "Node is unreachable:"
        return qtTrId("wallet-model-connection-host-unreach-error") + " " + getNodeAddress().c_str();
    case wallet::ErrorType::ConnectionAddrInUse:
    {
        auto localNodePort = AppModel::getInstance()->getSettings().getLocalNodePort();
        //% "The port %1 is already in use. Check if a wallet is already running on this machine or change the port settings."
        return qtTrId("wallet-model-connection-addr-in-use-error").arg(QString::number(localNodePort));
    }
    case wallet::ErrorType::TimeOutOfSync:
        //% "System time not synchronized."
        return qtTrId("wallet-model-time-sync-error");
    case wallet::ErrorType::HostResolvedError:
        //% "Incorrect node name or no Internet connection."
        return qtTrId("wallet-model-host-unresolved-error");
    default:
        //% "Unexpected error!"
        return qtTrId("wallet-model-undefined-error");
    }
}

bool WalletModel::isAddressWithCommentExist(const std::string& comment) const
{
    if (comment.empty())
    {
        return false;
    }
    for (const auto& it: m_addresses)
    {
        if (it.m_label == comment) {
            return true;
        }
    }
    return false;
}

void WalletModel::onStatus(const MWG::wallet::WalletStatus& status)
{
    emit walletStatus(status);
}

void WalletModel::onTxStatus(MWG::wallet::ChangeAction action, const std::vector<MWG::wallet::TxDescription>& items)
{
    emit txStatus(action, items);
}

void WalletModel::onSyncProgressUpdated(int done, int total)
{
    emit syncProgressUpdated(done, total);
}

void WalletModel::onChangeCalculated(MWG::Amount change)
{
    emit changeCalculated(change);
}

void WalletModel::onAllUtxoChanged(const std::vector<MWG::wallet::Coin>& utxos)
{
    emit allUtxoChanged(utxos);
}

void WalletModel::onAddresses(bool own, const std::vector<MWG::wallet::WalletAddress>& addrs)
{
    if (own)
    {
        m_addresses = addrs;
    }
    emit addressesChanged(own, addrs);
}

void WalletModel::onCoinsByTx(const std::vector<MWG::wallet::Coin>& coins)
{

}

void WalletModel::onAddressChecked(const std::string& addr, bool isValid)
{
    emit addressChecked(QString::fromStdString(addr), isValid);
}

void WalletModel::onGeneratedNewAddress(const MWG::wallet::WalletAddress& walletAddr)
{
    emit generatedNewAddress(walletAddr);
}

void WalletModel::onNewAddressFailed()
{
    emit newAddressFailed();
}

void WalletModel::onChangeCurrentWalletIDs(MWG::wallet::WalletID senderID, MWG::wallet::WalletID receiverID)
{
    emit changeCurrentWalletIDs(senderID, receiverID);
}

void WalletModel::onNodeConnectionChanged(bool isNodeConnected)
{
    emit nodeConnectionChanged(isNodeConnected);
}

void WalletModel::onWalletError(MWG::wallet::ErrorType error)
{
    emit walletError(error);
}

void WalletModel::FailedToStartWallet()
{
    //% "Failed to start wallet. Please check your wallet data location"
    AppModel::getInstance()->getMessages().addMessage(qtTrId("wallet-model-data-location-error"));
}

void WalletModel::onSendMoneyVerified()
{
    emit sendMoneyVerified();
}

void WalletModel::onCantSendToExpired()
{
    emit cantSendToExpired();
}

void WalletModel::onPaymentProofExported(const MWG::wallet::TxID& txID, const MWG::ByteBuffer& proof)
{
    string str;
    str.resize(proof.size() * 2);

    MWG::to_hex(str.data(), proof.data(), proof.size());
    emit paymentProofExported(txID, QString::fromStdString(str));
}
