#ifndef VPD_TRIGGER_SLEW_PICO_DST_MAKER_H
#define VPD_TRIGGER_SLEW_PICO_DST_MAKER_H

// STL
#include <memory>

// ROOT
#include "TTree.h"
#include "TFile.h"


#include "TreeAnalyzer.h"
using namespace jdb;

#include "VpdOfflineSlewPicoDstData.h"
#include "MuDst.h"

#include "HistoBins.h"


class VpdOfflineSlewPicoDstMaker : public TreeAnalyzer
{
protected:
	
	shared_ptr<DataAdapter> pico;
	VpdOfflineSlewPicoDstData data;

	static const int nChannels = 38;
	static const int startEast = 19;

	TTree * tree = nullptr;
	TFile * outFile = nullptr;

	vector<double> totBins[nChannels];
	vector<double> corrections[nChannels];

	vector<HistoBins> bins;

	bool wantVertex = false;
	vector<int> channelMask;





public:
	virtual const char* classname() const { return "VpdOfflineSlewPicoDstMaker"; }

	VpdOfflineSlewPicoDstMaker() {

	}
	~VpdOfflineSlewPicoDstMaker() {

	}

	virtual void initialize(){
	
		if ( "MuDst" == config.getString( nodePath + ".input.dst:treeName" ) ){
			pico = shared_ptr<MuDst>( new MuDst( chain ) );

			chain->SetBranchStatus( "*", 0 );
			chain->SetBranchStatus( "MuEvent.mEventSummary.mPrimaryVertexPos*", 1 );
			chain->SetBranchStatus( "BTofHit.*", 1 );
			
			wantVertex = true;
		}
		else {
			ERROR( classname(), "COULD not make a DataAdapter" );
		}

		DEBUG( classname(), "Data Adapter created" );

		string filename = config[ nodePath + ".output.tree" ];
		outFile = new TFile(filename.c_str(), "RECREATE");
		bookTree();

		if ( config.exists( nodePath + ".Slewing" ) ){
			INFO( classname(), "READING params" );
			readParameters();
		}

		channelMask = config.getIntVector( nodePath + ".ChannelMask", 1, 16 );
		
	}

	virtual void postMake(){
		DEBUG( classname(), "Saveing Output" );
		outFile->Write();
		outFile->Close();
	}


	virtual bool keepEvent( ){
		return true;
	}

	virtual void analyzeEvent() {
		
		// DEBUG( classname(), "" );
		outFile->cd();


		data.run = pico->runNumber();
		data.evt = pico->eventNumber(); 

		data.vertexX = pico->vX();
		data.vertexY = pico->vY();
		data.vertexZ = pico->vZ();


		if ( wantVertex && data.vertexZ == 0.0 ) return;

		fillVpdTrigger();

		tree->Fill();

	}


	

	void fillVpdTrigger();
	double correct( double le, double tot, int ch );


	void bookTree();
	void readParameters();

};




#endif