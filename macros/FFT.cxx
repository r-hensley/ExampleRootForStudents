#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void histo() {

	int n_bins = 1283;  // x-axis will be split into this many bins. if too low, FFT won't work well
  	double x_min = -2.;  // this can be anything
	double x_max = 9.;  // this can be anything
	double freq = 23.;  // this is a secret number that we want to get through FFT only
  
  	TH1F * hist = new TH1F("hist","hist", n_bins, x_min, x_max);  // will create FFT from this
  	TF1  * func = new TF1("func", "cos(2.0 * TMath::Pi() * [0] * x)", x_min, x_max);  // to create data
	func->SetParameter(0, freq);  // sets the [0] in the above line to freq
  
  	for (int bin = 1; bin <= n_bins; ++bin){
    		double bin_center = hist -> GetBinCenter(bin);
    		double func_value = func -> Eval (bin_center);
    		hist -> SetBinContent(bin, func_value);  // evaluate func at the center of each bin
  	}
  
  	TH1 * hist_transform;  // temporary unscaled object to contain the FFT
  	hist_transform = hist -> FFT(hist_transform, "MAG");
	
	TCanvas *c = new TCanvas("c","c",2000,1000);
	c->Divide(3,1);

	c->cd(1);
	hist->Draw();  // original function in the time domain

	c->cd(2);
	hist_transform->Draw();  // unscaled FFT result
	
	c->cd(3);
	TH1 * scaled_hist = (TH1*)hist_transform->Clone();  // create a copy of the above to scale
	double sample_rate = (x_max - x_min)/n_bins;  // only important for the next line
	double scale_factor = 1/(sample_rate * n_bins);  // how much we'll rescale the x-axis by
	scaled_hist->SetBins(n_bins, 0, n_bins * scale_factor);  // relabels the x-axis
	scaled_hist->Draw();

	printf("n_bins = %d\n", n_bins);
	printf("x_range = %f -> %f\n", x_min, x_max);
	printf("freq = %f\n", freq);
	printf("sample rate = %f\n", sample_rate);
	printf("max bin = %d\n", scaled_hist->GetMaximumBin());
	printf("Predicted freq = %f\n", scaled_hist->GetXaxis()->GetBinLowEdge(max_bin));
	
	double bin_hz = freq/max_bin;
	printf("Hz per bin = %f\n", bin_hz);
	
	double max_hz = (n_bins/2) * bin_hz;
	printf("Max Hz = %f\n", max_hz);
	
	printf("Scale factor = %f\n", scale_factor);

}

