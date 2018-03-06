/*
**      GPDSPCustom
**
**      Original Copyright (C) 2017 - 2018 HORIGUCHI Junshi.
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
**      Xcode 9.2 (Apple LLVM 9.0.0)
**
**      ofApp.h
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

#pragma once

#include "ofMain.h"
#include "GPDSP.hpp"
#include "ofxDatGui.h"

using namespace ir;

class ofApp : public ofBaseApp, public GPDSPSerializable {
    private:
        struct IORec {
            vector<ofSoundDevice> device;
            ofSoundStream stream;
            GPDSPFloat* buffer[2];
            ofMutex mutex;
        };
    
    private:
        IORec _i;
        IORec _o;
        GPDSPNodeRenderer _dsp;
        GPDSPError _error;
        GPDSPError _save;
        ofMutex _mutexIO;
        ofMutex _mutexParam;
        ofxDatGui* _gui;
        map<string, int> _select;
    
    public:
        void setup(void);
        void exit(void);
        void audioIn(float* buffer, int size, int channel);
        void audioOut(float* buffer, int size, int channel);
        void update(void);
        void draw(void);
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onToggleEvent(ofxDatGuiToggleEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
        // 復元を行うための関数
        GPDSPError load(GPDSPNodeRenderer* renderer, std::string const& type, std::string const& name, int format, tinyxml2::XMLElement const* element) noexcept;
    
        // 保存を行うための関数
        GPDSPError save(GPDSPNodeRenderer const& renderer, std::shared_ptr<GPDSPNode const> const& node, std::string const& name, tinyxml2::XMLElement* element) noexcept;
    private:
        void retouch(void);
        void makeIn(string const& in, int size, string const& out, ofColor const& color);
        void makeOut(string const& out, int size, string const& in, ofColor const& color);
        void syncIn(string const& label, int size);
        void syncOut(string const& label, int size);
        void selectIn(string const& in, int index, string const& out, string const& other);
        void selectOut(string const& out, int index, string const& in, string const& other);
        void scanDevice(string const& label, int in, int out, IORec* io);
        void startDevice(IORec* io, int index, int in, int out);
};