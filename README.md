# GPDSP
A real time / non-real time General Purpose DSP library, written in C++.  

This is a Digital Signal Processing Library for general purpose use (for example  
audio stream, data sensing, etc...) in C++ application.  
Very tiny implementation enables you to use this library under the small resource environment.  
It is simple to use, normally you have to use only GPDSPNodeRenderer class.  
GPDSP supports generatively constructed filters from external XML files (.gpdsp file).  
Both 32bits and 64bits floating point operations are supported.  

GPDSP supports next operations:  

o GPDSPBufferInputNode: inputs data from some data stream  
o GPDSPBufferOutputNode: outputs data to some data stream  
o GPDSPConstantNode: makes a constant value (such as DC bias)  
o GPDSPSignNode: compare value with zero  
o GPDSPGateNode: rectifies or clips a value (such as Diode)  
o GPDSPPeekNode: keeps the maximum amplitude  
o GPDSPAmplifyNode: amplifies a value (such as Transistor)  
o GPDSPDelayNode: makes single sample delay  
o GPDSPBufferNode: makes N samples delay  
o GPDSPSumNode: adds all input values  
o GPDSPMultiplyNode: multiplies all input values  
o GPDSPSquareRootNode: gets square rooted value  
o GPDSPGenericNode: generatively constructed filter  
o GPDSPSinWaveNode: generate sin wave with customizable frequency and phase  
o GPDSPTriangleWaveNode: generate triangle wave with customizable frequency and phase  
o GPDSPSawtoothWaveNode: generate sawtooth wave with customizable frequency and phase  
o GPDSPSquareWaveNode: generate square wave with customizable frequency and phase  

An audio processing example application for openFrameworks:  

<img width="960" alt="screenshot" src="https://user-images.githubusercontent.com/1215065/34238138-8f419cca-e643-11e7-875b-2ce06e682744.png">
<img width="960" alt="screenshot" src="https://user-images.githubusercontent.com/1215065/34238146-950483ca-e643-11e7-9e03-78d545a7fbbe.png">

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

o macOS High Sierra (10.13.2)  
o Xcode 9.0 (Apple LLVM 9.0.0)  
o openFrameworks 0.9.8  
