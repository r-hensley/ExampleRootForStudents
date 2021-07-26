R__LOAD_LIBRARY(../libData.so)
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include <TGraph.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TStyle.h>
#include "TH1.h"
#include "TF1.h"
#include <vector>

//HISTOGRAMS
void signal_FFT(char const *rootfile = "../data/1000evn_v3.root", int nevn = 100)
{
	using std::count;
	using std::endl;

	gStyle ->SetCanvasDefH(900);
	gStyle ->SetCanvasDefW(1500);

	// gSystem->Load("../libData.so");
	// gSystem->Load("../dataClasses/DataCint_rdict.pcm");

	TFile *fr = new TFile(rootfile);
	TTree *tr = (TTree*)fr->Get("rtree");

	RawEvent *revent = new RawEvent();

	Int_t ch_vpeaksum;
	Double_t event_time;
	Double_t first_event_time;
	Double_t difference;
	vector<Double_t> time_vector;
	
	Int_t nsum_ch3;

    double t_min = -50.;
    double t_max = 5000.;
    double nt = (t_max - t_min);
    
	int nevents = tr->GetEntries();
	
    TH1F *original = new TH1F("original", 
    						  "Time Events Histogram;Time;Magnitude", 
    						  nt, t_min, t_max);
    						 
    double t_min_2 = 0;
    double t_max_2 = 10000;
    double nt_2 = (t_max_2 - t_min_2);
    double sample_rate = ( t_max_2 - t_min_2 ) / nt_2;
    TH1F *hit_difference = new TH1F("hit_difference", 
    								"Relative to first hit;Time Difference;Magnitude", 
    								nt_2, t_min_2, t_max_2);
    								
    								
    
	tr->SetBranchAddress("ch3.", &revent);
	
	// loop over events
	for(int i= 0; i< nevn ;i++) {  
		tr->GetEntry(i);

		nsum_ch3 = revent->GetVPeakSumSize();
		first_event_time = 0.;

		// loop over hits in event
		for(int j =0; j< nsum_ch3 ;j++) {
		   
			event_time = revent->GetVPeakSumTime1()[j] * 0.001;
			original->Fill(event_time);
			
			if (first_event_time == 0) {
				first_event_time = event_time;
				// printf("First event time: %f\n", first_event_time);
			}
			else {
				difference = event_time - first_event_time;
				if (difference < 1000) {
					hit_difference -> Fill(difference);
					time_vector.push_back(difference);
				}
			}

		}
   
	}    
	
	TH1 *unscaled_FFT = 0;  // Temporary object

	TVirtualFFT::SetTransform(0);  // Not sure what this does
	unscaled_FFT = original -> FFT(unscaled_FFT, "MAG");  // Do FFT
	TH1 * final_FFT = (TH1*)unscaled_FFT->Clone();  // Clone the FFT to scale it
	final_FFT -> SetBins(nt, 0, nt / (sample_rate * nt));  // Scale FFT
    
	TCanvas *c1 = new TCanvas("c1", "Hit Difference", 200, 10, 1700, 900);
       
    c1->Divide(2,1);

    c1->cd(1);
    original->Draw();
    original->SetFillColor(30);
    
    c1->cd(2);
    hit_difference->Draw();
    hit_difference->SetFillColor(30);
    
	TCanvas *c2 = new TCanvas("c2", "Modulo histogram", 200, 10, 1700, 900);
	
	c2 -> cd();
	final_FFT -> GetXaxis() -> SetRange(50, nt_2 / 2);
	
	int max_bin = final_FFT->GetMaximumBin();
	printf("Max bin: %d\n", max_bin);
	Double_t max_bin_low_edge = final_FFT->GetXaxis()->GetBinLowEdge(max_bin);
	printf("Max bin location (freq. in MHz): %f\n", max_bin_low_edge);
	Double_t period = 1/max_bin_low_edge;
	printf("Predicted period (us): %f\n", period);
	
	TH1F *zoom_hist = new TH1F(
	"zoom_hist", 
	"Period-Modulated Histogram;Time (us);Acc. Trig. Events", 
	1000, 0, period);
	
	double split = 0.;  // split is only if the modulo splits the histogram in half
	for (int i = 0; i < time_vector.size(); i++) { 
		Double_t mod_result = fmod(time_vector[i], period);  // modulo
		if (split == 0.) {
			//printf("final result: %f\n", mod_result);
			zoom_hist->Fill(mod_result);
		}
		
		// if split = 0, ignore below
		else if (mod_result <= split) {
			printf("final result: %f\n", (mod_result + split));
			zoom_hist->Fill( (mod_result + split));
		}
		else if (split < mod_result) {
			printf("final result: %f\n", (mod_result - (period - split))); 
			zoom_hist->Fill( (mod_result - (period - split)));
		}
	}
    
    zoom_hist -> Draw();
    
    return 0;
}
