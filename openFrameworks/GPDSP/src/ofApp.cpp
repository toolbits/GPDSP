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
#define DRAWING_GAIN    (384)

void ofApp::setup(void)
{
    ofxDatGuiFolder* folder;
    
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofBackground(31, 31, 31);
    ofSetWindowTitle("GPDSP (General Purpose DSP) Example        2017 iridium.jp");
    ofSetDataPathRoot(ofFilePath::join(ofFilePath::getEnclosingDirectory(ofFilePath::removeTrailingSlash(ofFilePath::getCurrentExeDir())), "Resources"));
    
    _i.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0);
    _o.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0);
    _dsp.newNodeBufferInput("L-in", &_i.buffer[0], CHANNEL_SIZE);
    _dsp.newNodeBufferInput("R-in", &_i.buffer[1], CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("L-out", &_o.buffer[0], CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("R-out", &_o.buffer[1], CHANNEL_SIZE);
    
    _dsp.newNodeSum("L-sum", 3);
    _dsp.newNodeBuffer("L-bf1", 1);
    _dsp.newNodeBuffer("L-bf2", 1);
    _dsp.newNodeAmplify("LL-amp", 0.0);
    _dsp.newNodeAmplify("LR-amp", 0.0);
    _dsp.newNodeGate("L-gat", -1.0, +1.0);
    _dsp.newNodeSum("R-sum", 3);
    _dsp.newNodeBuffer("R-bf1", 1);
    _dsp.newNodeBuffer("R-bf2", 1);
    _dsp.newNodeAmplify("RR-amp", 0.0);
    _dsp.newNodeAmplify("RL-amp", 0.0);
    _dsp.newNodeGate("R-gat", -1.0, +1.0);
    
    _dsp.setLinkI("L-sum", 0, "L-in");
    _dsp.setLinkI("L-sum", 1, "LL-amp");
    _dsp.setLinkI("L-sum", 2, "RL-amp");
    _dsp.setLinkI("L-bf1", "L-sum");
    _dsp.setLinkI("LL-amp", "L-bf1");
    _dsp.setLinkI("L-bf2", "L-bf1");
    _dsp.setLinkI("LR-amp", "L-bf2");
    _dsp.setLinkI("L-gat", "L-sum");
    _dsp.setLinkI("L-out", "L-gat");
    
    _dsp.setLinkI("R-sum", 0, "R-in");
    _dsp.setLinkI("R-sum", 1, "RR-amp");
    _dsp.setLinkI("R-sum", 2, "LR-amp");
    _dsp.setLinkI("R-bf1", "R-sum");
    _dsp.setLinkI("RR-amp", "R-bf1");
    _dsp.setLinkI("R-bf2", "R-bf1");
    _dsp.setLinkI("RL-amp", "R-bf2");
    _dsp.setLinkI("R-gat", "R-sum");
    _dsp.setLinkI("R-out", "R-gat");
    
    _gui = new ofxDatGui(2, 2);
    _gui->addFRM();
    _gui->addBreak();
    scanDevice("input", CHANNEL_SIZE, -1, &_i);
    scanDevice("output", -1, CHANNEL_SIZE, &_o);
    _gui->addBreak();
    _gui->addSlider("delay [L] 1", 1, 10000, 1);
    _gui->getSlider("delay [L] 1")->setPrecision(0);
    _gui->addSlider("delay [L] 2", 1, 10000, 1);
    _gui->getSlider("delay [L] 2")->setPrecision(0);
    _gui->addSlider("delay [R] 1", 1, 10000, 1);
    _gui->getSlider("delay [R] 1")->setPrecision(0);
    _gui->addSlider("delay [R] 2", 1, 10000, 1);
    _gui->getSlider("delay [R] 2")->setPrecision(0);
    _gui->addBreak();
    folder = _gui->addFolder("delay send", ofColor(127, 255, 127));
    folder->addSlider("* L -> L", -1.0, 1.0, 0.0);
    folder->addSlider("* L -> R", -1.0, 1.0, 0.0);
    folder->addSlider("* R -> R", -1.0, 1.0, 0.0);
    folder->addSlider("* R -> L", -1.0, 1.0, 0.0);
    folder->addButton("* reset gains...");
    folder->expand();
    _gui->addBreak();
    _gui->addSlider("gate [L] max", 0.0, +1.0, +1.0);
    _gui->addSlider("gate [L] min", -1.0, 0.0, -1.0);
    _gui->addSlider("gate [R] max", 0.0, +1.0, +1.0);
    _gui->addSlider("gate [R] min", -1.0, 0.0, -1.0);
    _gui->addBreak();
    _gui->addButton("refresh...");
    _gui->onButtonEvent(this, &ofApp::onButtonEvent);
    _gui->onSliderEvent(this, &ofApp::onSliderEvent);
    _gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    _image.load("graph.png");
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
    _o.mutex.lock();
    _mutexParam.lock();
    _mutexIO.lock();
    _dsp.render(size);
    _mutexIO.unlock();
    _mutexParam.unlock();
    _o.mutex.unlock();
    memcpy(buffer, _o.buffer.data(), _o.buffer.size() * sizeof(float));
    return;
}

void ofApp::update(void)
{
    return;
}

void ofApp::draw(void)
{
    int i;
    bool valid;
    
    ofPushMatrix();
    ofTranslate(_gui->getWidth() + (ofGetWidth() - _gui->getWidth() - _image.getWidth()) / 2, ofGetHeight() / 2);
    ofSetColor(255, 255, 255, 63);
    _image.draw(0, -_image.getHeight() / 2);
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(_gui->getWidth() + (ofGetWidth() - _gui->getWidth() - BUFFER_SIZE) / 2, ofGetHeight() / 2);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetLineWidth(3.0);
    _i.mutex.lock();
    ofSetColor(63, 255, 127, 63);
    for (i = 1; i < BUFFER_SIZE; ++i) {
        ofDrawLine(i - 1, -_i.buffer[(i - 1) * 2 + 0] * DRAWING_GAIN - 125, i, -_i.buffer[i * 2 + 0] * DRAWING_GAIN - 125);
    }
    ofSetColor(255, 63, 127, 63);
    for (i = 1; i < BUFFER_SIZE; ++i) {
        ofDrawLine(i - 1, -_i.buffer[(i - 1) * 2 + 1] * DRAWING_GAIN + 100, i, -_i.buffer[i * 2 + 1] * DRAWING_GAIN + 100);
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
            ofDrawLine(i - 1, -_o.buffer[(i - 1) * 2 + 0] * DRAWING_GAIN - 100, i, -_o.buffer[i * 2 + 0] * DRAWING_GAIN - 100);
        }
    }
    else {
        ofSetColor(255, 0, 0);
        ofDrawLine(0, -100, BUFFER_SIZE, -100);
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
            ofDrawLine(i - 1, -_o.buffer[(i - 1) * 2 + 1] * DRAWING_GAIN + 125, i, -_o.buffer[i * 2 + 1] * DRAWING_GAIN + 125);
        }
    }
    else {
        ofSetColor(255, 0, 0);
        ofDrawLine(0, +125, BUFFER_SIZE, +125);
    }
    _o.mutex.unlock();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPopMatrix();
    return;
}

void ofApp::keyPressed(int key)
{
    return;
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->is("* reset gains...")) {
        _gui->getSlider("* L -> L")->setValue(0.0);
        _gui->getSlider("* L -> R")->setValue(0.0);
        _gui->getSlider("* R -> R")->setValue(0.0);
        _gui->getSlider("* R -> L")->setValue(0.0);
        _mutexParam.lock();
        _dsp.getNodeAmplify("LL-amp")->setGain(0.0);
        _dsp.getNodeAmplify("LR-amp")->setGain(0.0);
        _dsp.getNodeAmplify("RR-amp")->setGain(0.0);
        _dsp.getNodeAmplify("RL-amp")->setGain(0.0);
        _mutexParam.unlock();
    }
    else if (e.target->is("refresh...")) {
        _mutexParam.lock();
        _dsp.refresh();
        _mutexParam.unlock();
    }
    return;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if (e.target->is("delay [L] 1")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("L-bf1")->setSize(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("delay [L] 2")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("L-bf2")->setSize(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("delay [R] 1")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("R-bf1")->setSize(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("delay [R] 2")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("R-bf2")->setSize(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* L -> L")) {
        _mutexParam.lock();
        _dsp.getNodeAmplify("LL-amp")->setGain(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* L -> R")) {
        _mutexParam.lock();
        _dsp.getNodeAmplify("LR-amp")->setGain(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* R -> R")) {
        _mutexParam.lock();
        _dsp.getNodeAmplify("RR-amp")->setGain(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* R -> L")) {
        _mutexParam.lock();
        _dsp.getNodeAmplify("RL-amp")->setGain(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("gate [L] max")) {
        _mutexParam.lock();
        _dsp.getNodeGate("L-gat")->setMaximum(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("gate [L] min")) {
        _mutexParam.lock();
        _dsp.getNodeGate("L-gat")->setMinimum(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("gate [R] max")) {
        _mutexParam.lock();
        _dsp.getNodeGate("R-gat")->setMaximum(e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("gate [R] min")) {
        _mutexParam.lock();
        _dsp.getNodeGate("R-gat")->setMinimum(e.target->getValue());
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
        io->stream.setup(this, out, in, SAMPLING_RATE, BUFFER_SIZE, 4);
    }
    return;
}
