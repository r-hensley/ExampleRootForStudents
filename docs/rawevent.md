Inside the 1000evn\_v3.root file, there is the following directory structure: 

```
- rtree;11 [backup cycle] (tree for FMC228 analysis)  
  - ch0.  
  - ch1.  
  - ch2.  
  - ch3.  
- rtree;12 [current cycle] (tree for FMC228 analysis)  
  - ch0.  
  - ch1.  
  - ch2.  
  - ch3.  
```

The backup cycle has 979 entries, and the current cycle has 1001 entries.

Inside the channels in each of the trees is a variety of objects:  
  
- fGraphSignal (TGraph\*)  
- fAmp 
- fTime  
- fPeakHigh  
- fPeakHighTime  
- fPeakSum  
- fPeakSum0  
- fPeakSum0\_peak  
- fPeakSumTime0  
- fPeakSumTime1  
- fPeakSumTime2  
- fPeakSum\_tail  

Other than fGraphSignal which is of type TGraph\*, all the other objects have equivalent TArrayD and vector<Double_t> versions of them stored, with the vector versions being named fvAmp, fvTime, etc.
