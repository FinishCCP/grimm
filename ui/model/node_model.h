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
// limitations under the License

#pragma once

#include <QThread>
#include <memory>
#include <atomic>
#include <condition_variable>
#include "core/block_crypt.h"
#include "node/node.h"
#include "node/node_client.h"
#include "utility/io/errorhandling.h"
#include "utility/io/reactor.h"
#include "wallet/common.h"

class NodeModel
    : public QObject
    , private MWG::INodeClientObserver
{
    Q_OBJECT
public:

    NodeModel();

    void setKdf(MWG::Key::IKdf::Ptr);
    void setOwnerKey(MWG::Key::IPKdf::Ptr);
    void startNode();
    void stopNode();

    void start();

    bool isNodeRunning() const;

signals:
    void syncProgressUpdated(int done, int total);
    void startedNode();
    void stoppedNode();
    void failedToStartNode(MWG::wallet::ErrorType errorType);
    void failedToSyncNode(MWG::wallet::ErrorType errorType);

protected:
    void onSyncProgressUpdated(int done, int total) override;
    void onStartedNode() override;
    void onStoppedNode() override;
    void onFailedToStartNode(MWG::io::ErrorCode errorCode) override;
    void onSyncError(MWG::Node::IObserver::Error error) override;

    uint16_t getLocalNodePort() override;
    std::string getLocalNodeStorage() override;
    unsigned int getLocalNodeMiningThreads() override;
    std::string getTempDir() override;
    std::vector<std::string> getLocalNodePeers() override;


    void onNodeThreadFinished() override;

private:
    MWG::NodeClient m_nodeClient;
};
