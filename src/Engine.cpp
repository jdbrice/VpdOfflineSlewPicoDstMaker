

#include "XmlConfig.h"
#include "Engine.h"
using namespace jdb;

#include <iostream>
#include <exception>

#include "VpdOfflineSlewPicoDstMaker.h"

int main( int argc, char* argv[] ) {

	TaskFactory::registerTaskRunner<VpdOfflineSlewPicoDstMaker>( "VpdOfflineSlewPicoDstMaker" );
	Engine engine( argc, argv );

	return 0;
}
