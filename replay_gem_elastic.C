#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstring>

#include "TFile.h"
#include "TCanvas.h"

#include "read_config.h"
#include "hardcode.h"
#include "elastic_engine.h"

int replay_gem_elastic(const char* configfilename)
{
	ConfigFile configfile;
	int configfile_error = configfile.read_configfile(configfilename);

	if ( configfile_error == -1) // stop the program if the config file is incomplete 
	{
		std::cerr << "Program stopping: Error in configuration file.\n";
		return 1;
	}

	//Get the input/output paths and input/output file names
	TString input_dirpath = configfile.return_inputdir();
	TString output_dirpath = configfile.return_outputdir();
	TString input_filename = configfile.return_inputfilename();
	TString output_filename = configfile.return_outputfilename();

	//Open the root-file and copy the Tree
	TFile* inputrootfile = new TFile(Form("%s/%s", input_dirpath.Data(), input_filename.Data()), "OPEN");
        //Telling Form to concatenate two C-style strings with a slash (/) between them.	
	TTree* T = (TTree*)inputrootfile->Get("T");

        InitTree(T);

	Test(T);
//	//TCanvas *c1 = new TCanvas("c1","Canvas 1", 800, 600);
//        //in_T->Draw("sbs.gemFT.track.y:sbs.gemFT.track.x>>c1","sbs.gemFT.hit.layer==0","COLZ");
//	
//	Long64_t nentries = T->GetEntries();
//	const Int_t MAXHIT = 1000;
//
//	//double x, y;
//	//double nhits;
//	Double_t Module[MAXHIT];
//	Double_t xglobal[MAXHIT];
//	Double_t yglobal[MAXHIT];
//	Double_t zglobal[MAXHIT];
//
//	// Set addresses
//	//T->SetBranchAddress("sbs.gemFT.track.x", &x);
//	//T->SetBranchAddress("sbs.gemFT.track.y", &y);
//	//T->SetBranchAddress("sbs.gemFT.track.nhits", &nhits);
//	T->SetBranchAddress("sbs.gemFT.hit.module", &Module[0]);
//	T->SetBranchAddress("sbs.gemFT.hit.zglobal", &zglobal[0]);
//	T->SetBranchAddress("sbs.gemFT.hit.xglobal", &xglobal[0]);
//	T->SetBranchAddress("sbs.gemFT.hit.yglobal", &yglobal[0]);
//
//	// Loop
//	for (Long64_t i=0; i<nentries; ++i) {
//    		T->GetEntry(i);
//    		if (nhits == 8 && mod == 0) {
//        	std::cout << "Entry " << i << ": x = " << x << ", y = " << y << std::endl;
//    		}
//	}

	/*------------------------------------------------------------------------------------------------------------------------------*/

	FT_DB ft_db;
	int ft_db_error = ft_db.read_FT_DB(configfilename);

        if ( ft_db_error == -1) // stop the program if the config file is incomplete
        {
                std::cerr << "Program stopping: Error in FT DB file.\n";
                return 1;
        }

	//Get the input TF DB dir and filename
	TString ft_db_dir = ft_db.return_DB_dir();
	TString ft_db_filename = ft_db.return_FT_DB_filename();

	Init_FT_module_geometry(ft_db_dir, ft_db_filename);

	//Init_FT_module_2D_grid(Module, xglobal, yglobal, zglobal);

	
return 0;
}
