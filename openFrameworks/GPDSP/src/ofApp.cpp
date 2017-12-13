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
#define DRAWING_OFFSET  (100)
#define DRAWING_GAIN    (384)

void ofApp::setup(void)
{
    ofxDatGuiFolder* folder;
    
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofBackground(31, 31, 31);
    ofSetWindowTitle("GPDSP (General Purpose DSP) Example 0.6.0        2017 iridium.jp");
    ofSetDataPathRoot(ofFilePath::join(ofFilePath::getEnclosingDirectory(ofFilePath::removeTrailingSlash(ofFilePath::getCurrentExeDir())), "Resources"));
    
    _i.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0f);
    _o.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0f);
    _dsp.newNodeBufferInput("L-in", &_i.buffer[0], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferInput("R-in", &_i.buffer[1], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("L-out", &_o.buffer[0], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("R-out", &_o.buffer[1], BUFFER_SIZE, CHANNEL_SIZE);
    
    _dsp.newNodeSum("L-sum", 3);
    _dsp.newNodeBuffer("L-bf1", 1);
    _dsp.newNodeBuffer("L-bf2", 1);
    _dsp.newNodeAmplify("LL-amp", 0.0f);
    _dsp.newNodeAmplify("LR-amp", 0.0f);
    _dsp.newNodeGate("L-gat", -1.0f, +1.0f);
    _dsp.newNodeSum("R-sum", 3);
    _dsp.newNodeBuffer("R-bf1", 1);
    _dsp.newNodeBuffer("R-bf2", 1);
    _dsp.newNodeAmplify("RR-amp", 0.0f);
    _dsp.newNodeAmplify("RL-amp", 0.0f);
    _dsp.newNodeGate("R-gat", -1.0f, +1.0f);
    
    _dsp.setLinkPositiveI("L-sum", 0, "L-in", 0);
    _dsp.setLinkPositiveI("L-sum", 1, "LL-amp", 0);
    _dsp.setLinkPositiveI("L-sum", 2, "RL-amp", 0);
    _dsp.setLinkPositiveI("L-bf1", 0, "L-sum", 0);
    _dsp.setLinkPositiveI("LL-amp", 0, "L-bf1", 0);
    _dsp.setLinkPositiveI("L-bf2", 0, "L-bf1", 0);
    _dsp.setLinkPositiveI("LR-amp", 0, "L-bf2", 0);
    _dsp.setLinkPositiveI("L-gat", 0, "L-sum", 0);
    _dsp.setLinkPositiveI("L-out", 0, "L-gat", 0);
    
    _dsp.setLinkPositiveI("R-sum", 0, "R-in", 0);
    _dsp.setLinkPositiveI("R-sum", 1, "RR-amp", 0);
    _dsp.setLinkPositiveI("R-sum", 2, "LR-amp", 0);
    _dsp.setLinkPositiveI("R-bf1", 0, "R-sum", 0);
    _dsp.setLinkPositiveI("RR-amp", 0, "R-bf1", 0);
    _dsp.setLinkPositiveI("R-bf2", 0, "R-bf1", 0);
    _dsp.setLinkPositiveI("RL-amp", 0, "R-bf2", 0);
    _dsp.setLinkPositiveI("R-gat", 0, "R-sum", 0);
    _dsp.setLinkPositiveI("R-out", 0, "R-gat", 0);
    
    _gui = new ofxDatGui(2, 2);
    _gui->addFRM();
    _gui->addBreak();
    scanDevice("input", CHANNEL_SIZE, -1, &_i);
    scanDevice("output", -1, CHANNEL_SIZE, &_o);
    _gui->addBreak();
    folder = _gui->addFolder("delay size", ofColor(127, 127, 255));
    folder->addSlider("* L [1]", 1, 100, 1)->setPrecision(0);
    folder->addSlider("* L [2]", 1, 100, 1)->setPrecision(0);
    folder->addSlider("* R [1]", 1, 100, 1)->setPrecision(0);
    folder->addSlider("* R [2]", 1, 100, 1)->setPrecision(0);
    folder->addSlider("* expand range", 1, SAMPLING_RATE * 2 / 100, 1)->setPrecision(0);
    folder->expand();
    _gui->addBreak();
    folder = _gui->addFolder("delay send", ofColor(127, 255, 127));
    folder->addSlider("* L -> L", -1.0f, 1.0f, 0.0f);
    folder->addSlider("* L -> R", -1.0f, 1.0f, 0.0f);
    folder->addSlider("* R -> R", -1.0f, 1.0f, 0.0f);
    folder->addSlider("* R -> L", -1.0f, 1.0f, 0.0f);
    folder->addButton("* reset gains...");
    folder->expand();
    _gui->addBreak();
    _gui->addSlider("gate [L] plus", 0.0f, 1.0f, 1.0f);
    _gui->addSlider("gate [L] minus", 0.0f, 1.0f, 1.0f);
    _gui->addSlider("gate [R] plus", 0.0f, 1.0f, 1.0f);
    _gui->addSlider("gate [R] minus", 0.0f, 1.0f, 1.0f);
    _gui->addBreak();
    _gui->addButton("refresh...");
    _gui->onButtonEvent(this, &ofApp::onButtonEvent);
    _gui->onSliderEvent(this, &ofApp::onSliderEvent);
    _gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    _image.load("graph.png");
    ofxSmartFont::add("ofxbraitsch/fonts/Menlo.ttc", 6 * ((ofxDatGuiIsRetina()) ? (2) : (1)));
    _font = ofxSmartFont::get("Menlo.ttc");
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
#ifdef __GPDSP64
    size *= channel;
    for (int i = 0; i < size; ++i) {
        _i.buffer[i] = buffer[i];
    }
#else
    memcpy(_i.buffer.data(), buffer, _i.buffer.size() * sizeof(float));
#endif
    _mutexIO.unlock();
    _i.mutex.unlock();
    return;
}

void ofApp::audioOut(float* buffer, int size, int channel)
{
    int remain;
    GPDSPError error;
    
    _o.mutex.lock();
    _mutexParam.lock();
    _mutexIO.lock();
    if ((error = _dsp.render(size, &remain)) != GPDSPERROR_OK) {
        cout << "position [L-in]: " << _dsp.getNodeBufferInput("L-in")->getPosition() << endl;
        cout << "position [R-in]: " << _dsp.getNodeBufferInput("R-in")->getPosition() << endl;
        cout << "position [L-out]: " << _dsp.getNodeBufferOutput("L-out")->getPosition() << endl;
        cout << "position [R-out]: " << _dsp.getNodeBufferOutput("R-out")->getPosition() << endl;
        cout << "DSP error: " << _dsp.stringize(error) << ", remain = " << remain << endl;
        _dsp.rewind();
        _dsp.refresh();
    }
    _mutexIO.unlock();
    _mutexParam.unlock();
    _o.mutex.unlock();
#ifdef __GPDSP64
    size *= channel;
    for (int i = 0; i < size; ++i) {
        buffer[i] = _o.buffer[i];
    }
#else
    memcpy(buffer, _o.buffer.data(), _o.buffer.size() * sizeof(float));
#endif
    return;
}

void ofApp::draw(void)
{
    bool valid;
    int i;
    
    ofPushMatrix();
    ofTranslate(_gui->getWidth() + (ofGetWidth() - _gui->getWidth() - _image.getWidth()) / 2, ofGetHeight() / 2);
    ofSetColor(255, 255, 255, 63);
    _image.draw(0, -_image.getHeight() / 2);
    _o.mutex.lock();
    drawValueO("L-in", 191, -309, ofColor(63, 255, 127));
    drawValueO("L-sum", 472, -309, ofColor(63, 255, 127));
    drawValueO("L-gat", 864, -309, ofColor(63, 255, 127));
    drawValueO("L-bf1", 695, -193, ofColor(63, 255, 127));
    drawValueO("L-bf2", 587, -25, ofColor(63, 255, 127));
    drawValueO("LR-amp", 233, 270, ofColor(63, 255, 127));
    drawValueO("LL-amp", 412, -259, ofColor(63, 255, 127));
    drawValueO("R-in", 191, 320, ofColor(255, 63, 127));
    drawValueO("R-sum", 472, 320, ofColor(255, 63, 127));
    drawValueO("R-gat", 864, 320, ofColor(255, 63, 127));
    drawValueO("R-bf1", 695, 204, ofColor(255, 63, 127));
    drawValueO("R-bf2", 587, 36, ofColor(255, 63, 127));
    drawValueO("RL-amp", 233, -259, ofColor(255, 63, 127));
    drawValueO("RR-amp", 412, 270, ofColor(255, 63, 127));
    _o.mutex.unlock();
    ofPopMatrix();
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
    if (e.target->is("* reset gains...")) {
        _gui->getSlider("* L -> L")->setValue(0.0f);
        _gui->getSlider("* L -> R")->setValue(0.0f);
        _gui->getSlider("* R -> R")->setValue(0.0f);
        _gui->getSlider("* R -> L")->setValue(0.0f);
        _mutexParam.lock();
        _dsp.getNodeAmplify("LL-amp")->setGain(0.0f);
        _dsp.getNodeAmplify("LR-amp")->setGain(0.0f);
        _dsp.getNodeAmplify("RR-amp")->setGain(0.0f);
        _dsp.getNodeAmplify("RL-amp")->setGain(0.0f);
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
    if (e.target->is("* L [1]")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("L-bf1")->setSize(e.target->getValue() * _gui->getSlider("* expand range")->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* L [2]")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("L-bf2")->setSize(e.target->getValue() * _gui->getSlider("* expand range")->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* R [1]")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("R-bf1")->setSize(e.target->getValue() * _gui->getSlider("* expand range")->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* R [2]")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("R-bf2")->setSize(e.target->getValue() * _gui->getSlider("* expand range")->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("* expand range")) {
        _mutexParam.lock();
        _dsp.getNodeBuffer("L-bf1")->setSize(_gui->getSlider("* L [1]")->getValue() * e.target->getValue());
        _dsp.getNodeBuffer("L-bf2")->setSize(_gui->getSlider("* L [2]")->getValue() * e.target->getValue());
        _dsp.getNodeBuffer("R-bf1")->setSize(_gui->getSlider("* R [1]")->getValue() * e.target->getValue());
        _dsp.getNodeBuffer("R-bf2")->setSize(_gui->getSlider("* R [2]")->getValue() * e.target->getValue());
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
    else if (e.target->is("gate [L] plus")) {
        _mutexParam.lock();
        _dsp.getNodeGate("L-gat")->setMaximum(+e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("gate [L] minus")) {
        _mutexParam.lock();
        _dsp.getNodeGate("L-gat")->setMinimum(-e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("gate [R] plus")) {
        _mutexParam.lock();
        _dsp.getNodeGate("R-gat")->setMaximum(+e.target->getValue());
        _mutexParam.unlock();
    }
    else if (e.target->is("gate [R] minus")) {
        _mutexParam.lock();
        _dsp.getNodeGate("R-gat")->setMinimum(-e.target->getValue());
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

void ofApp::drawValueO(string const& name, int x, int y, ofColor const& color) const
{
    shared_ptr<GPDSPOutputtableNode> output;
    GPDSPFloat value;
    char temp[16];
    
    if ((output = dynamic_pointer_cast<GPDSPOutputtableNode>(_dsp.getNode(name))) == NULL) {
        ofSetColor(color);
        _font->draw(".......", x, y);
    }
    else if (output->getValueO(0, &value) != GPDSPERROR_OK) {
        ofSetColor(color);
        _font->draw("-------", x, y);
    }
    else if (isinf(value)) {
        ofSetColor(color);
        if (value > 0) {
            _font->draw(" +INF  ", x, y);
        }
        else {
            _font->draw(" -INF  ", x, y);
        }
    }
    else if (isnan(value)) {
        ofSetColor(color);
        _font->draw("  NAN  ", x, y);
    }
    else {
        value = static_cast<int>(value * 10000) / 10000.0f;
        snprintf(temp, sizeof(temp), "%+6.4f", static_cast<float>(value));
        ofSetColor(color, ofMap(fabs(value), 0.0f, 0.01f, 63, 255, true));
        _font->draw(temp, x, y);
    }
    return;
}
