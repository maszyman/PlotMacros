// makro do rysowania monitorow PID
// mps 9/08/2011

static  int      myDarkRed     = TColor::GetColor(128,0,0);
static  int      myDarkGreen   = TColor::GetColor(0,128,0);
static  int      myDarkBlue    = TColor::GetColor(0,0,128);

// infilename - root file with relevant histograms
// system - PP,APAP,PP
// status - Pass,Fail
// rWrite - 0-no,1-png,2-eps
// rPerformance - 0-no,1-yes (ALICE logo etc.)
// bin: 0 - all, 1- 0:5, 2- 5:10, etc
void drawPID(const char* infilename, const char* system, const char* status, Int_t rWrite, Int_t rPerformance, Int_t bin)
{


	TFile *f = new TFile(infilename, "read");

	// TPC dEdx
	TH2D* TPCdEdx =(TH2D*)f->Get(Form("TPCdEdxcut%s1%stpcM%i",status, system,0));

    if (!bin) {
            int minMultBin = 0;
            int maxMultBin = 6; // 8
    }
    else {
            int minMultBin = bin-1;
            int maxMultBin = bin; // 8
    }

	double EvMultall = 0;

	for (int i = minMultBin; i < maxMultBin; i++) {

		TH2D* TPCdEdxN =(TH2D*)f->Get(Form("TPCdEdxcut%s1%stpcM%i",status,system,i));
		TPCdEdx->Add(TPCdEdxN);

		cout << i << " " << TPCdEdxN->GetEntries() << endl;

		//delete hEvMult;
	}

	TCanvas *c2 = new TCanvas("TPC dEdx", "TPC dEdx");
	c2->SetGridx();
	c2->SetGridy();
	c2->SetFillColor(10);
	c2->SetRightMargin(1.9);
	c2->SetLogz();

	TPCdEdx->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
	TPCdEdx->GetXaxis()->SetRangeUser(0.0,6.0);
	TPCdEdx->GetYaxis()->SetTitle("dE/dx");
	TPCdEdx->GetZaxis()->SetLabelSize(0.03);
	TPCdEdx->Draw("colz");

// 	double a1 = -3000.0; double b1 =  1280.0;
// 	double a2 = -312.5;  double b2 =  312.5;
// 	double a3 = -200.0;  double b3 =  240.0;

//    TF1 *fa1 = new TF1("fa1","-1800*x+940",0.3,0.4);
//   fa1->Draw("same");
//   TF1 *fa2 = new TF1("fa2","-500.0*x+420.0",0.4,0.6);
//   fa2->Draw("same");
// TF1 *fa3 = new TF1("fa3","-216.7*x+250.0",0.6,0.9);
//    fa3->Draw("same");
// TF1 *fa4 = new TF1("fa4","-566.7*x+570.0",0.6,0.75);
//    fa4->Draw("same");
// TF1 *fa5 = new TF1("fa5","-2076.92*x+1476.15",0.47,0.6);
//    fa5->Draw("same");
//   cout<<TPCdEdx->GetNbinsX()<<endl;
//   cout<<TPCdEdx->GetNbinsY()<<endl;
//   for (int ii=0;ii<TPCdEdx->GetNbinsX();ii++){

//     for (int jj=0;jj<TPCdEdx->GetNbinsY();jj++){

//       cout<<"binX: "<<ii<<endl;
//       cout<<"binY: "<<jj<<endl;
//       cout<<"val: "<<TPCdEdx->GetBinContent(ii,jj)<<endl;
//     }
//   }

// 	TH1D *py = TPCdEdx->ProjectionY("py", 230, 232); // where firstYbin = 0 and lastYbin = 9
// 	TCanvas *c22 = new TCanvas("TPC2", "TPC2");
// 	py->Draw();

	postprocess(c2,Form("TPCdEdx%s",status),rWrite,rPerformance,system);

	// TPC Nsigma
	TH2D* TPCNsigma =(TH2D*)f->Get(Form("TPCNSigmacut%s1%stpcM%i",status,system,0));

	// int minMultBin = 0;
	// int maxMultBin = 2;
	double EvMultall = 0;

	for(int i = minMultBin; i<maxMultBin; i++) {
		//all
		TH2D* TPCNsigmaN =(TH2D*)f->Get(Form("TPCNSigmacut%s1%stpcM%i",status,system,i));
		TPCNsigma->Add(TPCNsigmaN);
		//delete hEvMult;
	}

	TCanvas *c3 = new TCanvas("TPC Nsigma", "TPC Nsigma");
	c3->SetGridx();
	c3->SetGridy();
	c3->SetFillColor(10);
	c3->SetRightMargin(1.7);
	c3->SetLogz();

	TPCNsigma->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
	TPCNsigma->GetXaxis()->SetRangeUser(0.0,5.0);
	TPCNsigma->GetYaxis()->SetTitle("number of sigmas");
	TPCNsigma->GetZaxis()->SetLabelSize(0.03);

	TPCNsigma->Draw("colz");

	postprocess(c3,Form("TPCNsigma%s",status),rWrite,rPerformance,system);

	// TOF Nsigma
	TH2D* TOFNsigma =(TH2D*)f->Get(Form("TOFNSigmacut%s1%stpcM%i",status,system,0));

	// int minMultBin = 1;
	// int maxMultBin = 1;
	double EvMultall = 0;

	for(int i = minMultBin; i<maxMultBin; i++) {
		//all
		TH2D* TOFNsigmaN =(TH2D*)f->Get(Form("TOFNSigmacut%s1%stpcM%i",status,system,i));
		TOFNsigma->Add(TOFNsigmaN);
		//delete hEvMult;
	}

	TCanvas *c4 = new TCanvas("TOF Nsigma", "TOF Nsigma");
	c4->SetGridx();
	c4->SetGridy();
	c4->SetFillColor(10);
	c4->SetRightMargin(1.7);
	c4->SetLogz();

	TOFNsigma->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
	TOFNsigma->GetXaxis()->SetRangeUser(0.0,5.0);
	TOFNsigma->GetYaxis()->SetTitle("number of sigmas from TOF");
	TOFNsigma->GetZaxis()->SetLabelSize(0.03);
	TOFNsigma->Draw("colz");

	postprocess(c4,Form("TOFNsigma%s",status),rWrite,rPerformance,system);

	// TOF time
	TH2D* TOFTime =(TH2D*)f->Get(Form("TOFTimecut%s1%stpcM%i",status,system,0));

	// int minMultBin = 1;
	// int maxMultBin = 1;
	double EvMultall = 0;

	for(int i = minMultBin; i<maxMultBin; i++) {
		//all
		TH2D* TOFTimeN =(TH2D*)f->Get(Form("TOFTimecut%s1%stpcM%i",status,system,i));
		TOFTime->Add(TOFTimeN);
		//delete hEvMult;
	}

	TCanvas *c5 = new TCanvas("TOF Time", "TOF Time");
	c5->SetGridx();
	c5->SetGridy();
	c5->SetFillColor(10);
	c5->SetRightMargin(1.7);
	c5->SetLogz();

	TOFTime->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
	TOFTime->GetYaxis()->SetTitle("GetTOFsignal - GetIntegratedTimes");
	TOFTime->GetYaxis()->SetTitleOffset(1.3);
	TOFTime->GetXaxis()->SetRangeUser(0,5);
	TOFTime->GetYaxis()->SetRangeUser(-7000,7000);
	//  TOFTime->GetZaxis()->SetLabelSize(0.03);
	TOFTime->Draw("colz");

	postprocess(c5,Form("TOFTime%s",status),rWrite,rPerformance,system);


	// TPC & TOF Nsigma
	TH2D* TPCTOFNsigma =(TH2D*)f->Get(Form("TPCTOFNSigmacut%s1%stpcM%i",status,system,0));

	int minMultBin = 1;
	int maxMultBin = 1;
	double EvMultall = 0;

	for(int i = minMultBin; i<maxMultBin; i++) {
		//all
		TH2D* TPCTOFNsigmaN =(TH2D*)f->Get(Form("TPCTOFNSigmacut%s1%stpcM%i",status,system,i));
		TOFNsigma->Add(TOFNsigmaN);
		//delete hEvMult;
	}

	TCanvas *c6 = new TCanvas("TOF Nsigma", "TOF Nsigma");
	c6->SetGridx();
	c6->SetGridy();
	c6->SetFillColor(10);
	c6->SetRightMargin(1.7);
	c6->SetLogz();

	TPCTOFNsigma->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
	TPCTOFNsigma->GetXaxis()->SetRangeUser(0.0,5.0);
	TPCTOFNsigma->GetYaxis()->SetTitle("#sqrt{#frac{n#sigma_{TPC} + n#sigma_{TOF}}{2}}");
	TPCTOFNsigma->GetZaxis()->SetLabelSize(0.03);
	TPCTOFNsigma->Draw("colz");

	postprocess(c6,Form("TPCTOFNsigma%s",status),rWrite,rPerformance,system);

}

void prepareAll() {

        myOptions(0);

        gROOT->ForceStyle();
        gStyle->SetPalette(1.0);

        TDatime now;
        int iDate = now.GetDate();
        int iYear=iDate/10000;
        int iMonth=(iDate%10000)/100;
        int iDay=iDate%100;
        char* cMonth[12]={"Jan","Feb","Mar","Apr","May","Jun",
                          "Jul","Aug","Sep","Oct","Nov","Dec"};
        char cStamp1[25],cStamp2[25];
        sprintf(cStamp1,"%i %s %i",iDay, cMonth[iMonth-1], iYear);
        sprintf(cStamp2,"%i/%.2d/%i",iDay, iMonth, iYear);
}

void myLegendSetUp(TLegend *currentLegend=0,float currentTextSize=0.07){
	currentLegend->SetTextFont(42);
	currentLegend->SetBorderSize(0);
	currentLegend->SetFillStyle(0);
	currentLegend->SetFillColor(0);
	currentLegend->SetMargin(0.25);
	currentLegend->SetTextSize(currentTextSize);
	currentLegend->SetEntrySeparation(0.5);
	return;
}

void myPadSetUp(TPad *currentPad, float currentLeft=0.11, float currentTop=0.04, float currentRight=0.04, float currentBottom=0.15){
	currentPad->SetLeftMargin(currentLeft);
	currentPad->SetTopMargin(currentTop);
	currentPad->SetRightMargin(currentRight);
	currentPad->SetBottomMargin(currentBottom);
	return;
}

void myGraphSetUp(TGraphErrors *currentGraph=0, Float_t currentMarkerSize = 1.0,
		  int currentMarkerStyle=20, int currentMarkerColor=0,
		  int currentLineStyle=1, int currentLineColor=0){
	currentGraph->SetMarkerSize(currentMarkerSize);
	currentGraph->SetMarkerStyle(currentMarkerStyle);
	currentGraph->SetMarkerColor(currentMarkerColor);
	currentGraph->SetLineStyle(currentLineStyle);
	currentGraph->SetLineColor(currentLineColor);
	return;
}

void myOptions(Int_t lStat=0){
	// Set gStyle
	int font = 42;
	// From plain
	gStyle->SetFrameBorderMode(0);
	gStyle->SetFrameFillColor(0);
	gStyle->SetCanvasBorderMode(0);
	gStyle->SetPadBorderMode(0);
	gStyle->SetPadColor(10);
	gStyle->SetCanvasColor(10);
	gStyle->SetTitleFillColor(10);
	gStyle->SetTitleBorderSize(1);
	gStyle->SetStatColor(10);
	gStyle->SetStatBorderSize(1);
	gStyle->SetLegendBorderSize(1);
	//
	gStyle->SetDrawBorder(0);
	gStyle->SetTextFont(font);
	gStyle->SetStatFont(font);
	gStyle->SetStatFontSize(0.05);
	gStyle->SetStatX(0.97);
	gStyle->SetStatY(0.98);
	gStyle->SetStatH(0.03);
	gStyle->SetStatW(0.3);
	gStyle->SetTickLength(0.02,"y");
	gStyle->SetEndErrorSize(3);
	//gStyle->SetLabelSize(0.05,"xyz");
	//gStyle->SetLabelFont(font,"xyz");
	//gStyle->SetLabelOffset(0.01,"xyz");
	//gStyle->SetTitleFont(font,"xyz");
	//gStyle->SetTitleOffset(1.0,"xyz");
	//gStyle->SetTitleSize(0.06,"xyz");
	gStyle->SetMarkerSize(1);
	gStyle->SetPalette(1,0);
	if (lStat){
		gStyle->SetOptTitle(1);
		gStyle->SetOptStat(1111);
		gStyle->SetOptFit(1111);
	}
	else {
		gStyle->SetOptTitle(0);
		gStyle->SetOptStat(0);
		gStyle->SetOptFit(0);
	}
}


void postprocess(TCanvas* c2, const char* name, Int_t rWrite, Int_t rPerformance,const char* system) {
	if (rPerformance){
		TLatex *alice = new TLatex(0.65,0.47,"Performance");
		alice->SetNDC();
		alice->SetTextColor(myDarkRed);
		//    alice->SetTextFont(42);
		alice->SetTextSize(0.05);
		alice->SetLineWidth(2);
		alice->Draw();

		TLatex *alice2 = new TLatex(0.62,0.41,"LHC10h - Pass2");
		alice2->SetNDC();
		alice2->SetTextColor(myDarkRed);
		//    alice->SetTextFont(42);
		alice2->SetTextSize(0.05);
		alice2->SetLineWidth(2);
		alice2->Draw();

		TText *date = new TText(0.68,0.35,cStamp2);
		date->SetNDC();
		date->SetTextFont(42);
		date->SetTextSize(0.04);
		date->Draw();

		//Acquire canvas proportions
		Double_t AliLogo_LowX = 0.67;
		Double_t AliLogo_LowY = 0.53;
		Double_t AliLogo_Height = 0.22;
		//ALICE logo is a png file that is 821x798 pixels->should be wider than a square
		Double_t AliLogo_Width  = (821./798.) * AliLogo_Height * gPad->GetWh() / gPad->GetWw();

		TPad *myPadLogo = new TPad("myPadLogo", "Pad for ALICE Logo",AliLogo_LowX,AliLogo_LowY,AliLogo_LowX+AliLogo_Width,AliLogo_LowY+AliLogo_Height);
		//    myPadLogo->SetFillColor(2); // color to first figure out where is the pad then comment !
		myPadSetUp(myPadLogo,0,0,0,0);
		//myPadLogo->SetFixedAspectRatio(1);
		myPadLogo->Draw();
		myPadLogo->cd();
		TASImage *myAliceLogo = new TASImage("alice_logo_transparent.png");
		myAliceLogo->Draw();
	}

	if (rWrite == 1){
		c2->SaveAs(Form("%s_%s.png",name,system));
	}
	if (rWrite == 2)
		c2->SaveAs(Form("%s.eps",name));

}
