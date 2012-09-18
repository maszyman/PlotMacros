// makro do rysowania monitorow PID
// mps 9/08/2011
// update 18/09/2012

static  int      myDarkRed     = TColor::GetColor(128,0,0);
static  int      myDarkGreen   = TColor::GetColor(0,128,0);
static  int      myDarkBlue    = TColor::GetColor(0,0,128);

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


// infilename - root file with relevant histograms
// system - PP,APAP,PP
// status - Pass,Fail
// rWrite - 0-no,1-png,2-eps
// rPerformance - 0-no,1-yes (ALICE logo etc.)
// bin: 0 - all, 1- 0:5, 2- 5:10, etc
void drawDCA(const char* infilename, const char* system, const char* status, Int_t rWrite, Int_t rPerformance, int isMC, Int_t bin, Int_t ptrange)
{

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

    TFile *f = new TFile(infilename, "read");

    // DCA xy
    TH2D* DCAxy =(TH2D*)f->Get(Form("DCARPtcut%s1%stpcM%d","Pass", system,0));


    if (!bin) {
        int minMultBin = 0;
        int maxMultBin = 6; // 8
    }
    else {
        int minMultBin = bin-1;
        int maxMultBin = bin; // 8
    }

    // int minMultBin = 0;
    // int maxMultBin = 0; // 8

    double EvMultall = 0;

    for(int i = minMultBin; i<maxMultBin; i++) {

        TH2D* DCAxyN = (TH2D*)f->Get(Form("DCARPtcut%s1%stpcM%d",status, system,i));
        DCAxy->Add(DCAxyN);

        cout<<i<<" "<<DCAxyN->GetEntries()<<endl;

        //delete hEvMult;
    }


    if (!isMC) {
        TCanvas *c2 = new TCanvas("DCA xy prim", "DCA xy prim");
        c2->SetGridx();
        c2->SetGridy();
        c2->SetFillColor(10);
        c2->SetRightMargin(1.9);
        c2->SetLogz();

        DCAxy->GetXaxis()->SetTitle("DCA_{XY} (cm)");
        DCAxy->GetXaxis()->SetRangeUser(-5.0,5.0);
        DCAxy->GetYaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
        // DCAxy->GetZaxis()->SetLabelSize(0.05);
        DCAxy->Draw("colz");
        postprocess(c2,Form("DCAxy%s",status),rWrite,rPerformance,system);

        // TCanvas *c4 = new TCanvas("DCA xy Projection X", "DCA xy Projection X");
        // c4->SetGridx();
        // c4->SetGridy();
        // c4->SetFillColor(10);
        // c4->SetRightMargin(1.9);
        // c4->SetLogy();

        gStyle->SetOptTitle(1);
        TCanvas *myCan = new TCanvas("myCan",cStamp1,600,400);
        myCan->Draw();
        myCan->cd();

        TPad *myPad = new TPad("myPad", "The pad",0,0,1,1);
        myPadSetUp(myPad,0.15,0.04,0.04,0.15);
        myPad->Draw();
        myPad->SetLogy();

        myPad->cd();

        if (ptrange == 0) {
            TH1D* pripp = (TH1D*)DCAxy->ProjectionX("zxc1",1,100);
            pripp->SetTitle("0.5 < #it{p}_{T} < 3 GeV/#it{c}");
        }
        else if (ptrange == 1) {
            TH1D* pripp = (TH1D*)DCAxy->ProjectionX("zxc1",15,33);
            pripp->SetTitle("0.5 < #it{p}_{T} < 1 GeV/#it{c}");
        }
        else if (ptrange == 2) {
            TH1D* pripp = (TH1D*)DCAxy->ProjectionX("zxc1",33,100);
            pripp->SetTitle("1 < #it{p}_{T} < 3 GeV/#it{c}");
        }

        pripp->SetYTitle("Number of Entries (normalized)");
        pripp->GetXaxis()->SetTitleSize(0.068);
        pripp->GetYaxis()->SetTitleSize(0.068);
        pripp->GetXaxis()->SetLabelSize(0.058);
        pripp->GetYaxis()->SetLabelSize(0.058);

        // pripp->SetLabelSize(0.05);
        // DCAxy->ProjectionX("asd",50,100)->SetYTitle("Number of Entries");
        // DCAxy->ProjectionX("asd",50,100)->SetTitle("1.0 < p_{T} < 2.0 GeV");
        // DCAxy->ProjectionX("asd",0,200)->SetTitle("");
        // DCAxy->ProjectionX("asd",50,100)->GetXaxis()->SetNdivisions(8);
        // DCAxy->ProjectionX("asd",50,100)->GetYaxis()->SetNdivisions(8);
        // DCAxy->ProjectionX("asd",50,100)->GetXaxis()->SetTitleSize(0.05);
        // DCAxy->ProjectionX("asd",50,100)->GetYaxis()->SetTitleSize(0.05);
        // DCAxy->ProjectionX("asd",50,100)->GetXaxis()->SetLabelSize(0.05);
        // DCAxy->ProjectionX("asd",50,100)->GetYaxis()->SetLabelSize(0.05);

        pripp->Draw("");

        //if (!isMC) {
        pripp->Scale(1./pripp->Integral());
        TFile* fout = new TFile("dca.root","update");
        pripp->SetName(Form("dcaxyMC%d",isMC));
        pripp->Write();
        //}

        postprocess(myCan,Form("DCAxy%sProX",status),rWrite,rPerformance,system);

    }
    else if (isMC) {

        TH2D* primp =(TH2D*)f->Get(Form("DCARPtcut%s1%stpcM%dprim","Pass", system,0));
        TH2D* weakp =(TH2D*)f->Get(Form("DCARPtcut%s1%stpcM%dweak","Pass", system,0));
        TH2D* matp =(TH2D*)f->Get(Form("DCARPtcut%s1%stpcM%dmat","Pass", system,0));

        // prim 2D
        TCanvas *c3prim = new TCanvas("DCA xy primary", "DCA xy primary");
        c3prim->SetGridx();
        c3prim->SetGridy();
        c3prim->SetFillColor(10);
        c3prim->SetRightMargin(1.9);
        c3prim->SetLogz();

        primp->GetXaxis()->SetTitle("DCA_{XY} (cm)");
        primp->GetXaxis()->SetRangeUser(-5.0,5.0);
        primp->GetYaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
        primp->GetZaxis()->SetLabelSize(0.03);
        primp->Draw("colz");
        postprocess(c3prim,Form("DCAxy%s",status),rWrite,rPerformance,system);

        // weak 2D
        TCanvas *c3 = new TCanvas("DCA xy weak", "DCA xy weak");
        c3->SetGridx();
        c3->SetGridy();
        c3->SetFillColor(10);
        c3->SetRightMargin(1.9);
        c3->SetLogz();

        weakp->GetXaxis()->SetTitle("DCA_{XY} (cm)");
        weakp->GetXaxis()->SetRangeUser(-5.0,5.0);
        weakp->GetYaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
        weakp->GetZaxis()->SetLabelSize(0.03);
        weakp->Draw("colz");
        postprocess(c3,Form("DCAxy%s",status),rWrite,rPerformance,system);

        // mat 2D
        TCanvas *c4 = new TCanvas("DCA xy mat", "DCA xy mat");
        c4->SetGridx();
        c4->SetGridy();
        c4->SetFillColor(10);
        c4->SetRightMargin(1.9);
        c4->SetLogz();

        matp->GetXaxis()->SetTitle("DCA_{XY} (cm)");
        matp->GetXaxis()->SetRangeUser(-5.0,5.0);
        matp->GetYaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
        matp->GetZaxis()->SetLabelSize(0.03);
        matp->Draw("colz");
        postprocess(c4,Form("DCAxy%s",status),rWrite,rPerformance,system);


        // prim proj
        gStyle->SetOptTitle(1);

        TCanvas *myCan3prim = new TCanvas("myCan3prim",cStamp1);
        myCan3prim->Draw();
        myCan3prim->cd();

        TPad *myPad3prim = new TPad("myPad3prim", "The pad3prim",0,0,1,1);
        myPadSetUp(myPad3prim,0.15,0.04,0.04,0.15);
        myPad3prim->Draw();
        myPad3prim->SetLogy();

        myPad3prim->cd();

        TH1D* primpp = (TH1D*)primp->ProjectionX("zxc22",0,100);
        primpp->Draw("");

        cout << primpp->FindBin(-0.1) << endl;
        cout << primpp->FindBin(0.1) << endl;

        cout << "primary in cut- " << primpp->Integral(191,211) << endl;
        cout << "primary all- " << primpp->Integral(1,400) << endl;

        postprocess(myCan3prim,Form("DCAxy%sProX",status),rWrite,rPerformance,system);


        // weak proj
        TCanvas *myCan3 = new TCanvas("myCan3",cStamp1);
        myCan3->Draw();
        myCan3->cd();

        TPad *myPad3 = new TPad("myPad3", "The pad3",0,0,1,1);
        myPadSetUp(myPad3,0.15,0.04,0.04,0.15);
        myPad3->Draw();
        myPad3->SetLogy();

        myPad3->cd();

        TH1D* weakpp = (TH1D*)weakp->ProjectionX("zxc2",0,100);
        weakpp->Draw("");

        cout << "weak in cut- " << weakpp->Integral(191,211) << endl;
        cout << "weak all- " << weakpp->Integral(1,400) << endl;

        postprocess(myCan3,Form("DCAxy%sProX",status),rWrite,rPerformance,system);

        // mat proj
        gStyle->SetOptTitle(1);
        TCanvas *myCan4 = new TCanvas("myCan4",cStamp1);
        myCan4->Draw();
        myCan4->cd();

        TPad *myPad4 = new TPad("myPad4", "The pad4",0,0,1,1);
        myPadSetUp(myPad4,0.15,0.04,0.04,0.15);
        myPad4->Draw();
        myPad4->SetLogy();

        myPad4->cd();

        TH1D* matpp = (TH1D*)matp->ProjectionX("zxc3",0,100);
        matpp->Draw("");

        cout << "material in cut- " << matpp->Integral(191,211) << endl;
        cout << "material all- " << matpp->Integral(1,400) << endl;

        postprocess(myCan4,Form("DCAxy%sProX",status),rWrite,rPerformance,system);

        cout  << "in cut: " << endl;
        cout << "prim - " << primpp->Integral(191,211) / (primpp->Integral(191,211)+weakpp->Integral(191,211)+matpp->Integral(191,211)) << endl;
        cout << "weak - " << weakpp->Integral(191,211) / (primpp->Integral(191,211)+weakpp->Integral(191,211)+matpp->Integral(191,211)) << endl;
        cout << "mat - " << matpp->Integral(191,211) / (primpp->Integral(191,211)+weakpp->Integral(191,211)+matpp->Integral(191,211)) << endl;

        cout << endl << "in cut / all " << endl;
        cout << "prim - " << primpp->Integral(191,211) / primpp->Integral(1,400) << endl;
        cout << "weak - " << weakpp->Integral(191,211) / weakpp->Integral(1,400) << endl;
        cout << "mat - " << matpp->Integral(191,211) / matpp->Integral(1,400) << endl;


        // _____sum____

        TH2D* psum = new TH2D("psum","",400, -2.0, 2.0, 100,0.0,2.0);
        psum->GetXaxis()->SetTitle("DCA_{xy} (cm)");
        psum->GetYaxis()->SetTitle("Number of Entries (normalized)");
        psum->GetXaxis()->SetLimits(-2,2);


        for (int i = 0; i < primp->GetNbinsX(); i++) {
            for (int j = 0; j < primp->GetNbinsY(); j++) {
                psum->SetBinContent(i,j,primp->GetBinContent(i,j)+weakp->GetBinContent(i,j)+matp->GetBinContent(i,j));
            }
        }

        gStyle->SetOptStat(0);

        TCanvas *cansum = new TCanvas("cansum",cStamp1,600,400);
        cansum->Draw();
        cansum->cd();

        TPad *padsum = new TPad("padsum", "The pad4",0,0,1,1);
        myPadSetUp(padsum,0.15,0.04,0.04,0.15);
        padsum->Draw();
        padsum->SetLogy();

        padsum->cd();

        // TCanvas* cansum = new TCanvas("cansum","cansum");
        // cansum->SetLogy();
        //psum->Draw("colz");
        TH1D* asd0 = (TH1D*)psum->ProjectionX("zxc",0,100);

        // asd0->Scale(1./asd0->Integral());
        // TFile* fout = new TFile("dca.root","update");
        // asd0->SetName(Form("dcaxyMC%d",isMC));
        // asd0->Write();

        asd0->GetXaxis()->SetTitle("DCA_{xy} (cm)");
        asd0->GetYaxis()->SetTitle("Number of Entries (normalized)");
        //asd0->SetMaximum(5000);
        //asd0->SetMinimum(0.00008);
        asd0->GetXaxis()->SetNdivisions(8);
        asd0->GetYaxis()->SetNdivisions(8);
        //asd0->GetYaxis()->SetTitleOffset(1.4);
        asd0->GetXaxis()->SetTitleSize(0.068);
        asd0->GetYaxis()->SetTitleSize(0.068);
        asd0->GetXaxis()->SetLabelSize(0.058);
        asd0->GetYaxis()->SetLabelSize(0.058);
        asd0->SetFillColor(kBlack);

        Double_t norm = asd0->Integral();
        //asd0->Scale(1./norm);
        //asd0->SetMinimum(0.00007);

        asd0->SetMarkerSize(1.3);
        asd0->SetMarkerColor(kBlack);
        asd0->SetMarkerStyle(20);
        asd0->Draw("pc");

        primpp->SetFillColor(kGreen+2);
        //primpp->Scale(1./norm);
        primpp->SetMarkerSize(1.3);
        primpp->SetMarkerColor(kGreen+2);
        primpp->SetMarkerStyle(20);
        primpp->Draw("psame");

        matpp->SetFillColor(kRed);
        //matpp->Scale(1./norm);
        matpp->SetMarkerSize(1.3);
        matpp->SetMarkerColor(kRed);
        matpp->SetMarkerStyle(20);
        matpp->Draw("psame");

        weakpp->SetFillColor(kBlue);
        //weakpp->Scale(1./norm);
        weakpp->SetMarkerSize(1.3);
        weakpp->SetMarkerColor(kBlue);
        weakpp->SetMarkerStyle(20);
        weakpp->Draw("psame");



        // _____endofsum____


        TLegend *myLegend = new TLegend(0.6,0.6,0.89,0.89);
        myLegend->SetFillColor(10);
        myLegend->SetBorderSize(0);

        myLegend->AddEntry(asd0,"all","f");
        myLegend->AddEntry(primpp,"primary","f");
        myLegend->AddEntry(weakpp,"weak decay","f");
        myLegend->AddEntry(matpp,"material","f");
        //myLegend->Draw("same");


        // logo

        TLatex *sys = new TLatex(0.16,0.91,"AMPT Pb-Pb #sqrt{s_{NN}} = 2.76 TeV");
        sys->SetNDC();
        sys->SetTextFont(42);
        sys->SetTextSize(0.05);
        sys->SetTextColor(kRed+2);
        sys->Draw();

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


        TText *date = new TText(0.27,0.5,cStamp2);
        date->SetNDC();
        date->SetTextFont(42);
        date->SetTextSize(0.04);
        date->Draw();

        //           //Acquire canvas proportions
//                 Double_t AliLogo_LowX = 0.27;
//                 Double_t AliLogo_LowY = 0.6;
//                 Double_t AliLogo_Height = 0.22;
//                 //ALICE logo is a png file that is 821x798 pixels->should be wider than a square
//                 Double_t AliLogo_Width  = (821./798.) * AliLogo_Height * gPad->GetWh() / gPad->GetWw();

//                 TPad *myPadLogo = new TPad("myPadLogo", "Pad for ALICE Logo",AliLogo_LowX,AliLogo_LowY,AliLogo_LowX+AliLogo_Width,AliLogo_LowY+AliLogo_Height);
//                 //    myPadLogo->SetFillColor(2); // color to first figure out where is the pad then comment !
//                 myPadSetUp(myPadLogo,0,0,0,0);
//                 myPadLogo->SetFixedAspectRatio(1);
//                 myPadLogo->Draw();
//                 myPadLogo->cd();

// //                TASImage *myAliceLogo = new TASImage("alice_preliminary.eps");
//                 TASImage *myAliceLogo = new TASImage("alice_performance.eps");
// //		TASImage *myAliceLogo = new TASImage("alice_logo_transparent.png");
//                 myAliceLogo->Draw();

        DrawALICELogo(0,0.27,0.55,0.7,0.8);

//logo

        // postprocess(cansum,Form("DCAxyMC%s",status),rWrite,rPerformance,system);
        cansum->SaveAs("DCAxyMC.png");
        cansum->SaveAs("DCAxyMC.eps");
    }

//__________________________________________________


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

void myPadSetUp(TPad *currentPad, float currentLeft=0.11, float currentTop=0.04, float currentRight=0.04, float currentBottom=0.25){
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
    gStyle->SetLabelSize(0.07,"xyz");
    gStyle->SetLabelFont(font,"xyz");
    gStyle->SetLabelOffset(0.01,"xyz");
    gStyle->SetTitleFont(font,"xyz");
    gStyle->SetTitleOffset(1.0,"xyz");
    gStyle->SetTitleSize(0.08,"xyz");
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
        TLatex *alice = new TLatex(0.22,0.57,"Pb-Pb #sqrt{s_{NN}} = 2.76 TeV");
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
        // alice2->Draw();

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


        TText *date = new TText(0.67,0.48,cStamp2);
        date->SetNDC();
        date->SetTextFont(42);
        date->SetTextSize(0.04);
        date->Draw();

//                 //Acquire canvas proportions
//                 Double_t AliLogo_LowX = 0.67;
//                 Double_t AliLogo_LowY = 0.53;
//                 Double_t AliLogo_Height = 0.22;
//                 //ALICE logo is a png file that is 821x798 pixels->should be wider than a square
//                 Double_t AliLogo_Width  = (821./798.) * AliLogo_Height * gPad->GetWh() / gPad->GetWw();

//                 TPad *myPadLogo = new TPad("myPadLogo", "Pad for ALICE Logo",AliLogo_LowX,AliLogo_LowY,AliLogo_LowX+AliLogo_Width,AliLogo_LowY+AliLogo_Height);
//                 //    myPadLogo->SetFillColor(2); // color to first figure out where is the pad then comment !
//                 myPadSetUp(myPadLogo,0,0,0,0);
//                 //myPadLogo->SetFixedAspectRatio(1);
//                 myPadLogo->Draw();
//                 myPadLogo->cd();
// //                TASImage *myAliceLogo = new TASImage("alice_performance.png");
//                 TASImage *myAliceLogo = new TASImage("alice_performance.eps");
// //    TASImage *myAliceLogo = new TASImage("alice_logo_transparent.png");
//                 myAliceLogo->Draw();

        DrawALICELogo(0,0.65,0.52,0.8,0.8);


    }

    if (rWrite == 1){
        c2->SaveAs(Form("%s_%s.png",name,system));
        //}
        //if (rWrite == 2)
        c2->SaveAs(Form("%s.eps",name));
    }
}
