//
//  FFT_mu2e.cpp
//  
//
//  Created by Raul Garrido Garcia on 6/30/21.
//
//from mu2e
#include "FFT_mu2e.hpp"
#include <iostream>
R__LOAD_LIBRARY(../libData.so)
#include <TTree.h>
#include <TFile.h>
#include <TGraph.h>
#include <TFile.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TProfile2D.h>
#include <TStyle.h>
//new
#include "TH1D.h"
#include "TVirtualFFT.h"
#include "TF1.h"
#include "TMath.h"

//HISTOGRAMS
void FFT_mu2e(char const *rootfile, int nevn)
{
    using std::count;
    using std::endl;
      
    gStyle ->SetCanvasDefH(900);
    gStyle ->SetCanvasDefW(1500);

    gSystem->Load("$PROJECT/libData.so");
    gSystem->Load("$PROJECT/dataClasses/DataCint_rdict.pcm");
      
    TFile *fr = new TFile(rootfile);
    TTree *tr = (TTree*)fr->Get("rtree");

    RawEvent *revent = new RawEvent();


  //Int_t ch_vpeakhigh;
 // Int_t ch_vpeakhigh_time_us;

  Int_t ch_vpeaksum;
  Int_t ch_vpeaksum_time1_ms;

  
  Int_t npeak_ch3;
  Int_t nsum_ch3;
    
  int nevents = tr->GetEntries();
    
  
    TCanvas *c1 = new TCanvas("c1", "Fast Fourier Transform peaksum time ms", 800, 600);
    c1->SetFillColor(45);
    c1->Divide(2,1);

    c1->cd(1);
    TH1::AddDirectory(kFALSE);
    
  
    TH1D *original = new TH1D("original", "Original", 1200, -50, 550);
    
  tr->SetBranchAddress("ch3.",&revent);
  printf("CH3 CH3 CH3 \n");
  for(int i= 0; i< nevn ;i++)
   {
        tr->GetEntry(i);
       /* npeak_ch3 = revent->GetVPeakHighSize();
        printf("event %d \t", i);
        printf("npeak_ch3 = %d\t",npeak_ch3);
    

        for(int j =0; j< npeak_ch3 ;j++)
        {
           ch_vpeakhigh=revent->GetVPeakHigh()[j];
           ch_vpeakhigh_time_us=revent->GetVPeakHighTime()[j]*0.001;
            
            printf("vpeakhigh = %d\t",ch_vpeakhigh);
            printf("vpeakhigh_time_us = %d\n",ch_vpeakhigh_time_us);
            
            
       original->Fill(ch_vpeakhigh_time_us);
  

       }*/
       
       nsum_ch3 = revent->GetVPeakSumSize();
       printf("event %d \t", i);
       printf("nsum_ch3 = %d\n",nsum_ch3);
       
       for(int j =0; j< nsum_ch3 ;j++)
       {
          ch_vpeaksum=revent->GetVPeakSum()[j];
          ch_vpeaksum_time1_ms=revent->GetVPeakSumTime1()[j]*0.000001;
           
           printf("vpeaksum = %d\t",ch_vpeaksum);
           printf("vpeaksum_time1_ms = %d\n",ch_vpeaksum_time1_ms);
           
           
      original->Fill(ch_vpeaksum_time1_ms);

       
       }
    
       original->Draw();
       original->SetFillColor(30);
     
       
       
       c1->cd(2);

       TH1 *first_trans =0;
       TVirtualFFT::SetTransform(0);
       first_trans = original->FFT(first_trans, "MAG");
       first_trans->SetTitle("Magnitude of the 1st transform");
       first_trans->Draw();
       first_trans->SetFillColor(30);
     
   }
  
  return 0;

}
