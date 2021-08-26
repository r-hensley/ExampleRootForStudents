// corresponde_first_events.cxx
// This file looks at the first hits in each event and plots on a histogram only events for which the first hit was within a certain central range of times for the first hit

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
#include "TH2.h"
#include <vector>
#include <TProfile2D.h> 

//HISTOGRAMS
void signal_FFT(char const *rootfile = "../data/1000evn_v3.root", int nevn = 100)
{
	using std::count;
	using std::endl;

	gStyle ->SetCanvasDefH(900);
	gStyle ->SetCanvasDefW(1500);

	gSystem->Load("../libData.so");
	gSystem->Load("../dataClasses/DataCint_rdict.pcm");

	TFile *fr = new TFile(rootfile);
	TTree *tr = (TTree*)fr->Get("rtree");

	RawEvent *revent = new RawEvent();

	Int_t ch_vpeaksum;
	Double_t event_time;
	Double_t first_event_time;
	Double_t last_event_time;
	Double_t difference;
	vector<Double_t> time_vector;
	
	Int_t nsum_ch3;
	
	// first peak: 68.8 - 69.5
	// second peak: 79.9 - 80.6
	// third peak: 90.8 - 91.6
	// fourth peak: 102.3 - 102.8
	Double_t lower_threshhold = 0.;  // currently not used!
	Double_t upper_threshhold = 1000.;  // currently not used!

	double freq_res = 0.0001;  // Width of bins in MHz on final FFT 
	double nyquist_freq = 0.12;
	double sample_rate = 1/(nyquist_freq * 2);

	double t_min = 0;
	double t_max = 1/freq_res;
	int nt = t_max / sample_rate;
    
	int nevents = tr->GetEntries();
	
	printf("Sample rate: %f\tt_max: %f\tnt: %d\n", sample_rate, t_max, nt);
	
	TH1F *original = new TH1F("original", "Time Events Histogram;Time;Magnitude", nt, t_min, t_max);

	TH1F *hit_difference = new TH1F("hit_difference", 
									"Relative to first hit;Time Difference;Magnitude", 
									nt, t_min, t_max);
					
    hit_difference -> GetXaxis() -> SetRangeUser(0, 5000);
    
    TH2F *modulated_th2f = new TH2F("modulated_th2f", "First 1000us of events (differences);Event time relative to first event (us);Event number", 
    500, 0, 1000,
    // 100, -11.134488/2, 11.134488/2, 
    100, 0, 100);

	tr->SetBranchAddress("ch3.", &revent);
	
	// loop over events
	for(int i= 0; i< nevn ;i++) {  
		tr->GetEntry(i);

		nsum_ch3 = revent->GetVPeakSumSize();
		first_event_time = 0.;
		last_event_time = 0.;


		// loop over hits in event
		for(int j =0; j< nsum_ch3 ;j++) {
		   
			event_time = revent->GetVPeakSumTime1()[j] * 0.001;
			// printf("%f\n", event_time);
			
			if (first_event_time == 0) {
				first_event_time = event_time;
				// printf("%f\t%f\t%f\t%f\n", first_event_time, event_time, 0., 0.);

				if (first_event_time < lower_threshhold || upper_threshhold < first_event_time) {
					// ignore all events with the first hit not being in this range
					// break;
				}
				else { original->Fill(event_time); }
			}
			
			else {
				original->Fill(event_time);
				difference = event_time - first_event_time;
				
				if (event_time - last_event_time > 1000) {
					first_event_time = event_time; 
					difference = event_time - first_event_time;
				}
				last_event_time = event_time; 
								
				hit_difference -> Fill(difference);
				time_vector.push_back(difference);
				
				Double_t mod_result = fmod(difference, 11.134488);
				
				if (mod_result > 11.134488 / 2) { mod_result = mod_result - 11.134488; }

				// modulated_th2f -> Fill( mod_result, (difference + first_event_time) / 1000 );
				modulated_th2f -> Fill( difference , i );
				
				// printf("%f\t%f\t%f\t%f\n", first_event_time, event_time, difference, mod_result);
				

				}
			}

		}
		
		
	
	// ~~~~~~~~~~~~~ Do FFT ~~~~~~~~~~~~~~~~~~ //
	
	
	TH1 *unscaled_FFT = 0;  // Temporary object

	TVirtualFFT::SetTransform(0);  // Not sure what this does
	unscaled_FFT = hit_difference -> FFT(unscaled_FFT, "MAG");  // Do FFT
	TH1 * final_FFT = (TH1*)unscaled_FFT->Clone();  // Clone the FFT to scale it
	final_FFT -> SetBins(nt, 0, nt / (sample_rate * nt));  // Scale FFT
    
	final_FFT -> GetXaxis() -> SetRange(50, nt / 2);  // Last  half of FFT is negative frequencies
	
	// ~~~~~~~~~~~~ Calculate period ~~~~~~~~~~~~~ //

	int max_bin = final_FFT -> GetMaximumBin();  // get max bin number
	
	Double_t max_bin_content = final_FFT -> GetBinContent(max_bin);
	Double_t before_bin_content = final_FFT -> GetBinContent(max_bin - 1);
	Double_t after_bin_content = final_FFT -> GetBinContent(max_bin + 1);

	// Interpolation algorithm http://www.add.ece.ufl.edu/4511/references/ImprovingFFTResoltuion.pdf
	Double_t bin_shift = (log(after_bin_content/before_bin_content))/(2*log((max_bin_content * max_bin_content)/(before_bin_content * after_bin_content)));
	
	Double_t new_bin = max_bin + bin_shift;
	Double_t freq = (new_bin - 1) * freq_res;

	Double_t max_bin_low_edge = final_FFT->GetXaxis()->GetBinLowEdge(max_bin);

	Double_t period = 1/freq;

	printf("Max bin: %d\n", max_bin);
	printf("Max bin location (freq. in MHz): %f\n", max_bin_low_edge);
	printf("Predicted frequency (MHz): %f\n", freq);
	printf("Predicted period (us): %f\n", period);
	
	period = 11.134488;
	
	// ~~~~~~ Plot first canvas: hit difference and FFT ~~~~~~~ //
	
	TCanvas *c1 = new TCanvas("c1", "Hit Difference", 200, 10, 1700, 900);

	c1->Divide(2,1);

    c1->cd(1);
    hit_difference->Draw();
    hit_difference->SetFillColor(30);
    
    c1->cd(2);
    final_FFT->Draw();
    final_FFT->SetFillColor(30);
    
    // ~~~~~~~~~~~~~ Create modulated histograms ~~~~~~~~~~~~~~~~~ //
    

	
	int time_per_bin_ns = 5; // ns per bin
	Double_t time_per_bin_us = time_per_bin_ns / 1000.; 
	
	t_min = -period / 2;  
	t_max = period / 2;
	nt = (t_max - t_min) / time_per_bin_us;
	
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
	
	// ~~~~~~~~~~~~~~~~~ Plot modulated histograms ~~~~~~~~~~~~~~~~~ //
	
	TCanvas *c2 = new TCanvas("c2", "Modulo histogram", 0, 0, 1700, 900);
    
    c2->Divide(3, 1); 
	
	c2->cd(1);
	zoom_hist->Draw();
	
	c2->cd(2);
	TH1F * zoom_hist2 = (TH1F*)zoom_hist->Clone();
	zoom_hist2 -> GetXaxis() -> SetRangeUser(4, 4.5);
	std::string period_text = "Period: " + std::to_string(period) + " us";	
	zoom_hist2 -> SetTitle(period_text.c_str());
	zoom_hist2 -> Draw();
	
	c2->cd(3);
	TH1F * zoom_hist3 = (TH1F*)zoom_hist -> Clone();
	zoom_hist3 -> GetXaxis() -> SetRangeUser(-0.5, 0.5);
	std::string first_hit_threshhold = "First hit threshhold: " + std::to_string(lower_threshhold).substr(0, std::to_string(lower_threshhold).find(".") + 2) + " - " + std::to_string(upper_threshhold).substr(0, std::to_string(upper_threshhold).find(".") + 2) + " us";
	zoom_hist3 -> SetTitle(first_hit_threshhold.c_str());
	zoom_hist3 -> SetTitle("No hit threshhold");
	zoom_hist3 -> Draw();
    
    zoom_hist3 -> Draw();
    
    // ~~~~~~~~~~~~~ Plot TProfile2D ~~~~~~~~~~~~~ //
    
    TCanvas *c3 = new TCanvas("c3", "Modulated Profile", 0, 0, 800, 800);
    
    c3 -> cd();
    
    gStyle -> SetPalette(0, 0);
    modulated_th2f -> Draw("col");
       
    return 0;
}
