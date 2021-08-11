#include <iostream>

R__LOAD_LIBRARY(../libData.so);

#include <TTree.h> 
#include <TBranch.h>
#include <TGraph.h> 
#include <TFile.h> 
#include <TSystem.h> 
#include "TCanvas.h" 
#include "TProfile2D.h" 
#include "TH1.h"
#include "TF1.h"

#include <TStyle.h> 
#include <vector>


void f(char const *rootfile = "../data/1000evn_v3.root", int nevn = 1000)
{
	gSystem->Load("../libData.so");  // from TSystem.h, loads things like RawEvent
	gSystem->Load("../dataClasses/DataCint_rdict.pcm");  // for root6 

	TFile *fr = new TFile(rootfile);

	TTree *tr = (TTree*)fr->Get("rtree");  // there is rtree;11 and rtree;12, this Get() defaults to highest number

	RawEvent *revent = new RawEvent();
	
	Double_t nsum;
	Double_t time0; Double_t time1; Double_t time2;
	Double_t area;
	Double_t area0;
	Double_t height;
	// vector<Double_t> time_vector;
	
	tr->SetBranchAddress("ch3.",&revent);  
	
	int t_min = 0;
	int t_max = 200;
	int nt = (t_max - t_min) / 4;
	
	TH1F * hit_length = new TH1F("hit_length", "Full length of hit;Time (ns);Amplitude", nt, t_min, t_max); 
	TH1F * first_hits = new TH1F("first_hits", "Times of first hits;Time (ms);Amplitude", 8000, 60, 140); 
	TH1F * peak_location = new TH1F("peak_location", "Time to max peak;Time (us);Amplitude", 13, 0, 52); 
	TH1F * peak_area = new TH1F("peak_area", "Peak area;Area;Amplitude", 3000, 0, 150000);
	TH1F * peak_area0 = new TH1F("GetVPeakSum0()", "Peak area;Area;Amplitude", 250, 0, 2000);
	TH1F * peak_high = new TH1F("GetVPeakHigh()", "Magnitude peak voltage;Voltage;Amplitude", 500, 0, 2100); 


	// ////////////// Pull data out of .ROOT file ///////////////
	
	
	for(int i= 0; i < nevn; i++) {
		tr->GetEntry(i);
		nsum = revent -> GetVPeakSumSize();  // number of peaks in this event
		
		/*
		printf("GetVAmpSize(): %f\n", revent -> GetVAmpSize());
		printf("GetVPeakHighSize(): %f\n", revent -> GetVPeakHighSize());
		printf("GetVPeakSum0Size(): %f\n", revent -> GetVPeakSum0Size());
		printf("GetVPeakSumSize(): %f\n", revent -> GetVPeakSumSize());
		printf("GetVPeakSum_tailSize(): %f\n", revent -> GetVPeakSum_tailSize());
		printf("GetVPeakSum0_peakSize(): %f\n\n", revent -> GetVPeakSum0_peakSize());
		*/

		for(int j =0; j < 1; j++)
		{	
			
			//time0 = revent -> GetVPeakSumTime0()[j];
			time1 = revent -> GetVPeakSumTime1()[j] * 0.001;
			//time2 = revent -> GetVPeakSumTime2()[j];
			
			// printf("%f\n", time1);
			
			// area = revent -> GetVPeakSum()[j];
			// area0 = revent -> GetVPeakSum0()[j];
			
			// height = revent -> GetVPeakHigh()[j];
			
			
			first_hits -> Fill(time1);
			//hit_length -> Fill(time2 - time1);
			//peak_location -> Fill(time0 - time1);
			// peak_area -> Fill(area * -1);
			// peak_area0 -> Fill(area0 * -1);
			
			// printf("GetVAmp(): %f\n", revent -> GetVAmp()[j]);
			// printf("GetVTime(): %f\n", revent -> GetVTime()[j]);
			// printf("GetVPeakSum_tail(): %f\n", revent -> GetVPeakSum_tail()[j]);
			
			/*
			printf("GetVPeakSum0_peak(): %f\n", revent -> GetVPeakSum0_peak()[j]);
			printf("GetVPeakHigh(): %f\n", revent -> GetVPeakHigh()[j]);
			printf("GetVPeakSum0(): %f\n", revent -> GetVPeakSum0()[j]);
			printf("GetVPeakSum(): %f\n", revent -> GetVPeakSum()[j]);
			
			printf("GetVPeakSumTime1(): %f\n", revent -> GetVPeakSumTime1()[j]);
			printf("GetVPeakHighTime(): %f\n", revent -> GetVPeakHighTime()[j]);
			printf("GetVPeakSumTime0(): %f\n", revent -> GetVPeakSumTime0()[j]);
			printf("GetVPeakSumTime2(): %f\n\n", revent -> GetVPeakSumTime2()[j]);
			*/
			
			// event_time = revent -> GetVPeakSumTime1()[j]; // in ns
			// printf("event_time 1: %f\n", event_time);
			// event_time_us = event_time * 0.001; // convert to us
			// time_vector.push_back(event_time_us);  // for later time histogram
		}
		
	}
	/*
	TCanvas *c1 = new TCanvas(
		"c1",  // name
		"Peak Lengths",  // title
		200,  // wtopx, pixel x-location of top-left corner of the canvas
		10,  // wtopy, pixel y-location of ...
		1800,  // width
		1000  // height
	);
	
	c1 -> Divide(2,1);
	
	c1 -> cd(1);
	hit_length -> Draw();
	
	c1 -> cd(2);
	peak_location -> Draw();
	*/
	
	/*
	TCanvas *c2 = new TCanvas(
		"c2",
		"Peak Area",
		200,
		10,
		800,
		800
	);
	
	c2 -> cd();
	peak_area -> Draw();
	*/
	
	TCanvas *c3 = new TCanvas(
		"c3",
		"Peak Area Sum0",
		200,
		10,
		800,
		800
	);
	
	c3 -> cd();
	first_hits -> Draw();
	
	return 0;

}
