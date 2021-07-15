#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

void histo() {

	int n_bins = 10000000;  // x-axis will be split into this many bins. if too low, FFT won't work well
  	double x_min = 0.;  // this can be anything
	double x_max = 100.;  // this can be anything
	double freq = 23.123456;  // this is a secret number that we want to get through FFT only 
	double sample_rate = (x_max - x_min)/n_bins;  // only important for the next line

  	TH1F * hist = new TH1F("hist","hist", n_bins, x_min, x_max);  // will create FFT from this
  	TF1  * func = new TF1("func", "cos(2.0 * TMath::Pi() * [0] * x)", x_min, x_max);  // to create data
	func->SetParameter(0, freq);  // sets the [0] in the above line to freq
  
	int array_n = 2 * ((n_bins+1)/2 + 1);
	Double_t *in = new Double_t[array_n];
	Double_t re_2, im_2;

	for (int i=0; i <= n_bins; i++) {
		in[i] = func -> Eval(i * sample_rate);
	}

	Int_t n_size = n_bins + 1;
	TVirtualFFT *fft_own = TVirtualFFT::FFT(1, &n_size, "R2C ES K");
	fft_own -> SetPoints(in);
	fft_own -> Transform();
	fft_own -> GetPoints(in);


  	for (int bin = 1; bin <= n_bins; ++bin){
    		Double_t bin_low_edge = hist -> GetBinLowEdge(bin);
    		Double_t func_value = func -> Eval (bin_low_edge);
    		hist -> SetBinContent(bin, func_value);  // evaluate func at the center of each bin
  	}


	Double_t edge_array_sample = 1/(30 * 100000);  // 2e-7
	Double_t edge_array[100001];
	for (int i=0; i <= 100001; i++) {
		edge_array[i] = i*edge_array_sample;
	}
  
  	TH1 * hist_transform = 0;  // temporary unscaled object to contain the FFT
  	// hist_transform = hist -> FFT(hist_transform, "MAG");
	hist_transform = TH1::TransformHisto(fft_own, hist_transform, "MAG");
	
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

	printf("n_bins = %d\n", n_bins);
	printf("x_range = %f -> %f\n", x_min, x_max);
	printf("freq = %f\n", freq);
	printf("sample rate = %f\n", sample_rate);
	double max_bin = scaled_hist->GetMaximumBin(); 
	printf("max bin = %f\n", max_bin);
	printf("Predicted freq = %f\n", scaled_hist->GetXaxis()->GetBinLowEdge(max_bin));
	
	double bin_hz = freq/max_bin;
	printf("Hz per bin = %f\n", bin_hz);
	
	double max_hz = (n_bins/2) * bin_hz;
	printf("Max Hz = %f\n", max_hz);
	
	printf("Scale factor = %f\n", scale_factor);

}

