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
**      GPDSPWaveNode.cpp
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

#include "GPDSPWaveNode.hpp"
#include <unordered_map>

namespace ir {

GPDSPWaveNode::GPDSPWaveNode(int rate) noexcept : _rate(rate), _resolution(defaultResolution()), _wave(NULL), _position(GPDSPFV(0.0))
{
}

GPDSPWaveNode::~GPDSPWaveNode(void) noexcept
{
}

GPDSPError GPDSPWaveNode::setResolution(GPDSPFloat resolution) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (resolution > GPDSPFV(0.0)) {
        if (resolution != _resolution) {
            if ((error = makeWave(resolution, &_wave)) == GPDSPERROR_OK) {
                _resolution = resolution;
                invalidate();
            }
        }
    }
    else {
        error = GPDSPERROR_INVALID_PARAM;
    }
    return error;
}

GPDSPError GPDSPWaveNode::fixate(void) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    clearO();
    clearI();
    if ((error = setCountI(2, "")) == GPDSPERROR_OK) {
        if ((error = setNameI(0, "frequency")) == GPDSPERROR_OK) {
            if ((error = setNameI(1, "phase")) == GPDSPERROR_OK) {
                error = setCountO(1, "out");
            }
        }
    }
    if (error != GPDSPERROR_OK) {
        clearO();
        clearI();
    }
    return error;
}

void GPDSPWaveNode::invalidate(void) noexcept
{
    GPDSPInputtableNode::invalidate();
    GPDSPOutputtableNode::invalidate();
    return;
}

GPDSPError GPDSPWaveNode::prepare(void) noexcept
{
    return GPDSPERROR_OK;
}

GPDSPError GPDSPWaveNode::process(void) noexcept
{
    GPDSPFloat frequency;
    GPDSPFloat phase;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getValueI(0, &frequency)) == GPDSPERROR_OK) {
        if ((error = getValueI(1, &phase)) == GPDSPERROR_OK) {
            if (!isnan(frequency) && !isinf(frequency) && !isnan(phase) && !isinf(phase)) {
                if ((phase = fmod(_position + phase * _rate, _rate)) < GPDSPFV(0.0)) {
                    phase += _rate;
                }
                if (_wave != NULL) {
                    phase *= _resolution;
                    error = setValueO(0, (*_wave)[phase].first + (*_wave)[phase].second * modf(phase, &phase));
                }
                else {
                    error = setValueO(0, getWave(phase / _rate));
                }
                if (error == GPDSPERROR_OK) {
                    if ((_position = fmod(_position + frequency, _rate)) < GPDSPFV(0.0)) {
                        _position += _rate;
                    }
                }
            }
            else {
                error = setValueO(0, NAN);
            }
        }
    }
    return error;
}

void GPDSPWaveNode::rewind(void) noexcept
{
    _position = GPDSPFV(0.0);
    return;
}

GPDSPError GPDSPWaveNode::makeWave(GPDSPFloat resolution, std::vector<std::pair<GPDSPFloat, GPDSPFloat> > const** wave) const noexcept
{
    static std::unordered_map<std::string, std::vector<std::pair<GPDSPFloat, GPDSPFloat> > > s_wave;
    std::unordered_map<std::string, std::vector<std::pair<GPDSPFloat, GPDSPFloat> > >::iterator it;
    std::string key;
    char string[256];
    int size;
    int i;
    GPDSPError error(GPDSPERROR_OK);
    
    if (_rate > 0) {
        if (!isinf(resolution)) {
            size = ceil(_rate * resolution);
            if (snprintf(string, sizeof(string), "%d", size) >= 0) {
                if ((key = getName()) != "") {
                    key = key + ":" + string;
                    if ((it = s_wave.find(key)) != s_wave.end()) {
                        *wave = &it->second;
                    }
                    else {
                        try {
                            s_wave[key] = std::vector<std::pair<GPDSPFloat, GPDSPFloat> >(size, std::make_pair(GPDSPFV(0.0), GPDSPFV(0.0)));
                        }
                        catch (std::bad_alloc const&) {
                            error = GPDSPERROR_NO_MEMORY;
                        }
                        if (error == GPDSPERROR_OK) {
                            if ((it = s_wave.find(key)) != s_wave.end()) {
                                fillWave(&it->second);
                                for (i = 0; i < static_cast<int>(it->second.size()); ++i) {
                                    it->second[i].second = it->second[(i + 1) % it->second.size()].first - it->second[i].first;
                                }
                                *wave = &it->second;
                            }
                            else {
                                error = GPDSPERROR_FAILED;
                            }
                        }
                        if (error != GPDSPERROR_OK) {
                            s_wave.erase(key);
                        }
                    }
                }
                else {
                    error = GPDSPERROR_FAILED;
                }
            }
            else {
                error = GPDSPERROR_FAILED;
            }
        }
        else {
            *wave = NULL;
        }
    }
    else {
        error = GPDSPERROR_INVALID_STATE;
    }
    return error;
}

}// end of namespace
