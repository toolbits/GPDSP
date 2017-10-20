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
**      GPDSPNodeRenderer.hpp
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

#ifndef __GPDSPNODERENDERER_HPP
#define __GPDSPNODERENDERER_HPP

#include <string>
#include <map>
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

namespace ir {

class GPDSPNodeRenderer {
    private:
                std::map<std::string, std::shared_ptr<GPDSPNode> >
                                            _node;
                std::map<std::string, GPDSPRewindableNode*>
                                            _rewindable;
                std::map<std::string, GPDSPRefreshableNode*>
                                            _refreshable;
                std::vector<GPDSPNode*>     _jit;
    
    public:
        explicit                            GPDSPNodeRenderer           (void);
                                            ~GPDSPNodeRenderer          (void);
                std::shared_ptr<GPDSPBufferInputNode>
                                            getNodeBufferInput          (std::string const& name) const;
                std::shared_ptr<GPDSPBufferOutputNode>
                                            getNodeBufferOutput         (std::string const& name) const;
                std::shared_ptr<GPDSPConstantNode>
                                            getNodeConstant             (std::string const& name) const;
                std::shared_ptr<GPDSPGateNode>
                                            getNodeGate                 (std::string const& name) const;
                std::shared_ptr<GPDSPPeekNode>
                                            getNodePeek                 (std::string const& name) const;
                std::shared_ptr<GPDSPAmplifyNode>
                                            getNodeAmplify              (std::string const& name) const;
                std::shared_ptr<GPDSPDelayNode>
                                            getNodeDelay                (std::string const& name) const;
                std::shared_ptr<GPDSPBufferNode>
                                            getNodeBuffer               (std::string const& name) const;
                std::shared_ptr<GPDSPSumNode>
                                            getNodeSum                  (std::string const& name) const;
                std::shared_ptr<GPDSPMultiplyNode>
                                            getNodeMultiply             (std::string const& name) const;
                std::shared_ptr<GPDSPNode>  getNode                     (std::string const& name) const;
                bool                        getCountI                   (std::string const& name, int* count) const;
                bool                        getCountO                   (std::string const& name, int* count) const;
                bool                        setLinkI                    (std::string const& name, std::string const& source);
                bool                        setLinkI                    (std::string const& name, int index, std::string const& source);
                bool                        getLinkI                    (std::string const& name, std::string* source) const;
                bool                        getLinkI                    (std::string const& name, int index, std::string* source) const;
                bool                        setLinkO                    (std::string const& name, std::string const& sink);
                bool                        setLinkO                    (std::string const& name, std::string const& sink, int index);
                bool                        getValueI                   (std::string const& name, float* value) const;
                bool                        getValueI                   (std::string const& name, int index, float* value) const;
                bool                        getValueP                   (std::string const& name, float* value) const;
                bool                        getValueO                   (std::string const& name, float* value) const;
                bool                        newNodeBufferInput          (std::string const& name, float const* buffer, int interleave);
                bool                        newNodeBufferOutput         (std::string const& name, float* buffer, int interleave);
                bool                        newNodeConstant             (std::string const& name, float constant);
                bool                        newNodeGate                 (std::string const& name, float minimum, float maximum);
                bool                        newNodePeek                 (std::string const& name);
                bool                        newNodeAmplify              (std::string const& name, float gain);
                bool                        newNodeDelay                (std::string const& name);
                bool                        newNodeBuffer               (std::string const& name, int size);
                bool                        newNodeSum                  (std::string const& name, int count);
                bool                        newNodeMultiply             (std::string const& name, int count);
                bool                        newNode                     (std::string const& name, std::shared_ptr<GPDSPNode> node);
                void                        deleteNode                  (std::string const& name);
                void                        clearNode                   (void);
                bool                        render                      (int count);
                void                        refresh                     (void);
                void                        refresh                     (std::string const& name);
    private:
                                            GPDSPNodeRenderer           (GPDSPNodeRenderer const&);
                GPDSPNodeRenderer&          operator=                   (GPDSPNodeRenderer const&);
};

inline GPDSPNodeRenderer::GPDSPNodeRenderer(void)
{
}

inline GPDSPNodeRenderer::~GPDSPNodeRenderer(void)
{
    clearNode();
}

inline std::shared_ptr<GPDSPBufferInputNode> GPDSPNodeRenderer::getNodeBufferInput(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPBufferInputNode>(getNode(name));
}

inline std::shared_ptr<GPDSPBufferOutputNode> GPDSPNodeRenderer::getNodeBufferOutput(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPBufferOutputNode>(getNode(name));
}

inline std::shared_ptr<GPDSPConstantNode> GPDSPNodeRenderer::getNodeConstant(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPConstantNode>(getNode(name));
}

inline std::shared_ptr<GPDSPGateNode> GPDSPNodeRenderer::getNodeGate(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPGateNode>(getNode(name));
}

inline std::shared_ptr<GPDSPPeekNode> GPDSPNodeRenderer::getNodePeek(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPPeekNode>(getNode(name));
}

inline std::shared_ptr<GPDSPAmplifyNode> GPDSPNodeRenderer::getNodeAmplify(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPAmplifyNode>(getNode(name));
}

inline std::shared_ptr<GPDSPDelayNode> GPDSPNodeRenderer::getNodeDelay(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPDelayNode>(getNode(name));
}

inline std::shared_ptr<GPDSPBufferNode> GPDSPNodeRenderer::getNodeBuffer(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPBufferNode>(getNode(name));
}

inline std::shared_ptr<GPDSPSumNode> GPDSPNodeRenderer::getNodeSum(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPSumNode>(getNode(name));
}

inline std::shared_ptr<GPDSPMultiplyNode> GPDSPNodeRenderer::getNodeMultiply(std::string const& name) const
{
    return std::dynamic_pointer_cast<GPDSPMultiplyNode>(getNode(name));
}

}// end of namespace

#endif
