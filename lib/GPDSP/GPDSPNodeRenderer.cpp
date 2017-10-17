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

namespace ir {

std::shared_ptr<GPDSPNode> GPDSPNodeRenderer::getNode(std::string const& name) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPNode> result;
    
    if ((it = _node.find(name)) != _node.end()) {
        result = it->second;
    }
    return result;
}

bool GPDSPNodeRenderer::getCountI(std::string const& name, int* count) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPPolyInputtableNode> node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if (count != NULL) {
            if ((node = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
                *count = node->getCountI();
            }
            else if (std::dynamic_pointer_cast<GPDSPMonoInputtableNode>(it->second)) {
                *count = 1;
            }
            else {
                *count = 0;
            }
        }
        result = true;
    }
    return result;
}

bool GPDSPNodeRenderer::getCountO(std::string const& name, int* count) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if (count != NULL) {
            if (std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) {
                *count = 1;
            }
            else {
                *count = 0;
            }
        }
        result = true;
    }
    return result;
}

bool GPDSPNodeRenderer::setLinkI(std::string const& name, std::string const& source)
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPMonoInputtableNode> mono;
    std::shared_ptr<GPDSPPolyInputtableNode> poly;
    std::shared_ptr<GPDSPOutputtableNode> node;
    int i;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((mono = std::dynamic_pointer_cast<GPDSPMonoInputtableNode>(it->second)) != NULL ||
            (poly = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
            if (source != "") {
                if ((it = _node.find(source)) != _node.end()) {
                    if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
                        result = true;
                    }
                }
            }
            else {
                result = true;
            }
            if (result) {
                if (mono != NULL) {
                    result = mono->setLinkI(node.get());
                }
                else if (poly->getCountI() > 0) {
                    for (i = 0; i < poly->getCountI(); ++i) {
                        if (!poly->setLinkI(i, node.get())) {
                            result = false;
                        }
                    }
                }
                else {
                    result = false;
                }
            }
        }
    }
    return result;
}

bool GPDSPNodeRenderer::setLinkI(std::string const& name, int index, std::string const& source)
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPPolyInputtableNode> poly;
    std::shared_ptr<GPDSPOutputtableNode> node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((poly = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
            if (source != "") {
                if ((it = _node.find(source)) != _node.end()) {
                    if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
                        result = poly->setLinkI(index, node.get());
                    }
                }
            }
            else {
                result = poly->setLinkI(index, node.get());
            }
        }
    }
    return result;
}

bool GPDSPNodeRenderer::getLinkI(std::string const& name, std::string* source) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPMonoInputtableNode> mono;
    GPDSPOutputtableNode const* node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((mono = std::dynamic_pointer_cast<GPDSPMonoInputtableNode>(it->second)) != NULL) {
            if (mono->getLinkI(&node)) {
                if (node != NULL) {
                    for (it = _node.begin(); it != _node.end(); ++it) {
                        if (it->second.get() == node) {
                            if (source != NULL) {
                                *source = it->first;
                            }
                            result = true;
                            break;
                        }
                    }
                }
                else {
                    if (source != NULL) {
                        *source = "";
                    }
                    result = true;
                }
            }
        }
    }
    return result;
}

bool GPDSPNodeRenderer::getLinkI(std::string const& name, int index, std::string* source) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPPolyInputtableNode> poly;
    GPDSPOutputtableNode const* node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((poly = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
            if (poly->getLinkI(index, &node)) {
                if (node != NULL) {
                    for (it = _node.begin(); it != _node.end(); ++it) {
                        if (it->second.get() == node) {
                            if (source != NULL) {
                                *source = it->first;
                            }
                            result = true;
                            break;
                        }
                    }
                }
                else {
                    if (source != NULL) {
                        *source = "";
                    }
                    result = true;
                }
            }
        }
    }
    return result;
}

bool GPDSPNodeRenderer::setLinkO(std::string const& name, std::string const& sink)
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode> node;
    std::shared_ptr<GPDSPMonoInputtableNode> mono;
    std::shared_ptr<GPDSPPolyInputtableNode> poly;
    GPDSPOutputtableNode const* temp;
    int i;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            if (sink != "") {
                if ((it = _node.find(sink)) != _node.end()) {
                    if ((mono = std::dynamic_pointer_cast<GPDSPMonoInputtableNode>(it->second)) != NULL) {
                        result = mono->setLinkI(node.get());
                    }
                    else if ((poly = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
                        if (poly->getCountI() > 0) {
                            result = true;
                            for (i = 0; i < poly->getCountI(); ++i) {
                                if (!poly->setLinkI(i, node.get())) {
                                    result = false;
                                }
                            }
                        }
                    }
                }
            }
            else {
                result = true;
                for (it = _node.begin(); it != _node.end(); ++it) {
                    if ((mono = std::dynamic_pointer_cast<GPDSPMonoInputtableNode>(it->second)) != NULL) {
                        if (mono->getLinkI(&temp)) {
                            if (temp == node.get()) {
                                if (!mono->setLinkI(NULL)) {
                                    result = false;
                                }
                            }
                        }
                        else {
                            result = false;
                        }
                    }
                    else if ((poly = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
                        for (i = 0; i < poly->getCountI(); ++i) {
                            if (poly->getLinkI(i, &temp)) {
                                if (temp == node.get()) {
                                    if (!poly->setLinkI(i, NULL)) {
                                        result = false;
                                    }
                                }
                            }
                            else {
                                result = false;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

bool GPDSPNodeRenderer::setLinkO(std::string const& name, std::string const& sink, int index)
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode> node;
    std::shared_ptr<GPDSPPolyInputtableNode> poly;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            if ((it = _node.find(sink)) != _node.end()) {
                if ((poly = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
                    result = poly->setLinkI(index, node.get());
                }
            }
        }
    }
    return result;
}

bool GPDSPNodeRenderer::getValueI(std::string const& name, float* value) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPMonoInputtableNode> node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPMonoInputtableNode>(it->second)) != NULL) {
            result = node->getValueI(value);
        }
    }
    return result;
}

bool GPDSPNodeRenderer::getValueI(std::string const& name, int index, float* value) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPPolyInputtableNode> node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPPolyInputtableNode>(it->second)) != NULL) {
            result = node->getValueI(index, value);
        }
    }
    return result;
}

bool GPDSPNodeRenderer::getValueP(std::string const& name, float* value) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPInputtableNode> node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPInputtableNode>(it->second)) != NULL) {
            result = node->getValueP(value);
        }
    }
    return result;
}

bool GPDSPNodeRenderer::getValueO(std::string const& name, float* value) const
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator it;
    std::shared_ptr<GPDSPOutputtableNode> node;
    bool result(false);
    
    if ((it = _node.find(name)) != _node.end()) {
        if ((node = std::dynamic_pointer_cast<GPDSPOutputtableNode>(it->second)) != NULL) {
            result = node->getValueO(value);
        }
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeBufferInput(std::string const& name, float const* buffer, int interleave)
{
    std::shared_ptr<GPDSPBufferInputNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPBufferInputNode>()) != NULL) {
        node->setBuffer(buffer, interleave);
        result = newNode(name, node);
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeBufferOutput(std::string const& name, float* buffer, int interleave)
{
    std::shared_ptr<GPDSPBufferOutputNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPBufferOutputNode>()) != NULL) {
        node->setBuffer(buffer, interleave);
        result = newNode(name, node);
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeConstant(std::string const& name, float constant)
{
    std::shared_ptr<GPDSPConstantNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPConstantNode>()) != NULL) {
        node->setConstant(constant);
        result = newNode(name, node);
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeGate(std::string const& name, float minimum, float maximum)
{
    std::shared_ptr<GPDSPGateNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPGateNode>()) != NULL) {
        node->setMinimum(minimum);
        node->setMaximum(maximum);
        result = newNode(name, node);
    }
    return result;
}

bool GPDSPNodeRenderer::newNodePeek(std::string const& name)
{
    std::shared_ptr<GPDSPPeekNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPPeekNode>()) != NULL) {
        result = newNode(name, node);
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeAmplify(std::string const& name, float gain)
{
    std::shared_ptr<GPDSPAmplifyNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPAmplifyNode>()) != NULL) {
        node->setGain(gain);
        result = newNode(name, node);
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeDelay(std::string const& name)
{
    std::shared_ptr<GPDSPDelayNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPDelayNode>()) != NULL) {
        result = newNode(name, node);
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeBuffer(std::string const& name, int size)
{
    std::shared_ptr<GPDSPBufferNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPBufferNode>()) != NULL) {
        if (node->setSize(size)) {
            result = newNode(name, node);
        }
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeSum(std::string const& name, int count)
{
    std::shared_ptr<GPDSPSumNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPSumNode>()) != NULL) {
        if (node->setCountI(count)) {
            result = newNode(name, node);
        }
    }
    return result;
}

bool GPDSPNodeRenderer::newNodeMultiply(std::string const& name, int count)
{
    std::shared_ptr<GPDSPMultiplyNode> node;
    bool result(false);
    
    if ((node = std::make_shared<GPDSPMultiplyNode>()) != NULL) {
        if (node->setCountI(count)) {
            result = newNode(name, node);
        }
    }
    return result;
}

bool GPDSPNodeRenderer::newNode(std::string const& name, std::shared_ptr<GPDSPNode> node)
{
    std::shared_ptr<GPDSPRewindableNode> rewindable;
    std::shared_ptr<GPDSPRefreshableNode> refresh;
    bool result(false);
    
    if (name != "" && node != NULL) {
        if (_node.find(name) == _node.end()) {
            _node[name] = node;
            if ((rewindable = std::dynamic_pointer_cast<GPDSPRewindableNode>(node)) != NULL) {
                _rewindable[name] = rewindable.get();
            }
            if ((refresh = std::dynamic_pointer_cast<GPDSPRefreshableNode>(node)) != NULL) {
                _refresh[name] = refresh.get();
            }
            result = true;
        }
    }
    return result;
}

void GPDSPNodeRenderer::deleteNode(std::string const& name)
{
    if (_node.find(name) != _node.end()) {
        setLinkO(name, "");
        _refresh.erase(name);
        _rewindable.erase(name);
        _node.erase(name);
    }
    return;
}

void GPDSPNodeRenderer::clearNode(void)
{
    _refresh.clear();
    _rewindable.clear();
    _node.clear();
    return;
}

bool GPDSPNodeRenderer::render(int count)
{
    std::map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator nit;
    std::map<std::string, GPDSPRewindableNode*>::const_iterator rit;
    int remain;
    bool result(false);
    
    if (count >= 0) {
        for (rit = _rewindable.begin(); rit != _rewindable.end(); ++rit) {
            rit->second->rewind();
        }
        while (--count >= 0) {
            for (nit = _node.begin(); nit != _node.end(); ++nit) {
                nit->second->invalidate();
            }
            do {
                remain = 0;
                for (nit = _node.begin(); nit != _node.end(); ++nit) {
                    if (!nit->second->process()) {
                        ++remain;
                    }
                }
            } while (remain > 0);
        }
        result = true;
    }
    return result;
}

void GPDSPNodeRenderer::refresh(void)
{
    std::map<std::string, GPDSPRefreshableNode*>::const_iterator it;
    
    for (it = _refresh.begin(); it != _refresh.end(); ++it) {
        it->second->refresh();
    }
    return;
}

void GPDSPNodeRenderer::refresh(std::string const& name)
{
    std::map<std::string, GPDSPRefreshableNode*>::const_iterator it;
    
    if ((it = _refresh.find(name)) != _refresh.end()) {
        it->second->refresh();
    }
    return;
}

}// end of namespace
