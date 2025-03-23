#pragma once
#include <QObject>
#include "wallet/common.h"

namespace MWGui
{
    QString toString(const MWG::wallet::WalletID&);
    QString toString(const MWG::Merkle::Hash&);
    QString MWGToString(const MWG::Amount& value);
    QString toString(const MWG::Timestamp& ts);

    class Filter
    {
    public:
        Filter(size_t size = 12);
        void addSample(double value);
        double getAverage() const;
        double getMedian() const;
    private:
        std::vector<double> _samples;
        size_t _index;
    };
}
