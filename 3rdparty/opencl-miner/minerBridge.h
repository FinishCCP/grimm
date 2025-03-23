// MWG OpenCL Miner
// Miner<->stratum bridge class
// Copyright 2018 The Beam Team / Copyright 2019 The Grimm Team
// Copyright 2018 Wilke Trei
// Copyright 2025 MWG Team
#pragma once
#include <vector>
#include <stdint.h>

namespace MWGMiner {

class minerBridge {
public:
    virtual ~minerBridge() = default;

    virtual bool hasWork() = 0;
    virtual void getWork(int64_t*, uint64_t*, uint8_t*, uint32_t*) = 0;

    virtual void handleSolution(int64_t&, uint64_t&, std::vector<uint32_t>&, uint32_t) = 0;
};

}
