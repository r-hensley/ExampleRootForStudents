#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void histo() {

	delete gROOT->FindObject("func");
	delete gROOT->FindObject("histo1");
	delete gROOT->FindObject("histo2");
	delete gROOT->FindObject("out_MAG");
	delete gROOT->FindObject("c");  // only here because I kept restarting the code

	// play with these three parameters and see how it affects the final two graphs
	int freq = 4; 
	double x_max = 20;
	double n_bins = 100;
	//
	double sample_rate = x_max / n_bins;

	TF1 *func = new TF1("func","cos(2.0 * TMath::Pi() * [0] * x)", 0, x_max);
	func->SetParameter(0, freq); // sets the [0] in the above line to freq

	TH1F *histo1 = new TH1F("histo1","histo1", n_bins, 0, x_max); // initial plot of func
	TH1 *hm = 0;  // temporary object to hold the FFT before I rescale it
	double rescale_factor = 7./40.;
	TH1F *histo2 = new TH1F("histo2","histo2", n_bins, 0, n_bins * rescale_factor); // eventual final scaled FFT plot

	for(double i; i < n_bins; i++) {
		histo1->SetBinContent(i, func->Eval(i * sample_rate));
	}

	TCanvas *c = new TCanvas("c","c",1800,1000);
	c->Divide(2,1);

	c->cd(1);
	TVirtualFFT::SetTransform(0);
	hm = histo1->FFT(hm, "MAG");
	hm->Draw();


	for (double j=0; j < n_bins; j++) {
		histo2->SetBinContent(j, hm->GetBinContent(j));
	}

	c->cd(2);
	histo2->Draw();
}

