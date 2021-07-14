//
// take raw data and plot profile
//

#include <iostream>

R__LOAD_LIBRARY(../libData.so);  // this only works if you have $PROJECT defined in .bashrc

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
#include "TCanvas.h" // https://root.cern.ch/doc/master/classTCanvas.html

// 2D Histograms, displays mean value of Z + error for each cell in
#include "TProfile2D.h" // https://root.cern.ch/doc/master/classTProfile2D.html
#include "TH1.h"
#include "TF1.h"


// Define special styles, editing gStyle pointer edits default style
#include <TStyle.h> // https://root.cern.ch/doc/master/classTStyle.html

#include <vector>

// void means this function does not have a return value
// first argument is called "rootfile" of type char*
// second argument is called "nevn" of type int
// We write `testShowProf_rawevent("../data/1000evn_v3.root",100)`

void signal_FFT(char const *rootfile = "../data/1000evn_v3.root", int nevn = 100)
{
	using std::count;  // lets you use count without having to type std::count everytime
	using std::endl;  // lets you use std::endl without having to type std::endl everytime
	gStyle ->SetCanvasDefH(900);  // Arrow sets the SetCanvasDefH variable of the gStyle pointer
	gStyle ->SetCanvasDefW(1500);  // Sets default width/height of canvases

	gSystem->Load("../libData.so");  // from TSystem.h, loads things like RawEvent
	gSystem->Load("../dataClasses/DataCint_rdict.pcm");  // for root6 

	TFile *fr = new TFile(rootfile);  // loads the data in the .root file

	TTree *tr = (TTree*)fr->Get("rtree");  // creates a tree to store the data from data "fr"

	RawEvent *revent = new RawEvent();

	Int_t ch_vpeaksum;

	Double_t event_time;

	Int_t nsum_ch3; 

	double t_min = 1000.;  
	double t_max = 2000.;
	double nt = 5*(t_max - t_min);
	
	vector<double> time_vector;

	TH1F *hist = new TH1F("hist", "Time events histogram;Time (us);Acc. Trig. Events", nt, t_min, t_max);

	tr->SetBranchAddress("ch3.",&revent);  // Relates revent to our .root file data here
	// Specifically, it chooses to read out data from channel 3 in the root file


	for(int i= 0; i< nevn ;i++) {
		// starts to read data from the .root file (now imported as a TTree file called tr)
		tr->GetEntry(i);
		nsum_ch3 = revent->GetVPeakSumSize();  // number of peaks in this event

		for(int j =0; j< nsum_ch3 ;j++)
		{
			event_time = revent->GetVPeakSumTime1()[j]*0.001; //convert to us
			if (t_min < event_time && event_time < t_max) {
				time_vector.push_back(event_time);
				hist->Fill(event_time);
			}
		}
	}
	

	printf("%zu\n", time_vector.size());

	/* 	

	int peak1 = 0;
	int peak2 = 0;
	int peak3 = 0;
	int peak4 = 0;
	int peak5 = 0;

	Int_t bin_number;
	Double_t bin_content;  

	if (0 < event_time && event_time < 3000) {
				hist->Fill(event_time);
			}
					
			else if (067000 < event_time && event_time < 070000) {
				hist->Fill(event_time - 67000 + 3000);
			}
			
			else if (267000 < event_time && event_time < 270000) {
				hist->Fill(event_time - 267000 + 6000);
			}
			else if (334000 < event_time && event_time < 337000) {
				hist->Fill(event_time - 334000 + 9000);
			} 
			else if (534000 < event_time && event_time < 537000) {
				hist->Fill(event_time - 534000 + 12000);
			}

	for (int bin = 0; bin <= nt; bin++) {
		time = hist->GetBinLowEdge(bin);
		if (bin_content > 150) {
					
		   		}
			) {
		

	bin_number = hist->GetBin(event_time);
	bin_content = hist->GetBinContent(bin_number);

	if (bin_content > 150.) {
		   printf("Bin number: %d\t", bin_number);
		   printf("Bin time: %f\t", event_time);
		   printf("Bin content: %f\n", bin_content);
	}

	// if (0 < event_time < 3000) {
	//        Double_t peak1 = hist->GetXaxis()->event_time
		   hist->Fill(event_time);
	// }
	// else if (

	*/

	TCanvas *c1 = new TCanvas(
		"c1",  // name
		"Fourier Transform",  // title
		200,  // wtopx, pixel x-location of top-left corner of the canvas
		10,  // wtopy, pixel y-location of ...
		1800,  // width
		1000  // height
	);
 
	// ////////////// Start making plots ////////////////

	c1->Divide(3,1);

	c1->cd(1);
	hist->Draw();

	TH1 *unscaled_FFT = 0;  // Temporary object
	TVirtualFFT::SetTransform(0);  // Not sure what this does
	unscaled_FFT = hist->FFT(unscaled_FFT, "MAG");  // Do FFT
	TH1 * final_FFT = (TH1*)unscaled_FFT->Clone();  // Clone the FFT to scale it
	double sample_rate = (t_max - t_min) / nt; 
	final_FFT -> SetBins(nt, 0, nt / (sample_rate * nt));  // Scale FFT

	c1->cd(2);
	unscaled_FFT->SetTitle("Unscaled full FFT result;Unscaled frequency;Magnitude");
	unscaled_FFT->Draw();

	c1->cd(3);
	final_FFT->GetXaxis()->SetRange(10,1000);  // skip first bin which is just average DC power
	final_FFT->SetTitle("FFT Result (First 1000 Bins);Frequency (MHz);Magnitude");

	final_FFT->Draw();

	printf("First 1000 bins of FFT result: \n");
	//  for (int i=0; i < 100; i++) {
	//	  printf("%d: ", i);
	//	  printf("%f\n", final_FFT->GetBinContent(i));
	//  }
	int max_bin = final_FFT->GetMaximumBin();
	printf("Max bin: %d\n", max_bin);
	double max_bin_low_edge = final_FFT->GetXaxis()->GetBinLowEdge(max_bin);
	printf("Max bin location (freq. in MHz): %f\n", max_bin_low_edge);
	double period = 1/max_bin_low_edge;
	printf("Predicted period (us): %f\n", period);
	
	t_min = 4;  
	t_max = 8;
	nt = 200*(t_max - t_min);
	
	TH1F *zoom_hist = new TH1F("zoom_hist", "Modulated Period Histogram;Time (us);Acc. Trig. Events", nt, t_min, t_max);
	double split = 0.;
	for (int i = 0; i < time_vector.size(); i++) {
		double mod_result = fmod(time_vector[i], period);
		printf("time: %f\t", time_vector[i]);
		printf("mod result: %f\t", mod_result);
		if (split == 0.) {
			printf("final result: %f\n", mod_result);
			zoom_hist->Fill(mod_result);
		}
		else if (mod_result <= split) {
			printf("final result: %f\n", mod_result + split);
			zoom_hist->Fill(mod_result + split);
		}
		else if (split < mod_result) {
			printf("final result: %f\n", mod_result - (period - split)); 
			zoom_hist->Fill(mod_result - (period - split) );
		}
	}
  
	TCanvas *c2 = new TCanvas(
		"c2",  // name
		"Fine Peak Detail",  // title
		200,  // wtopx, pixel x-location of top-left corner of the canvas
		10,  // wtopy, pixel y-location of ...
		1800,  // width
		1000  // height
		);
		
	c2->Divide(2, 1); 
	
	c2->cd(1);
	zoom_hist->Draw();
	
	TH1 *unscaled_modulated_FFT = 0;  // Temporary object
	TVirtualFFT::SetTransform(0);  // Not sure what this does
	unscaled_modulated_FFT = zoom_hist->FFT(unscaled_modulated_FFT, "MAG");  // Do FFT
	TH1 * final_modulated_FFT = (TH1*)unscaled_modulated_FFT->Clone();  // Clone the FFT to scale it
	sample_rate = (t_max - t_min) / nt; 
	final_modulated_FFT -> SetBins(nt, 0, nt / (sample_rate * nt));  // Scale FFT
	
	final_modulated_FFT->GetXaxis()->SetRange(20,nt/2);  // skip first bin which is just average DC power
	
	c2->cd(2);
	final_modulated_FFT->Draw();
	
	return 0;

}
