//
// take raw data and plot profile
//

// brackets look for header files in predetermined directory path, used for system header files
// looks relative to current folder, used for user header files

// TTree is a list of columns/branches called TBranch
#include <TTree.h> // https://root.cern.ch/doc/master/classTTree.html

// TGraph is a graph class to make graphs by specifying all x and y points
#include <TGraph.h> // https://root.cern.ch/doc/master/classTGraph.html

// References a ROOT data file
#include <TFile.h> // https://root.cern.ch/doc/master/classTFile.html

// TSystem interacts with the OS. For us, this is the Load command
#include <TSystem.h> // https://root.cern.ch/doc/master/classTSystem.html

// Creates a window to put graphs in (we create three)
#include <TCanvas.h> // https://root.cern.ch/doc/master/classTCanvas.html

// 2D Histograms, displays mean value of Z + error for each cell in
#include <TProfile2D.h> // https://root.cern.ch/doc/master/classTProfile2D.html

// Define special styles, editing gStyle pointer edits default style
#include <TStyle.h> // https://root.cern.ch/doc/master/classTStyle.html

#include "RawEvent.h" 


// void means this function does not have a return value
// first argument is called "rootfile" of type char*
// second argument is called "nevn" of type int
// We write `testShowProf_rawevent("../data/1000evn_v3.root",100)`
void testShowProf_rawevent(char* rootfile, int nevn)
{

  using std::count;  // lets you use count without having to type std::count everytime
  using std::endl;  // lets you use std::endl without having to type std::endl everytime
  gStyle ->SetCanvasDefH(900);  // Arrow sets the SetCanvasDefH variable of the gStyle pointer
  gStyle ->SetCanvasDefW(1500);  // Sets default width/height of canvases

  gSystem->Load("../libData.so");  // from TSystem.h, loads things like RawEvent

  TFile *fr = new TFile(rootfile);  // loads the data in the .root file

  TTree *tr = (TTree*)fr->Get("rtree");  // creates a tree to store the data from data "fr"
  // In this case, our file 1000evn_v3.root has two keys inside, both named rtree of type TTree (maybe for each batch?), one is called "Cycle 11", one is "Cycle 12"
  // To view what is inside it, go to the data folder, open root, type `TFile f("1000evn_v3.root")` then `TBrowser browser`.
  // More info here: https://root.cern/manual/storing_root_objects/

  // RawEvent objects store events in a format close to the raw data that comes out of the detector  
  RawEvent *revent = new RawEvent();

  TGraph *gr[10000];
  Int_t x[10000];
  Int_t y[10000];
  Int_t peakhigh[10000];
  Int_t vpeakhigh[10000];
  Int_t vpeaksum[10000];
  Int_t peaktime[10000];
  Int_t peaksum[10000];
  Int_t peaksumtime1[10000];
  Int_t peaksumtime2[10000];


  Int_t ch_vpeakhigh[10000][10000];
  Int_t ch_vpeakhigh_time_us[10000][10000];
  Int_t ch_vpeakhigh_time_ns[10000][10000];

  Long_t ch_vpeaksum0[10000][10000];
  Int_t ch_vpeaksum0_peak[10000][10000];
  Long_t ch_vpeaksum_tail[10000][10000];
  Long_t ch_vpeaksum_peak[10000][10000];


  Int_t ch_vpeaksum[10000][10000];
  Int_t ch_vpeaksum_time0_us[10000][10000];
  UInt_t ch_vpeaksum_time0_ns[10000][10000];

  Double_t ch_vpeaksum_time1_ms[10000][10000];
  Double_t ch_vpeaksum_time1_us[10000][10000];
  UInt_t ch_vpeaksum_time1_ns[10000][10000];

  Double_t ch_vpeaksum_time2_us[10000][10000];
  UInt_t ch_vpeaksum_time2_ns[10000][10000];

  Int_t npeak_ch0[10000];
  Int_t npeak_ch1[10000];
  Int_t npeak_ch2[10000];
  Int_t npeak_ch3[10000];

  Int_t nsum_ch0[10000];
  Int_t nsum_ch1[10000];
  Int_t nsum_ch2[10000];
  Int_t nsum_ch3[10000];

  // TGraph *gr[10000];  // Defined twice
  Int_t nraw[10000];
  Int_t raw_x[10000];
  Int_t raw_y[10000];

  int nevents = tr->GetEntries();



  hprof2d_vpeakheight_ms = new TProfile2D("hprof2d_vpeakheight_ms","vprofile 2D peakheight ms  ",600,-50,550,5000,0,5000,-2500,0);

  hprof2d_vpeaksum_ms = new TProfile2D("hprof2d_vpeaksum_ms","vprofile 2D peaksum ms  ",600,-50,550,5000,0,5000,-30000,0);
  hprof2d_vpeaksum_160ms = new TProfile2D("hprof2d_vpeaksum_160ms","vprofile 2D peaksum 160ms  ",210,-50,160,5000,0,5000,-30000,0);
  hprof2d_vpeaksum_us_batch1 = new TProfile2D("hprof2d_vpeaksum_us_batch1","vprofile 2D peaksum us batch 1 ",3000,0,3000,5000,0,5000,-30000,0);
  hprof2d_vpeaksum_us_batch2 = new TProfile2D("hprof2d_vpeaksum_us_batch2","vprofile 2D peaksum us batch 2 ",3000,66000,69000,5000,0,5000,-30000,0);

  tr->SetBranchAddress("ch3.",&revent);  // Relates revent to our .root file data here
  // Specifically, it chooses to read out data from channel 3 in the root file
  printf("CH3 CH3 CH3 \n");
  for(int i= 0; i< nevn ;i++)
  //for(int i= 0; i< nevents ;i++)
  //for(int i= 0; i< 10 ;i++)
   { 
   	// starts to read data from the .root file (now imported as a TTree file called tr)
        tr->GetEntry(i);
	nraw[i] = revent -> GetVAmpSize();
        npeak_ch3[i] = revent->GetVPeakHighSize();
        nsum_ch3[i] = revent->GetVPeakSumSize();
	printf("event %d \t", i);  // %d inserts the variable "i" into the string at %d
        printf("nraw_ch3 = %d\t",nraw[i]);
        printf("npeak_ch3 = %d\t",npeak_ch3[i]);
        printf("nsum_ch3 = %d\n",nsum_ch3[i]);


        for(int j =0; j< npeak_ch3[i] ;j++)
        {	   
           ch_vpeakhigh[i][j]=revent->GetVPeakHigh()[j];
           ch_vpeakhigh_time_us[i][j]=revent->GetVPeakHighTime()[j]*0.001; //convert to us
           ch_vpeakhigh_time_ns[i][j]=revent->GetVPeakHighTime()[j]; //ns
           hprof2d_vpeakheight_ms -> Fill(ch_vpeakhigh_time_us[i][j]*0.001,i,ch_vpeakhigh[i][j],1);       
       }               

       for(int j =0; j< nsum_ch3[i] ;j++)
       {
          ch_vpeaksum0[i][j]=revent->GetVPeakSum0()[j];
          ch_vpeaksum[i][j]=revent->GetVPeakSum()[j];
          ch_vpeaksum0_peak[i][j]=revent->GetVPeakSum0_peak()[j];

          ch_vpeaksum_time0_us[i][j]=revent->GetVPeakSumTime0()[j]*0.001; //convert to us
          ch_vpeaksum_time0_ns[i][j]=revent->GetVPeakSumTime0()[j]; //ns

          ch_vpeaksum_time1_ms[i][j]=revent->GetVPeakSumTime1()[j]*0.000001; //convert to ms
          ch_vpeaksum_time1_us[i][j]=revent->GetVPeakSumTime1()[j]*0.001; //convert to us
          ch_vpeaksum_time1_ns[i][j]=revent->GetVPeakSumTime1()[j]; //ns
          //printf("time sum1 = %d \t", ch_vpeaksum_time1_ns[i][j]);
          //printf("time sum0 = %d \t", ch_vpeaksum_time0_ns[i][j]);

          ch_vpeaksum_time2_us[i][j]=revent->GetVPeakSumTime2()[j]*0.001; //convert to us
          ch_vpeaksum_time2_ns[i][j]=revent->GetVPeakSumTime2()[j]; //ns
          //printf("time sum2 = %d \n", ch_vpeaksum_time2_ns[i][j]);


          hprof2d_vpeaksum_ms -> Fill(ch_vpeaksum_time1_ms[i][j],i,ch_vpeaksum[i][j],1);

             if(ch_vpeaksum_time1_us[i][j] < 3000)
               {
                hprof2d_vpeaksum_us_batch1 -> Fill(ch_vpeaksum_time1_us[i][j],i,ch_vpeaksum[i][j],1);
               } else if(ch_vpeaksum_time1_us[i][j] > 66000 && ch_vpeaksum_time1_us[i][j] < 69000)
               {
                hprof2d_vpeaksum_us_batch2 -> Fill(ch_vpeaksum_time1_us[i][j],i,ch_vpeaksum[i][j],1);
               } 
       }
      

  }
        
        
 
  

   TCanvas *c_prof_ms = new TCanvas("c_prof_ms","profile sum ms ",200,10,700,500);
   TCanvas *c_prof_us_batch1 = new TCanvas("c_prof_us_batch1","profile sum us batch1",200,10,700,500);
   TCanvas *c_prof_us_batch2 = new TCanvas("c_prof_us_batch2","profile sum us batch2",200,10,700,500);

  c_prof_ms->cd();
  hprof2d_vpeaksum_ms->Draw("colz");

  c_prof_us_batch1->cd();
  hprof2d_vpeaksum_us_batch1->Draw("colz");

  c_prof_us_batch2->cd();
  hprof2d_vpeaksum_us_batch2->Draw("colz");

}
