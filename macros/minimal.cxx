//
// take raw data and plot profile
//

#include <iostream>

R__LOAD_LIBRARY(../libData.so)  // this only works if you have $PROJECT defined in .bashrc

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
#include <TH1.h>

// Define special styles, editing gStyle pointer edits default style
#include <TStyle.h> // https://root.cern.ch/doc/master/classTStyle.html

// void means this function does not have a return value
// first argument is called "rootfile" of type char*
// second argument is called "nevn" of type int
// We write `testShowProf_rawevent("../data/1000evn_v3.root",100)`

void f()
{
  char const *rootfile = "../data/1000evn_v3.root";
  int nevn = 100;
  using std::count;  // lets you use count without having to type std::count everytime
  using std::endl;  // lets you use std::endl without having to type std::endl everytime
  gStyle ->SetCanvasDefH(900);  // Arrow sets the SetCanvasDefH variable of the gStyle pointer
  gStyle ->SetCanvasDefW(1500);  // Sets default width/height of canvases


  gSystem->Load("../libData.so");  // from TSystem.h, loads things like RawEvent
  gSystem->Load("../dataClasses/DataCint_rdict.pcm");  // for root6 

  TFile *fr = new TFile(rootfile);  // loads the data in the .root file

  TTree *tr = (TTree*)fr->Get("rtree");  // creates a tree to store the data from data "fr"
  // In this case, our file 1000evn_v3.root has two keys inside, both named rtree of type TTree (maybe for each batch?), one is called "Cycle 11", one is "Cycle 12"
  // To view what is inside it, go to the data folder, open root, type `TFile f("1000evn_v3.root")` then `TBrowser browser`.
  // More info here: https://root.cern/manual/storing_root_objects/

  // RawEvent objects store events in a format close to the raw data that comes out of the detector  
  RawEvent *revent = new RawEvent();

  Int_t ch_vpeaksum;

  Double_t ch_vpeaksum_time1_ms;

  Int_t nsum_ch3;  

  TProfile2D *hprof2d_vpeaksum_ms = new TProfile2D(
		  "hprof2d_vpeaksum_ms",  // name
		  "vprofile 2D peaksum ms  ",  // title
		  
		  600,  // nx
		  //2,
		  -50,  // xlow
		  550,  // xup

		  5000,  // ny
		  0,  // ylow
		  500,  // yup

		  -30000,  // zlow, ignore events with voltage below this
		  0  // zup
		  );

  tr->SetBranchAddress("ch3.",&revent);  // Relates revent to our .root file data here
  // Specifically, it chooses to read out data from channel 3 in the root file

  int total_count = 0;

  for(int i= 0; i< nevn ;i++)
   {
      printf("%d", i);	   
   	// starts to read data from the .root file (now imported as a TTree file called tr)
        tr->GetEntry(i);
        nsum_ch3 = revent->GetVPeakSumSize();  // number of peaks in this event
        printf("event %d \t", i);  // %d inserts the variable "i" into the string at %d
        printf("nsum_ch3 = %d\n",nsum_ch3);

       for(int j =0; j< nsum_ch3 ;j++)
       {
	       total_count++;
          ch_vpeaksum=revent->GetVPeakSum()[j];

          ch_vpeaksum_time1_ms=revent->GetVPeakSumTime1()[j]*0.000001; //convert to ms

	printf("ch_vpeaksum_time1_ms: %.2f\t", ch_vpeaksum_time1_ms);
	printf("i: %d\t", i);
	printf("ch_vpeaksum: %d\t", ch_vpeaksum);
	printf("(%d, %d)\n", j, total_count);


          hprof2d_vpeaksum_ms -> Fill(
			  ch_vpeaksum_time1_ms,
			  i,
			  ch_vpeaksum,
			  1
			  );

       }
  }
  

  TCanvas *c_prof_ms = new TCanvas(
		  "c_prof_ms",  // name
		  "profile sum ms ",  // title
		  200,  // wtopx, pixel x-location of top-left corner of the canvas
		  10,  // wtopy, pixel y-location of ...
		  700,  // width
		  500  // height
		  );

  TCanvas *xhist = new TCanvas(
		  "xhist",  // name
		  "xhist",  // title
		  200,  // wtopx, pixel x-location of top-left corner of the canvas
		  10,  // wtopy, pixel y-location of ...
		  700,  // width
		  500  // height
		  );

  TCanvas *yhist = new TCanvas(
		  "yhist",  // name
		  "yhist",  // title
		  200,  // wtopx, pixel x-location of top-left corner of the canvas
		  10,  // wtopy, pixel y-location of ...
		  700,  // width
		  500  // height
		  );

  c_prof_ms->cd();
  hprof2d_vpeaksum_ms->Draw("colz");

  xhist->cd();
  TH1* hx = hprof2d_vpeaksum_ms->ProfileX();
  hx->Draw();

  yhist->cd();
  TH1* hy = hprof2d_vpeaksum_ms->ProfileY();
  hy->Draw();

  return 0;

}
