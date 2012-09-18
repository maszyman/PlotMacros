// makro do rysowania rozkladu multiplicity
// mps 9/08/2011
// update 18/09/2012

static  int      myDarkRed     = TColor::GetColor(128,0,0);
static  int      myDarkGreen   = TColor::GetColor(0,128,0);
static  int      myDarkBlue    = TColor::GetColor(0,0,128);

drawMultiplicity(const char* infilename, const char* system, Int_t rWrite, Int_t rPerformance) {

        prepareAll();

        TFile *f = new TFile(infilename, "read");

        int minMultBin = 0; // 0, 2, 3
        int maxMultBin = 6; // 2, 6, 8

        double EvMultall = 0;

        for (int i = minMultBin; i < maxMultBin; i++) {

                if (i == minMultBin)
                        TH1D* hEvMult =(TH1D*)f->Get(Form("EvMultcutPass%stpcM%i",system,i));
                else
                        hEvMult->Add((TH1D*)f->Get(Form("EvMultcutPass%stpcM%i",system,i)));

                EvMultall += ((TH1D*)f->Get(Form("EvMultcutPass%stpcM%i",system,i)))->GetEntries();
                cout << "MultBin " << i << " " << ((TH1D*)f->Get(Form("EvMultcutPass%stpcM%i",system,i)))->GetEntries() << endl;
                //delete hEvMult;
        }

        cout << "Number of events (all bins): " << EvMultall << endl;

        TCanvas *c2 = new TCanvas("mult", "Event Multiplicity");
        c2->SetGridx();
        c2->SetGridy();
        c2->SetFillColor(10);
        c2->SetLogy();

        hEvMult->GetXaxis()->SetTitle("multiplicity");
        hEvMult->GetYaxis()->SetTitle("number of events");
        hEvMult->GetXaxis()->SetRangeUser(0,4000);
        hEvMult->Draw("");

        if (rPerformance){
                TLatex *alice = new TLatex(0.35,0.27,"Performance");
                alice->SetNDC();
                alice->SetTextColor(myDarkRed);
                alice->SetTextFont(42);
                alice->SetTextSize(0.05);
                alice->SetLineWidth(2);
                alice->Draw();

                TLatex *alice2 = new TLatex(0.32,0.21,"LHC10h - Pass2");
                alice2->SetNDC();
                alice2->SetTextColor(myDarkRed);
                alice->SetTextFont(42);
                alice2->SetTextSize(0.05);
                alice2->SetLineWidth(2);
                alice2->Draw();

                TText *date = new TText(0.38,0.15,cStamp2);
                date->SetNDC();
                date->SetTextFont(42);
                date->SetTextSize(0.04);
                date->Draw();

                //Acquire canvas proportions
                Double_t AliLogo_LowX = 0.37;
                Double_t AliLogo_LowY = 0.33;
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

        if (rWrite == 1)
                c2->SaveAs(Form("multiplicity%s.png",system));

        if (rWrite == 2)
                c2->SaveAs(Form("multiplicity%s.eps",system));

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

