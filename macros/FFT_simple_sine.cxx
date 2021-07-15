#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void histo() {
	double freq_res = 0.0001;
	double nyquist_freq = 50;
	double sample_rate = 1/(nyquist_freq * 2.);

  	double x_min = 0.;
	double x_max = 1/freq_res;  
	//double x_max = 10;
	//int n_bins = 100;
	int n_bins = x_max/sample_rate;  // x-axis will be split into this many bins. if too low, FFT won't work well

	printf("%f\t%d\n", x_max, n_bins);
	
	double freq = 23.123456;  // this is a secret number that we want to get through FFT only

  	TH1F * hist = new TH1F("hist","hist", n_bins, x_min, x_max);  // will create FFT from this
  	TF1  * func = new TF1("func", "cos(2.0 * TMath::Pi() * [0] * x)", x_min, x_max);  // to create data
	func->SetParameter(0, freq);  // sets the [0] in the above line to freq
  
  	for (int bin = 1; bin <= n_bins; ++bin){
    		double bin_center = hist -> GetBinLowEdge(bin);
    		double func_value = func -> Eval (bin_center);
    		hist -> SetBinContent(bin, func_value);  // evaluate func at the center of each bin
  	}

	
  	TH1 * hist_transform = 0;  // temporary unscaled object to contain the FFT
  	hist_transform = hist -> FFT(hist_transform, "MAG");
	
	TCanvas *c = new TCanvas("c","c",2000,1000);
	c->Divide(3,1);

	c->cd(1);
	hist->Draw();  // original function in the time domain

	c->cd(2);
	hist_transform->Draw();  // unscaled FFT result
	
	c->cd(3);
	TH1 * scaled_hist = (TH1*)hist_transform->Clone();  // create a copy of the above to scale
	double scale_factor = 1/(sample_rate * n_bins);  // how much we'll rescale the x-axis by
	scaled_hist->SetBins(n_bins, 0, n_bins * scale_factor);  // relabels the x-axis
	scaled_hist->Draw();

	printf("%d\t", n_bins);
	printf("%f\t", x_min);
	printf("%f\t", x_max);
	printf("%f\t", freq);
	printf("%f\t", sample_rate);
	
	double max_bin = scaled_hist->GetMaximumBin(); 
	printf("%f\t", max_bin);
	double pred_freq = scaled_hist->GetXaxis()->GetBinLowEdge(max_bin);
	printf("%f\t", pred_freq);
	
	double bin_hz = pred_freq/(max_bin-1);
	printf("%f\t", bin_hz);
	
	double max_hz = (n_bins/2) * bin_hz;
	printf("%f\t", max_hz);
	
	printf("%f\n", scale_factor);
	
	return 0;
}

