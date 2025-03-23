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

#include "node/node.h"

class TxGenerator
{
public:
	using Inputs = std::vector<MWG::Input>;
public:
	TxGenerator(MWG::Key::IKdf& kdf);

	void GenerateInputInTx(MWG::Height h, MWG::Amount v, MWG::Key::Type keyType = MWG::Key::Type::Coinbase, uint32_t ind = 0);
	void GenerateOutputInTx(MWG::Height h, MWG::Amount v, MWG::Key::Type keyType = MWG::Key::Type::Regular, bool isPublic = false, uint32_t ind = 0);
	void GenerateKernel(MWG::Height h, MWG::Amount fee = 0, uint32_t ind = 0);
	void GenerateKernel();

	const MWG::proto::NewTransaction& GetTransaction();
	bool IsValid() const;

	void Sort();
	void SortInputs();
	void SortOutputs();
	void SortKernels();

	void ZeroOffset();

	Inputs GenerateInputsFromOutputs();

private:
	MWG::Key::IKdf& m_Kdf;
	MWG::proto::NewTransaction m_MsgTx;
	ECC::Scalar::Native m_Offset;
};
