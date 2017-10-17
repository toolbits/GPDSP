# GPDSP
A General Purpose DSP Library, written in C++.  

This is a Digital Signal Processing Library for general purpose use in C++ application.  
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
