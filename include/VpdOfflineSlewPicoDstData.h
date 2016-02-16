#ifndef TOF_PICO_DST_DATA_H
#define TOF_PICO_DST_DATA_H



const Int_t kMaxChannels = 19;


struct VpdOfflineSlewPicoDstData {

	Int_t  run, evt;

	// VPD trigger side bbq electronics
	Double_t   totEast[kMaxChannels], 
			leEast[kMaxChannels],
			fastTotEast,
			fastLeEast,
			avgLeEast;

	Int_t   fastChEast, 
			nEast;


	Double_t   totWest[kMaxChannels], 
			leWest[kMaxChannels],
			fastTotWest, 
			fastLeWest,
			avgLeWest;
			
	Int_t   fastChWest, 
			nWest;

	Float_t vertexX, vertexY, vertexZ;

};

#endif