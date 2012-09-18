static  int      myDarkRed     = TColor::GetColor(128,0,0);
static  int      myDarkGreen   = TColor::GetColor(0,128,0);
static  int      myDarkBlue    = TColor::GetColor(0,0,128);

Double_t calculateNormalizationFactor(TH1D *num,TH1D *den, Double_t qlo=0.3,Double_t qhi=0.4)
{
	Double_t binlo = num->GetXaxis()->FindFixBin(qlo);
	Double_t binhi = num->GetXaxis()->FindFixBin(qhi);
	Double_t integralNum = num->Integral(binlo, binhi);
	Double_t integralDen = den->Integral(binlo, binhi);
	return integralDen / integralNum;
}

void getHist (const char* infilename, Int_t rWrite, int cent) {

	TFile *f = new TFile(infilename, "read");
	//TList *list = (TList*)f->Get("femtolist");

	TFile *outfileCF[3];

	// zapis dla FitResCorr

	string str (infilename);
	string str1 ("field1");
	string str3 ("field3");
	size_t found1 = str.find(str1);
	size_t found3 = str.find(str3);

	if (found1 != string::npos) {
		TFile* fileOutFRC = new TFile(Form("CFPAPdata%s.root",str1), "recreate");
		cout << str1 << endl;
	}
	else if (found3 != string::npos) {
		TFile* fileOutFRC = new TFile(Form("CFPAPdata%s.root",str3), "recreate");
		cout << str3 << endl;
	}

	char name[100];
	char title[100];

	Int_t rbPAP = 1;
	Double_t range = 1;
	Double_t rangeuser = 0.3;

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

	TCanvas *c1=new TCanvas("c1",cStamp1,1024,768);
//	TCanvas *c1=new TCanvas("c1",cStamp1,1024,768);
	c1->SetFillColor(10);
	TPad *pad1=new TPad("pad1","this is pad1",0.01,0.67,0.66,0.99);
	TPad *pad3=new TPad("pad3","this is pad3",0.01,0.34,0.66,0.66);
	TPad *pad5=new TPad("pad5","this is pad5",0.01,0.01,0.66,0.33);
	TPad *pad2=new TPad("pad2","this is pad2",0.67,0.67,0.99,0.99);
	TPad *pad4=new TPad("pad4","this is pad4",0.67,0.34,0.99,0.66);
	TPad *pad6=new TPad("pad6","this is pad6",0.67,0.01,0.99,0.33);
	pad1->SetFillColor(10);
	pad2->SetFillColor(10);
	pad3->SetFillColor(10);
	pad4->SetFillColor(10);
	pad5->SetFillColor(10);
	pad6->SetFillColor(10);
	pad1->Draw();
	pad2->Draw();
	pad3->Draw();
	pad4->Draw();
	pad5->Draw();
	pad6->Draw();

	pad1->SetLeftMargin(0.07);
	pad1->SetRightMargin(0.01);
	pad1->SetTopMargin(0.01);
	pad1->SetBottomMargin(0.14);
	pad2->SetLeftMargin(0.11);
	pad2->SetRightMargin(0.01);
	pad2->SetTopMargin(0.01);
	pad2->SetBottomMargin(0.14);
	pad3->SetLeftMargin(0.07);
	pad3->SetRightMargin(0.01);
	pad3->SetTopMargin(0.01);
	pad3->SetBottomMargin(0.14);
	pad4->SetLeftMargin(0.11);
	pad4->SetRightMargin(0.01);
	pad4->SetTopMargin(0.01);
	pad4->SetBottomMargin(0.14);
	pad5->SetLeftMargin(0.07);
	pad5->SetRightMargin(0.01);
	pad5->SetTopMargin(0.01);
	pad5->SetBottomMargin(0.13);
	pad6->SetLeftMargin(0.11);
	pad6->SetRightMargin(0.01);
	pad6->SetTopMargin(0.01);
	pad6->SetBottomMargin(0.14);

	TLegend *myLegend = new TLegend(0.7,0.6,0.99,0.99);
	myLegendSetUp(myLegend,0.08);

	TH1D* nump[12][12];
	TH1D* numn[12][12];
	TH1D* denp[12][12];
	TH1D* denn[12][12];


	int a = 1;
	char numNamePout[100];
	char denNamePout[100];
	char numNameNout[100];
	char denNameNout[100];
	char numNamePside[100];
	char denNamePside[100];
	char numNameNside[100];
	char denNameNside[100];
	char numNamePlong[100];
	char denNamePlong[100];
	char numNameNlong[100];
	char denNameNlong[100];

	TLine *l= new TLine(0.0, 1.0, range, 1.0);
	l->SetLineWidth(2.);

    TFile* f01 = new TFile("asym.root","update");

	//for (int i = 1; i < 7; ++i) {		// kT
	//zmieniac 1-2, 3-4, 5-6
	for (int j = cent; j < cent+1; ++j) {		// cent
		cout<<j<<endl;
		// sprintf(numNamePout,"nump");
		// sprintf(numNameNout,"numn");
		// sprintf(denNamePout,"denp");
		// sprintf(denNameNout,"denn");
		sprintf(numNamePout,"NumOutPckstarPAPtpcM%d",j-1);
		cout<<numNamePout<<endl;
		sprintf(numNameNout,"NumOutNckstarPAPtpcM%d",j-1);
		sprintf(denNamePout,"DenOutPckstarPAPtpcM%d",j-1);
		sprintf(denNameNout,"DenOutNckstarPAPtpcM%d",j-1);
		sprintf(numNamePside,"NumSidePckstarPAPtpcM%d",j-1);
		sprintf(numNameNside,"NumSideNckstarPAPtpcM%d",j-1);
		sprintf(denNamePside,"DenSidePckstarPAPtpcM%d",j-1);
		sprintf(denNameNside,"DenSideNckstarPAPtpcM%d",j-1);
		sprintf(numNamePlong,"NumLongPckstarPAPtpcM%d",j-1);
		sprintf(numNameNlong,"NumLongNckstarPAPtpcM%d",j-1);
		sprintf(denNamePlong,"DenLongPckstarPAPtpcM%d",j-1);
		sprintf(denNameNlong,"DenLongNckstarPAPtpcM%d",j-1);

		nump[j-1][0] = (TH1D*)f->Get(numNamePout);
		numn[j-1][0] = (TH1D*)f->Get(numNameNout);
		denp[j-1][0] = (TH1D*)f->Get(denNamePout);
		denn[j-1][0] = (TH1D*)f->Get(denNameNout);

		nump[j-1][1] = (TH1D*)f->Get(numNamePside);
		numn[j-1][1] = (TH1D*)f->Get(numNameNside);
		denp[j-1][1] = (TH1D*)f->Get(denNamePside);
		denn[j-1][1] = (TH1D*)f->Get(denNameNside);

		nump[j-1][2] = (TH1D*)f->Get(numNamePlong);
		numn[j-1][2] = (TH1D*)f->Get(numNameNlong);
		denp[j-1][2] = (TH1D*)f->Get(denNamePlong);
		denn[j-1][2] = (TH1D*)f->Get(denNameNlong);

		    nump[j-1][0] -> Rebin(rbPAP);
		    numn[j-1][0] -> Rebin(rbPAP);
		    denp[j-1][0] -> Rebin(rbPAP);
		    denn[j-1][0] -> Rebin(rbPAP);
		    nump[j-1][1] -> Rebin(rbPAP);
		    numn[j-1][1] -> Rebin(rbPAP);
		    denp[j-1][1] -> Rebin(rbPAP);
		    denn[j-1][1] -> Rebin(rbPAP);
		    nump[j-1][2] -> Rebin(rbPAP);
		    numn[j-1][2] -> Rebin(rbPAP);
		    denp[j-1][2] -> Rebin(rbPAP);
		    denn[j-1][2] -> Rebin(rbPAP);

		// TODO: zapis licznika i mianownika dla CorrFita
		//    outfileCF[j-1] = new TFile(Form("PAP_m%d_CorrFit.root",j-1),"recreate");	// plik wyjsciowy do corrfita
//     nump[j-1][0]->Write();
//     denp[j-1][0]->Write();
//     nump[j-1][1]->Write();
//     denp[j-1][1]->Write();
//     nump[j-1][2]->Write();
//     denp[j-1][2]->Write();
//     numn[j-1][0]->Write();
//     denn[j-1][0]->Write();
//     numn[j-1][1]->Write();
//     denn[j-1][1]->Write();
//     numn[j-1][2]->Write();
//     denn[j-1][2]->Write();

		double range = 0.5;

        //if (j == 2) {

                // nump[1][0]->Add(nump[1][0]);
                // numn[1][0]->Add(numn[1][0]);
                // denp[1][0]->Add(denp[1][0]);
                // denn[1][0]->Add(denn[1][0]);
		//out cf
		Double_t norm = calculateNormalizationFactor(nump[j-1][0], denp[j-1][0]);
		//    cout<<norm<<endl;

		nump[j-1][0]->Divide(denp[j-1][0]);
		nump[j-1][0]->Scale(norm);

		norm = calculateNormalizationFactor(numn[j-1][0], denn[j-1][0]);
		//    cout<<norm<<endl;

		numn[j-1][0]->Divide(denn[j-1][0]);
		numn[j-1][0]->Scale(norm);

		int bin = nump[j-1][0]->GetNbinsX()+numn[j-1][0]->GetNbinsX();
		//cout<<bin<<endl;

		TH1D *cfOut = new TH1D(Form("cfOutPAPm%d",j-1),"cfOut",bin,-range,range);

		cfOut->GetXaxis()->SetTitle("sign(k*_{out})* k* (GeV/c)");
		cfOut->GetYaxis()->SetTitle("C (k*)");
		cfOut->GetXaxis()->SetTitleOffset(1.0);
		cfOut->GetYaxis()->SetTitleOffset(0.5);
		cfOut->SetMaximum(1.09);
		cfOut->SetMinimum(0.94);

		for (int i=1;i<=bin/2;++i){
			cfOut->SetBinContent(i,numn[j-1][0]->GetBinContent(bin/2+1-i));
			cfOut->SetBinError(i,numn[j-1][0]->GetBinError(bin/2+1-i));
			//      cout << i << " " << numn[j-1][0]->GetBinContent(bin/2+1-i) << endl;
		}
		for (int i=bin/2+1;i<=bin;++i){
			cfOut->SetBinContent(i,nump[j-1][0]->GetBinContent(i-bin/2));
			cfOut->SetBinError(i,nump[j-1][0]->GetBinError(i-bin/2));
			//      cout << i << " " << nump[j-1][0]->GetBinContent(i-bin/2) << endl;

		}

		// zapis dla FitResCorr

		cfOut->Write();

		pad1->cd();
		// if (j == 1) {

		cfOut->GetXaxis()->SetRangeUser(-0.3, 0.3);
		//cfOut->SetMinimum(0.6);
		//cfOut->SetMaximum(1.6);
		cfOut->SetMarkerStyle(20);
		cfOut->SetMarkerColor(2);
		cfOut->Draw();
		myLegend->AddEntry(cfOut,"centrality 0-10%","p");
		// }
		// else if (j == 2){
		//   cfOut->SetMarkerStyle(21);
		//   cfOut->SetMarkerColor(3);
		//   cfOut->Draw("same");
		//   myLegend->AddEntry(cfOut,"centrality 10-30%","p");
		// }
		// else if (j == 3){
		//   cfOut->SetMarkerStyle(22);
		//   cfOut->SetMarkerColor(4);
		//   cfOut->Draw("same");
		//   myLegend->AddEntry(cfOut,"centrality 30-70%","p");
		//   myLegend->Draw();

		// }

		//out dr
		nump[j-1][0]->Divide(numn[j-1][0]);
		nump[j-1][0]->SetXTitle("k* (GeV/c)");
		nump[j-1][0]->SetYTitle("double ratio (out)");
		nump[j-1][0]->SetMaximum(1.09);
		nump[j-1][0]->SetMinimum(0.94);

		pad2->cd();
		// if (j == 1) {
		nump[j-1][0]->GetXaxis()->SetRangeUser(0.0, 0.3);
		//nump[j-1][0]->SetMinimum(0.6);
		//nump[j-1][0]->SetMaximum(1.6);
		nump[j-1][0]->SetMarkerStyle(20);
		nump[j-1][0]->SetMarkerColor(2);

		nump[j-1][0]->Write();
		nump[j-1][0]->Draw();
		// }
		// else if (j == 3){
		//   nump[j-1][0]->SetMarkerStyle(21);
		//   nump[j-1][0]->SetMarkerColor(3);

		//   nump[j-1][0]->Draw("same");
		// }
		// else if (j == 5){

		//   nump[j-1][0]->SetMarkerStyle(22);
		//   nump[j-1][0]->SetMarkerColor(4);

		//   nump[j-1][0]->Draw("same");
		// }

        //    }

		//side cf
		Double_t norm = calculateNormalizationFactor(nump[j-1][1], denp[j-1][1]);
		//    cout<<norm<<endl;

		nump[j-1][1]->Divide(denp[j-1][1]);
		nump[j-1][1]->Scale(norm);

		norm = calculateNormalizationFactor(numn[j-1][1], denn[j-1][1]);
		//    cout<<norm<<endl;

		numn[j-1][1]->Divide(denn[j-1][1]);
		numn[j-1][1]->Scale(norm);

		int bin = nump[j-1][1]->GetNbinsX()+numn[j-1][1]->GetNbinsX();
		//cout<<bin<<endl;

		TH1D *cfSide = new TH1D("cfSide","cfSide",bin,-range,range);
		cfSide->GetXaxis()->SetTitle("sign(k*_{side})* k* (GeV/c)");
		cfSide->GetYaxis()->SetTitle("C (k*)");
		cfSide->GetXaxis()->SetTitleOffset(1.0);
		cfSide->GetYaxis()->SetTitleOffset(0.5);
		cfSide->SetMaximum(1.09);
		cfSide->SetMinimum(0.94);

		for (int i=1;i<=bin/2;++i){
			cfSide->SetBinContent(i,numn[j-1][1]->GetBinContent(bin/2+1-i));
			cfSide->SetBinError(i,numn[j-1][1]->GetBinError(bin/2+1-i));
		}
		for (int i=bin/2+1;i<=bin;++i){
			cfSide->SetBinContent(i,nump[j-1][1]->GetBinContent(i-bin/2));
			cfSide->SetBinError(i,nump[j-1][1]->GetBinError(i-bin/2));
		}

		pad3->cd();
		// if (j == 1) {

		cfSide->GetXaxis()->SetRangeUser(-0.3, 0.3);
		//cfSide->SetMinimum(0.6);
		//cfSide->SetMaximum(1.6);
		cfSide->SetMarkerStyle(20);
		cfSide->SetMarkerColor(2);
		cfSide->Draw();
		// }
		// else if (j == 2){
		//   cfSide->SetMarkerStyle(21);
		//   cfSide->SetMarkerColor(3);
		//   cfSide->Draw("same");

		// }
		// else if (j == 3){
		//   cfSide->SetMarkerStyle(22);
		//   cfSide->SetMarkerColor(4);
		//   cfSide->Draw("same");

		// }

		//side dr
		nump[j-1][1]->Divide(numn[j-1][1]);
		nump[j-1][1]->SetXTitle("k* (GeV/c)");
		nump[j-1][1]->SetYTitle("double ratio (side)" );
		nump[j-1][1]->SetMaximum(1.18);
		nump[j-1][1]->SetMinimum(0.82);

		pad4->cd();
		// if (j == 1) {
		nump[j-1][1]->GetXaxis()->SetRangeUser(0.0, 0.3);
		//nump[j-1][1]->SetMinimum(0.6);
		//nump[j-1][1]->SetMaximum(1.6);
		nump[j-1][1]->SetMarkerStyle(20);
		nump[j-1][1]->SetMarkerColor(2);

		nump[j-1][1]->Draw();
		// }
		// else if (j == 2){
		//   nump[j-1][1]->SetMarkerStyle(21);
		//   nump[j-1][1]->SetMarkerColor(3);

		//   nump[j-1][1]->Draw("same");
		// }
		// else if (j == 3){

		//   nump[j-1][1]->SetMarkerStyle(22);
		//   nump[j-1][1]->SetMarkerColor(4);

		//   nump[j-1][1]->Draw("same");
		// }

		//long cf
		Double_t norm = calculateNormalizationFactor(nump[j-1][2], denp[j-1][2]);
		//    cout<<norm<<endl;

		nump[j-1][2]->Divide(denp[j-1][2]);
		nump[j-1][2]->Scale(norm);

		norm = calculateNormalizationFactor(numn[j-1][2], denn[j-1][2]);
		//    cout<<norm<<endl;

		numn[j-1][2]->Divide(denn[j-1][2]);
		numn[j-1][2]->Scale(norm);

		int bin = nump[j-1][2]->GetNbinsX()+numn[j-1][2]->GetNbinsX();
		//cout<<bin<<endl;

		TH1D *cfLong = new TH1D("cfLong","cfLong",bin,-range,range);

		cfLong->GetXaxis()->SetTitle("sign(k*_{long})* k* (GeV/c)");
		cfLong->GetYaxis()->SetTitle("C (k*)");
		cfLong->GetXaxis()->SetTitleOffset(0.9);
		cfLong->GetYaxis()->SetTitleOffset(0.5);
		cfLong->SetMaximum(1.09);
		cfLong->SetMinimum(0.94);

		for (int i=1;i<=bin/2;++i){
			cfLong->SetBinContent(i,numn[j-1][2]->GetBinContent(bin/2+1-i));
			cfLong->SetBinError(i,numn[j-1][2]->GetBinError(bin/2+1-i));
		}
		for (int i=bin/2+1;i<=bin;++i){
			cfLong->SetBinContent(i,nump[j-1][2]->GetBinContent(i-bin/2));
			cfLong->SetBinError(i,nump[j-1][2]->GetBinError(i-bin/2));
		}

		pad5->cd();
		// if (j == 1) {

		cfLong->GetXaxis()->SetRangeUser(-0.3, 0.3);
		//cfLong->SetMinimum(0.6);
		//cfLong->SetMaximum(1.6);
		cfLong->SetMarkerStyle(20);
		cfLong->SetMarkerColor(2);
		cfLong->Draw();
		// }
		// else if (j == 2){
		//   cfLong->SetMarkerStyle(21);
		//   cfLong->SetMarkerColor(3);
		//   cfLong->Draw("same");

		// }
		// else if (j == 3){
		//   cfLong->SetMarkerStyle(22);
		//   cfLong->SetMarkerColor(4);
		//   cfLong->Draw("same");

		// }

		//long dr
		nump[j-1][2]->Divide(numn[j-1][2]);
		nump[j-1][2]->SetXTitle("k* (GeV/c)");
		nump[j-1][2]->SetYTitle("double ratio (long)");
		nump[j-1][2]->SetMaximum(1.09);
		nump[j-1][2]->SetMinimum(0.94);

		pad6->cd();
		// if (j == 1) {
		nump[j-1][2]->GetXaxis()->SetRangeUser(0.0, 0.3);
		//nump[j-1][2]->SetMinimum(0.6);
		//nump[j-1][2]->SetMaximum(1.6);
		nump[j-1][2]->SetMarkerStyle(20);
		nump[j-1][2]->SetMarkerColor(2);

		nump[j-1][2]->Draw();
		// }
		// else if (j == 2){
		//   nump[j-1][2]->SetMarkerStyle(21);
		//   nump[j-1][2]->SetMarkerColor(3);

		//   nump[j-1][2]->Draw("same");
		// }
		// else if (j == 3){

		//   nump[j-1][2]->SetMarkerStyle(22);
		//   nump[j-1][2]->SetMarkerColor(4);

		//   nump[j-1][2]->Draw("same");
		// }

		//l->Draw("same");


	}

	//}

	sprintf(name,"%s_corrfun_ktAll.png",system);
	//if (rWrite == 1)  c1->SaveAs("fig_template.eps");
	if (rWrite == 1) {
            c1->SaveAs("cfPAPnonID.png");
            c1->SaveAs("cfPAPnonID.eps");
    }
}

void drawNonID(Int_t rWrite = 0, Int_t rPerformance = 0, const char* infilename, int cent){

	myOptions();
	gROOT->ForceStyle();

	getHist(infilename,rWrite,cent);

	if (rPerformance){
		TLatex *alice = new TLatex(0.75,0.34,"Performance");
		alice->SetNDC();
		alice->SetTextColor(myDarkRed);
		//    alice->SetTextFont(42);
		alice->SetTextSize(0.05);
		alice->SetLineWidth(2);
		alice->Draw();

		TText *date = new TText(0.78,0.28,cStamp2);
		date->SetNDC();
		date->SetTextFont(42);
		date->SetTextSize(0.04);
		date->Draw();

		//Acquire canvas proportions
		Double_t AliLogo_LowX = 0.77;
		Double_t AliLogo_LowY = 0.37;
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
	gStyle->SetLabelSize(0.05,"xyz");
	gStyle->SetLabelFont(font,"xyz");
	gStyle->SetLabelOffset(0.01,"xyz");
	gStyle->SetTitleFont(font,"xyz");
	gStyle->SetTitleOffset(1.0,"xyz");
	gStyle->SetTitleSize(0.06,"xyz");
	gStyle->SetMarkerSize(1.0);
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
