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

#pragma once

#include <QObject>

#include "wallet/wallet_client.h"

class WalletModel
    : public QObject
    , public MWG::wallet::WalletClient
{
    Q_OBJECT
public:

    using Ptr = std::shared_ptr<WalletModel>;

    WalletModel(MWG::wallet::IWalletDB::Ptr walletDB, const std::string& nodeAddr, MWG::io::Reactor::Ptr reactor);
    ~WalletModel() override;

    QString GetErrorString(MWG::wallet::ErrorType type);
    bool isAddressWithCommentExist(const std::string& comment) const;

signals:
    void walletStatus(const MWG::wallet::WalletStatus& status);
    void txStatus(MWG::wallet::ChangeAction, const std::vector<MWG::wallet::TxDescription>& items);
    void syncProgressUpdated(int done, int total);
    void changeCalculated(MWG::Amount change);
    void allUtxoChanged(const std::vector<MWG::wallet::Coin>& utxos);
    void addressesChanged(bool own, const std::vector<MWG::wallet::WalletAddress>& addresses);
    void generatedNewAddress(const MWG::wallet::WalletAddress& walletAddr);
    void newAddressFailed();
    void changeCurrentWalletIDs(MWG::wallet::WalletID senderID, MWG::wallet::WalletID receiverID);
    void nodeConnectionChanged(bool isNodeConnected);
    void walletError(MWG::wallet::ErrorType error);
    void sendMoneyVerified();
    void cantSendToExpired();
    void paymentProofExported(const MWG::wallet::TxID& txID, const QString& proof);
    void addressChecked(const QString& addr, bool isValid);

private:
    void onStatus(const MWG::wallet::WalletStatus& status) override;
    void onTxStatus(MWG::wallet::ChangeAction, const std::vector<MWG::wallet::TxDescription>& items) override;
    void onSyncProgressUpdated(int done, int total) override;
    void onChangeCalculated(MWG::Amount change) override;
    void onAllUtxoChanged(const std::vector<MWG::wallet::Coin>& utxos) override;
    void onAddresses(bool own, const std::vector<MWG::wallet::WalletAddress>& addrs) override;
    void onGeneratedNewAddress(const MWG::wallet::WalletAddress& walletAddr) override;
    void onNewAddressFailed() override;
    void onChangeCurrentWalletIDs(MWG::wallet::WalletID senderID, MWG::wallet::WalletID receiverID) override;
    void onNodeConnectionChanged(bool isNodeConnected) override;
    void onWalletError(MWG::wallet::ErrorType error) override;
    void FailedToStartWallet() override;
    void onSendMoneyVerified() override;
    void onCantSendToExpired() override;
    void onPaymentProofExported(const MWG::wallet::TxID& txID, const MWG::ByteBuffer& proof) override;
    void onCoinsByTx(const std::vector<MWG::wallet::Coin>& coins) override;
    void onAddressChecked(const std::string& addr, bool isValid) override;

private:
    std::vector<MWG::wallet::WalletAddress> m_addresses;
};
