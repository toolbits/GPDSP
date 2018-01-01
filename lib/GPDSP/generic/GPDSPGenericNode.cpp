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
**      GPDSPGenericNode.cpp
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

#include "GPDSPGenericNode.hpp"
#include "GPDSPGenericInputNode.hpp"
#include "GPDSPGenericOutputNode.hpp"

namespace ir {

GPDSPGenericNode::GPDSPGenericNode(int rate) : _rate(rate)
{
}

GPDSPGenericNode::~GPDSPGenericNode(void)
{
    close();
}

GPDSPError GPDSPGenericNode::open(std::string const& file)
{
    std::shared_ptr<GPDSPNode> node;
    std::shared_ptr<GPDSPGenericInputNode> input;
    std::shared_ptr<GPDSPGenericOutputNode> output;
    std::string name;
    int index;
    GPDSPError error(GPDSPERROR_OK);
    
    if (_file == "") {
        if (file != "") {
            if ((error = _renderer.setRate(_rate)) == GPDSPERROR_OK) {
                if ((error = _renderer.load(file)) == GPDSPERROR_OK) {
                    _renderer.iterateNode();
                    while (_renderer.hasNextNode()) {
                        name = _renderer.getNextNode();
                        node = _renderer.getNode(name);
                        if ((input = std::dynamic_pointer_cast<GPDSPGenericInputNode>(node)) != NULL) {
                            if ((error = nameToIndex(name, &index)) == GPDSPERROR_OK) {
                                if (index + 1 > static_cast<int>(_input.size())) {
                                    try {
                                        _input.resize(index + 1, NULL);
                                    }
                                    catch (std::bad_alloc const&) {
                                        error = GPDSPERROR_NO_MEMORY;
                                    }
                                }
                                if (error == GPDSPERROR_OK) {
                                    if (_input[index] == NULL) {
                                        _input[index] = input.get();
                                    }
                                    else {
                                        error = GPDSPERROR_ALREADY_EXIST;
                                    }
                                }
                            }
                        }
                        else if ((output = std::dynamic_pointer_cast<GPDSPGenericOutputNode>(node)) != NULL) {
                            if ((error = nameToIndex(name, &index)) == GPDSPERROR_OK) {
                                if (index + 1 > static_cast<int>(_output.size())) {
                                    try {
                                        _output.resize(index + 1, NULL);
                                    }
                                    catch (std::bad_alloc const&) {
                                        error = GPDSPERROR_NO_MEMORY;
                                    }
                                }
                                if (error == GPDSPERROR_OK) {
                                    if (_output[index] == NULL) {
                                        _output[index] = output.get();
                                    }
                                    else {
                                        error = GPDSPERROR_ALREADY_EXIST;
                                    }
                                }
                            }
                        }
                        if (error != GPDSPERROR_OK) {
                            break;
                        }
                    }
                    if (error == GPDSPERROR_OK) {
                        if ((error = setCountI(static_cast<int>(_input.size()), "")) == GPDSPERROR_OK) {
                            if ((error = setCountO(static_cast<int>(_output.size()), "")) == GPDSPERROR_OK) {
                                _renderer.iterateNode();
                                while (_renderer.hasNextNode()) {
                                    name = _renderer.getNextNode();
                                    node = _renderer.getNode(name);
                                    if (std::dynamic_pointer_cast<GPDSPGenericInputNode>(node) != NULL) {
                                        if ((error = nameToIndex(name, &index)) == GPDSPERROR_OK) {
                                            error = setNameI(index, name);
                                        }
                                    }
                                    else if (std::dynamic_pointer_cast<GPDSPGenericOutputNode>(node) != NULL) {
                                        if ((error = nameToIndex(name, &index)) == GPDSPERROR_OK) {
                                            error = setNameO(index, name);
                                        }
                                    }
                                    if (error != GPDSPERROR_OK) {
                                        break;
                                    }
                                }
                                if (error == GPDSPERROR_OK) {
                                    _file = file;
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            error = GPDSPERROR_INVALID_PARAM;
        }
        if (error != GPDSPERROR_OK) {
            close();
        }
    }
    else {
        error = GPDSPERROR_INVALID_STATE;
    }
    return error;
}

void GPDSPGenericNode::close(void)
{
    clearO();
    clearI();
    _output.clear();
    _input.clear();
    _file = "";
    _renderer.clearNode();
    _renderer.setRate(0);
    return;
}

GPDSPError GPDSPGenericNode::fixate(void)
{
    return GPDSPERROR_OK;
}

void GPDSPGenericNode::invalidate(void)
{
    GPDSPInputtableNode::invalidate();
    GPDSPOutputtableNode::invalidate();
    _renderer.invalidate();
    return;
}

GPDSPError GPDSPGenericNode::prepare(void)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = _renderer.prepare()) == GPDSPERROR_OK) {
        error = copyOutput();
    }
    return error;
}

GPDSPError GPDSPGenericNode::process(void)
{
    GPDSPError state;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = copyInput()) == GPDSPERROR_OK) {
        error = _renderer.process();
        switch (error) {
            case GPDSPERROR_OK:
            case GPDSPERROR_FRAGMENT:
                state = error;
                if ((error = copyOutput()) == GPDSPERROR_OK) {
                    error = state;
                }
                break;
            case GPDSPERROR_LOOP:
                error = GPDSPERROR_WAIT;
                break;
            default:
                break;
        }
    }
    return error;
}

void GPDSPGenericNode::rewind(void)
{
    _renderer.rewind();
    return;
}

void GPDSPGenericNode::refresh(void)
{
    _renderer.refresh();
    return;
}

GPDSPError GPDSPGenericNode::copyInput(void)
{
    GPDSPFloat value;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    for (i = 0; i < static_cast<int>(_input.size()); ++i) {
        error = getValueI(i, &value);
        switch (error) {
            case GPDSPERROR_OK:
                if (_input[i] != NULL) {
                    error = _input[i]->setValueO(0, value);
                }
                break;
            case GPDSPERROR_WAIT:
                error = GPDSPERROR_OK;
                break;
            default:
                break;
        }
        if (error != GPDSPERROR_OK) {
            break;
        }
    }
    return error;
}

GPDSPError GPDSPGenericNode::copyOutput(void)
{
    GPDSPFloat value;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    for (i = 0; i < static_cast<int>(_output.size()); ++i) {
        if (_output[i] != NULL) {
            error = _output[i]->getValueI(0, &value);
            switch (error) {
                case GPDSPERROR_OK:
                    error = setValueO(i, value);
                    break;
                case GPDSPERROR_WAIT:
                    error = GPDSPERROR_OK;
                    break;
                default:
                    break;
            }
        }
        else {
            error = setValueO(i, GPDSPFV(0.0));
        }
        if (error != GPDSPERROR_OK) {
            break;
        }
    }
    return error;
}

GPDSPError GPDSPGenericNode::nameToIndex(std::string const& name, int* index)
{
    std::string string;
    std::string::size_type dot;
    int number;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((dot = name.find('.')) != std::string::npos) {
        if ((string = name.substr(0, dot)) != "") {
            if (string.find_first_not_of("0123456789") == std::string::npos) {
                number = atoi(string.c_str());
                if (0 <= number && number < 256) {
                    *index = number;
                }
                else {
                    error = GPDSPERROR_INVALID_RANGE;
                }
            }
            else {
                error = GPDSPERROR_INVALID_FORMAT;
            }
        }
        else {
            error = GPDSPERROR_INVALID_FORMAT;
        }
    }
    else {
        error = GPDSPERROR_INVALID_FORMAT;
    }
    return error;
}

}// end of namespace
