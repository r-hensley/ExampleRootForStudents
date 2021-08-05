Inside the `1000evn\_v3.root` file, there is the following directory structure:

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

#### Branches

Inside the channels in each of the trees is a variety of objects (and my yet-vague guesses at to what each represent): 
  
- fGraphSignal (TGraph\*)
- fPeakHigh - voltage? 1001 rows, each row has ~500-1000 instances, values range from around 0 to -2000
- fPeakSum0\_peak - completely equal to fPeakHigh
- fPeakSum0 - voltage? - not sure what difference between this and fPeakHigh is
- fPeakSum - peak area - ranges from 0 to -100,000, a few random values of -26843545 (decimal representation of hex 1999999)
- fPeakSumTime0 - time of tip of biggest peak
- fPeakSumTime1 - time of beginning of peak
- fPeakSumTime2 - time of end of peak
- fPeakHighTime - times of centers of every peak, even joined peaks - full data, times of peaks
- fAmp - 1001 rows, all 0
- fTime - 1001 rows, all 0
- fPeakSum\_tail - 1001 rows, all 0


Other than `fGraphSignal` which is of type `TGraph*`, all the other objects have equivalent `TArrayD` and `vector<Double_t>` versions of them stored, with the vector versions being named `fvAmp`, `fvTime`, etc.

#### Available functions

Some of the functions available in the RawEvent class and what they typically return for our data.
- `GetVPeakSum0_peakSize` > (Double_t) ~ 500 - 1000
- `GetVPeakSumSize()` / `GetVPeakSum0Size()` / `GetVPeakSum0_peakSize()` > (Double_t) ~ 300 - 500
- `GetVAmpSize()` / `GetVPeakSum_tailSize()` > (Double_t) 0
- `GetVPeakSum0_peak()`: -804.000000
- `GetVPeakHigh()`: -804.000000
- `GetVPeakSum0()`: -121.000000
- `GetVPeakSum()`: -13813.000000
- `GetVPeakSumTime1()`: 313980.000000
- `GetVPeakHighTime()`: 313986.000000
- `GetVPeakSumTime0()`: 313988.000000
- `GetVPeakSumTime2()`: 314016.000000
- `GetVAmp()`: No data
- `GetVTime()`: No data
- `GetVPeakSum_tail()`: No data



