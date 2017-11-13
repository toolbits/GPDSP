/*
**      General Purpose DSP Library
**
**      Original Copyright (C) 2017 - 2017 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      Version     openFrameworks
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
#include "GPDSPBufferInputNode.hpp"
#include "GPDSPBufferOutputNode.hpp"
#include "GPDSPConstantNode.hpp"
#include "GPDSPGateNode.hpp"
#include "GPDSPPeekNode.hpp"
#include "GPDSPAmplifyNode.hpp"
#include "GPDSPDelayNode.hpp"
#include "GPDSPBufferNode.hpp"
#include "GPDSPSumNode.hpp"
#include "GPDSPMultiplyNode.hpp"
#include "GPDSPGenericNode.hpp"

namespace ir {

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
    std::shared_ptr<GPDSPInputtableNode> node;
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
    std::shared_ptr<GPDSPOutputtableNode> node;
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

GPDSPError GPDSPNodeRenderer::setLinkI(std::string const& name, int index, std::string const& source, int which)
{
    std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode> input;
    std::shared_ptr<GPDSPOutputtableNode> output;
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
    std::shared_ptr<GPDSPInputtableNode> input;
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
    std::shared_ptr<GPDSPOutputtableNode> output;
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
    std::shared_ptr<GPDSPOutputtableNode> output;
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
    std::shared_ptr<GPDSPInputtableNode> node;
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
    std::shared_ptr<GPDSPOutputtableNode> node;
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
        if ((error = node->setCountI(count)) == GPDSPERROR_OK) {
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
        if ((error = node->setCountI(count)) == GPDSPERROR_OK) {
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
        node = std::make_shared<GPDSPGenericNode>();
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        if ((error = node->open(file)) == GPDSPERROR_OK) {
            error = newNode(name, node);
        }
    }
    return error;
}

GPDSPError GPDSPNodeRenderer::newNode(std::string const& name, std::shared_ptr<GPDSPNode> node)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (name != "" && node != NULL) {
        if (_node.find(name) == _node.end()) {
            
            // TEST:
            node->__name = name;
            
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

GPDSPError GPDSPNodeRenderer::load(std::string const& file)
{
    GPDSPError error(GPDSPERROR_LIMIT);
    
    return error;
}

GPDSPError GPDSPNodeRenderer::save(std::string const& file) const
{
    GPDSPError error(GPDSPERROR_LIMIT);
    
    return error;
}

std::string GPDSPNodeRenderer::stringize(GPDSPError error)
{
    char result[64] = "\0";
    
    switch (error) {
        case GPDSPERROR_OK:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_OK)", error);
            break;
        case GPDSPERROR_WAIT:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_WAIT)", error);
            break;
        case GPDSPERROR_FRAGMENT:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_FRAGMENT)", error);
            break;
        case GPDSPERROR_LOOP:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_LOOP)", error);
            break;
        case GPDSPERROR_NO_MEMORY:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_MEMORY)", error);
            break;
        case GPDSPERROR_NO_NODE:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_NO_NODE)", error);
            break;
        case GPDSPERROR_ALREADY_EXIST:
            snprintf(result, sizeof(result), "%d (GPDSPERROR_ALREADY_EXIST)", error);
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
        default:
            snprintf(result, sizeof(result), "%d (unknown error)", error);
            break;
    }
    return result;
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
    
    // TEST:
    {
        printf("JIT [");
        for (int i = 0; i < _sequence.size(); ++i) {
            printf("%s", (_sequence[i].first != NULL) ? (_sequence[i].first->__name.c_str()) : ("*"));
            if (i < _sequence.size() - 1) {
                printf(" > ");
            }
        }
        printf("]\n");
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
