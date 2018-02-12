/*
**      GPDSPGenerative
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
    ofSetWindowTitle("GPDSPGenerative 0.9.1        2017 - 2018 iridium.jp");
    ofSetDataPathRoot(ofFilePath::join(ofFilePath::getEnclosingDirectory(ofFilePath::removeTrailingSlash(ofFilePath::getCurrentExeDir())), "Resources"));
    
    _i.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0f);
    _o.buffer.resize(BUFFER_SIZE * CHANNEL_SIZE, 0.0f);
    _dsp.setRate(SAMPLING_RATE);
    _dsp.newNodeBufferInput("in[L]", &_i.buffer[0], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferInput("in[R]", &_i.buffer[1], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("out[L]", &_o.buffer[0], BUFFER_SIZE, CHANNEL_SIZE);
    _dsp.newNodeBufferOutput("out[R]", &_o.buffer[1], BUFFER_SIZE, CHANNEL_SIZE);
    _error = GPDSPERROR_OK;
    _save = GPDSPERROR_LIMIT;
    
    _gui = new ofxDatGui(2, 2);
    _gui->addFRM();
    _gui->addBreak();
    _gui->addLabel("Error: N/A")->setLabelColor(ofColor(63, 255, 127));
    _gui->addBreak();
    _gui->addLabel("File: N/A")->setLabelColor(ofColor(63, 255, 127));
    _gui->addBreak();
    scanDevice("input", CHANNEL_SIZE, -1, &_i);
    scanDevice("output", -1, CHANNEL_SIZE, &_o);
    _gui->addBreak();
    makeIn("in[L]", 4, "out[L]", ofColor(63, 255, 127));
    makeIn("in[R]", 4, "out[R]", ofColor(255, 63, 127));
    makeOut("out[L]", 4, "in[L]", ofColor(63, 255, 127));
    makeOut("out[R]", 4, "in[R]", ofColor(255, 63, 127));
    _gui->addBreak();
    _gui->addButton("rewind...");
    _gui->addButton("refresh...");
    _gui->onButtonEvent(this, &ofApp::onButtonEvent);
    _gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    _image.load("graph.png");
    
    syncIn("in[L]", 0);
    selectIn("in[L]", 1, "out[L]", "in[R]");
    syncIn("in[R]", 0);
    selectIn("in[R]", 1, "out[R]", "in[L]");
    syncOut("out[L]", 0);
    syncOut("out[R]", 0);
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
    _o.mutex.lock();
    _mutexParam.lock();
    _mutexIO.lock();
    if ((_error = _dsp.render(size)) != GPDSPERROR_OK) {
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

void ofApp::update(void)
{
    GPDSPError error;
    
    if ((error = _error) != _save) {
        _gui->getLabel("Error: N/A")->setLabel("Error: " + _dsp.stringize(error));
        _gui->getLabel("Error: N/A")->setLabelColor((error == GPDSPERROR_OK) ? (ofColor(63, 255, 127)) : (ofColor(255, 63, 127)));
        _save = error;
    }
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

void ofApp::dragEvent(ofDragInfo dragInfo)
{
    int count;
    GPDSPError error;
    
    if (dragInfo.files.size() > 0) {
        if (ofFile::doesFileExist(dragInfo.files[0]) && !ofDirectory::doesDirectoryExist(dragInfo.files[0])) {
            if (chdir(ofFilePath::getEnclosingDirectory(dragInfo.files[0]).c_str()) == 0) {
                _mutexParam.lock();
                _dsp.removeNode("generic");
                syncIn("in[L]", 0);
                selectIn("in[L]", 0, "out[L]", "in[R]");
                syncIn("in[R]", 0);
                selectIn("in[R]", 0, "out[R]", "in[L]");
                syncOut("out[L]", 0);
                selectOut("out[L]", 0, "in[L]", "out[R]");
                syncOut("out[R]", 0);
                selectOut("out[R]", 0, "in[R]", "out[L]");
                if ((error = _dsp.newNodeGeneric("generic", dragInfo.files[0])) == GPDSPERROR_OK) {
                    if (_dsp.getCountI("generic", &count) == GPDSPERROR_OK) {
                        syncIn("in[L]", count);
                        if (count > 0) {
                            selectIn("in[L]", 2, "out[L]", "in[R]");
                        }
                        syncIn("in[R]", count);
                        if (count > 1) {
                            selectIn("in[R]", 3, "out[R]", "in[L]");
                        }
                        if (count <= 0) {
                            ofSystemAlertDialog("Generic file has no input.\n\n" + ofFilePath::getFileName(dragInfo.files[0]));
                        }
                    }
                    if (_dsp.getCountO("generic", &count) == GPDSPERROR_OK) {
                        syncOut("out[L]", count);
                        if (count > 0) {
                            selectOut("out[L]", 2, "in[L]", "out[R]");
                        }
                        syncOut("out[R]", count);
                        if (count > 1) {
                            selectOut("out[R]", 3, "in[R]", "out[L]");
                        }
                        if (count <= 0) {
                            ofSystemAlertDialog("Generic file has no output.\n\n" + ofFilePath::getFileName(dragInfo.files[0]));
                        }
                    }
                    _gui->getLabel("File: N/A")->setLabel("File: " + ofFilePath::getFileName(dragInfo.files[0]));
                    _gui->getLabel("File: N/A")->setLabelColor(ofColor(63, 255, 127));
                }
                else {
                    _gui->getLabel("File: N/A")->setLabel("File: " + _dsp.stringize(error));
                    _gui->getLabel("File: N/A")->setLabelColor(ofColor(255, 63, 127));
                    ofSystemAlertDialog("Generic file open failed : " + _dsp.stringize(error) + "\n\n" + ofFilePath::getFileName(dragInfo.files[0]));
                }
                _mutexParam.unlock();
            }
        }
    }
    return;
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->is("rewind...")) {
        _mutexParam.lock();
        _dsp.rewind();
        _mutexParam.unlock();
    }
    else if (e.target->is("refresh...")) {
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
    else if (e.target->is("in[L] -> N/A")) {
        _mutexParam.lock();
        selectIn("in[L]", e.target->getSelected()->getIndex(), "out[L]", "in[R]");
        _mutexParam.unlock();
    }
    else if (e.target->is("in[R] -> N/A")) {
        _mutexParam.lock();
        selectIn("in[R]", e.target->getSelected()->getIndex(), "out[R]", "in[L]");
        _mutexParam.unlock();
    }
    else if (e.target->is("out[L] <- N/A")) {
        _mutexParam.lock();
        selectOut("out[L]", e.target->getSelected()->getIndex(), "in[L]", "out[R]");
        _mutexParam.unlock();
    }
    else if (e.target->is("out[R] <- N/A")) {
        _mutexParam.lock();
        selectOut("out[R]", e.target->getSelected()->getIndex(), "in[R]", "out[L]");
        _mutexParam.unlock();
    }
    return;
}

void ofApp::makeIn(string const& in, int size, string const& out, ofColor const& color)
{
    vector<string> param;
    int i;
    
    param.push_back(in + " -> N/A");
    param.push_back(in + " -> out[L]");
    for (i = 0; i < size; ++i) {
        param.push_back("");
    }
    _gui->addDropdown(in + " -> N/A", param);
    _gui->getDropdown(in + " -> N/A")->getChildAt(0)->setLabelColor(ofColor(127, 127, 127));
    for (i = 0; i < size; ++i) {
        _gui->getDropdown(in + " -> N/A")->getChildAt(i + 2)->setLabelColor(color);
    }
    return;
}

void ofApp::makeOut(string const& out, int size, string const& in, ofColor const& color)
{
    vector<string> param;
    int i;
    
    param.push_back(out + " <- N/A");
    param.push_back(out + " <- in[L]");
    for (i = 0; i < size; ++i) {
        param.push_back("");
    }
    _gui->addDropdown(out + " <- N/A", param);
    _gui->getDropdown(out + " <- N/A")->getChildAt(0)->setLabelColor(ofColor(127, 127, 127));
    for (i = 0; i < size; ++i) {
        _gui->getDropdown(out + " <- N/A")->getChildAt(i + 2)->setLabelColor(color);
    }
    return;
}

void ofApp::syncIn(string const& label, int size)
{
    ofxDatGuiDropdown* dropdown;
    string name;
    int i;
    
    dropdown = _gui->getDropdown(label + " -> N/A");
    for (i = 0; i < dropdown->size(); ++i) {
        dropdown->getChildAt(i)->setVisible(i - 2 < size);
        if (_dsp.getNameI("generic", i - 2, &name) == GPDSPERROR_OK) {
            dropdown->getChildAt(i)->setLabel(label + " -> " + name);
        }
    }
    return;
}

void ofApp::syncOut(string const& label, int size)
{
    ofxDatGuiDropdown* dropdown;
    string name;
    int i;
    
    dropdown = _gui->getDropdown(label + " <- N/A");
    for (i = 0; i < dropdown->size(); ++i) {
        dropdown->getChildAt(i)->setVisible(i - 2 < size);
        if (_dsp.getNameO("generic", i - 2, &name) == GPDSPERROR_OK) {
            dropdown->getChildAt(i)->setLabel(label + " <- " + name);
        }
    }
    return;
}

void ofApp::selectIn(string const& in, int index, string const& out, string const& other)
{
    _dsp.clearLinkO(in);
    if (_select[out + " <- N/A"] == 1) {
        _gui->getDropdown(out + " <- N/A")->select(0);
        _select[out + " <- N/A"] = 0;
    }
    if (index >= 2) {
        _dsp.setLinkPositiveI("generic", index - 2, in, 0);
        if (_select[other + " -> N/A"] == index) {
            _gui->getDropdown(other + " -> N/A")->select(0);
            _select[other + " -> N/A"] = 0;
        }
    }
    else if (index >= 1) {
        _dsp.setLinkPositiveI(out, 0, in, 0);
        _gui->getDropdown(out + " <- N/A")->select(index);
        _select[out + " <- N/A"] = index;
    }
    _gui->getDropdown(in + " -> N/A")->select(index);
    _select[in + " -> N/A"] = index;
    return;
}

void ofApp::selectOut(string const& out, int index, string const& in, string const& other)
{
    _dsp.clearLinkI(out);
    if (_select[in + " -> N/A"] == 1) {
        _gui->getDropdown(in + " -> N/A")->select(0);
        _select[in + " -> N/A"] = 0;
    }
    if (index >= 2) {
        _dsp.clearLinkO("generic", index - 2);
        _dsp.setLinkPositiveI(out, 0, "generic", index - 2);
        if (_select[other + " <- N/A"] == index) {
            _gui->getDropdown(other + " <- N/A")->select(0);
            _select[other + " <- N/A"] = 0;
        }
    }
    else if (index >= 1) {
        _dsp.clearLinkO(in);
        _dsp.setLinkPositiveI(out, 0, in, 0);
        _gui->getDropdown(in + " -> N/A")->select(index);
        _select[in + " -> N/A"] = index;
    }
    _gui->getDropdown(out + " <- N/A")->select(index);
    _select[out + " <- N/A"] = index;
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
