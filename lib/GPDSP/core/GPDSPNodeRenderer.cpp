/*
**      General Purpose DSP Library
**
**      Original Copyright (C) 2017 - 2018 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      Version     POSIX / C++11
**      Website     http://iridium.jp/
**      E-mail      zap00365@nifty.com
**
**      This source code is for Xcode.
**      Xcode 9.2 (Apple LLVM 9.0.0)
**
**      GPDSPNodeRenderer.cpp
**
**      ------------------------------------------------------------------------
**
**      The MIT License (MIT)
**
**      Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
**      associated documentation files (the "Software"), to deal in the Software without restriction,
**      including without limitation the rights to use, copy, modify, merge, publish, distribute,
**      sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
**      furnished to do so, subject to the following conditions:
**      The above copyright notice and this permission notice shall be included in all copies or
**      substantial portions of the Software.
**      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
**      BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
**      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
**      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
**      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
**      以下に定める条件に従い、本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）の複製を
**      取得するすべての人に対し、ソフトウェアを無制限に扱うことを無償で許可します。
**      これには、ソフトウェアの複製を使用、複写、変更、結合、掲載、頒布、サブライセンス、および、または販売する権利、
**      およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます。
**      上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製または重要な部分に記載するものとします。
**      ソフトウェアは「現状のまま」で、明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。
**      ここでいう保証とは、商品性、特定の目的への適合性、および権利非侵害についての保証も含みますが、それに限定されるものではありません。
**      作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフトウェアに起因または関連し、
**      あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求、損害、その他の義務について何らの責任も負わないものとします。
*/

#include "GPDSPNodeRenderer.hpp"
#include <map>
#include "GPDSPSerializable.hpp"
#include "GPDSPFlexOutputtableNode.hpp"
#include "../algorithm/GPDSPBufferInputNode.hpp"
#include "../algorithm/GPDSPBufferOutputNode.hpp"
#include "../algorithm/GPDSPConstantNode.hpp"
#include "../algorithm/GPDSPSignNode.hpp"
#include "../algorithm/GPDSPGateNode.hpp"
#include "../algorithm/GPDSPPeakNode.hpp"
#include "../algorithm/GPDSPAmplifyNode.hpp"
#include "../algorithm/GPDSPDelayNode.hpp"
#include "../algorithm/GPDSPBufferNode.hpp"
#include "../algorithm/GPDSPSumNode.hpp"
#include "../algorithm/GPDSPMultiplyNode.hpp"
#include "../algorithm/GPDSPSquareRootNode.hpp"
#include "../generic/GPDSPGenericNode.hpp"
#include "../generic/GPDSPGenericInputNode.hpp"
#include "../generic/GPDSPGenericOutputNode.hpp"

namespace ir {

GPDSPError GPDSPNodeRenderer::setRate(int rate) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (_node.size() <= 0) {
        if (rate >= 0) {
            _rate = rate;
        }
        else {
            error = GPDSPERROR_INVALID_PARAM;
        }
    }
    else {
        error = GPDSPERROR_INVALID_STATE;
    }
    return error;
}

std::shared_ptr<GPDSPBufferInputNode> GPDSPNodeRenderer::getNodeBufferInput(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPBufferInputNode>(getNode(name));
}

std::shared_ptr<GPDSPBufferOutputNode> GPDSPNodeRenderer::getNodeBufferOutput(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPBufferOutputNode>(getNode(name));
}

std::shared_ptr<GPDSPConstantNode> GPDSPNodeRenderer::getNodeConstant(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPConstantNode>(getNode(name));
}

std::shared_ptr<GPDSPSignNode> GPDSPNodeRenderer::getNodeSign(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPSignNode>(getNode(name));
}

std::shared_ptr<GPDSPGateNode> GPDSPNodeRenderer::getNodeGate(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPGateNode>(getNode(name));
}

std::shared_ptr<GPDSPPeakNode> GPDSPNodeRenderer::getNodePeak(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPPeakNode>(getNode(name));
}

std::shared_ptr<GPDSPAmplifyNode> GPDSPNodeRenderer::getNodeAmplify(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPAmplifyNode>(getNode(name));
}

std::shared_ptr<GPDSPDelayNode> GPDSPNodeRenderer::getNodeDelay(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPDelayNode>(getNode(name));
}

std::shared_ptr<GPDSPBufferNode> GPDSPNodeRenderer::getNodeBuffer(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPBufferNode>(getNode(name));
}

std::shared_ptr<GPDSPSumNode> GPDSPNodeRenderer::getNodeSum(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPSumNode>(getNode(name));
}

std::shared_ptr<GPDSPMultiplyNode> GPDSPNodeRenderer::getNodeMultiply(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPMultiplyNode>(getNode(name));
}

std::shared_ptr<GPDSPSquareRootNode> GPDSPNodeRenderer::getNodeSquareRoot(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPSquareRootNode>(getNode(name));
}

std::shared_ptr<GPDSPSinWaveNode> GPDSPNodeRenderer::getNodeSinWave(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPSinWaveNode>(getNode(name));
}

std::shared_ptr<GPDSPTriangleWaveNode> GPDSPNodeRenderer::getNodeTriangleWave(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPTriangleWaveNode>(getNode(name));
}

std::shared_ptr<GPDSPSawtoothWaveNode> GPDSPNodeRenderer::getNodeSawtoothWave(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPSawtoothWaveNode>(getNode(name));
}

std::shared_ptr<GPDSPSquareWaveNode> GPDSPNodeRenderer::getNodeSquareWave(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPSquareWaveNode>(getNode(name));
}

std::shared_ptr<GPDSPGenericNode> GPDSPNodeRenderer::getNodeGeneric(std::string const& name) const noexcept
{
    return std::dynamic_pointer_cast<GPDSPGenericNode>(getNode(name));
}

std::shared_ptr<GPDSPNode> GPDSPNodeRenderer::getNode(std::string const& name) const noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPNode> result;
    
    if ((it = _node.find(name)) != _node.end()) {
        result = it->second;
    }
    return result;
}

GPDSPError GPDSPNodeRenderer::setCountI(std::string const& name, int count, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPFlexInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->setCountI(count, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getCountI(std::string const& name, int* count) const noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if (count != NULL) {
            if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode const>(it->second)) != NULL) {
                *count = node->getCountI();
            }
            else {
                *count = 0;
            }
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::setCountO(std::string const& name, int count, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPFlexOutputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexOutputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->setCountO(count, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getCountO(std::string const& name, int* count) const noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if (count != NULL) {
            if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode const>(it->second)) != NULL) {
                *count = node->getCountO();
            }
            else {
                *count = 0;
            }
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::setNameI(std::string const& name, int index, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->setNameI(index, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getNameI(std::string const& name, int index, std::string* what) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->getNameI(index, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::setNameO(std::string const& name, int index, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPOutputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeOutputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->setNameO(index, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getNameO(std::string const& name, int index, std::string* what) const noexcept
{
    std::shared_ptr<GPDSPOutputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeOutputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->getNameO(index, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::setLinkPositiveI(std::string const& name, int index, std::string const& from, int which) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> input;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &input)) == GPDSPERROR_OK) {
        if ((error = getNodeOutputtable(from, &output)) == GPDSPERROR_OK) {
            error = input->setLinkPositiveI(index, output.get(), which);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::setLinkNegativeI(std::string const& name, int index, std::string const& from, int which) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> input;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &input)) == GPDSPERROR_OK) {
        if ((error = getNodeOutputtable(from, &output)) == GPDSPERROR_OK) {
            error = input->setLinkNegativeI(index, output.get(), which);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::setLinkConstantI(std::string const& name, int index, GPDSPFloat constant) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->setLinkConstantI(index, constant);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getModeI(std::string const& name, int index, GPDSPMode* mode) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->getModeI(index, mode);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getLinkI(std::string const& name, int index, std::string* from, int* which) const noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> input;
    GPDSPOutputtableNode const* node;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &input)) == GPDSPERROR_OK) {
        if ((error = input->getLinkI(index, &node, &number)) == GPDSPERROR_OK) {
            if (node != NULL) {
                for (it = _node.begin(); it != _node.end(); ++it) {
                    if (it->second.get() == node) {
                        break;
                    }
                }
                if (it != _node.end()) {
                    if (from != NULL) {
                        *from = it->first;
                    }
                    if (which != NULL) {
                        *which = number;
                    }
                }
                else {
                    error = GPDSPERROR_NO_NODE;
                }
            }
            else {
                if (from != NULL) {
                    *from = "";
                }
                if (which != NULL) {
                    *which = 0;
                }
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getLinkI(std::string const& name, int index, GPDSPFloat* constant) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->getLinkI(index, constant);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name, int index) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->clearLinkI(index);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name, GPDSPMode mode) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        node->clearLinkI(mode);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name, std::string const& from, int which) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> input;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &input)) == GPDSPERROR_OK) {
        if ((error = getNodeOutputtable(from, &output)) == GPDSPERROR_OK) {
            input->clearLinkI(output.get(), which);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name, std::string const& from) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> input;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &input)) == GPDSPERROR_OK) {
        if ((error = getNodeOutputtable(from, &output)) == GPDSPERROR_OK) {
            input->clearLinkI(output.get());
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name, GPDSPFloat constant) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        node->clearLinkI(constant);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        node->clearLinkI();
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkO(std::string const& name, int index) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    std::shared_ptr<GPDSPInputtableNode> input;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeOutputtable(name, &output)) == GPDSPERROR_OK) {
        for (it = _node.begin(); it != _node.end(); ++it) {
            if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
                input->clearLinkI(output.get(), index);
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearLinkO(std::string const& name) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    std::shared_ptr<GPDSPInputtableNode> input;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeOutputtable(name, &output)) == GPDSPERROR_OK) {
        for (it = _node.begin(); it != _node.end(); ++it) {
            if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
                input->clearLinkI(output.get());
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getValueI(std::string const& name, int index, GPDSPFloat* value) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->getValueI(index, value);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getValueO(std::string const& name, int index, GPDSPFloat* value) const noexcept
{
    std::shared_ptr<GPDSPOutputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeOutputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->getValueO(index, value);
    }
    return error;
}

std::string GPDSPNodeRenderer::getNextNode(void) const noexcept
{
    std::string result;
    
    if (_nit != _node.end()) {
        result = _nit->first;
        ++_nit;
    }
    return result;
}

std::string GPDSPNodeRenderer::findNode(std::shared_ptr<GPDSPNode const> const& node) const noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::string result;
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        if (node == it->second) {
            result = it->first;
            break;
        }
    }
    return result;
}

GPDSPError GPDSPNodeRenderer::findNameI(std::string const& name, std::string const& what, int* index) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> node;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        if ((number = node->findNameI(what)) >= 0) {
            if (index != NULL) {
                *index = number;
            }
        }
        else {
            error = GPDSPERROR_NO_FOUND;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::findNameO(std::string const& name, std::string const& what, int* index) const noexcept
{
    std::shared_ptr<GPDSPOutputtableNode const> node;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeOutputtable(name, &node)) == GPDSPERROR_OK) {
        if ((number = node->findNameO(what)) >= 0) {
            if (index != NULL) {
                *index = number;
            }
        }
        else {
            error = GPDSPERROR_NO_FOUND;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::findModeI(std::string const& name, GPDSPMode mode, int* index) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> node;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        if ((number = node->findModeI(mode)) >= 0) {
            if (index != NULL) {
                *index = number;
            }
        }
        else {
            error = GPDSPERROR_NO_FOUND;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::findLinkI(std::string const& name, std::string const& from, int which, int* index) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> input;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &input)) == GPDSPERROR_OK) {
        if ((error = getNodeOutputtable(from, &output)) == GPDSPERROR_OK) {
            if ((number = input->findLinkI(output.get(), which)) >= 0) {
                if (index != NULL) {
                    *index = number;
                }
            }
            else {
                error = GPDSPERROR_NO_FOUND;
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::findLinkI(std::string const& name, std::string const& from, int* index) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> input;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &input)) == GPDSPERROR_OK) {
        if ((error = getNodeOutputtable(from, &output)) == GPDSPERROR_OK) {
            if ((number = input->findLinkI(output.get())) >= 0) {
                if (index != NULL) {
                    *index = number;
                }
            }
            else {
                error = GPDSPERROR_NO_FOUND;
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::findLinkI(std::string const& name, GPDSPFloat constant, int* index) const noexcept
{
    std::shared_ptr<GPDSPInputtableNode const> node;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeInputtable(name, &node)) == GPDSPERROR_OK) {
        if ((number = node->findLinkI(constant)) >= 0) {
            if (index != NULL) {
                *index = number;
            }
        }
        else {
            error = GPDSPERROR_NO_FOUND;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeBufferInput(std::string const& name, GPDSPFloat const* buffer, int length, int interleave) noexcept
{
    std::shared_ptr<GPDSPBufferInputNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPBufferInputNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setBuffer(buffer, length, interleave)) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeBufferOutput(std::string const& name, GPDSPFloat* buffer, int length, int interleave) noexcept
{
    std::shared_ptr<GPDSPBufferOutputNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPBufferOutputNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setBuffer(buffer, length, interleave)) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeConstant(std::string const& name, GPDSPFloat constant) noexcept
{
    std::shared_ptr<GPDSPConstantNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPConstantNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        node->setConstant(constant);
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeSign(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPSignNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPSignNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeGate(std::string const& name, GPDSPFloat minimum, GPDSPFloat maximum) noexcept
{
    std::shared_ptr<GPDSPGateNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPGateNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        node->setMinimum(minimum);
        node->setMaximum(maximum);
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodePeak(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPPeakNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPPeakNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeAmplify(std::string const& name, GPDSPFloat gain) noexcept
{
    std::shared_ptr<GPDSPAmplifyNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPAmplifyNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        node->setGain(gain);
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeDelay(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPDelayNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPDelayNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeBuffer(std::string const& name, int size) noexcept
{
    std::shared_ptr<GPDSPBufferNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPBufferNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setSize(size)) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeSum(std::string const& name, int count) noexcept
{
    std::shared_ptr<GPDSPSumNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPSumNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setCountI(count, "in")) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeMultiply(std::string const& name, int count) noexcept
{
    std::shared_ptr<GPDSPMultiplyNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPMultiplyNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setCountI(count, "in")) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeSquareRoot(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPSquareRootNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPSquareRootNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeSinWave(std::string const& name, GPDSPFloat resolution) noexcept
{
    std::shared_ptr<GPDSPSinWaveNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPSinWaveNode>(_rate);
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setResolution(resolution)) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeTriangleWave(std::string const& name, GPDSPFloat resolution) noexcept
{
    std::shared_ptr<GPDSPTriangleWaveNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPTriangleWaveNode>(_rate);
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setResolution(resolution)) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeSawtoothWave(std::string const& name, GPDSPFloat resolution) noexcept
{
    std::shared_ptr<GPDSPSawtoothWaveNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPSawtoothWaveNode>(_rate);
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setResolution(resolution)) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeSquareWave(std::string const& name, GPDSPFloat resolution) noexcept
{
    std::shared_ptr<GPDSPSquareWaveNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPSquareWaveNode>(_rate);
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->setResolution(resolution)) == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeGeneric(std::string const& name, std::string const& file) noexcept
{
    std::shared_ptr<GPDSPGenericNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPGenericNode>(_rate);
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if (file != "") {
            error = node->open(file);
        }
        if (error == GPDSPERROR_OK) {
            error = appendNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::appendNode(std::string const& name, std::shared_ptr<GPDSPNode> const& node) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    GPDSPError error(GPDSPERROR_OK);
    
    if (name != "" && node != NULL) {
        for (it = _node.begin(); it != _node.end(); ++it) {
            if (name == it->first || node == it->second) {
                break;
            }
        }
        if (it == _node.end()) {
            if ((error = node->fixate()) == GPDSPERROR_OK) {
                try {
                    _node[name] = node;
                }
                catch (std::bad_alloc const&) {
                    error = GPDSPERROR_NO_MEMORY;
                }
                if (error == GPDSPERROR_OK) {
                    _sequence.clear();
                    _wait.clear();
                    _nit = _node.end();
                }
            }
        }
        else {
            error = GPDSPERROR_ALREADY_EXIST;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::appendI(std::string const& name, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPFlexInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->appendI(what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::appendO(std::string const& name, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPFlexOutputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexOutputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->appendO(what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::insertI(std::string const& name, int index, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPFlexInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->insertI(index, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::insertO(std::string const& name, int index, std::string const& what) noexcept
{
    std::shared_ptr<GPDSPFlexOutputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexOutputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->insertO(index, what);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::removeNode(std::string const& name) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    error = clearLinkO(name);
    switch (error) {
        case GPDSPERROR_INVALID_NODE:
            error = GPDSPERROR_OK;
        case GPDSPERROR_OK:
            _sequence.clear();
            _wait.clear();
            _node.erase(name);
            _nit = _node.end();
            break;
        default:
            break;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::removeI(std::string const& name, int index) noexcept
{
    std::shared_ptr<GPDSPFlexInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexInputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->removeI(index);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::removeO(std::string const& name, int index) noexcept
{
    std::shared_ptr<GPDSPFlexOutputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexOutputtable(name, &node)) == GPDSPERROR_OK) {
        error = node->removeO(index);
    }
    return error;
}

void GPDSPNodeRenderer::clearNode(void) noexcept
{
    _sequence.clear();
    _wait.clear();
    _node.clear();
    _nit = _node.end();
    return;
}

GPDSPError GPDSPNodeRenderer::clearI(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPFlexInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexInputtable(name, &node)) == GPDSPERROR_OK) {
        node->clearI();
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::clearO(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPFlexOutputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getNodeFlexOutputtable(name, &node)) == GPDSPERROR_OK) {
        node->clearO();
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::renameNode(std::string const& name, std::string const& alternate) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    GPDSPError error(GPDSPERROR_OK);
    
    if (alternate != "") {
        if ((it = _node.find(name)) != _node.end()) {
            if (alternate != name) {
                if (_node.find(alternate) == _node.end()) {
                    try {
                        _node[alternate] = it->second;
                    }
                    catch (std::bad_alloc const&) {
                        error = GPDSPERROR_NO_MEMORY;
                    }
                    if (error == GPDSPERROR_OK) {
                        _node.erase(name);
                        _nit = _node.end();
                    }
                }
                else {
                    error = GPDSPERROR_ALREADY_EXIST;
                }
            }
        }
        else {
            error = GPDSPERROR_NO_NODE;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

void GPDSPNodeRenderer::invalidate(void) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        it->second->invalidate();
    }
    return;
}

GPDSPError GPDSPNodeRenderer::prepare(void) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    GPDSPError error(GPDSPERROR_OK);
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        if ((error = it->second->prepare()) != GPDSPERROR_OK) {
            break;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::process(void) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (_wait.size() > 0) {
        error = makeSequence();
    }
    else if (_sequence.size() > 0) {
        if ((error = optimizeSequence()) == GPDSPERROR_WAIT) {
            if ((error = makeWait()) == GPDSPERROR_OK) {
                error = makeSequence();
            }
        }
    }
    else if (_node.size() > 0) {
        if ((error = makeWait()) == GPDSPERROR_OK) {
            error = makeSequence();
        }
    }
    switch (error) {
        case GPDSPERROR_OK:
        case GPDSPERROR_FRAGMENT:
        case GPDSPERROR_LOOP:
            break;
        default:
            _sequence.clear();
            _wait.clear();
            break;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::render(int count, int* remain) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (count >= 0) {
        while (--count >= 0) {
            invalidate();
            if ((error = prepare()) == GPDSPERROR_OK) {
                do {
                    error = process();
                } while (error == GPDSPERROR_FRAGMENT);
            }
            if (error != GPDSPERROR_OK) {
                break;
            }
        }
        if (remain != NULL) {
            *remain = count + 1;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::rewind(std::string const& name) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRewindableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPRewindableNode>(it->second)) != NULL) {
            node->rewind();
            it->second->invalidate();
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

void GPDSPNodeRenderer::rewind(void) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRewindableNode> node;
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        if ((node = std::dynamic_pointer_cast<GPDSPRewindableNode>(it->second)) != NULL) {
            node->rewind();
            it->second->invalidate();
        }
    }
    return;
}

GPDSPError GPDSPNodeRenderer::refresh(std::string const& name) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRefreshableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPRefreshableNode>(it->second)) != NULL) {
            node->refresh();
            it->second->invalidate();
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

void GPDSPNodeRenderer::refresh(void) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRefreshableNode> node;
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        if ((node = std::dynamic_pointer_cast<GPDSPRefreshableNode>(it->second)) != NULL) {
            node->refresh();
            it->second->invalidate();
        }
    }
    return;
}

GPDSPError GPDSPNodeRenderer::load(std::string const& file, GPDSPSerializable* serializable) noexcept
{
    struct {
        tinyxml2::XMLDocument dom;
        tinyxml2::XMLElement const* gpdsp;
        union {
            struct {
                tinyxml2::XMLElement const* meta;
            };
            struct {
                tinyxml2::XMLElement const* formula;
                tinyxml2::XMLElement const* instance;
                tinyxml2::XMLElement const* param;
            };
        };
    } xml;
    std::string string;
    std::string name;
    union {
        struct {
            int length;
            int interleave;
        };
        GPDSPFloat constant;
        struct {
            GPDSPFloat minimum;
            GPDSPFloat maximum;
        };
        GPDSPFloat gain;
        int size;
        int count;
        GPDSPFloat resolution;
    } param;
    int format;
    GPDSPError error(GPDSPERROR_OK);
    
    if (_node.size() <= 0) {
        switch (xml.dom.LoadFile(file.c_str())) {
            case tinyxml2::XML_SUCCESS:
                if ((xml.gpdsp = xml.dom.FirstChildElement("gpdsp")) != NULL) {
                    for (; xml.gpdsp != NULL; xml.gpdsp = xml.gpdsp->NextSiblingElement("gpdsp")) {
                        if ((xml.meta = xml.gpdsp->FirstChildElement("meta")) != NULL) {
                            if ((error = readRateTag(xml.meta, _rate)) == GPDSPERROR_OK) {
                                if ((error = readFormatTag(xml.meta, &format)) == GPDSPERROR_OK) {
                                    if ((xml.formula = xml.gpdsp->FirstChildElement("formula")) != NULL) {
                                        for (xml.instance = xml.formula->FirstChildElement(); xml.instance != NULL; xml.instance = xml.instance->NextSiblingElement()) {
                                            name = "";
                                            if ((error = readTag(xml.instance, "name", false, &name)) == GPDSPERROR_OK) {
                                                if (xml.instance->Name() != NULL) {
                                                    string = xml.instance->Name();
                                                    if (string == "GPDSPBufferInputNode") {
                                                        param.length = 0;
                                                        param.interleave = 0;
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            if ((error = readTag(xml.param, "length", true, &param.length)) == GPDSPERROR_OK) {
                                                                error = readTag(xml.param, "interleave", true, &param.interleave);
                                                            }
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeBufferInput(name, NULL, param.length, param.interleave);
                                                        }
                                                    }
                                                    else if (string == "GPDSPBufferOutputNode") {
                                                        param.length = 0;
                                                        param.interleave = 0;
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            if ((error = readTag(xml.param, "length", true, &param.length)) == GPDSPERROR_OK) {
                                                                error = readTag(xml.param, "interleave", true, &param.interleave);
                                                            }
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeBufferOutput(name, NULL, param.length, param.interleave);
                                                        }
                                                    }
                                                    else if (string == "GPDSPConstantNode") {
                                                        param.constant = GPDSPConstantNode::defaultConstant();
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "constant", true, format, &param.constant);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeConstant(name, param.constant);
                                                        }
                                                    }
                                                    else if (string == "GPDSPSignNode") {
                                                        error = newNodeSign(name);
                                                    }
                                                    else if (string == "GPDSPGateNode") {
                                                        param.minimum = GPDSPGateNode::defaultMinimum();
                                                        param.maximum = GPDSPGateNode::defaultMaximum();
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            if ((error = readTag(xml.param, "minimum", true, format, &param.minimum)) == GPDSPERROR_OK) {
                                                                error = readTag(xml.param, "maximum", true, format, &param.maximum);
                                                            }
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeGate(name, param.minimum, param.maximum);
                                                        }
                                                    }
                                                    else if (string == "GPDSPPeakNode") {
                                                        error = newNodePeak(name);
                                                    }
                                                    else if (string == "GPDSPAmplifyNode") {
                                                        param.gain = GPDSPAmplifyNode::defaultGain();
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "gain", true, format, &param.gain);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeAmplify(name, param.gain);
                                                        }
                                                    }
                                                    else if (string == "GPDSPDelayNode") {
                                                        error = newNodeDelay(name);
                                                    }
                                                    else if (string == "GPDSPBufferNode") {
                                                        param.size = 0;
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "size", true, &param.size);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeBuffer(name, param.size);
                                                        }
                                                    }
                                                    else if (string == "GPDSPSumNode") {
                                                        if ((error = countInputTag(xml.instance, &param.count)) == GPDSPERROR_OK) {
                                                            if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                                error = readTag(xml.param, "count", true, &param.count);
                                                            }
                                                            if (error == GPDSPERROR_OK) {
                                                                error = newNodeSum(name, param.count);
                                                            }
                                                        }
                                                    }
                                                    else if (string == "GPDSPMultiplyNode") {
                                                        if ((error = countInputTag(xml.instance, &param.count)) == GPDSPERROR_OK) {
                                                            if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                                error = readTag(xml.param, "count", true, &param.count);
                                                            }
                                                            if (error == GPDSPERROR_OK) {
                                                                error = newNodeMultiply(name, param.count);
                                                            }
                                                        }
                                                    }
                                                    else if (string == "GPDSPSquareRootNode") {
                                                        error = newNodeSquareRoot(name);
                                                    }
                                                    else if (string == "GPDSPSinWaveNode") {
                                                        param.resolution = GPDSPSinWaveNode::defaultResolution();
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "resolution", true, format, &param.resolution);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeSinWave(name, param.resolution);
                                                        }
                                                    }
                                                    else if (string == "GPDSPTriangleWaveNode") {
                                                        param.resolution = GPDSPTriangleWaveNode::defaultResolution();
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "resolution", true, format, &param.resolution);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeTriangleWave(name, param.resolution);
                                                        }
                                                    }
                                                    else if (string == "GPDSPSawtoothWaveNode") {
                                                        param.resolution = GPDSPSawtoothWaveNode::defaultResolution();
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "resolution", true, format, &param.resolution);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeSawtoothWave(name, param.resolution);
                                                        }
                                                    }
                                                    else if (string == "GPDSPSquareWaveNode") {
                                                        param.resolution = GPDSPSquareWaveNode::defaultResolution();
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "resolution", true, format, &param.resolution);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeSquareWave(name, param.resolution);
                                                        }
                                                    }
                                                    else if (string == "GPDSPGenericNode") {
                                                        string = "";
                                                        if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                            error = readTag(xml.param, "file", true, &string);
                                                        }
                                                        if (error == GPDSPERROR_OK) {
                                                            error = newNodeGeneric(name, string);
                                                        }
                                                    }
                                                    else if (string == "GPDSPGenericInputNode") {
                                                        error = newNodeGenericInput(name);
                                                    }
                                                    else if (string == "GPDSPGenericOutputNode") {
                                                        error = newNodeGenericOutput(name);
                                                    }
                                                    else if (serializable != NULL) {
                                                        error = serializable->load(this, string, name, format, xml.instance);
                                                    }
                                                    else {
                                                        error = GPDSPERROR_NO_SUPPORT;
                                                    }
                                                }
                                                else {
                                                    error = GPDSPERROR_FAILED;
                                                }
                                            }
                                            if (error != GPDSPERROR_OK) {
                                                break;
                                            }
                                        }
                                        if (error == GPDSPERROR_OK) {
                                            for (xml.instance = xml.formula->FirstChildElement(); xml.instance != NULL; xml.instance = xml.instance->NextSiblingElement()) {
                                                name = "";
                                                if ((error = readTag(xml.instance, "name", false, &name)) == GPDSPERROR_OK) {
                                                    error = readInputTag(xml.instance, name, format);
                                                }
                                                if (error != GPDSPERROR_OK) {
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            error = GPDSPERROR_INVALID_FORMAT;
                        }
                        if (error != GPDSPERROR_NO_FOUND) {
                            break;
                        }
                    }
                }
                else {
                    error = GPDSPERROR_INVALID_FORMAT;
                }
                break;
            case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
                error = GPDSPERROR_NO_FILE;
                break;
            default:
                error = GPDSPERROR_FAILED;
                break;
        }
        if (error != GPDSPERROR_OK) {
            clearNode();
        }
    }
    else {
        error = GPDSPERROR_INVALID_STATE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::save(std::string const& file, GPDSPSerializable* serializable) const noexcept
{
    struct {
        tinyxml2::XMLDocument dom;
        tinyxml2::XMLDeclaration* decl;
        tinyxml2::XMLElement* gpdsp;
        union {
            struct {
                tinyxml2::XMLElement* meta;
            };
            struct {
                tinyxml2::XMLElement* formula;
                tinyxml2::XMLElement* instance;
                tinyxml2::XMLElement* param;
            };
        };
    } xml;
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator uit;
    std::map<std::string, std::shared_ptr<GPDSPNode const> >::const_iterator mit;
    std::map<std::string, std::shared_ptr<GPDSPNode const> > map;
    std::shared_ptr<GPDSPBufferInputNode const> bufferInput;
    std::shared_ptr<GPDSPBufferOutputNode const> bufferOutput;
    std::shared_ptr<GPDSPConstantNode const> constant;
    std::shared_ptr<GPDSPSignNode const> sign;
    std::shared_ptr<GPDSPGateNode const> gate;
    std::shared_ptr<GPDSPPeakNode const> peak;
    std::shared_ptr<GPDSPAmplifyNode const> amplify;
    std::shared_ptr<GPDSPDelayNode const> delay;
    std::shared_ptr<GPDSPBufferNode const> buffer;
    std::shared_ptr<GPDSPSumNode const> sum;
    std::shared_ptr<GPDSPMultiplyNode const> multiply;
    std::shared_ptr<GPDSPSquareRootNode const> squareRoot;
    std::shared_ptr<GPDSPSinWaveNode const> sinWave;
    std::shared_ptr<GPDSPTriangleWaveNode const> triangleWave;
    std::shared_ptr<GPDSPSawtoothWaveNode const> sawtoothWave;
    std::shared_ptr<GPDSPSquareWaveNode const> squareWave;
    std::shared_ptr<GPDSPGenericNode const> generic;
    std::shared_ptr<GPDSPGenericInputNode const> genericInput;
    std::shared_ptr<GPDSPGenericOutputNode const> genericOutput;
    union {
        struct {
            int length;
            int interleave;
        };
    } param;
    GPDSPError error(GPDSPERROR_OK);
    
    for (uit = _node.begin(); uit != _node.end(); ++uit) {
        try {
            map[uit->first] = uit->second;
        }
        catch (std::bad_alloc const&) {
            error = GPDSPERROR_NO_MEMORY;
        }
        if (error != GPDSPERROR_OK) {
            break;
        }
    }
    if (error == GPDSPERROR_OK) {
        if ((xml.decl = xml.dom.NewDeclaration()) != NULL) {
            if (xml.dom.InsertEndChild(xml.decl) != NULL) {
                if ((xml.gpdsp = xml.dom.NewElement("gpdsp")) != NULL) {
                    if (xml.dom.InsertEndChild(xml.gpdsp) != NULL) {
                        if ((error = addTag(xml.gpdsp, "meta", &xml.meta)) == GPDSPERROR_OK) {
                            if ((error = writeRateTag(xml.meta, _rate)) == GPDSPERROR_OK) {
                                if ((error = writeFormatTag(xml.meta)) == GPDSPERROR_OK) {
                                    if ((error = addTag(xml.gpdsp, "formula", &xml.formula)) == GPDSPERROR_OK) {
                                        for (mit = map.begin(); mit != map.end(); ++mit) {
                                            if ((error = addTag(xml.formula, "", &xml.instance)) == GPDSPERROR_OK) {
                                                if ((error = writeTag(xml.instance, "name", mit->first)) == GPDSPERROR_OK) {
                                                    if ((bufferInput = std::dynamic_pointer_cast<GPDSPBufferInputNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPBufferInputNode");
                                                        if (!bufferInput->isDelegate()) {
                                                            if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                                param.length = 0;
                                                                param.interleave = 0;
                                                                bufferInput->getBufferReadonly(&param.length, &param.interleave);
                                                                if ((error = writeTag(xml.param, "length", param.length)) == GPDSPERROR_OK) {
                                                                    error = writeTag(xml.param, "interleave", param.interleave);
                                                                }
                                                            }
                                                        }
                                                    }
                                                    else if ((bufferOutput = std::dynamic_pointer_cast<GPDSPBufferOutputNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPBufferOutputNode");
                                                        if (!bufferOutput->isDelegate()) {
                                                            if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                                param.length = 0;
                                                                param.interleave = 0;
                                                                bufferOutput->getBufferReadonly(&param.length, &param.interleave);
                                                                if ((error = writeTag(xml.param, "length", param.length)) == GPDSPERROR_OK) {
                                                                    error = writeTag(xml.param, "interleave", param.interleave);
                                                                }
                                                            }
                                                        }
                                                    }
                                                    else if ((constant = std::dynamic_pointer_cast<GPDSPConstantNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPConstantNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "constant", constant->getConstant());
                                                        }
                                                    }
                                                    else if ((sign = std::dynamic_pointer_cast<GPDSPSignNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPSignNode");
                                                    }
                                                    else if ((gate = std::dynamic_pointer_cast<GPDSPGateNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPGateNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            if ((error = writeTag(xml.param, "minimum", gate->getMinimum())) == GPDSPERROR_OK) {
                                                                error = writeTag(xml.param, "maximum", gate->getMaximum());
                                                            }
                                                        }
                                                    }
                                                    else if ((peak = std::dynamic_pointer_cast<GPDSPPeakNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPPeakNode");
                                                    }
                                                    else if ((amplify = std::dynamic_pointer_cast<GPDSPAmplifyNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPAmplifyNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "gain", amplify->getGain());
                                                        }
                                                    }
                                                    else if ((delay = std::dynamic_pointer_cast<GPDSPDelayNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPDelayNode");
                                                    }
                                                    else if ((buffer = std::dynamic_pointer_cast<GPDSPBufferNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPBufferNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "size", buffer->getSize());
                                                        }
                                                    }
                                                    else if ((sum = std::dynamic_pointer_cast<GPDSPSumNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPSumNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "count", sum->getCountI());
                                                        }
                                                    }
                                                    else if ((multiply = std::dynamic_pointer_cast<GPDSPMultiplyNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPMultiplyNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "count", multiply->getCountI());
                                                        }
                                                    }
                                                    else if ((squareRoot = std::dynamic_pointer_cast<GPDSPSquareRootNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPSquareRootNode");
                                                    }
                                                    else if ((sinWave = std::dynamic_pointer_cast<GPDSPSinWaveNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPSinWaveNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "resolution", sinWave->getResolution());
                                                        }
                                                    }
                                                    else if ((triangleWave = std::dynamic_pointer_cast<GPDSPTriangleWaveNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPTriangleWaveNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "resolution", triangleWave->getResolution());
                                                        }
                                                    }
                                                    else if ((sawtoothWave = std::dynamic_pointer_cast<GPDSPSawtoothWaveNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPSawtoothWaveNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "resolution", sawtoothWave->getResolution());
                                                        }
                                                    }
                                                    else if ((squareWave = std::dynamic_pointer_cast<GPDSPSquareWaveNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPSquareWaveNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "resolution", squareWave->getResolution());
                                                        }
                                                    }
                                                    else if ((generic = std::dynamic_pointer_cast<GPDSPGenericNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPGenericNode");
                                                        if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "file", generic->getFile());
                                                        }
                                                    }
                                                    else if ((genericInput = std::dynamic_pointer_cast<GPDSPGenericInputNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPGenericInputNode");
                                                    }
                                                    else if ((genericOutput = std::dynamic_pointer_cast<GPDSPGenericOutputNode const>(mit->second)) != NULL) {
                                                        xml.instance->SetName("GPDSPGenericOutputNode");
                                                    }
                                                    else if (serializable != NULL) {
                                                        if ((error = serializable->save(*this, mit->second, mit->first, xml.instance)) == GPDSPERROR_OK) {
                                                            if (xml.instance->Name() != NULL) {
                                                                if (strlen(xml.instance->Name()) <= 0) {
                                                                    error = GPDSPERROR_INVALID_FORMAT;
                                                                }
                                                            }
                                                            else {
                                                                error = GPDSPERROR_FAILED;
                                                            }
                                                        }
                                                    }
                                                    else {
                                                        error = GPDSPERROR_NO_SUPPORT;
                                                    }
                                                    if (error == GPDSPERROR_OK) {
                                                        error = writeInputTag(xml.instance, mit->second, mit->first);
                                                    }
                                                }
                                            }
                                            if (error != GPDSPERROR_OK) {
                                                break;
                                            }
                                        }
                                        if (error == GPDSPERROR_OK) {
                                            if (xml.formula->NoChildren()) {
                                                xml.gpdsp->DeleteChild(xml.formula);
                                            }
                                            if (xml.dom.SaveFile(file.c_str()) != tinyxml2::XML_SUCCESS) {
                                                error = GPDSPERROR_FAILED;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else {
                        error = GPDSPERROR_FAILED;
                    }
                }
                else {
                    error = GPDSPERROR_NO_MEMORY;
                }
            }
            else {
                error = GPDSPERROR_FAILED;
            }
        }
        else {
            error = GPDSPERROR_NO_MEMORY;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::addTag(tinyxml2::XMLElement* parent, std::string const& tag, tinyxml2::XMLElement** child) noexcept
{
    tinyxml2::XMLDocument* xml;
    tinyxml2::XMLElement* element;
    GPDSPError error(GPDSPERROR_OK);
    
    if (parent != NULL && child != NULL) {
        if ((xml = parent->GetDocument()) != NULL) {
            if ((element = xml->NewElement(tag.c_str())) != NULL) {
                if (parent->InsertEndChild(element) != NULL) {
                    *child = element;
                }
                else {
                    error = GPDSPERROR_FAILED;
                }
            }
            else {
                error = GPDSPERROR_NO_MEMORY;
            }
        }
        else {
            error = GPDSPERROR_FAILED;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeTag(tinyxml2::XMLElement* parent, std::string const& tag, int value) noexcept
{
    std::string string;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = stringize("", value, &string)) == GPDSPERROR_OK) {
        error = writeTag(parent, tag, string);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeTag(tinyxml2::XMLElement* parent, std::string const& tag, GPDSPFloat value) noexcept
{
    std::string string;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = stringize("", value, &string)) == GPDSPERROR_OK) {
        error = writeTag(parent, tag, string);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeTag(tinyxml2::XMLElement* parent, std::string const& tag, std::string const& value) noexcept
{
    tinyxml2::XMLDocument* xml;
    tinyxml2::XMLElement* element;
    tinyxml2::XMLText* text;
    GPDSPError error(GPDSPERROR_OK);
    
    if (parent != NULL) {
        if ((xml = parent->GetDocument()) != NULL) {
            if ((element = xml->NewElement(tag.c_str())) != NULL) {
                if (parent->InsertEndChild(element) != NULL) {
                    if ((text = xml->NewText(value.c_str())) != NULL) {
                        if (element->InsertEndChild(text) == NULL) {
                            error = GPDSPERROR_FAILED;
                        }
                    }
                    else {
                        error = GPDSPERROR_NO_MEMORY;
                    }
                }
                else {
                    error = GPDSPERROR_FAILED;
                }
            }
            else {
                error = GPDSPERROR_NO_MEMORY;
            }
        }
        else {
            error = GPDSPERROR_FAILED;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::readTag(tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, int* value) noexcept
{
    tinyxml2::XMLElement const* element;
    int value32;
    GPDSPError error(GPDSPERROR_OK);
    
    if (parent != NULL && value != NULL) {
        if ((element = parent->FirstChildElement(tag.c_str())) != NULL) {
            if (!element->NoChildren()) {
                if (element->QueryIntText(&value32) == tinyxml2::XML_SUCCESS) {
                    *value = value32;
                }
                else {
                    error = GPDSPERROR_INVALID_FORMAT;
                }
            }
        }
        else if (!implicit) {
            error = GPDSPERROR_INVALID_FORMAT;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::readTag(tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, int format, GPDSPFloat* value) noexcept
{
    tinyxml2::XMLElement const* element;
    union {
        float value32;
        double value64;
    };
    GPDSPError error(GPDSPERROR_OK);
    
    if (parent != NULL && value != NULL) {
        if ((element = parent->FirstChildElement(tag.c_str())) != NULL) {
            if (!element->NoChildren()) {
                switch (format) {
                    case sizeof(float) * 8:
                        if (element->QueryFloatText(&value32) == tinyxml2::XML_SUCCESS) {
                            *value = value32;
                        }
                        else {
                            error = GPDSPERROR_INVALID_FORMAT;
                        }
                        break;
                    case sizeof(double) * 8:
                        if (element->QueryDoubleText(&value64) == tinyxml2::XML_SUCCESS) {
                            *value = value64;
                        }
                        else {
                            error = GPDSPERROR_INVALID_FORMAT;
                        }
                        break;
                    default:
                        error = GPDSPERROR_INVALID_FORMAT;
                        break;
                }
            }
        }
        else if (!implicit) {
            error = GPDSPERROR_INVALID_FORMAT;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::readTag(tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, std::string* value) noexcept
{
    tinyxml2::XMLElement const* element;
    GPDSPError error(GPDSPERROR_OK);
    
    if (parent != NULL && value != NULL) {
        if ((element = parent->FirstChildElement(tag.c_str())) != NULL) {
            if (!element->NoChildren()) {
                if (element->GetText() != NULL) {
                    *value = element->GetText();
                }
                else {
                    error = GPDSPERROR_FAILED;
                }
            }
        }
        else if (!implicit) {
            error = GPDSPERROR_INVALID_FORMAT;
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

std::string GPDSPNodeRenderer::stringize(GPDSPError error) noexcept
{
    char result[256] = "\0";
    
    switch (error) {
        case GPDSPERROR_OK:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_OK)", error);
            break;
        case GPDSPERROR_WAIT:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_WAIT)", error);
            break;
        case GPDSPERROR_IGNORE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_IGNORE)", error);
            break;
        case GPDSPERROR_FRAGMENT:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_FRAGMENT)", error);
            break;
        case GPDSPERROR_LOOP:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_LOOP)", error);
            break;
        case GPDSPERROR_NO_SUPPORT:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_SUPPORT)", error);
            break;
        case GPDSPERROR_NO_FILE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_FILE)", error);
            break;
        case GPDSPERROR_NO_MEMORY:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_MEMORY)", error);
            break;
        case GPDSPERROR_NO_FOUND:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_FOUND)", error);
            break;
        case GPDSPERROR_NO_NODE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_NODE)", error);
            break;
        case GPDSPERROR_ALREADY_EXIST:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_ALREADY_EXIST)", error);
            break;
        case GPDSPERROR_INVALID_STATE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_INVALID_STATE)", error);
            break;
        case GPDSPERROR_INVALID_PARAM:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_INVALID_PARAM)", error);
            break;
        case GPDSPERROR_INVALID_RANGE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_INVALID_RANGE)", error);
            break;
        case GPDSPERROR_INVALID_FORMAT:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_INVALID_FORMAT)", error);
            break;
        case GPDSPERROR_INVALID_NODE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_INVALID_NODE)", error);
            break;
        case GPDSPERROR_FAILED:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_FAILED)", error);
            break;
        default:
            snprintf(result, sizeof(result), "%d (unknown error)", error);
            break;
    }
    return result;
}

GPDSPError GPDSPNodeRenderer::getNodeInputtable(std::string const& name, std::shared_ptr<GPDSPInputtableNode>* node) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode> input;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            *node = input;
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getNodeInputtable(std::string const& name, std::shared_ptr<GPDSPInputtableNode const>* node) const noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> input;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode const>(it->second)) != NULL) {
            *node = input;
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getNodeFlexInputtable(std::string const& name, std::shared_ptr<GPDSPFlexInputtableNode>* node) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPFlexInputtableNode> input;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((input = std::dynamic_pointer_cast<GPDSPFlexInputtableNode>(it->second)) != NULL) {
            *node = input;
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getNodeOutputtable(std::string const& name, std::shared_ptr<GPDSPOutputtableNode>* node) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((output = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            *node = output;
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getNodeOutputtable(std::string const& name, std::shared_ptr<GPDSPOutputtableNode const>* node) const noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((output = std::dynamic_pointer_cast<GPDSPOutputtableNode const>(it->second)) != NULL) {
            *node = output;
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::getNodeFlexOutputtable(std::string const& name, std::shared_ptr<GPDSPFlexOutputtableNode>* node) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPFlexOutputtableNode> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((output = std::dynamic_pointer_cast<GPDSPFlexOutputtableNode>(it->second)) != NULL) {
            *node = output;
        }
        else {
            error = GPDSPERROR_INVALID_NODE;
        }
    }
    else {
        error = GPDSPERROR_NO_NODE;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeGenericInput(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPGenericInputNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPGenericInputNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeGenericOutput(std::string const& name) noexcept
{
    std::shared_ptr<GPDSPGenericOutputNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPGenericOutputNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        error = appendNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::makeWait(void) noexcept
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator nit;
    std::vector<std::pair<GPDSPNode*, GPDSPError> >::const_iterator sit;
    GPDSPError error(GPDSPERROR_OK);
    
    for (nit = _node.begin(); nit != _node.end(); ++nit) {
        for (sit = _sequence.begin(); sit != _sequence.end(); ++sit) {
            if (nit->second.get() == sit->first) {
                if (sit->second == GPDSPERROR_OK) {
                    break;
                }
            }
        }
        if (sit == _sequence.end()) {
            try {
                _wait.push_back(nit->second.get());
            }
            catch (std::bad_alloc const&) {
                error = GPDSPERROR_NO_MEMORY;
            }
        }
        if (error != GPDSPERROR_OK) {
            break;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::makeSequence(void) noexcept
{
    std::vector<GPDSPNode*>::const_iterator it;
    std::vector<GPDSPNode*>::size_type size;
    std::vector<GPDSPNode*>::size_type pass;
    GPDSPError error(GPDSPERROR_OK);
    
    size = _sequence.size();
    while (_wait.size() > 0) {
        pass = _sequence.size();
        for (it = _wait.begin(); it != _wait.end(); ) {
            error = (*it)->process();
            switch (error) {
                case GPDSPERROR_OK:
                case GPDSPERROR_FRAGMENT:
                    try {
                        _sequence.push_back(std::make_pair(*it, error));
                    }
                    catch (std::bad_alloc const&) {
                        error = GPDSPERROR_NO_MEMORY;
                    }
                    if (error == GPDSPERROR_OK) {
                        it = _wait.erase(it);
                        break;
                    }
                    else if (error != GPDSPERROR_FRAGMENT) {
                        break;
                    }
                case GPDSPERROR_WAIT:
                    ++it;
                    error = GPDSPERROR_OK;
                    break;
                case GPDSPERROR_IGNORE:
                    it = _wait.erase(it);
                    error = GPDSPERROR_OK;
                    break;
                default:
                    break;
            }
            if (error != GPDSPERROR_OK) {
                break;
            }
        }
        if (error == GPDSPERROR_OK) {
            if (_sequence.size() <= pass) {
                if (_sequence.size() > size) {
                    try {
                        _sequence.push_back(std::make_pair(static_cast<GPDSPNode*>(NULL), GPDSPERROR_OK));
                    }
                    catch (std::bad_alloc const&) {
                        error = GPDSPERROR_NO_MEMORY;
                    }
                    if (error == GPDSPERROR_OK) {
                        error = GPDSPERROR_FRAGMENT;
                    }
                }
                else {
                    error = GPDSPERROR_LOOP;
                }
            }
        }
        if (error != GPDSPERROR_OK) {
            break;
        }
    }
    if (error == GPDSPERROR_OK) {
        _jit = _sequence.begin();
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::optimizeSequence(void) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    while (_jit != _sequence.end()) {
        if (_jit->first != NULL) {
            error = _jit->first->process();
            switch (error) {
                case GPDSPERROR_OK:
                case GPDSPERROR_FRAGMENT:
                    if (error == _jit->second) {
                        ++_jit;
                        error = GPDSPERROR_OK;
                        break;
                    }
                    else {
                        _jit->second = error;
                        ++_jit;
                        error = GPDSPERROR_WAIT;
                    }
                case GPDSPERROR_WAIT:
                    _jit = _sequence.erase(_jit, _sequence.end());
                    break;
                default:
                    break;
            }
        }
        else {
            ++_jit;
            error = GPDSPERROR_FRAGMENT;
        }
        if (error != GPDSPERROR_OK) {
            break;
        }
    }
    if (error == GPDSPERROR_OK) {
        if (_jit == _sequence.end()) {
            _jit = _sequence.begin();
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeInputTag(tinyxml2::XMLElement* parent, std::shared_ptr<GPDSPNode const> const& node, std::string const& name) const noexcept
{
    tinyxml2::XMLElement* element;
    tinyxml2::XMLElement* terminal;
    std::shared_ptr<GPDSPInputtableNode const> input;
    std::string string;
    GPDSPMode mode;
    GPDSPFloat value;
    int index;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode const>(node)) != NULL) {
        if ((error = addTag(parent, "input", &element)) == GPDSPERROR_OK) {
            for (i = 0; i < input->getCountI(); ++i) {
                if ((error = stringize("::", i, &string)) == GPDSPERROR_OK) {
                    if ((error = addTag(element, string, &terminal)) == GPDSPERROR_OK) {
                        if ((error = input->getModeI(i, &mode)) == GPDSPERROR_OK) {
                            switch (mode) {
                                case GPDSPMODE_NONE:
                                    break;
                                case GPDSPMODE_POSITIVE:
                                    if ((error = getLinkI(name, i, &string, &index)) == GPDSPERROR_OK) {
                                        error = writeLinkTag(terminal, string, index, true);
                                    }
                                    break;
                                case GPDSPMODE_NEGATIVE:
                                    if ((error = getLinkI(name, i, &string, &index)) == GPDSPERROR_OK) {
                                        error = writeLinkTag(terminal, string, index, false);
                                    }
                                    break;
                                case GPDSPMODE_CONSTANT:
                                    if ((error = getLinkI(name, i, &value)) == GPDSPERROR_OK) {
                                        error = writeLinkTag(terminal, value);
                                    }
                                    break;
                                default:
                                    error = GPDSPERROR_FAILED;
                                    break;
                            }
                        }
                        if (error == GPDSPERROR_OK) {
                            if (terminal->NoChildren()) {
                                element->DeleteChild(terminal);
                            }
                        }
                    }
                }
                if (error != GPDSPERROR_OK) {
                    break;
                }
            }
            if (error == GPDSPERROR_OK) {
                if (element->NoChildren()) {
                    parent->DeleteChild(element);
                }
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::readInputTag(tinyxml2::XMLElement const* parent, std::string const& name, int format) noexcept
{
    tinyxml2::XMLElement const* element;
    tinyxml2::XMLElement const* terminal;
    std::string string;
    GPDSPFloat value;
    int number;
    int index;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((element = parent->FirstChildElement("input")) != NULL) {
        for (terminal = element->FirstChildElement(); terminal != NULL; terminal = terminal->NextSiblingElement()) {
            if (terminal->Name() != NULL) {
                if ((error = numberize("::", terminal->Name(), &number)) == GPDSPERROR_OK) {
                    if (number >= 0) {
                        if (terminal->FirstChildElement("constant") == NULL) {
                            if (terminal->FirstChildElement("negative") == NULL) {
                                if ((error = readLinkTag(terminal, &string, &index)) == GPDSPERROR_OK) {
                                    if (string != "") {
                                        error = setLinkPositiveI(name, number, string, index);
                                    }
                                }
                            }
                            else if (terminal->FirstChildElement("positive") == NULL) {
                                if ((error = readLinkTag(terminal, &string, &index)) == GPDSPERROR_OK) {
                                    if (string != "") {
                                        error = setLinkNegativeI(name, number, string, index);
                                    }
                                }
                            }
                            else {
                                error = GPDSPERROR_INVALID_FORMAT;
                            }
                        }
                        else if (terminal->FirstChildElement("positive") == NULL && terminal->FirstChildElement("negative") == NULL) {
                            if ((error = readLinkTag(terminal, format, &value)) == GPDSPERROR_OK) {
                                error = setLinkConstantI(name, number, value);
                            }
                        }
                        else {
                            error = GPDSPERROR_INVALID_FORMAT;
                        }
                    }
                    else {
                        error = GPDSPERROR_INVALID_RANGE;
                    }
                }
            }
            else {
                error = GPDSPERROR_FAILED;
            }
            if (error != GPDSPERROR_OK) {
                break;
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::countInputTag(tinyxml2::XMLElement const* parent, int* count) noexcept
{
    tinyxml2::XMLElement const* element;
    tinyxml2::XMLElement const* terminal;
    int maximum;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    maximum = 0;
    if ((element = parent->FirstChildElement("input")) != NULL) {
        for (terminal = element->FirstChildElement(); terminal != NULL; terminal = terminal->NextSiblingElement()) {
            if (terminal->Name() != NULL) {
                if ((error = numberize("::", terminal->Name(), &number)) == GPDSPERROR_OK) {
                    if (number >= 0) {
                        if (number + 1 > maximum) {
                            maximum = number + 1;
                        }
                    }
                    else {
                        error = GPDSPERROR_INVALID_RANGE;
                    }
                }
            }
            else {
                error = GPDSPERROR_FAILED;
            }
            if (error != GPDSPERROR_OK) {
                break;
            }
        }
    }
    if (error == GPDSPERROR_OK) {
        *count = maximum;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeRateTag(tinyxml2::XMLElement* parent, int rate) noexcept
{
    return writeTag(parent, "rate", rate);
}

GPDSPError GPDSPNodeRenderer::readRateTag(tinyxml2::XMLElement const* parent, int rate) noexcept
{
    tinyxml2::XMLElement const* element;
    int minimum;
    int maximum;
    GPDSPError error(GPDSPERROR_OK);
    
    minimum = 0;
    if ((error = readTag(parent, "rate", false, &minimum)) == GPDSPERROR_OK) {
        if (minimum >= 0) {
            if (minimum > 0 && rate > 0 && minimum != rate) {
                error = GPDSPERROR_NO_FOUND;
            }
        }
        else {
            error = GPDSPERROR_INVALID_RANGE;
        }
    }
    else if ((element = parent->FirstChildElement("rate")) != NULL) {
        minimum = 0;
        if ((error = readTag(element, "minimum", true, &minimum)) == GPDSPERROR_OK) {
            if (minimum >= 0) {
                maximum = 0;
                if ((error = readTag(element, "maximum", true, &maximum)) == GPDSPERROR_OK) {
                    if (maximum >= 0) {
                        if (maximum <= 0) {
                            maximum = INT_MAX;
                        }
                        if (rate > 0 && (rate < minimum || maximum < rate)) {
                            error = GPDSPERROR_NO_FOUND;
                        }
                    }
                    else {
                        error = GPDSPERROR_INVALID_RANGE;
                    }
                }
            }
            else {
                error = GPDSPERROR_INVALID_RANGE;
            }
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeFormatTag(tinyxml2::XMLElement* parent) noexcept
{
    return writeTag(parent, "format", static_cast<int>(sizeof(GPDSPFloat) * 8));
}

GPDSPError GPDSPNodeRenderer::readFormatTag(tinyxml2::XMLElement const* parent, int* format) noexcept
{
    int value;
    GPDSPError error(GPDSPERROR_OK);
    
    value = sizeof(GPDSPFloat) * 8;
    if ((error = readTag(parent, "format", false, &value)) == GPDSPERROR_OK) {
        *format = value;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeLinkTag(tinyxml2::XMLElement* parent, std::string const& node, int index, bool positive) noexcept
{
    tinyxml2::XMLElement* element;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = addTag(parent, (positive) ? ("positive") : ("negative"), &element)) == GPDSPERROR_OK) {
        if ((error = writeTag(parent, "node", node)) == GPDSPERROR_OK) {
            error = writeTag(parent, "index", index);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeLinkTag(tinyxml2::XMLElement* parent, GPDSPFloat constant) noexcept
{
    return writeTag(parent, "constant", constant);
}

GPDSPError GPDSPNodeRenderer::readLinkTag(tinyxml2::XMLElement const* parent, std::string* node, int* index) noexcept
{
    std::string string;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    string = "";
    if ((error = readTag(parent, "node", false, &string)) == GPDSPERROR_OK) {
        number = 0;
        if ((error = readTag(parent, "index", true, &number)) == GPDSPERROR_OK) {
            *node = string;
            *index = number;
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::readLinkTag(tinyxml2::XMLElement const* parent, int format, GPDSPFloat* constant) noexcept
{
    GPDSPFloat value;
    GPDSPError error(GPDSPERROR_OK);
    
    value = GPDSPFV(0.0);
    if ((error = readTag(parent, "constant", false, format, &value)) == GPDSPERROR_OK) {
        *constant = value;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::stringize(std::string const& prefix, int param, std::string* result) noexcept
{
    char string[256];
    GPDSPError error(GPDSPERROR_OK);
    
    if (snprintf(string, sizeof(string), "%d", param) >= 0) {
        *result = prefix + string;
    }
    else {
        error = GPDSPERROR_FAILED;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::stringize(std::string const& prefix, GPDSPFloat param, std::string* result) noexcept
{
    char string[256];
    GPDSPError error(GPDSPERROR_OK);
    
    if (snprintf(string, sizeof(string), GPDSPFP, param) >= 0) {
        *result = prefix + string;
    }
    else {
        error = GPDSPERROR_FAILED;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::numberize(std::string const& prefix, std::string const& param, int* result) noexcept
{
    int value;
    GPDSPError error(GPDSPERROR_OK);
    
    if (sscanf(param.c_str(), (prefix + "%d").c_str(), &value) == 1) {
        *result = value;
    }
    else {
        error = GPDSPERROR_INVALID_FORMAT;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::numberize(std::string const& prefix, std::string const& param, GPDSPFloat* result) noexcept
{
    GPDSPFloat value;
    GPDSPError error(GPDSPERROR_OK);
    
    if (sscanf(param.c_str(), (prefix + GPDSPFS).c_str(), &value) == 1) {
        *result = value;
    }
    else {
        error = GPDSPERROR_INVALID_FORMAT;
    }
    return error;
}

}// end of namespace
