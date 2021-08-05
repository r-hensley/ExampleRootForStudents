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


void f(char const *rootfile = "../data/1000evn_v3.root", int nevn = 15)
{
	gSystem->Load("../libData.so");  // from TSystem.h, loads things like RawEvent
	gSystem->Load("../dataClasses/DataCint_rdict.pcm");  // for root6 

	TFile *fr = new TFile(rootfile);

	TTree *tr = (TTree*)fr->Get("rtree");  // there is rtree;11 and rtree;12, this Get() defaults to highest number

	RawEvent *revent = new RawEvent();
	
	Double_t nsum;
	Double_t event_time; 
	Double_t event_time_us; 
	vector<Double_t> time_vector;
	
	tr->SetBranchAddress("ch3.",&revent);  
	
	// tr->GetEntry(0);
	
	// nsum = revent -> GetVPeakSumSize();
	// printf("GetVPeakSumSize(): %f\n", nsum); 
	
	// TBranch * br = tr->GetBranch("ch3.fvPeakSumTime1");
	
	// printf("Manual attainment of vector\n");
	
	// const Double_t * event_time_2 = revent -> GetVPeakSumTime1();
	
	/*
	printf("GetVPeakSumTime1: %f\n", *event_time_2);
	printf("GetVPeakSumTime1: %f\n", revent -> GetVPeakSumTime1()[1]);
	printf("GetVPeakSumTime1: %f\n", revent -> GetVPeakSumTime1()[2]);
	printf("GetVPeakSumTime1: %f\n", revent -> GetVPeakSumTime1()[3]);
	printf("GetVPeakSumTime1: %f\n", revent -> GetVPeakSumTime1()[4]);
	*/
	
	// Double_t size = nsum_vector.size();
	// printf("Size of GetVPeakSum(): %f\n", size);


	// ////////////// Pull data out of .ROOT file ///////////////
	

	int count = 0;
	
	for(int i= 0; i< nevn; i++) {
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

		for(int j =0; j < 3; j++)
		{	
			count++;
			// event_time = revent -> GetVPeakSumTime1();
			
			
			// printf("GetVAmp(): %f\n", revent -> GetVAmp()[j]);
			// printf("GetVTime(): %f\n", revent -> GetVTime()[j]);
			// printf("GetVPeakSum_tail(): %f\n", revent -> GetVPeakSum_tail()[j]);
			
			printf("GetVPeakSum0_peak(): %f\n", revent -> GetVPeakSum0_peak()[j]);
			printf("GetVPeakHigh(): %f\n", revent -> GetVPeakHigh()[j]);
			printf("GetVPeakSum0(): %f\n", revent -> GetVPeakSum0()[j]);
			printf("GetVPeakSum(): %f\n", revent -> GetVPeakSum()[j]);
			
			printf("GetVPeakSumTime1(): %f\n", revent -> GetVPeakSumTime1()[j]);
			printf("GetVPeakHighTime(): %f\n", revent -> GetVPeakHighTime()[j]);
			printf("GetVPeakSumTime0(): %f\n", revent -> GetVPeakSumTime0()[j]);
			printf("GetVPeakSumTime2(): %f\n\n", revent -> GetVPeakSumTime2()[j]);
			
			// event_time = revent -> GetVPeakSumTime1()[j]; // in ns
			// printf("event_time 1: %f\n", event_time);
			// event_time_us = event_time * 0.001; // convert to us
			// time_vector.push_back(event_time_us);  // for later time histogram
		}
		
	}
	
	printf("Final count value: %d\n", count);

	// printf("time_vector.size(): %zu\n", time_vector.size());
	
	return 0;

}
