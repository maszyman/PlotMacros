// makro do rysowania funkcji korelacyjnych w binach multiplicity
// mps 10/08/2011

static  int      myDarkRed     = TColor::GetColor(128,0,0);
static  int      myDarkGreen   = TColor::GetColor(0,128,0);
static  int      myDarkBlue    = TColor::GetColor(0,0,128);

const char* figureInfo = "Pb-Pb #sqrt{s_{NN}} = 2.76 TeV";
//const char* figureInfo = "Pb-Pb @ 2.76 TeV";
//const char* figureInfo = "Pb-Pb events at #sqrt{s_{NN}} = 2.76 TeV";
const char* production1 = "LHC10h";
const char* production2 = "LHC11h";
const char* asdad ="";

char cStamp1[25],cStamp2[25];


void DrawALICELogo(Bool_t prel, Float_t x1, Float_t y1, Float_t x2, Float_t y2)
{
  // correct for aspect ratio of figure plus aspect ratio of pad (coordinates are NDC!)
  x2 = x1 + (y2 - y1) * (466. / 523) * gPad->GetWh() * gPad->GetHNDC() / (gPad->GetWNDC() * gPad->GetWw());
  // Printf("%f %f %f %f", x1, x2, y1, y2);

  TPad *myPadLogo = new TPad("myPadLogo", "Pad for ALICE Logo", x1, y1, x2, y2);
  myPadLogo->SetLeftMargin(0);
  myPadLogo->SetTopMargin(0);
  myPadLogo->SetRightMargin(0);
  myPadLogo->SetBottomMargin(0);
  myPadLogo->Draw();
  myPadLogo->cd();
  TASImage *myAliceLogo = new TASImage((prel) ? "~/alice_logo_preliminary.eps" : "~/alice_logo_performance.eps");
  myAliceLogo->Draw();
}


void setDate()
{
        TDatime now;
        int iDate = now.GetDate();
        int iYear=iDate/10000;
        int iMonth=(iDate%10000)/100;
        int iDay=iDate%100;
        char* cMonth[12]={"Jan","Feb","Mar","Apr","May","Jun",
                          "Jul","Aug","Sep","Oct","Nov","Dec"};
        sprintf(cStamp1,"%i %s %i",iDay, cMonth[iMonth-1], iYear);
        sprintf(cStamp2,"%i/%.2d/%i",iDay, iMonth, iYear);

}

Double_t calculateNormalizationFactor(TH1D *num,TH1D *den, Double_t qlo=0.3,Double_t qhi=0.4)
{
        Double_t binlo = num->GetXaxis()->FindFixBin(qlo);
        Double_t binhi = num->GetXaxis()->FindFixBin(qhi);
        Double_t integralNum = num->Integral(binlo, binhi);
        Double_t integralDen = den->Integral(binlo, binhi);
        return integralDen / integralNum;
}

void postprocess(TCanvas* c2, const char* name, Int_t rWrite, Int_t rPerformance,const char* system, const char* field, int kT)
{

        TLatex *sys = new TLatex(0.4,0.9,figureInfo);
        sys->SetNDC();
        sys->SetTextFont(42);
        sys->SetTextSize(0.06);
        sys->SetTextColor(myDarkRed);
        sys->Draw();
        TLatex *sys2 = new TLatex(0.42,0.8,asdad);
        sys2->SetNDC();
        sys2->SetTextFont(42);
        sys2->SetTextSize(0.05);
        sys2->SetTextColor(myDarkRed);
        sys2->Draw("same");

        if (rPerformance){
                // TLatex *alice = new TLatex(0.75,0.47,"Performance");
                // alice->SetNDC();
                // alice->SetTextColor(myDarkRed);
                // alice->SetTextFont(42);
                // alice->SetTextSize(0.05);
                // alice->SetLineWidth(2);
                // alice->Draw();

                if (rPerformance > 2)
                        TLatex *alice2 = new TLatex(0.72,0.51,production2);
                else
                        TLatex *alice2 = new TLatex(0.72,0.51,production1);
                alice2->SetNDC();
                alice2->SetTextColor(myDarkRed);
                alice2->SetTextFont(42);
                alice2->SetTextSize(0.05);
                alice2->SetLineWidth(2);
//		alice2->Draw();

                TText *date = new TText(0.77,0.35,cStamp2);
                date->SetNDC();
                date->SetTextFont(42);
                date->SetTextSize(0.04);
//                date->Draw();

                //Acquire canvas proportions
                Double_t AliLogo_LowX = 0.77;
                Double_t AliLogo_LowY = 0.4;
                Double_t AliLogo_Height = 0.22;
                //ALICE logo is a png file that is 821x798 pixels->should be wider than a square
                Double_t AliLogo_Width  = (821./798.) * AliLogo_Height * gPad->GetWh() / gPad->GetWw();

                TPad *myPadLogo = new TPad("myPadLogo", "Pad for ALICE Logo",AliLogo_LowX,AliLogo_LowY,AliLogo_LowX+AliLogo_Width,AliLogo_LowY+AliLogo_Height);
                //    myPadLogo->SetFillColor(2); // color to first figure out where is the pad then comment !
                myPadSetUp(myPadLogo,0,0,0,0);
                myPadLogo->SetFixedAspectRatio(1);
                myPadLogo->Draw();
                myPadLogo->cd();
                TASImage *myAliceLogo = new TASImage("alice_preliminary.eps");
//                TASImage *myAliceLogo = new TASImage("alice_performance.eps");
//		TASImage *myAliceLogo = new TASImage("alice_logo_transparent.png");
//                myAliceLogo->Draw();
        }

        TLatex *sigma = new TLatex(0.45,0.5,"#sigma = #sqrt{#sigma_{stat}^{2} + #sigma_{syst}^{2}}");
		sigma->SetNDC();
		sigma->SetTextColor(kRed+2);
		sigma->SetTextFont(42);
		sigma->SetTextSize(0.04);
		sigma->SetLineWidth(2);
		sigma->Draw();
        DrawALICELogo(1,0.7,0.55,0.8,0.8);


        c2->SaveAs(Form("cf_%s_%s_kT%d.png",system,field,kT));
        c2->SaveAs(Form("cf_%s_%s_kT%d.eps",system,field,kT));

        if (rWrite == 1)
                c2->SaveAs(Form("%s.png",name));

        if (rWrite == 2)
                c2->SaveAs(Form("%s.eps",name));

}

void drawCorrFunMergedBins (const char* infilename,const char* infilename2,const char* system, const char* field, Int_t rWrite, Int_t rPerformance, Int_t kT)
{

        myOptions();
        gROOT->ForceStyle();
        setDate();

//         if (strcmp(field,"f1") == 0) {
//                 if (strcmp(system,"PP") == 0)
// //			asdad = "p-p correlation functions (field --)";
//                         asdad = "p-p correlation functions LHC11h";
//                 else  if (strcmp(system,"APAP") == 0)
//                         asdad = "#bar{p}-#bar{p} correlation functions (field --)";
//                 else  if (strcmp(system,"PAP") == 0)
//                         asdad = "p-#bar{p} correlation functions (field --)";
//         }
//         else if (strcmp(field,"f3") == 0){
//                 if (strcmp(system,"PP") == 0)
//                         asdad = "p-p correlation functions (field ++)";
//                 else  if (strcmp(system,"APAP") == 0)
//                         asdad = "#bar{p}-#bar{p} correlation functions (field ++)";
//                 else  if (strcmp(system,"PAP") == 0)
//                         asdad = "p-#bar{p} correlation functions (field ++)";
//         }

        asdad = "#LT k_{T}#GT = 1.3 GeV/c";


        TFile *f = new TFile(infilename, "read");
        TFile *f2 = new TFile(infilename2, "read");

        //TList *list = (TList*)f->Get("femtolist");
        TFile* ofile = new TFile(Form("cf_%s_%s.root",system,field),"recreate");

        char name[100];
        char title[100];

        sprintf(name,system);

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

        TCanvas *myCan = new TCanvas("myCan",cStamp1);
        myCan->Draw();
        myCan->cd();

        TPad *myPad = new TPad("myPad", "The pad",0,0,1,1);
        myPadSetUp(myPad,0.15,0.04,0.04,0.15);
        myPad->Draw();
        myPad->cd();

        TH1D* nump[11];
        TH1D* denp[11];

        int a = 1;
        char numName[100];
        char denName[100];
        char numNameN[100];
        char denNameN[100];

        TLine *l= new TLine(0.0, 1.0, 0.9, 1.0);
        l->SetLineWidth(2.);

        TLegend *myLegend = new TLegend(0.4,0.6,0.7,0.75);
        myLegendSetUp(myLegend,0.04);

        int multBins = 7;
        int rebin = 1;

        //mergowanie centralnosci:
        // 0 0-5
        // 1 5-10
        // 2 10-20
        // 3 20-30
        // 4 30-40
        // 5 40-50
        // 6 50-60
        // 7 60-70
        // 8 70-80
        // 9 80-90
        int merge1 = 1;
        int merge2 = 3;
        int merge3 = 5;

        //int kT = 0;

        for (int j = 0; j < multBins; ++j) {

                if (strcmp(system,"PP") == 0)
                        sprintf(numName,"NumcqinvPPtpcM%dkT%d",j,kT);
                else if (strcmp(system,"APAP") == 0)
                        sprintf(numName,"NumcqinvAPAPtpcM%dkT%d",j,kT);
                else if (strcmp(system,"PAP") == 0)	{
                        // sprintf(numName,"NumcqinvPAPtpcM%dkT%d",j,kT);

                        sprintf(numName,"NumLongPckstarPAPtpcM%dkT%d",j,kT);
                        sprintf(numNameN,"NumLongNckstarPAPtpcM%dkT%d",j,kT);
                }
                if (strcmp(system,"PP") == 0)
                        sprintf(denName,"DencqinvPPtpcM%dkT%d",j,kT);
                else if (strcmp(system,"APAP") == 0)
                        sprintf(denName,"DencqinvAPAPtpcM%dkT%d",j,kT);
                else if (strcmp(system,"PAP") == 0)	{
                        // sprintf(denName,"DencqinvPAPtpcM%dkT%d",j,kT);

                        sprintf(denName,"DenLongPckstarPAPtpcM%dkT%d",j,kT);
                        sprintf(denNameN,"DenLongNckstarPAPtpcM%dkT%d",j,kT);
                }
                if (j == 0) {
                        if (strcmp(system,"PAP") == 0) {
                                if (j < 2) {
                                        nump[j] = (TH1D*)f->Get(numName);
                                        denp[j] = (TH1D*)f->Get(denName);
                                }
                                else {
                                        nump[j] = (TH1D*)f2->Get(numName);
                                        denp[j] = (TH1D*)f2->Get(denName);
                                }

                                // nump[j] ->Add( (TH1D*)f->Get(numNameN));
                                // denp[j] ->Add( (TH1D*)f->Get(denNameN));
                        }
                        else {
                                if (j < 2) {
                                        nump[j] = (TH1D*)f->Get(numName);
                                        denp[j] = (TH1D*)f->Get(denName);
                                }
                                else {
                                        nump[j] = (TH1D*)f2->Get(numName);
                                        denp[j] = (TH1D*)f2->Get(denName);
                                }

                                // nump[j] = (TH1D*)f->Get(numName);
                                // denp[j] = (TH1D*)f->Get(denName);
                                cout << "No Of Pairs in numerator " << nump[0]->GetEntries() << endl;

                        }
                }
                else if (j < merge1+1) {
                        if (strcmp(system,"PAP") == 0) {
                                if ( j  < 3) {
                                        nump[0] ->Add( (TH1D*)f->Get(numName));
                                        denp[0] ->Add( (TH1D*)f->Get(denName));
                                }
                                else {
                                        nump[0] ->Add( (TH1D*)f2->Get(numName));
                                        denp[0] ->Add( (TH1D*)f2->Get(denName));

                                }
                                // nump[0] ->Add( (TH1D*)f->Get(numNameN));
                                // denp[0] ->Add( (TH1D*)f->Get(denNameN));
                        }
                        else {
                                // nump[0] ->Add( (TH1D*)f->Get(numName));
                                // denp[0] ->Add( (TH1D*)f->Get(denName));
                                if ( j  < 3) {
                                        nump[0] ->Add( (TH1D*)f->Get(numName));
                                        denp[0] ->Add( (TH1D*)f->Get(denName));
                                }
                                else {
                                        nump[0] ->Add( (TH1D*)f2->Get(numName));
                                        denp[0] ->Add( (TH1D*)f2->Get(denName));
                                }
                        }

                        if (j == merge1){
                                cout << "no of pairs in numerator " << nump[0]->GetEntries() << endl;

                                nump[0]->Rebin(rebin);
                                denp[0]->Rebin(rebin);

                                nump[0]->Write();


		// // TODO: zapis licznika i mianownika dla CorrFita
		// outfileCF[0]  = new TFile(Form("%s_m%d_CorrFit.root",system,j-1),"recreate");
		// nump[0]->Write();
		// denp[0]->Write();

                                Double_t norm = calculateNormalizationFactor(nump[0], denp[0],0.3,0.4);
                                nump[0]->Divide(denp[0]);
                                nump[0]->Scale(norm);
                                nump[0]->SetName(Form("corrfun_c%d",0));
                                nump[0]->Write();
                                nump[0]->SetMarkerStyle(0+20);
                                nump[0]->SetMarkerColor(kBlack);
                                nump[0]->SetMarkerSize(1.3);
                                nump[0]->GetXaxis()->SetRangeUser(0.0, 0.3);
                                if (strcmp(system,"PAP") == 0){
                                        nump[0]->SetMaximum(1.25);
                                        nump[0]->SetMinimum(0.94);
                                        nump[0]->SetXTitle("q_{inv} (GeV/c)");
                                        nump[0]->SetYTitle("C(q_{inv})");
                                        // nump[0]->SetXTitle("k* (GeV/c)");
                                        // nump[0]->SetYTitle("C (k*)");
                                }
                                else{
                                        nump[0]->SetMaximum(1.78);
                                        nump[0]->SetMinimum(0.92);
                                        nump[0]->SetXTitle("q_{inv} (GeV/c)");
                                        nump[0]->SetYTitle("C(q_{inv})");
                                }
                                nump[0]->SetTitle("");

                                nump[0]->SetLabelSize(0.05,"xyz");
                                nump[0]->SetLabelFont(42,"xyz");
                                nump[0]->SetLabelOffset(0.01,"xyz");
                                nump[0]->SetTitleFont(42,"xyz");
                                nump[0]->SetTitleOffset(1.0,"xyz");
                                nump[0]->SetTitleSize(0.06,"xyz");

                                nump[0]->GetXaxis()->SetTitleSize(0.065);
                                nump[0]->GetYaxis()->SetTitleSize(0.065);
                                nump[0]->GetXaxis()->SetNdivisions(8);
                                nump[0]->GetYaxis()->SetNdivisions(8);
                                nump[0]->Draw("P");

                                if (strcmp(system,"PP") == 0){
                                        myLegend->AddEntry(nump[0],"pp 0-10%","p");
                                }
                                else if (strcmp(system,"APAP") == 0){
                                        myLegend->AddEntry(nump[0],"#bar{p}#bar{p} 0-10%","p");
                                }
                                else if (strcmp(system,"PAP") == 0){
                                        myLegend->AddEntry(nump[0],"p#bar{p} %s 0-10%","p");
                                }

                                // if (kT == 0)
                                //         myLegend->AddEntry(nump[0], "0-10%, k_{T} < 0.9 GeV/c","p");
                                // else if (kT == 1)
                                //         myLegend->AddEntry(nump[0], "0-10%, k_{T} > 0.9 GeV/c","p");
                        }
                }
                else if (j == merge1+1) {
                        if (strcmp(system,"PAP") == 0) {
                                if (j < 2) {
                                        nump[j] = (TH1D*)f->Get(numName);
                                        denp[j] = (TH1D*)f->Get(denName);
                                }
                                else {
                                        nump[j] = (TH1D*)f2->Get(numName);
                                        denp[j] = (TH1D*)f2->Get(denName);
                                }
                                // nump[j] ->Add( (TH1D*)f->Get(numNameN));
                                // denp[j] ->Add( (TH1D*)f->Get(denNameN));
                        }
                        else {
                                if (j < 2) {
                                        nump[j] = (TH1D*)f->Get(numName);
                                        denp[j] = (TH1D*)f->Get(denName);
                                }
                                else {
                                        cout << "asd" << endl;
                                        cout << numName << endl;
                                        nump[j] = (TH1D*)f2->Get(numName);
                                        denp[j] = (TH1D*)f2->Get(denName);
                                }
                        }
                }
                else if (j < merge2+1){
                        if (strcmp(system,"PAP") == 0) {
                                if ( j < 2) {
                                        nump[merge1+1] ->Add( (TH1D*)f->Get(numName));
                                        denp[merge1+1] ->Add( (TH1D*)f->Get(denName));
                                        nump[merge1+1] ->Add( (TH1D*)f->Get(numNameN));
                                        denp[merge1+1] ->Add( (TH1D*)f->Get(denNameN));
                                }
                                else {
                                        nump[merge1+1] ->Add( (TH1D*)f2->Get(numName));
                                        denp[merge1+1] ->Add( (TH1D*)f2->Get(denName));
                                        nump[merge1+1] ->Add( (TH1D*)f2->Get(numNameN));
                                        denp[merge1+1] ->Add( (TH1D*)f2->Get(denNameN));
                                }

                        }
                        else {
                                if ( j < 2) {
                                        nump[merge1+1] ->Add( (TH1D*)f->Get(numName));
                                        denp[merge1+1] ->Add( (TH1D*)f->Get(denName));
                                }
                                else {
                                        nump[merge1+1] ->Add( (TH1D*)f2->Get(numName));
                                        denp[merge1+1] ->Add( (TH1D*)f2->Get(denName));
                                }
                        }
                        if (j == merge2){
                                cout << "no of pairs in numerator " << nump[merge1+1]->GetEntries() << endl;
                                nump[merge1+1]->Rebin(rebin);
                                denp[merge1+1]->Rebin(rebin);

                                nump[merge1+1]->Write();
                                Double_t norm = calculateNormalizationFactor(nump[merge1+1], denp[merge1+1],0.3,0.4);
                                nump[merge1+1]->Divide(denp[merge1+1]);
                                nump[merge1+1]->Scale(norm);
                                nump[merge1+1]->SetName(Form("corrfun_c%d",merge1+1));
                                nump[merge1+1]->Write();
                                nump[merge1+1]->SetMarkerStyle(21);
                                nump[merge1+1]->SetMarkerColor(kRed);
                                nump[merge1+1]->SetMarkerSize(1.3);
                                nump[merge1+1]->Draw("same");


                                if (strcmp(system,"PP") == 0){
                                        myLegend->AddEntry(nump[merge1+1],"pp 10-30%","p");
                                }
                                else if (strcmp(system,"APAP") == 0){
                                        myLegend->AddEntry(nump[merge1+1],"#bar{p}#bar{p} 10-30%","p");
                                }
                                else if (strcmp(system,"PAP") == 0){
                                        myLegend->AddEntry(nump[merge1+1],"p#bar{p} %s 10-30%","p");
                                }

                                //    	myLegend->AddEntry(nump[merge1+1],Form("#bar{p}-#bar{p} %s", "10-30%"),"p");
                        }
                }
                else if (j == merge2+1) {
                        if (strcmp(system,"PAP") == 0) {
                                if (j < 2) {
                                        nump[j] = (TH1D*)f->Get(numName);
                                        denp[j] = (TH1D*)f->Get(denName);
                                        nump[j] ->Add( (TH1D*)f->Get(numNameN));
                                        denp[j] ->Add( (TH1D*)f->Get(denNameN));
                                }
                                else {
                                        nump[j] = (TH1D*)f2->Get(numName);
                                        denp[j] = (TH1D*)f2->Get(denName);
                                        nump[j] ->Add( (TH1D*)f2->Get(numNameN));
                                        denp[j] ->Add( (TH1D*)f2->Get(denNameN));

                                }
                        }
                        else {
                                if (j < 2) {
                                        nump[j] = (TH1D*)f->Get(numName);
                                        denp[j] = (TH1D*)f->Get(denName);
                                }
                                else {
                                        nump[j] = (TH1D*)f2->Get(numName);
                                        denp[j] = (TH1D*)f2->Get(denName);
                                }
                        }
                }
                else if (j < merge3+1){
                        if (strcmp(system,"PAP") == 0) {
                                if (j < 2){
                                        nump[merge2+1] ->Add( (TH1D*)f->Get(numName));
                                        denp[merge2+1] ->Add( (TH1D*)f->Get(denName));
                                        nump[merge2+1] ->Add( (TH1D*)f->Get(numNameN));
                                        denp[merge2+1] ->Add( (TH1D*)f->Get(denNameN));
                                }
                                else {
                                        nump[merge2+1] ->Add( (TH1D*)f2->Get(numName));
                                        denp[merge2+1] ->Add( (TH1D*)f2->Get(denName));
                                        nump[merge2+1] ->Add( (TH1D*)f2->Get(numNameN));
                                        denp[merge2+1] ->Add( (TH1D*)f2->Get(denNameN));
                                }
                        }
                        else {
                                if (j < 2) {
                                        nump[merge2+1] ->Add( (TH1D*)f->Get(numName));
                                        denp[merge2+1] ->Add( (TH1D*)f->Get(denName));
                                }
                                else {
                                        nump[merge2+1] ->Add( (TH1D*)f2->Get(numName));
                                        denp[merge2+1] ->Add( (TH1D*)f2->Get(denName));
                                }
                        }

                        if (j == merge3){
                                cout << "no of pairs in numerator " << nump[merge2+1]->GetEntries() << endl;
                                nump[merge2+1]->Rebin(rebin);
                                denp[merge2+1]->Rebin(rebin);

                                nump[merge2+1]->Write();
                                Double_t norm = calculateNormalizationFactor(nump[merge2+1], denp[merge2+1],0.3,0.4);
                                nump[merge2+1]->Divide(denp[merge2+1]);
                                nump[merge2+1]->Scale(norm);
                                nump[merge2+1]->SetName(Form("corrfun_c%d",merge2+1));
                                nump[merge2+1]->Write();
                                nump[merge2+1]->SetMarkerStyle(22);
                                nump[merge2+1]->SetMarkerColor(kBlue);
                                nump[merge2+1]->SetMarkerSize(1.3);
                                nump[merge2+1]->Draw("same");

                                if (strcmp(system,"PP") == 0){
                                        myLegend->AddEntry(nump[merge2+1],"pp 30-50%","p");
                                }
                                else if (strcmp(system,"APAP") == 0){
                                        myLegend->AddEntry(nump[merge2+1],"#bar{p}#bar{p} 30-50%","p");
                                }
                                else if (strcmp(system,"PAP") == 0){
                                        myLegend->AddEntry(nump[merge2+1],"p#bar{p} %s 30-50%","p");
                                }

//                                myLegend->AddEntry(nump[merge2+1],"pp 30-50%","p");
                                //    	myLegend->AddEntry(nump[merge2+1],Form("#bar{p}-#bar{p} %s", "30-70%"),"p");
                        }
                }
        }

        //}

        myLegend->Draw("same");
        postprocess(myCan,Form("cf%s",system),rWrite,rPerformance,system,field,kT);

}

void myLegendSetUp(TLegend *currentLegend=0,float currentTextSize=0.07)
{
        currentLegend->SetTextFont(42);
        currentLegend->SetBorderSize(0);
        currentLegend->SetFillStyle(0);
        currentLegend->SetFillColor(0);
        currentLegend->SetMargin(0.25);
        currentLegend->SetTextSize(currentTextSize);
        currentLegend->SetEntrySeparation(0.5);
        return;
}

void myPadSetUp(TPad *currentPad, float currentLeft=0.11, float currentTop=0.04, float currentRight=0.04, float currentBottom=0.15)
{
        currentPad->SetLeftMargin(currentLeft);
        currentPad->SetTopMargin(currentTop);
        currentPad->SetRightMargin(currentRight);
        currentPad->SetBottomMargin(currentBottom);
        return;
}

void myGraphSetUp(TGraphErrors *currentGraph=0, Float_t currentMarkerSize = 1.0,
                  int currentMarkerStyle=20, int currentMarkerColor=0,
                  int currentLineStyle=1, int currentLineColor=0)
{
        currentGraph->SetMarkerSize(currentMarkerSize);
        currentGraph->SetMarkerStyle(currentMarkerStyle);
        currentGraph->SetMarkerColor(currentMarkerColor);
        currentGraph->SetLineStyle(currentLineStyle);
        currentGraph->SetLineColor(currentLineColor);
        return;
}

void myOptions(Int_t lStat=0)
{
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
        gStyle->SetLabelSize(0.065,"xyz");
        gStyle->SetLabelFont(font,"xyz");
        gStyle->SetLabelOffset(0.01,"xyz");
        gStyle->SetTitleFont(font,"xyz");
        gStyle->SetTitleOffset(1.0,"xyz");
        gStyle->SetTitleSize(0.06,"xyz");
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
