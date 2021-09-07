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
// We write `testShowProf_rawevent("../data/1000evn_v3.root",10)`

Double_t calculate_FFT(TH1F* histo_in, int nt, double sample_rate, double freq_res) {
		delete gROOT -> FindObject("final_FFT");
		delete gROOT -> FindObject("out_MAG");
		delete gROOT -> FindObject("out_MAG");
		delete gROOT -> FindObject("c1");
		
		TCanvas *c1 = new TCanvas(
		"c1",  // name
		"Fourier Transform",  // title
		200,  // wtopx, pixel x-location of top-left corner of the canvas
		10,  // wtopy, pixel y-location of ...
		1250,  // width
		1000  // height
		);
	 
		// ////////////// Start making plots ////////////////

		c1->Divide(2,1);

		c1->cd(1);
		histo_in -> Draw();

		TH1 *unscaled_FFT = 0;  // Temporary object
		// unscaled_FFT = TH1::TransformHisto(array_fft, unscaled_FFT, "RE");  // from alternate method

		TVirtualFFT::SetTransform(0);  // Not sure what this does
		unscaled_FFT = histo_in -> FFT(unscaled_FFT, "MAG");  // Do FFT
		TH1 * final_FFT = (TH1*)unscaled_FFT->Clone();  // Clone the FFT to scale it
		printf("Sample rate: %f\nMax bin label: %f\n", sample_rate, nt / (sample_rate * nt));
		final_FFT -> SetBins(nt, 0, nt / (sample_rate * nt));  // Scale FFT

		c1->cd(2);
		unscaled_FFT->SetTitle("Unscaled full FFT result;Unscaled frequency;Magnitude");
		unscaled_FFT->Draw();

		c1->cd(3);
		final_FFT->GetXaxis()->SetRange(50,nt/2);  // skip first bin which is just average DC power
		final_FFT->SetTitle("FFT Result (First 1000 Bins);Frequency (MHz);Magnitude");

		final_FFT->Draw();

		// ////////// Calculate period, print stats /////////////
		
		int max_bin = final_FFT -> GetMaximumBin();  // get max bin number
		
		Double_t max_bin_content = final_FFT -> GetBinContent(max_bin);
		Double_t before_bin_content = final_FFT -> GetBinContent(max_bin - 1);
		Double_t after_bin_content = final_FFT -> GetBinContent(max_bin + 1);

		// Interpolation algorithm http://www.add.ece.ufl.edu/4511/references/ImprovingFFTResoltuion.pdf
		Double_t bin_shift = (log(after_bin_content/before_bin_content))/(2*log((max_bin_content * max_bin_content)/(before_bin_content * after_bin_content)));
		
		Double_t new_bin = max_bin + bin_shift;
		Double_t freq = (new_bin - 1) * freq_res;

		Double_t max_bin_low_edge = final_FFT -> GetXaxis() -> GetBinLowEdge(max_bin);

		Double_t period = 1/freq;

		printf("Max bin: %d\n", max_bin);
		printf("Max bin location (freq. in MHz): %f\n", max_bin_low_edge);
		printf("Predicted frequency (MHz): %f\n", freq);
		printf("Predicted period (us): %f\n", period);

		return period;
		
	}

void signal_FFT(char const *rootfile = "../data/1000evn_v3.root", int nevn = 1000)
{
	delete gROOT -> FindObject("c1");
	delete gROOT -> FindObject("c2");
	delete gROOT -> FindObject("c3");
	delete gROOT -> FindObject("c4");
	delete gROOT -> FindObject("c5");
	delete gROOT -> FindObject("c6");
	delete gROOT -> FindObject("hist");
	delete gROOT -> FindObject("hit_difference");
	delete gROOT -> FindObject("time_vs_modulated");
	delete gROOT -> FindObject("event_vs_modulated");
	delete gROOT -> FindObject("event_vs_difference");
	delete gROOT -> FindObject("event_vs_time");
	delete gROOT -> FindObject("out_MAG");
	
	gSystem->Load("../libData.so");  // from TSystem.h, loads things like RawEvent
	gSystem->Load("../dataClasses/DataCint_rdict.pcm");  // for root6 

	TFile *fr = new TFile(rootfile);  // loads the data in the .root file

	TTree *tr = (TTree*)fr->Get("rtree");  // creates a tree to store the data from data "fr"

	RawEvent *revent = new RawEvent();

	Int_t ch_vpeaksum;

	Double_t event_time;
	Double_t event_time_us;
	Double_t first_event_time;
	Double_t last_event_time;
	Double_t difference;
	Double_t mod_result;
	Int_t above_100ms = 0;
	Int_t num_of_events = 0;

	Int_t nsum_ch3; 

	// double freq_res = 0.001;  // Width of bins in MHz on final FFT 
	double nyquist_freq = 3;
	double sample_rate = 1/(nyquist_freq * 2);

	double t_min = 150 * 1000;
	double t_max = 235 * 1000;
	double freq_res = 1 / (t_max - t_min);
	int nt = (t_max - t_min) / sample_rate;

	vector<Double_t> time_vector;

	printf("Sample rate: %f\tt_max: %f\tnt: %d\n", sample_rate, t_max, nt);
	
	TH1F *hist = new TH1F("hist", "Time events histogram;Time (us);Acc. Trig. Events", nt, t_min, t_max);
	
	TH1F *hit_difference = new TH1F("hit_difference", 
									"Relative to first hit;Time Difference;Magnitude", 
									nt, 0, t_max - t_min);
									
	TH2F *time_vs_modulated = new TH2F("time_vs_modulated", "Modulated periods per event time;Modulated time (us);Event time (ms)", 
    50, -11.134488/2, 11.134488/2,
    // 100, -11.134488/2, 11.134488/2, 
    200, 0, 550);	
    
    TH2F *event_vs_modulated = new TH2F("event_vs_modulated", "Modulated periods per event number;Modulated time (us);Event number", 
    50, -11.134488/2, 11.134488/2,
    // 100, -11.134488/2, 11.134488/2, 
    250, 0, 1000);	
    
    TH2F *event_vs_difference = new TH2F("event_vs_difference", "Hit differences per event;Hit differences (us);Event number", 
    10000, 0, 10000,
    // 100, -11.134488/2, 11.134488/2, 
    250, 0, 1000);	
    
    TH2F *event_vs_time = new TH2F("event_vs_time", "Event times per event;Event time (us);Event number", 
    550, 0, 550,
    // 100, -11.134488/2, 11.134488/2, 
    100, 0, 1000);


	// ////////////// Pull data out of .ROOT file ///////////////
	
	
	tr->SetBranchAddress("ch3.",&revent);  // Relates revent to our .root file data here
	// Specifically, it chooses to read out data from channel 3 in the root file

	for(int i= 0; i< nevn ;i++) {
		// starts to read data from the .root file (now imported as a TTree file called tr)
		tr->GetEntry(i);
		nsum_ch3 = revent->GetVPeakSumSize();  // number of peaks in this event
		
		first_event_time = 0.;
		last_event_time = 0.;

		for(int j =0; j< nsum_ch3 ;j++)
		{
			event_time = revent->GetVPeakSumTime1()[j] * 0.001; // in us
			
			
			// First hit thresshold!
			// first peak: 68.8 - 69.5
			// second peak: 79.9 - 80.6
			// third peak: 90.8 - 91.6
			// fourth peak: 102.3 - 102.8
			if ( j == 0 ) {
				if (event_time < 60 || 105 < event_time) { break; }
				}
				
			if ( event_time < 150 * 1000 || 235 * 1000 < event_time ) {
				continue;  // only take values in range of bunch extractions
			}
				
			num_of_events++;
			hist -> Fill(event_time);
			
			if (first_event_time == 0) {
				first_event_time = event_time;
				
			}
			
			difference = event_time - first_event_time;
			
			if (event_time - last_event_time > 1000) {
				first_event_time = event_time; 
				difference = event_time - first_event_time;
			}
			last_event_time = event_time; 
			
			hit_difference -> Fill(difference);
			time_vector.push_back(event_time);
			
			mod_result = fmod(difference, 11.134488);
			
			if (mod_result > 11.134488 / 2) { mod_result = mod_result - 11.134488; }

			// modulated_th2f -> Fill( mod_result, (difference + first_event_time) / 1000 );
			time_vs_modulated -> Fill( mod_result , event_time / 1000 );
			event_vs_modulated -> Fill( mod_result, i );
			event_vs_difference -> Fill( difference, i );
			event_vs_time -> Fill( event_time / 1000, i ); 
			
			if (event_time / 1000 > 80) { above_100ms++; }
			
			// printf("%f\t%f\t%f\t%f\n", first_event_time, event_time, difference, mod_result);		
			
			
		
		}
	}

	printf("time_vector.size(): %zu\n", time_vector.size());
	printf("Number of hits greater than 80ms: %d\n", above_100ms);
	printf("Number of events: %d\n", num_of_events);
	
	/*  ALTERNATE METHOD OF FFT, gives same period
	Double_t *FFT_in = &time_vector[0];
	Int_t n_size = time_vector.size() + 1;
	Double_t re, im;
	TVirtualFFT *array_fft = TVirtualFFT::FFT(1, &n_size, "R2C ES K");
	array_fft->SetPoints(FFT_in);
	array_fft->Transform();
	array_fft->GetPoints(FFT_in);
	*/
	

	printf("\n** Hit differences histogram: **\n");
	Double_t hit_difference_period = calculate_FFT(hit_difference, nt, sample_rate, freq_res);

	printf("\n** Original histogram: **\n");
	Double_t original_period = calculate_FFT(hist, nt, sample_rate, freq_res);	
	
	printf("\n");
	
	Double_t period = 9993.428571429;

	// /////////// Construct second canvas of histograms //////////////

	int time_per_bin_ns = 3; // ns per bin
	Double_t time_per_bin_us = time_per_bin_ns / 1000.; 
	
	t_min = -period/2;  
	t_max = period/2;
	nt = t_max / time_per_bin_us;
	
	TH1F *zoom_hist = new TH1F(
	"zoom_hist", 
	"Period-Modulated Histogram;Time (us);Acc. Trig. Events", 
	nt, t_min, t_max);
	
	// shift period/2 <--> period to -period/2 <--> 0
	for (int i = 0; i < time_vector.size(); i++) { 
		Double_t mod_result = fmod(time_vector[i], period);  // modulo
		if (mod_result <= period / 2) { zoom_hist -> Fill( mod_result ); }
		else if (period / 2 < mod_result) { zoom_hist -> Fill( mod_result - period ); }
	}
  
	TCanvas *c2 = new TCanvas(
		"c2",  // name
		"Fine Peak Detail",  // title
		200,  // wtopx, pixel x-location of top-left corner of the canvas
		10,  // wtopy, pixel y-location of ...
		1800,  // width
		1000  // height
		);
		
	// c2->Divide(3, 1); 
	
	c2->cd(1);
	zoom_hist->Draw();
	
	/*	
	c2->cd(2);
	TH1F * zoom_hist2 = (TH1F*)zoom_hist->Clone();
	zoom_hist2 -> GetXaxis() -> SetRangeUser(1.95, 4.00);
	std::string period_text = "Period: " + std::to_string(period) + " us";	
	zoom_hist2 -> SetTitle(period_text.c_str());
	zoom_hist2 -> Draw();
	
	c2->cd(3);
	TH1F * zoom_hist3 = (TH1F*)zoom_hist->Clone();
	zoom_hist3 -> GetXaxis() -> SetRangeUser(2.1, 2.25);
	std::string time_per_bin_text = "Time per bin: " + std::to_string(time_per_bin_ns) + " ns";
	zoom_hist3 -> SetTitle(time_per_bin_text.c_str());
	zoom_hist3 -> Draw();
	*/
	
	// ///////////////////////
	
	TCanvas *c3 = new TCanvas("c3", "Modulated Profile", 0, 0, 800, 800);
    
    c3 -> cd();
    
    // gStyle -> SetPalette(0, 0);
    time_vs_modulated -> Draw("colz");
    
	// ////////////////////////
	
	TCanvas *c4 = new TCanvas("c4", "Modulated Profile", 800, 0, 800, 800);
    
    c4 -> cd();
    
    // gStyle -> SetPalette(0, 0);
    event_vs_modulated -> Draw("colz");
    
    // ////////////////
    
    TCanvas *c5 = new TCanvas("c5", "Modulated Profile", 0, 800, 800, 800);
    
    c5 -> cd();
    
    // gStyle -> SetPalette(0, 0);
    event_vs_difference -> Draw("colz");
    
    // ////////////////
    
    TCanvas *c6 = new TCanvas("c6", "Modulated Profile", 800, 800, 800, 800);
    
    c6 -> cd();
    
    // gStyle -> SetPalette(0, 0);
    event_vs_time -> Draw("colz");
    
    time_vector.clear();
    
    delete tr;
	delete revent;	

	
	return 0;

}
