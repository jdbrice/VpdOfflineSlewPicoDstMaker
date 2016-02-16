#ifndef DATA_ADAPTER_H
#define DATA_ADAPTER_H

#include <string>
using namespace std;

class DataAdapter
{
public:
	DataAdapter() {}
	~DataAdapter() {}


	virtual int tdc( string side = "east", string crate="bbq", int tube = 0 ){
		if ( "east" == side || "East" == side || "EAST" == side ){
			if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){

			} else {

			}
		}
		if ( "west" == side || "West" == side || "WEST" == side ){
			if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){

			} else {

			}
		}
		return 0;
	}
	virtual int adc( string side = "east", string crate="bbq", int tube = 0 ) { return 0; }

	virtual int fastTdc( string side = "east", string crate="bbq" ) { return 0; }


	virtual Float_t vX() {
		return 0;
	}
	virtual Float_t vY() {
		return 0;
	}
	virtual Float_t vZ() {
		return 0;
	}

	virtual Int_t runNumber(){
		return 0;
	}
	virtual Int_t eventNumber(){
		return 0;
	}


	
};


#endif