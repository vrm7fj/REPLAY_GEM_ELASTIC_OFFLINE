#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <array>

#include "TFile.h"
#include "TVector3.h"
#include "hardcode.h"

#ifndef ELASTIC_ENGINE_H
#define ELASTIC_ENGINE_H

//Structor to hold module parameters
struct module_para_struct {
	std::vector<TVector3> module_pos;	
	std::vector<TVector3> module_size;	
	std::vector<TVector3> module_angle;	
	std::vector<std::array<int, 1>> module_layer;	
};

std::map<TString, module_para_struct> Init_FT_module_geometry(const char* ft_db_dir, const char* ft_db_filename)
{       

	//map to hold module paramters and a key
	std::map<TString, module_para_struct> FT_module_para;

	Int_t GEM_FT_NMODULE = 0; //initalize the number of GEM modules - will be read from DB
	/*---------------------------------------------------------------------------------------------------------*/
        	
	//ifstream the dat file
 	TString s_ft_db_infile = Form("%s/%s", ft_db_dir, ft_db_filename); //Dont need .Data() - already const char*
	ifstream ft_db_infile(s_ft_db_infile);
	TString currentline;

	if (!ft_db_infile.is_open()) {
		std::cerr << "Error: Could not open db_sbs.gemFT.dat!!" << std::endl;
	}

	//Lets write something to read number of modules from the DB
	while ( currentline.ReadLine( ft_db_infile ))
	{
		if (!currentline.BeginsWith("#"))
		{
                	TString sbs_gemFT_modules = "sbs.gemFT.modules";	
			TObjArray *ft_tokens = currentline.Tokenize(" ");
		    	int nft_tokens = ft_tokens->GetEntries();
			//std::cout << "ft_tokens: " << nft_tokens << std::endl;
			TString sval_0 = ( (TObjString*)(*ft_tokens)[0] )->GetString();

				if ( sval_0 == sbs_gemFT_modules) {
                                	GEM_FT_NMODULE = nft_tokens - 2; // first word followed by a "=" sign
				}
		delete ft_tokens; 
		}
	}

	ft_db_infile.clear(); 
	ft_db_infile.seekg(0, std::ios::beg); //reset currentline to zero!

	//std::cout << "N_MODULES: " << GEM_FT_NMODULE << std::endl;
        
	//Define an array of module wise parameters for the front tracker in the format sbs.gemFT.mX.YYYYY
        std::vector<TString> gemFT_module_size;
        std::vector<TString> gemFT_module_position;
        std::vector<TString> gemFT_module_angle;
        std::vector<TString> gemFT_module_layer;

        for (Int_t module_ = 0; module_ < GEM_FT_NMODULE; module_++) {
        	gemFT_module_size.push_back(Form("sbs.gemFT.m%d.size", module_));
                gemFT_module_position.push_back(Form("sbs.gemFT.m%d.position", module_));
                gemFT_module_angle.push_back(Form("sbs.gemFT.m%d.angle", module_));
                gemFT_module_layer.push_back(Form("sbs.gemFT.m%d.layer", module_));
        }

	//Now we have to read the db file. 
	while ( currentline.ReadLine( ft_db_infile ))
	{
		TObjArray *ft_tokens = currentline.Tokenize(" ");
		Int_t nft_tokens = ft_tokens->GetEntries();
		
		if (!currentline.BeginsWith("#"))
		{

			TString sval_0 = ( (TObjString*)(*ft_tokens)[0] )->GetString();

                        for (Int_t module_ = 0; module_ < GEM_FT_NMODULE; module_ ++) {

				TString mod_key = Form("m%d", module_);

				if ( sval_0 == gemFT_module_position[module_]) {

					double val_1 = ( (TObjString*)(*ft_tokens)[2] )->GetString().Atof(); //There is a reason for repeating this
					double val_2 = ( (TObjString*)(*ft_tokens)[3] )->GetString().Atof();
					double val_3 = ( (TObjString*)(*ft_tokens)[4] )->GetString().Atof();
                                        FT_module_para[mod_key].module_pos.emplace_back(val_1, val_2, val_3);

				} else if ( sval_0 == gemFT_module_size[module_]) {
	
					double val_1 = ( (TObjString*)(*ft_tokens)[2] )->GetString().Atof();
					double val_2 = ( (TObjString*)(*ft_tokens)[3] )->GetString().Atof();
					double val_3 = ( (TObjString*)(*ft_tokens)[4] )->GetString().Atof();
                                        FT_module_para[mod_key].module_size.emplace_back(val_1, val_2, val_3);

				} else if ( sval_0 == gemFT_module_angle[module_]) {
	
					double val_1 = ( (TObjString*)(*ft_tokens)[2] )->GetString().Atof();
					double val_2 = ( (TObjString*)(*ft_tokens)[3] )->GetString().Atof();
					double val_3 = ( (TObjString*)(*ft_tokens)[4] )->GetString().Atof();
                                        FT_module_para[mod_key].module_angle.emplace_back(val_1, val_2, val_3);

				} else if ( sval_0 == gemFT_module_layer[module_]) {
					double val_1 = ( (TObjString*)(*ft_tokens)[2] )->GetString().Atof();
                                        FT_module_para[mod_key].module_layer.push_back({static_cast<int>(val_1)});

				}
			}

	 	delete ft_tokens;
		}
	}

	ft_db_infile.close();

	std::cout << "\n----------FT Module Parameters----------\n";

	for (const auto& entry : FT_module_para) {
	
		const TString& module_key = entry.first;
		const module_para_struct& module_para = entry.second;

		std::cout << module_key << " | ";
		// Layer
		std::cout << "Layer: ";
		for (const auto& layer : module_para.module_layer) {
		    std::cout << "(" << layer[0] << ") ";
		}

		// Positions
		std::cout << "| Positions: ";
		for (const auto& pos : module_para.module_pos) {
		    std::cout << "(" << pos.X() << "," << pos.Y() << "," << pos.Z() << ") ";
		}
		
		// Dimensions
		std::cout << "| Dimensions: ";
		for (const auto& size : module_para.module_size) {
		    std::cout << "(" << size.X() << "," << size.Y() << "," << size.Z() << ") ";
		}
		
		// Angles
		std::cout << "| Angles: ";
		for (const auto& angle : module_para.module_angle) {
		    std::cout << "(" << angle.X() << "," << angle.Y() << "," << angle.Z() << ") ";
		}

       	std::cout << "\n";
    	}

return FT_module_para;	
}

/*-------------------------------------------------------------------------------------------------------------------------*/

//Structor to hold module 2D grid parameters
//struct module_2D_grid_para {
//	std::vector<std::array<double, 3>> XGrid;	
//	std::vector<std::array<double, 3>> YGrid;	
//	std::vector<std::array<double, 3>> ZGrid;	
//	std::vector<std::array<int, 1>> m_layer;	
//};
//
//std::map<TString, module_2D_grid_para> Init_FT_module_2D_grid(Double_t hmod, Double_t hxglobal, Double_t hyglobal, Double_t hzglobal)
//{
//
//        //map to hold module paramters and a key
//        std::map<TString, module_2D_grid_para> FT_module_2D_Grid_para;
//
//
//	return FT_module_2D_grid_para;
//}

/*--------------------------------------------------------------------------------------------------------------------------*/
// Inialize Root Tree here

Double_t 	MODULE[MAXHIT];
Double_t 	XGLOBAL[MAXHIT];
Double_t	YGLOBAL[MAXHIT];
Double_t 	ZGLOBAL[MAXHIT];

Long64_t nentries = 0;

void InitTree(TTree* T)
{
	nentries = T->GetEntries();	

	T->SetBranchAddress("sbs.gemFT.hit.module", &MODULE);
	T->SetBranchAddress("sbs.gemFT.hit.xglobal", &XGLOBAL);
	T->SetBranchAddress("sbs.gemFT.hit.yglobal", &YGLOBAL);
	T->SetBranchAddress("sbs.gemFT.hit.zglobal", &ZGLOBAL);
}

/*--------------------------------------------------------------------------------------------------------------------------*/
// Test Ground

void Test(TTree* T)
{ 
	InitTree(T);

	for(Long64_t i = 0; i < nentries; ++i)
	{
		T->GetEntry(i);
		if (MODULE[i] == 0){
			std::cout << "Entry: " << i << "	xglobal: " << XGLOBAL[i] << std::endl;
		}
	}
}



#endif
