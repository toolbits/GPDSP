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
**      GPDSPInputtableNode.cpp
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

#include "GPDSPInputtableNode.hpp"

namespace ir {

GPDSPInputtableNode::GPDSPInputtableNode(void)
{
}

GPDSPInputtableNode::~GPDSPInputtableNode(void)
{
    _terminal.clear();
}

GPDSPError GPDSPInputtableNode::setNameI(int index, std::string const& what)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        _terminal[index].name = what;
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::getNameI(int index, std::string* what) const
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (what != NULL) {
            *what = _terminal[index].name;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::getModeI(int index, GPDSPMode* mode) const
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (mode != NULL) {
            *mode = _terminal[index].mode;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::setLinkPositiveI(int index, GPDSPOutputtableNode const* from, int which)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (_terminal[index].mode != GPDSPMODE_POSITIVE || from != _terminal[index].node || which != _terminal[index].index) {
            _terminal[index].mode = GPDSPMODE_POSITIVE;
            _terminal[index].node = from;
            _terminal[index].index = which;
            invalidate();
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::setLinkNegativeI(int index, GPDSPOutputtableNode const* from, int which)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (_terminal[index].mode != GPDSPMODE_NEGATIVE || from != _terminal[index].node || which != _terminal[index].index) {
            _terminal[index].mode = GPDSPMODE_NEGATIVE;
            _terminal[index].node = from;
            _terminal[index].index = which;
            invalidate();
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::setLinkConstantI(int index, float constant)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (_terminal[index].mode != GPDSPMODE_CONSTANT || constant != _terminal[index].constant) {
            _terminal[index].mode = GPDSPMODE_CONSTANT;
            _terminal[index].constant = constant;
            invalidate();
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::getLinkI(int index, GPDSPOutputtableNode const** from, int* which) const
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (_terminal[index].mode == GPDSPMODE_POSITIVE || _terminal[index].mode == GPDSPMODE_NEGATIVE) {
            if (from != NULL) {
                *from = _terminal[index].node;
            }
            if (which != NULL) {
                *which = _terminal[index].index;
            }
        }
        else {
            error = GPDSPERROR_INVALID_PARAM;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::getLinkI(int index, float* constant) const
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (_terminal[index].mode == GPDSPMODE_CONSTANT) {
            if (constant != NULL) {
                *constant = _terminal[index].constant;
            }
        }
        else {
            error = GPDSPERROR_INVALID_PARAM;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::clearLinkI(int index)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        if (_terminal[index].mode != GPDSPMODE_NONE) {
            _terminal[index].mode = GPDSPMODE_NONE;
            invalidate();
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

void GPDSPInputtableNode::clearLinkI(GPDSPMode mode)
{
    int i;
    
    for (i = 0; i < _terminal.size(); ++i) {
        if (_terminal[i].mode == mode) {
            if (_terminal[i].mode != GPDSPMODE_NONE) {
                _terminal[i].mode = GPDSPMODE_NONE;
                invalidate();
            }
        }
    }
    return;
}

void GPDSPInputtableNode::clearLinkI(GPDSPOutputtableNode const* from, int which)
{
    int i;
    
    for (i = 0; i < _terminal.size(); ++i) {
        if ((_terminal[i].mode == GPDSPMODE_POSITIVE || _terminal[i].mode == GPDSPMODE_NEGATIVE) && _terminal[i].node == from && _terminal[i].index == which) {
            _terminal[i].mode = GPDSPMODE_NONE;
            invalidate();
        }
    }
    return;
}

void GPDSPInputtableNode::clearLinkI(GPDSPOutputtableNode const* from)
{
    int i;
    
    for (i = 0; i < _terminal.size(); ++i) {
        if ((_terminal[i].mode == GPDSPMODE_POSITIVE || _terminal[i].mode == GPDSPMODE_NEGATIVE) && _terminal[i].node == from) {
            _terminal[i].mode = GPDSPMODE_NONE;
            invalidate();
        }
    }
    return;
}

void GPDSPInputtableNode::clearLinkI(void)
{
    int i;
    
    for (i = 0; i < _terminal.size(); ++i) {
        if (_terminal[i].mode != GPDSPMODE_NONE) {
            _terminal[i].mode = GPDSPMODE_NONE;
            invalidate();
        }
    }
    return;
}

GPDSPError GPDSPInputtableNode::getValueI(int index, float* value) const
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        switch (_terminal[index].mode) {
            case GPDSPMODE_POSITIVE:
                if (_terminal[index].node != NULL) {
                    error = _terminal[index].node->getValueO(_terminal[index].index, value);
                }
                else if (value != NULL) {
                    *value = 0.0f;
                }
                break;
            case GPDSPMODE_NEGATIVE:
                if (_terminal[index].node != NULL) {
                    if ((error = _terminal[index].node->getValueO(_terminal[index].index, value)) == GPDSPERROR_OK) {
                        if (value != NULL) {
                            *value = -*value;
                        }
                    }
                }
                else if (value != NULL) {
                    *value = 0.0f;
                }
                break;
            case GPDSPMODE_CONSTANT:
                if (value != NULL) {
                    *value = _terminal[index].constant;
                }
                break;
            default:
                if (value != NULL) {
                    *value = 0.0f;
                }
                break;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

int GPDSPInputtableNode::findNameI(std::string const& what) const
{
    int i;
    int result(-1);
    
    for (i = 0; i < _terminal.size(); ++i) {
        if (_terminal[i].name == what) {
            result = i;
            break;
        }
    }
    return result;
}

int GPDSPInputtableNode::findLinkI(GPDSPMode mode) const
{
    int i;
    int result(-1);
    
    for (i = 0; i < _terminal.size(); ++i) {
        if (_terminal[i].mode == mode) {
            result = i;
            break;
        }
    }
    return result;
}

int GPDSPInputtableNode::findLinkI(GPDSPOutputtableNode const* from, int which) const
{
    int i;
    int result(-1);
    
    for (i = 0; i < _terminal.size(); ++i) {
        if ((_terminal[i].mode == GPDSPMODE_POSITIVE || _terminal[i].mode == GPDSPMODE_NEGATIVE) && _terminal[i].node == from && _terminal[i].index == which) {
            result = i;
            break;
        }
    }
    return result;
}

int GPDSPInputtableNode::findLinkI(GPDSPOutputtableNode const* from) const
{
    int i;
    int result(-1);
    
    for (i = 0; i < _terminal.size(); ++i) {
        if ((_terminal[i].mode == GPDSPMODE_POSITIVE || _terminal[i].mode == GPDSPMODE_NEGATIVE) && _terminal[i].node == from) {
            result = i;
            break;
        }
    }
    return result;
}

void GPDSPInputtableNode::invalidate(void)
{
    return;
}

GPDSPError GPDSPInputtableNode::setCountI(int count, std::string const& what)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (count < 0) {
        count = 0;
    }
    if (count != _terminal.size()) {
        try {
            _terminal.resize(count, TerminalRec{what, GPDSPMODE_NONE});
        }
        catch (std::bad_alloc const&) {
            error = GPDSPERROR_NO_MEMORY;
        }
        if (error == GPDSPERROR_OK) {
            invalidate();
        }
    }
    return error;
}

GPDSPError GPDSPInputtableNode::appendI(std::string const& what)
{
    GPDSPError error(GPDSPERROR_OK);
    
    try {
        _terminal.push_back(TerminalRec{what, GPDSPMODE_NONE});
    }
    catch (std::bad_alloc const&) {
        error = GPDSPERROR_NO_MEMORY;
    }
    if (error == GPDSPERROR_OK) {
        invalidate();
    }
    return error;
}

GPDSPError GPDSPInputtableNode::insertI(int index, std::string const& what)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        try {
            _terminal.insert(_terminal.begin() + index, TerminalRec{what, GPDSPMODE_NONE});
        }
        catch (std::bad_alloc const&) {
            error = GPDSPERROR_NO_MEMORY;
        }
        if (error == GPDSPERROR_OK) {
            invalidate();
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPInputtableNode::removeI(int index)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _terminal.size()) {
        _terminal.erase(_terminal.begin() + index);
        invalidate();
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

void GPDSPInputtableNode::clearI(void)
{
    _terminal.clear();
    invalidate();
    return;
}

}// end of namespace
