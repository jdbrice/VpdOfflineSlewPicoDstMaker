#include "VpdOfflineSlewPicoDstMaker.h"



void VpdOfflineSlewPicoDstMaker::fillVpdTrigger(){


	MuDst * mu = (MuDst*) pico.get();
	TRACE( classname( ), "# BTOF Hit = " << mu->BTofHit_ );

	int nToTry = mu->BTofHit_;
	if ( nToTry > 2000 )
		nToTry = 2000;

	for ( int iHit = 0; iHit < nToTry; iHit++ ){

		int tray = 0;
		tray = (int)mu->BTofHit_mTray[ iHit ];

		if ( tray <= 0 || tray > 122)
			continue;

		TRACE( classname(), "Tray = " << tray );
		
		double tot = mu->BTofHit_mTrailingEdgeTime[ iHit ] - mu->BTofHit_mLeadingEdgeTime[ iHit ];

		if ( tray == 121 ) { // WEST VPD
			data.nWest++;
			int tubeId = mu->BTofHit_mCell[ iHit ] - 1;
			DEBUG( classname(), "tubeId(W) = " << tubeId );

			data.leWest[ tubeId ] = correct( mu->BTofHit_mLeadingEdgeTime[ iHit ], tot, tubeId );
			data.totWest[ tubeId ] = tot;

		} else if ( tray == 122 ) { // EAST VPD
			data.nEast++;
			int tubeId = mu->BTofHit_mCell[ iHit ] - 1;
			DEBUG( classname(), "tubeId(E) = " << startEast + tubeId );
			
			data.leEast[ tubeId ] = correct( mu->BTofHit_mLeadingEdgeTime[ iHit ], tot, tubeId + startEast );
			data.totEast[ tubeId ] = tot;
		} 
	}


	double fastEast=100000, fastWest=1000000;
	int fastChEast, fastChWest;
	for ( int iCh = 0; iCh < 19; iCh ++ ){
		if ( data.leEast[ iCh ] < fastEast && data.leEast[iCh] > 100 && data.totEast[iCh] > 10 ){
			fastChEast = iCh;
			fastEast = data.leEast[ iCh ];
		}

		if ( data.leWest[ iCh ] < fastWest && data.leWest[iCh] > 100 && data.totWest[iCh] > 10 ){
			fastChWest = iCh;
			fastWest = data.leWest[ iCh ];
		}
	}


	data.fastChEast = fastChEast;
	data.fastChWest = fastChWest;

	data.fastLeEast = fastEast;
	data.fastLeWest = fastWest;

	data.fastTotEast = data.totEast[ fastChEast ];
	data.fastTotWest = data.totWest[ fastChWest ];

	// calculate the mean on each side
	double totalEast = 0, totalWest = 0;
	int nEast = 0, nWest = 0;
	for ( int iCh = 0; iCh < 19; iCh++ ){
		if ( 10 < data.totEast[ iCh ] && 2 < data.leEast[ iCh ] ){
			totalEast += data.leEast[ iCh ];
			nEast++;
		}
		if ( 10 < data.totWest[ iCh ] && 2 < data.leWest[ iCh ] ){
			totalWest += data.leWest[ iCh ];
			nWest++;
		}
	}

	data.avgLeWest = totalWest / (double)nWest;
	data.avgLeEast = totalEast / (double)nEast;


} // fillVpdTrigger





void VpdOfflineSlewPicoDstMaker::bookTree(){

	outFile->cd();
	tree = new TTree( "vpd", "VpdOfflineSlewPicoDst data" );
	tree->SetAutoSave( 1000 );
	
	tree->Branch("run",&data.run,"run/i");
	tree->Branch("evt",&data.evt,"evt/i");
	
	tree->Branch("totEast",&data.totEast,"totEast[19]/D");
	tree->Branch("totWest",&data.totWest,"totWest[19]/D");
	tree->Branch("leEast",&data.leEast,"leEast[19]/D");
	tree->Branch("leWest",&data.leWest,"leWest[19]/D");

	tree->Branch("fastLeEast",&data.fastLeEast,"fastLeEast/D");
	tree->Branch("fastLeWest",&data.fastLeWest,"fastLeWest/D");
	tree->Branch("fastTotEast",&data.fastTotEast,"fastTotEast/D");
	tree->Branch("fastTotWest",&data.fastTotWest,"fastTotWest/D");

	tree->Branch("avgLeEast",&data.avgLeEast,"avgLeEast/D");
	tree->Branch("avgLeWest",&data.avgLeWest,"avgLeWest/D");

	tree->Branch("fastChEast",&data.fastChEast,"fastChEast/i");
	tree->Branch("fastChWest",&data.fastChWest,"fastChWest/i");

	tree->Branch("vertexX",&data.vertexX,"vertexX/f");
	tree->Branch("vertexY",&data.vertexY,"vertexY/f");
	tree->Branch("vertexZ",&data.vertexZ,"vertexZ/f");

	
}


/**
 * Reads parameter files in either to compare or to run
 * and produce calibration plots from the existing parameter files
 */
void VpdOfflineSlewPicoDstMaker::readParameters(  ){

	DEBUG( classname(), "" )
	
	string inName =  config[ nodePath + ".Slewing:url" ];

	ifstream infile;
	infile.open( inName.c_str() );

	bool good = false;

	DEBUG( classname(), " Reading slewing corrections from " << inName );
	if ( infile.is_open() ){
		for ( int i = 0 ; i < nChannels; i++ ){

			int channel = -1;
			infile >> channel;

			int tBins = 0;
			infile >> tBins;
			tBins++;
			DEBUG( classname(), "tBins " << tBins );
			if ( 	channel >= 1 && channel <= 38  ){

				double tmp = 0;
				for ( int j=0; j < tBins; j++ ){
					infile >> tmp;
					totBins[ channel - 1 ].push_back( tmp );//[ j ] = tmp;
					if ( channel == 1 )
						DEBUG( classname(), " bin[ " << j << " ] = " << tmp );
				}
				
				double bZero = 0;
				for ( int j=0; j < tBins; j++ ){
					infile >> tmp;
					corrections[ channel - 1 ].push_back( tmp ); //[ j ] = tmp;
					if ( 0 == j )
						bZero = tmp;
					DEBUG( classname(), "Cor[ " << channel - 1 << " ][ " << j << " ] = " << tmp );
				}
			} else {
				ERROR( classname(), "Bad file format, cannot load parameters" );
				good = false;
			}
		} // loop channels
	} else {
		ERROR( classname(), " Cannot open file " );
		good = false;
	}

	infile.close();


	for ( int iCh = 0; iCh < nChannels; iCh ++ ){
		HistoBins b( totBins[ iCh ] );
		bins.push_back( b );
	}


}// readParameters


double VpdOfflineSlewPicoDstMaker::correct( double le, double tot, int ch ) {
	int bin = bins[ ch ].findBin( tot );

	if ( -1 == bin ) bin = 0;
	if ( -2 == bin ) bin = corrections[ ch ].size() - 1;
	if ( corrections[ ch ].size() <= bin  ){
		ERROR( classname(), "Invalid corrections bin = " << bin << ", but size() = " << corrections[ch].size() );
		return le;
	}
	return le - corrections[ ch ][ bin ];
}