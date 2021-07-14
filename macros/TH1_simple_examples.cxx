#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TRandom.h>
#include <TProfile2D.h>


void getrandomh() {

	TRandom *r = new TRandom();

	TH1F *hist1 = new TH1F("hist1", "1D", 100, -3, 3);
	TH2F *hist2 = new TH2F("hist2", "2D", 100, -3, 3, 100, -3, 3);
	TH3F *hist3 = new TH3F("hist3", "3D", 100, -3, 3, 100, -3, 3, 100, -3, 3);
	TProfile2D *profile = new TProfile2D("profile","2D Profile", 10, -5, 5, 10, -5, 5, -3, 3);

	Double_t data[500000];

	for(int i; i<500000; i++) {
		Double_t x1 = r->Gaus();
		Double_t x2 = r->Gaus();
		Double_t x3 = r->Gaus();
		hist1->Fill(x1);
		hist2->Fill(x1,x2);
		hist3->Fill(x1,x2,x3);
		profile->Fill(x1, x2, x3);
	}


	TCanvas *c1 = new TCanvas("c1","c1", 1800, 1000);
	c1->Divide(3,1);
	c1->cd(1);
	hist1->Draw();
	c1->cd(2);
	hist2->Draw("col");
	c1->cd(3);
	hist3->Draw();

	TCanvas *c2 = new TCanvas("c2", "c2", 800, 1000);
	c2->cd();
	profile->Draw("colz");


	return 0;

}


