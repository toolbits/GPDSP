/*
**      General Purpose DSP Library
**
**      Original Copyright (C) 2017 - 2017 HORIGUCHI Junshi.
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
**      Xcode 9.0 (Apple LLVM 9.0.0)
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
#include "../algorithm/GPDSPBufferInputNode.hpp"
#include "../algorithm/GPDSPBufferOutputNode.hpp"
#include "../algorithm/GPDSPConstantNode.hpp"
#include "../algorithm/GPDSPGateNode.hpp"
#include "../algorithm/GPDSPPeekNode.hpp"
#include "../algorithm/GPDSPAmplifyNode.hpp"
#include "../algorithm/GPDSPDelayNode.hpp"
#include "../algorithm/GPDSPBufferNode.hpp"
#include "../algorithm/GPDSPSumNode.hpp"
#include "../algorithm/GPDSPMultiplyNode.hpp"
#include "../generic/GPDSPGenericNode.hpp"
#include "../generic/GPDSPGenericInputNode.hpp"
#include "../generic/GPDSPGenericOutputNode.hpp"

namespace ir {

GPDSPError GPDSPNodeRenderer::setRate(int rate)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (rate >= 0) {
        if (_node.size() <= 0) {
            _rate = rate;
        }
        else {
            error = GPDSPERROR_INVALID_STATE;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

std::shared_ptr<GPDSPBufferInputNode> GPDSPNodeRenderer::getNodeBufferInput(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPBufferInputNode>(getNode(name));
}

std::shared_ptr<GPDSPBufferOutputNode> GPDSPNodeRenderer::getNodeBufferOutput(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPBufferOutputNode>(getNode(name));
}

std::shared_ptr<GPDSPConstantNode> GPDSPNodeRenderer::getNodeConstant(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPConstantNode>(getNode(name));
}

std::shared_ptr<GPDSPGateNode> GPDSPNodeRenderer::getNodeGate(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPGateNode>(getNode(name));
}

std::shared_ptr<GPDSPPeekNode> GPDSPNodeRenderer::getNodePeek(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPPeekNode>(getNode(name));
}

std::shared_ptr<GPDSPAmplifyNode> GPDSPNodeRenderer::getNodeAmplify(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPAmplifyNode>(getNode(name));
}

std::shared_ptr<GPDSPDelayNode> GPDSPNodeRenderer::getNodeDelay(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPDelayNode>(getNode(name));
}

std::shared_ptr<GPDSPBufferNode> GPDSPNodeRenderer::getNodeBuffer(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPBufferNode>(getNode(name));
}

std::shared_ptr<GPDSPSumNode> GPDSPNodeRenderer::getNodeSum(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPSumNode>(getNode(name));
}

std::shared_ptr<GPDSPMultiplyNode> GPDSPNodeRenderer::getNodeMultiply(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPMultiplyNode>(getNode(name));
}

std::shared_ptr<GPDSPGenericNode> GPDSPNodeRenderer::getNodeGeneric(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPGenericNode>(getNode(name));
}

std::shared_ptr<GPDSPNode> GPDSPNodeRenderer::getNode(std::string const& name) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPNode> result;
    
    if ((it = _node.find(name)) != _node.end()) {
        result = it->second;
    }
    return result;
}

GPDSPError GPDSPNodeRenderer::getCountI(std::string const& name, int* count) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if (count != NULL) {
            if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
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

GPDSPError GPDSPNodeRenderer::getCountO(std::string const& name, int* count) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if (count != NULL) {
            if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
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

GPDSPError GPDSPNodeRenderer::setNameI(std::string const& name, int index, std::string const& what)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            error = node->setNameI(index, what);
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

GPDSPError GPDSPNodeRenderer::getNameI(std::string const& name, int index, std::string* what) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            error = node->getNameI(index, what);
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

GPDSPError GPDSPNodeRenderer::setNameO(std::string const& name, int index, std::string const& what)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            error = node->setNameO(index, what);
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

GPDSPError GPDSPNodeRenderer::getNameO(std::string const& name, int index, std::string* what) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            error = node->getNameO(index, what);
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

GPDSPError GPDSPNodeRenderer::setLinkI(std::string const& name, int index, std::string const& source, int which)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode> input;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            if ((it = _node.find(source)) != _node.end()) {
                if ((output = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
                    error = input->setLinkI(index, output.get(), which);
                }
                else {
                    error = GPDSPERROR_INVALID_NODE;
                }
            }
            else {
                error = GPDSPERROR_NO_NODE;
            }
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

GPDSPError GPDSPNodeRenderer::getLinkI(std::string const& name, int index, std::string* source, int* which) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> input;
    GPDSPOutputtableNode const* node;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            if ((error = input->getLinkI(index, &node, &number)) == GPDSPERROR_OK) {
                if (node != NULL) {
                    for (it = _node.begin(); it != _node.end(); ++it) {
                        if (it->second.get() == node) {
                            break;
                        }
                    }
                    if (it != _node.end()) {
                        if (source != NULL) {
                            *source = it->first;
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
                    if (source != NULL) {
                        *source = "";
                    }
                    if (which != NULL) {
                        *which = 0;
                    }
                }
            }
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

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name, int index)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            error = node->setLinkI(index, NULL, 0);
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

GPDSPError GPDSPNodeRenderer::clearLinkI(std::string const& name)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode> node;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            for (i = 0; i < node->getCountI(); ++i) {
                node->setLinkI(i, NULL, 0);
            }
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

GPDSPError GPDSPNodeRenderer::clearLinkO(std::string const& name, int index)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    std::shared_ptr<GPDSPInputtableNode> input;
    GPDSPOutputtableNode const* node;
    int number;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((output = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            for (it = _node.begin(); it != _node.end(); ++it) {
                if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
                    for (i = 0; i < input->getCountI(); ++i) {
                        if (input->getLinkI(i, &node, &number) == GPDSPERROR_OK) {
                            if (node == output.get() && number == index) {
                                input->setLinkI(i, NULL, 0);
                            }
                        }
                    }
                }
            }
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

GPDSPError GPDSPNodeRenderer::clearLinkO(std::string const& name)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> output;
    std::shared_ptr<GPDSPInputtableNode> input;
    GPDSPOutputtableNode const* node;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((output = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            for (it = _node.begin(); it != _node.end(); ++it) {
                if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
                    for (i = 0; i < input->getCountI(); ++i) {
                        if (input->getLinkI(i, &node, NULL) == GPDSPERROR_OK) {
                            if (node == output.get()) {
                                input->setLinkI(i, NULL, 0);
                            }
                        }
                    }
                }
            }
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

GPDSPError GPDSPNodeRenderer::getValueI(std::string const& name, int index, float* value) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            error = node->getValueI(index, value);
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

GPDSPError GPDSPNodeRenderer::getValueO(std::string const& name, int index, float* value) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode const> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            error = node->getValueO(index, value);
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

std::string GPDSPNodeRenderer::getNextNode(void) const
{
    std::string result;
    
    if (_nit != _node.end()) {
        result = _nit->first;
        ++_nit;
    }
    return result;
}

std::string GPDSPNodeRenderer::findNode(std::shared_ptr<GPDSPNode const> const& node) const
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

GPDSPError GPDSPNodeRenderer::newNodeBufferInput(std::string const& name, float const* buffer, int length, int interleave)
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
            error = newNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeBufferOutput(std::string const& name, float* buffer, int length, int interleave)
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
            error = newNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeConstant(std::string const& name, float constant)
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
        error = newNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeGate(std::string const& name, float minimum, float maximum)
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
        error = newNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodePeek(std::string const& name)
{
    std::shared_ptr<GPDSPPeekNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        node = std::make_shared<GPDSPPeekNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        error = newNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeAmplify(std::string const& name, float gain)
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
        error = newNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeDelay(std::string const& name)
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
        error = newNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeBuffer(std::string const& name, int size)
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
            error = newNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeSum(std::string const& name, int count)
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
            error = newNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeMultiply(std::string const& name, int count)
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
            error = newNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeGeneric(std::string const& name, std::string const& file)
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
            error = newNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNode(std::string const& name, std::shared_ptr<GPDSPNode> const& node)
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

GPDSPError GPDSPNodeRenderer::deleteNode(std::string const& name)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = clearLinkO(name)) == GPDSPERROR_OK) {
        _sequence.clear();
        _wait.clear();
        _node.erase(name);
        _nit = _node.end();
    }
    return error;
}

void GPDSPNodeRenderer::clearNode(void)
{
    _sequence.clear();
    _wait.clear();
    _node.clear();
    _nit = _node.end();
    return;
}

void GPDSPNodeRenderer::invalidate(void)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        it->second->invalidate();
    }
    return;
}

GPDSPError GPDSPNodeRenderer::prepare(void)
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

GPDSPError GPDSPNodeRenderer::process(void)
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

GPDSPError GPDSPNodeRenderer::render(int count, int* remain)
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

GPDSPError GPDSPNodeRenderer::rewind(std::string const& name)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRewindableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPRewindableNode>(it->second)) != NULL) {
            node->rewind();
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

void GPDSPNodeRenderer::rewind(void)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRewindableNode> node;
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        if ((node = std::dynamic_pointer_cast<GPDSPRewindableNode>(it->second)) != NULL) {
            node->rewind();
        }
    }
    return;
}

GPDSPError GPDSPNodeRenderer::refresh(std::string const& name)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRefreshableNode> node;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPRefreshableNode>(it->second)) != NULL) {
            node->refresh();
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

void GPDSPNodeRenderer::refresh(void)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPRefreshableNode> node;
    
    for (it = _node.begin(); it != _node.end(); ++it) {
        if ((node = std::dynamic_pointer_cast<GPDSPRefreshableNode>(it->second)) != NULL) {
            node->refresh();
        }
    }
    return;
}

GPDSPError GPDSPNodeRenderer::load(std::string const& file, GPDSPError (*callback)(GPDSPNodeRenderer& renderer, std::string const& type, std::string const& name, tinyxml2::XMLElement const* element, void* info), void* info)
{
    struct {
        tinyxml2::XMLDocument dom;
        tinyxml2::XMLElement const* gpdsp;
        union {
            struct {
                tinyxml2::XMLElement const* meta;
                tinyxml2::XMLElement const* rate;
            };
            struct {
                tinyxml2::XMLElement const* formula;
                tinyxml2::XMLElement const* instance;
                union {
                    struct {
                        tinyxml2::XMLElement const* param;
                    };
                    struct {
                        tinyxml2::XMLElement const* input;
                        tinyxml2::XMLElement const* node;
                    };
                };
            };
        };
    } xml;
    std::string name;
    std::string string;
    union {
        struct {
            int length;
            int interval;
        };
        float constant;
        struct {
            float minimum;
            float maximum;
        };
        float gain;
        int size;
        int count;
    } param;
    int minimum;
    int maximum;
    int index;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    switch (xml.dom.LoadFile(file.c_str())) {
        case tinyxml2::XML_SUCCESS:
            if ((xml.gpdsp = xml.dom.FirstChildElement("gpdsp")) != NULL) {
                for (; xml.gpdsp != NULL; xml.gpdsp = xml.gpdsp->NextSiblingElement("gpdsp")) {
                    if ((xml.meta = xml.gpdsp->FirstChildElement("meta")) != NULL) {
                        minimum = 0;
                        if ((error = readTag(xml.meta, "rate", false, &minimum)) == GPDSPERROR_OK) {
                            if (minimum >= 0) {
                                if (minimum > 0 && _rate > 0 && minimum != _rate) {
                                    error = GPDSPERROR_NO_RATE;
                                }
                            }
                            else {
                                error = GPDSPERROR_INVALID_RANGE;
                            }
                        }
                        else if ((xml.rate = xml.meta->FirstChildElement("rate")) != NULL) {
                            minimum = 0;
                            if ((error = readTag(xml.rate, "minimum", true, &minimum)) == GPDSPERROR_OK) {
                                if (minimum >= 0) {
                                    maximum = 0;
                                    if ((error = readTag(xml.rate, "maximum", true, &maximum)) == GPDSPERROR_OK) {
                                        if (maximum >= 0) {
                                            if (maximum <= 0) {
                                                maximum = INT_MAX;
                                            }
                                            if (_rate > 0 && (_rate < minimum || maximum < _rate)) {
                                                error = GPDSPERROR_NO_RATE;
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
                        else {
                            error = GPDSPERROR_INVALID_FORMAT;
                        }
                        if (error == GPDSPERROR_OK) {
                            if ((xml.formula = xml.gpdsp->FirstChildElement("formula")) != NULL) {
                                for (xml.instance = xml.formula->FirstChildElement(); xml.instance != NULL; xml.instance = xml.instance->NextSiblingElement()) {
                                    name = "";
                                    if ((error = readTag(xml.instance, "name", false, &name)) == GPDSPERROR_OK) {
                                        if (xml.instance->Name() != NULL) {
                                            string = xml.instance->Name();
                                            if (string == "GPDSPBufferInputNode") {
                                                param.length = 0;
                                                param.interval = 0;
                                                if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                    if ((error = readTag(xml.param, "length", true, &param.length)) == GPDSPERROR_OK) {
                                                        error = readTag(xml.param, "interval", true, &param.interval);
                                                    }
                                                }
                                                if (error == GPDSPERROR_OK) {
                                                    error = newNodeBufferInput(name, NULL, param.length, param.interval);
                                                }
                                            }
                                            else if (string == "GPDSPBufferOutputNode") {
                                                param.length = 0;
                                                param.interval = 0;
                                                if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                    if ((error = readTag(xml.param, "length", true, &param.length)) == GPDSPERROR_OK) {
                                                        error = readTag(xml.param, "interval", true, &param.interval);
                                                    }
                                                }
                                                if (error == GPDSPERROR_OK) {
                                                    error = newNodeBufferOutput(name, NULL, param.length, param.interval);
                                                }
                                            }
                                            else if (string == "GPDSPConstantNode") {
                                                param.constant = GPDSPConstantNode::defaultConstant();
                                                if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                    error = readTag(xml.param, "constant", true, &param.constant);
                                                }
                                                if (error == GPDSPERROR_OK) {
                                                    error = newNodeConstant(name, param.constant);
                                                }
                                            }
                                            else if (string == "GPDSPGateNode") {
                                                param.minimum = GPDSPGateNode::defaultMinimum();
                                                param.maximum = GPDSPGateNode::defaultMaximum();
                                                if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                    if ((error = readTag(xml.param, "minimum", true, &param.minimum)) == GPDSPERROR_OK) {
                                                        error = readTag(xml.param, "maximum", true, &param.maximum);
                                                    }
                                                }
                                                if (error == GPDSPERROR_OK) {
                                                    error = newNodeGate(name, param.minimum, param.maximum);
                                                }
                                            }
                                            else if (string == "GPDSPPeekNode") {
                                                error = newNodePeek(name);
                                            }
                                            else if (string == "GPDSPAmplifyNode") {
                                                param.gain = GPDSPAmplifyNode::defaultGain();
                                                if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                    error = readTag(xml.param, "gain", true, &param.gain);
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
                                                param.count = 0;
                                                if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                    error = readTag(xml.param, "count", true, &param.count);
                                                }
                                                if (error == GPDSPERROR_OK) {
                                                    error = newNodeSum(name, param.count);
                                                }
                                            }
                                            else if (string == "GPDSPMultiplyNode") {
                                                param.count = 0;
                                                if ((xml.param = xml.instance->FirstChildElement("param")) != NULL) {
                                                    error = readTag(xml.param, "count", true, &param.count);
                                                }
                                                if (error == GPDSPERROR_OK) {
                                                    error = newNodeMultiply(name, param.count);
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
                                            else if (callback != NULL) {
                                                error = (*callback)(*this, string, name, xml.instance, info);
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
                                            if ((xml.input = xml.instance->FirstChildElement("input")) != NULL) {
                                                for (xml.node = xml.input->FirstChildElement("node"), i = 0; xml.node != NULL; xml.node = xml.node->NextSiblingElement("node"), ++i) {
                                                    string = "";
                                                    if ((error = readTag(xml.node, "name", false, &string)) == GPDSPERROR_OK) {
                                                        index = 0;
                                                        if ((error = readTag(xml.node, "index", true, &index)) == GPDSPERROR_OK) {
                                                            if (string != "") {
                                                                error = setLinkI(name, i, string, index);
                                                            }
                                                        }
                                                    }
                                                    if (error != GPDSPERROR_OK) {
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if (error != GPDSPERROR_OK) {
                                            break;
                                        }
                                    }
                                }
                            }
                            else {
                                error = GPDSPERROR_INVALID_FORMAT;
                            }
                        }
                    }
                    else {
                        error = GPDSPERROR_INVALID_FORMAT;
                    }
                    if (error != GPDSPERROR_NO_RATE) {
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
    return error;
}

GPDSPError GPDSPNodeRenderer::save(std::string const& file, GPDSPError (*callback)(GPDSPNodeRenderer const& renderer, std::shared_ptr<GPDSPNode const> const& node, std::string const& name, tinyxml2::XMLElement* element, void* info), void* info) const
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode const> input;
    std::shared_ptr<GPDSPBufferInputNode const> bufferInput;
    std::shared_ptr<GPDSPBufferOutputNode const> bufferOutput;
    std::shared_ptr<GPDSPConstantNode const> constant;
    std::shared_ptr<GPDSPGateNode const> gate;
    std::shared_ptr<GPDSPPeekNode const> peek;
    std::shared_ptr<GPDSPAmplifyNode const> amplify;
    std::shared_ptr<GPDSPDelayNode const> delay;
    std::shared_ptr<GPDSPBufferNode const> buffer;
    std::shared_ptr<GPDSPSumNode const> sum;
    std::shared_ptr<GPDSPMultiplyNode const> multiply;
    std::shared_ptr<GPDSPGenericNode const> generic;
    std::shared_ptr<GPDSPGenericInputNode const> genericInput;
    std::shared_ptr<GPDSPGenericOutputNode const> genericOutput;
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
                union {
                    struct {
                        tinyxml2::XMLElement* param;
                    };
                    struct {
                        tinyxml2::XMLElement* input;
                        tinyxml2::XMLElement* node;
                    };
                };
            };
        };
    } xml;
    std::string string;
    union {
        struct {
            int length;
            int interval;
        };
    } param;
    int index;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((xml.decl = xml.dom.NewDeclaration()) != NULL) {
        if (xml.dom.InsertEndChild(xml.decl) != NULL) {
            if ((xml.gpdsp = xml.dom.NewElement("gpdsp")) != NULL) {
                if (xml.dom.InsertEndChild(xml.gpdsp) != NULL) {
                    if ((error = addTag(xml.gpdsp, "meta", &xml.meta)) == GPDSPERROR_OK) {
                        if ((error = writeTag(xml.meta, "rate", _rate)) == GPDSPERROR_OK) {
                            if ((error = addTag(xml.gpdsp, "formula", &xml.formula)) == GPDSPERROR_OK) {
                                for (it = _node.begin(); it != _node.end(); ++it) {
                                    if ((error = addTag(xml.formula, "", &xml.instance)) == GPDSPERROR_OK) {
                                        if ((error = writeTag(xml.instance, "name", it->first)) == GPDSPERROR_OK) {
                                            if ((bufferInput = std::dynamic_pointer_cast<GPDSPBufferInputNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPBufferInputNode");
                                                if (!bufferInput->isDelegate()) {
                                                    if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                        param.length = 0;
                                                        param.interval = 0;
                                                        bufferInput->getReadonlyBuffer(&param.length, &param.interval);
                                                        if ((error = writeTag(xml.param, "length", param.length)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "interval", param.interval);
                                                        }
                                                    }
                                                }
                                            }
                                            else if ((bufferOutput = std::dynamic_pointer_cast<GPDSPBufferOutputNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPBufferOutputNode");
                                                if (!bufferOutput->isDelegate()) {
                                                    if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                        param.length = 0;
                                                        param.interval = 0;
                                                        bufferOutput->getReadonlyBuffer(&param.length, &param.interval);
                                                        if ((error = writeTag(xml.param, "length", param.length)) == GPDSPERROR_OK) {
                                                            error = writeTag(xml.param, "interval", param.interval);
                                                        }
                                                    }
                                                }
                                            }
                                            else if ((constant = std::dynamic_pointer_cast<GPDSPConstantNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPConstantNode");
                                                if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                    error = writeTag(xml.param, "constant", constant->getConstant());
                                                }
                                            }
                                            else if ((gate = std::dynamic_pointer_cast<GPDSPGateNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPGateNode");
                                                if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                    if ((error = writeTag(xml.param, "minimum", gate->getMinimum())) == GPDSPERROR_OK) {
                                                        error = writeTag(xml.param, "maximum", gate->getMaximum());
                                                    }
                                                }
                                            }
                                            else if ((peek = std::dynamic_pointer_cast<GPDSPPeekNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPPeekNode");
                                            }
                                            else if ((amplify = std::dynamic_pointer_cast<GPDSPAmplifyNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPAmplifyNode");
                                                if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                    error = writeTag(xml.param, "gain", amplify->getGain());
                                                }
                                            }
                                            else if ((delay = std::dynamic_pointer_cast<GPDSPDelayNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPDelayNode");
                                            }
                                            else if ((buffer = std::dynamic_pointer_cast<GPDSPBufferNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPBufferNode");
                                                if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                    error = writeTag(xml.param, "size", buffer->getSize());
                                                }
                                            }
                                            else if ((sum = std::dynamic_pointer_cast<GPDSPSumNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPSumNode");
                                                if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                    error = writeTag(xml.param, "count", sum->getCountI());
                                                }
                                            }
                                            else if ((multiply = std::dynamic_pointer_cast<GPDSPMultiplyNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPMultiplyNode");
                                                if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                    error = writeTag(xml.param, "count", multiply->getCountI());
                                                }
                                            }
                                            else if ((generic = std::dynamic_pointer_cast<GPDSPGenericNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPGenericNode");
                                                if ((error = addTag(xml.instance, "param", &xml.param)) == GPDSPERROR_OK) {
                                                    error = writeTag(xml.param, "file", generic->getFile());
                                                }
                                            }
                                            else if ((genericInput = std::dynamic_pointer_cast<GPDSPGenericInputNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPGenericInputNode");
                                            }
                                            else if ((genericOutput = std::dynamic_pointer_cast<GPDSPGenericOutputNode>(it->second)) != NULL) {
                                                xml.instance->SetName("GPDSPGenericOutputNode");
                                            }
                                            else if (callback != NULL) {
                                                if ((error = (*callback)(*this, it->second, it->first, xml.instance, info)) == GPDSPERROR_OK) {
                                                    if (xml.instance->Name() != NULL) {
                                                        if ((string = xml.instance->Name()) == "") {
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
                                                if ((input = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
                                                    if (input->getCountI() > 0) {
                                                        if ((error = addTag(xml.instance, "input", &xml.input)) == GPDSPERROR_OK) {
                                                            for (i = 0; i < input->getCountI(); ++i) {
                                                                if ((error = getLinkI(it->first, i, &string, &index)) == GPDSPERROR_OK) {
                                                                    if ((error = addTag(xml.input, "node", &xml.node)) == GPDSPERROR_OK) {
                                                                        if ((error = writeTag(xml.node, "name", string)) == GPDSPERROR_OK) {
                                                                            error = writeTag(xml.node, "index", index);
                                                                        }
                                                                    }
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
                                    }
                                    if (error != GPDSPERROR_OK) {
                                        break;
                                    }
                                }
                                if (error == GPDSPERROR_OK) {
                                    if (xml.dom.SaveFile(file.c_str()) != tinyxml2::XML_SUCCESS) {
                                        error = GPDSPERROR_FAILED;
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
    return error;
}

GPDSPError GPDSPNodeRenderer::addTag(tinyxml2::XMLElement* parent, std::string const& tag, tinyxml2::XMLElement** child)
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

GPDSPError GPDSPNodeRenderer::writeTag(tinyxml2::XMLElement* parent, std::string const& tag, int value)
{
    char string[256];
    GPDSPError error(GPDSPERROR_OK);
    
    if (snprintf(string, sizeof(string), "%d", value) >= 0) {
        error = writeTag(parent, tag, string);
    }
    else {
        error = GPDSPERROR_FAILED;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeTag(tinyxml2::XMLElement* parent, std::string const& tag, float value)
{
    char string[256];
    GPDSPError error(GPDSPERROR_OK);
    
    if (snprintf(string, sizeof(string), "%f", value) >= 0) {
        error = writeTag(parent, tag, string);
    }
    else {
        error = GPDSPERROR_FAILED;
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::writeTag(tinyxml2::XMLElement* parent, std::string const& tag, std::string const& value)
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

GPDSPError GPDSPNodeRenderer::readTag(tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, int* value)
{
    tinyxml2::XMLElement const* element;
    GPDSPError error(GPDSPERROR_OK);
    
    if (parent != NULL && value != NULL) {
        if ((element = parent->FirstChildElement(tag.c_str())) != NULL) {
            if (!element->NoChildren()) {
                if (element->QueryIntText(value) != tinyxml2::XML_SUCCESS) {
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

GPDSPError GPDSPNodeRenderer::readTag(tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, float* value)
{
    tinyxml2::XMLElement const* element;
    GPDSPError error(GPDSPERROR_OK);
    
    if (parent != NULL && value != NULL) {
        if ((element = parent->FirstChildElement(tag.c_str())) != NULL) {
            if (!element->NoChildren()) {
                if (element->QueryFloatText(value) != tinyxml2::XML_SUCCESS) {
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

GPDSPError GPDSPNodeRenderer::readTag(tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, std::string* value)
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

std::string GPDSPNodeRenderer::stringize(GPDSPError error)
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
        case GPDSPERROR_NO_RATE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_RATE)", error);
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

GPDSPError GPDSPNodeRenderer::newNodeGenericInput(std::string const& name)
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
        error = newNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNodeGenericOutput(std::string const& name)
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
        error = newNode(name, node);
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::makeWait(void)
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

GPDSPError GPDSPNodeRenderer::makeSequence(void)
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

GPDSPError GPDSPNodeRenderer::optimizeSequence(void)
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

}// end of namespace
