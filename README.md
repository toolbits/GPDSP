# GPDSP
A General Purpose DSP Library, written in C++.  

This is a Digital Signal Processing Library for general purpose use (for example  
audio stream, data sensing, etc...) in C++ application.  
Very tiny implementation enables you to use this library under the small resource environment.  
Very simple to use, normally you have to use only GPDSPNodeRenderer class.  

GPDSP supports next operations:  

o GPDSPBufferInputNode: inputs data from some data stream  
o GPDSPBufferOutputNode: outputs data to some data stream  
o GPDSPConstantNode: makes a constant value (such as DC bias)  
o GPDSPGateNode: rectifies or clips a value (such as Diode)  
o GPDSPPeekNode: keeps the maximum amplitude  
o GPDSPAmplifyNode: amplifies a value (such as Transistor)  
o GPDSPDelayNode: single sample delay  
o GPDSPBufferNode: N samples delay  
o GPDSPSumNode: adds all input values  
o GPDSPMultiplyNode: multiplies all input values  

An audio processing example application for openFrameworks:  

<img width="960" alt="screenshot" src="https://user-images.githubusercontent.com/1215065/31654098-dfcbfa94-b35f-11e7-82df-b0464a10f22a.png">

The openFrameworks exmaple code needs ofxDatGui addon.  
Please install ofxDatGui first:  
https://github.com/braitsch/ofxDatGui

When you compile the example, please locates the project under:  

of_v0.9.8_osx_release/
    apps/
        myApps/
            someCode/
            ...
    GPDSP/  <<-- here
        openFrameworks/
            GPDSP/
                GPDSP.xcodeproj
                ...

Compiled binary is here:  
https://github.com/toolbits/GPDSP/tree/master/release

Development environment:  

o macOS High Sierra (10.13)  
o Xcode 9.0 (Apple LLVM 9.0.0)  
o openFrameworks 0.9.8  
