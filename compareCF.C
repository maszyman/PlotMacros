// makro do rysowania funkcji korelacyjnych w binach multiplicity
// mps 10/08/2011

static  int      myDarkRed     = TColor::GetColor(128,0,0);
static  int      myDarkGreen   = TColor::GetColor(0,128,0);
static  int      myDarkBlue    = TColor::GetColor(0,0,128);

//const char* figureInfo = "Pb-Pb events at #sqrt{s_{NN}} = 2.76 TeV";
const char* figureInfo = "Pb-Pb #sqrt{s_{NN}} = 2.76 TeV";
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
		// alice2->Draw();

		// if (strcmp(system,"PAP") == 0)
        TText *date = new TText(0.77,0.45,cStamp2);
		// else
		// 	TText *date = new TText(0.78,0.35,cStamp2);

        date->SetNDC();
        date->SetTextFont(42);
        date->SetTextSize(0.04);
        // date->Draw();

        //Acquire canvas proportions
        Double_t AliLogo_LowX = 0.77;

//		if (strcmp(system,"PAP") == 0)
        Double_t AliLogo_LowY = 0.5;
//		else
//			Double_t AliLogo_LowY = 0.4;

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

    c2->SaveAs(Form("cf_%s_%s.png",system,field));
    c2->SaveAs(Form("cf_%s_%s.eps",system,field));

    if (rWrite == 1)
        c2->SaveAs(Form("%s.png",name));

    if (rWrite == 2)
        c2->SaveAs(Form("%s.eps",name));

}

// kT
// 0 - all
// 1 - 0.1-0.3
// 2 - 0.3-0.6
// 2 - 0.6-1.0
// 3 - 1.0->

void compareCF (int kT = 0, const char* system, int cent)
{

    myOptions();
    gROOT->ForceStyle();
    setDate();

    // TFile *ff1 = new TFile(Form("cf_%s_f1_kT%d.root",system,kT), "read");
    // TFile *ff3 = new TFile(Form("cf_%s_f3_kT%d.root",system,kT), "read");

    TFile *ff1 = new TFile(Form("cf_%s_f1_kT%d_old.root",system,kT), "read");
    TFile *ff3 = new TFile(Form("cf_%s_f1_kT%d_new.root",system,kT), "read");

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

    TLegend *myLegend = new TLegend (0.4,0.6,0.7,0.75);
    myLegendSetUp(myLegend,0.04);

    TH1D* cff1;
    TH1D* cff3;

    // for (int j = 0; j < 5; j += 2) {

        cff1 = (TH1D*)ff1->Get(Form("corrfun_c%d",cent));
        cff3 = (TH1D*)ff3->Get(Form("corrfun_c%d",cent));

        // if (j == 0) {

            cff1->SetMarkerStyle(20);
            cff1->SetMarkerSize(1.3);
            cff1->SetMarkerColor(kBlack);

            cff3->SetMarkerStyle(24);
            cff3->SetMarkerSize(1.3);
            cff3->SetMarkerColor(kBlack);

            cff1->Draw();
            cff3->Draw("same");

    //     }
    //     else if (j == 2){

    //         cff1->SetMarkerStyle(21);
    //         cff1->SetMarkerSize(1.3);
    //         cff1->SetMarkerColor(kRed+2);

    //         cff3->SetMarkerStyle(25);
    //         cff3->SetMarkerSize(1.3);
    //         cff3->SetMarkerColor(kRed+2);

    //         cff1->Draw("same");
    //         cff3->Draw("same");


    //     }

    // }




    // myLegend->Draw("same");
    // postprocess(myCan,Form("cf%s",system),rWrite,rPerformance,system,field,kT);

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

