void mergeNonID ( const char* infilename, const char* ofilename, int c1, int c2) {

	TFile *f = new TFile(infilename, "read");

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

	Int_t rbPAP = 2;

	for (int j = c1; j < c2; ++j) {		// cent

		cout<<j<<endl;

		sprintf(numNamePout,"NumOutPckstarPAPtpcM%d",j-1);
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


        if (j > c1) {
            nump[c1-1][0] ->Add ( nump[j-1][0] ) ;
            numn[c1-1][0] ->Add ( numn[j-1][0] ) ;
            denp[c1-1][0] ->Add ( denp[j-1][0] ) ;
            denn[c1-1][0] ->Add ( denn[j-1][0] ) ;

            nump[c1-1][1] ->Add ( nump[j-1][0] ) ;
            numn[c1-1][1] ->Add ( numn[j-1][0] ) ;
            denp[c1-1][1] ->Add ( denp[j-1][0] ) ;
            denn[c1-1][1] ->Add ( denn[j-1][0] ) ;

            nump[c1-1][2] ->Add ( nump[j-1][0] ) ;
            numn[c1-1][2] ->Add ( numn[j-1][0] ) ;
            denp[c1-1][2] ->Add ( denp[j-1][0] ) ;
            denn[c1-1][2] ->Add ( denn[j-1][0] ) ;

        }

    }


    TFile* outfile = new TFile(Form("%s.root",ofilename),"recreate");
    nump[c1-1][0] ->Write (  ) ;
    numn[c1-1][0] ->Write (  ) ;
    denp[c1-1][0] ->Write (  ) ;
    denn[c1-1][0] ->Write (  ) ;

    nump[c1-1][1] ->Write (  ) ;
    numn[c1-1][1] ->Write (  ) ;
    denp[c1-1][1] ->Write (  ) ;
    denn[c1-1][1] ->Write (  ) ;

    nump[c1-1][2] ->Write (  ) ;
    numn[c1-1][2] ->Write (  ) ;
    denp[c1-1][2] ->Write (  ) ;
    denn[c1-1][2] ->Write (  ) ;
}
