/*
**      GPDSPGenerative
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
**      ofApp.cpp
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

#include "ofApp.h"

#define SAMPLING_RATE   (44100)
#define CHANNEL_SIZE    (2)
#define BUFFER_SIZE     (1024)
#define DRAWING_OFFSET  (100)
#define DRAWING_GAIN    (384)

void ofApp::setup(void)
{
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofBackground(31, 31, 31);
    ofSetWindowTitle("GPDSP (General Purpose DSP) Example 0.3.0        2017 iridium.jp");
    ofSetDataPathRoot(ofFilePath::join(ofFilePath::getEnclosingDirectory(ofFilePath::removeTrailingSlash(ofFilePath::getCurrentExeDir())), "Resources"));
    
    _i.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0f);
    _o.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0f);
    _dsp.newNodeBufferInput("L-in", &_i.buffer[0], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferInput("R-in", &_i.buffer[1], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("L-out", &_o.buffer[0], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("R-out", &_o.buffer[1], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeGeneric("gen", "sample.gpdsp");
    _dsp.setLinkI("gen", 0, "L-in", 0);
    _dsp.setLinkI("gen", 1, "R-in", 0);
    _dsp.setLinkI("L-out", 0, "gen", 0);
    _dsp.setLinkI("R-out", 0, "gen", 1);
    
    _gui = new ofxDatGui(2, 2);
    _gui->addFRM();
    _gui->addBreak();
    scanDevice("input", CHANNEL_SIZE, -1, &_i);
    scanDevice("output", -1, CHANNEL_SIZE, &_o);
    _gui->addBreak();
    _gui->addButton("refresh...");
    _gui->onButtonEvent(this, &ofApp::onButtonEvent);
    _gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    return;
}

void ofApp::exit(void)
{
    _o.stream.close();
    _i.stream.close();
    _dsp.clearNode();
    return;
}

void ofApp::audioIn(float* buffer, int size, int channel)
{
    _i.mutex.lock();
    _mutexIO.lock();
    memcpy(_i.buffer.data(), buffer, _i.buffer.size() * sizeof(float));
    _mutexIO.unlock();
    _i.mutex.unlock();
    return;
}

void ofApp::audioOut(float* buffer, int size, int channel)
{
    GPDSPError error;
    
    _o.mutex.lock();
    _mutexParam.lock();
    _mutexIO.lock();
    if ((error = _dsp.render(size, &size)) != GPDSPERROR_OK) {
        cout << "position [L-in]: " << _dsp.getNodeBufferInput("L-in")->getPosition() << endl;
        cout << "position [R-in]: " << _dsp.getNodeBufferInput("R-in")->getPosition() << endl;
        cout << "position [L-out]: " << _dsp.getNodeBufferOutput("L-out")->getPosition() << endl;
        cout << "position [R-out]: " << _dsp.getNodeBufferOutput("R-out")->getPosition() << endl;
        cout << "DSP error: " << _dsp.stringize(error) << ", remain = " << size << endl;
        _dsp.rewind();
        _dsp.refresh();
    }
    _mutexIO.unlock();
    _mutexParam.unlock();
    _o.mutex.unlock();
    memcpy(buffer, _o.buffer.data(), _o.buffer.size() * sizeof(float));
    return;
}

void ofApp::draw(void)
{
    bool valid;
    int i;
    
    ofPushMatrix();
    ofTranslate(_gui->getWidth() + (ofGetWidth() - _gui->getWidth() - BUFFER_SIZE) / 2, ofGetHeight() / 2);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetLineWidth(3.0);
    _i.mutex.lock();
    ofSetColor(63, 255, 127, 63);
    for (i = 1; i < BUFFER_SIZE; ++i) {
        ofDrawLine(i - 1, -_i.buffer[(i - 1) * 2 + 0] * DRAWING_GAIN - DRAWING_OFFSET, i, -_i.buffer[i * 2 + 0] * DRAWING_GAIN - DRAWING_OFFSET);
    }
    ofSetColor(255, 63, 127, 63);
    for (i = 1; i < BUFFER_SIZE; ++i) {
        ofDrawLine(i - 1, -_i.buffer[(i - 1) * 2 + 1] * DRAWING_GAIN + DRAWING_OFFSET, i, -_i.buffer[i * 2 + 1] * DRAWING_GAIN + DRAWING_OFFSET);
    }
    _i.mutex.unlock();
    _o.mutex.lock();
    valid = true;
    for (i = 0; i < BUFFER_SIZE; ++i) {
        if (isinf(_o.buffer[i * 2 + 0]) || isnan(_o.buffer[i * 2 + 0])) {
            valid = false;
        }
    }
    if (valid) {
        ofSetColor(63, 255, 127);
        for (i = 1; i < BUFFER_SIZE; ++i) {
            ofDrawLine(i - 1, -_o.buffer[(i - 1) * 2 + 0] * DRAWING_GAIN - DRAWING_OFFSET, i, -_o.buffer[i * 2 + 0] * DRAWING_GAIN - DRAWING_OFFSET);
        }
    }
    else {
        ofSetColor(255, 0, 0);
        ofDrawLine(0, -DRAWING_OFFSET, BUFFER_SIZE, -DRAWING_OFFSET);
    }
    valid = true;
    for (i = 0; i < BUFFER_SIZE; ++i) {
        if (isinf(_o.buffer[i * 2 + 1]) || isnan(_o.buffer[i * 2 + 1])) {
            valid = false;
        }
    }
    if (valid) {
        ofSetColor(255, 63, 127);
        for (i = 1; i < BUFFER_SIZE; ++i) {
            ofDrawLine(i - 1, -_o.buffer[(i - 1) * 2 + 1] * DRAWING_GAIN + DRAWING_OFFSET, i, -_o.buffer[i * 2 + 1] * DRAWING_GAIN + DRAWING_OFFSET);
        }
    }
    else {
        ofSetColor(255, 0, 0);
        ofDrawLine(0, +DRAWING_OFFSET, BUFFER_SIZE, +DRAWING_OFFSET);
    }
    _o.mutex.unlock();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPopMatrix();
    return;
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->is("refresh...")) {
        _mutexParam.lock();
        _dsp.refresh();
        _mutexParam.unlock();
    }
    return;
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    if (e.target->is("input")) {
        startDevice(&_i, e.target->getSelected()->getIndex(), CHANNEL_SIZE, 0);
    }
    else if (e.target->is("output")) {
        startDevice(&_o, e.target->getSelected()->getIndex(), 0, CHANNEL_SIZE);
    }
    return;
}

void ofApp::scanDevice(string const& label, int in, int out, IORec* io)
{
    vector<string> split;
    vector<string> param;
    int i;
    
    io->device = io->stream.getMatchingDevices("", (in >= 0) ? (in) : (UINT_MAX), (out >= 0) ? (out) : (UINT_MAX));
    for (i = 0; i < io->device.size(); ++i) {
        split = ofSplitString(io->device[i].name, ": ");
        if (split.size() > 1) {
            split.erase(split.begin());
        }
        param.push_back(label + ": " + ofJoinString(split, ": "));
    }
    _gui->addDropdown(label, param);
    return;
}

void ofApp::startDevice(IORec* io, int index, int in, int out)
{
    if (0 <= index && index < io->device.size()) {
        io->stream.close();
        io->stream.setDeviceID(io->device[index].deviceID);
        io->stream.setup(this, out, in, SAMPLING_RATE, BUFFER_SIZE, 8);
    }
    return;
}
