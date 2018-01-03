#include "PakManager.h"

#include "MsgPak.h"
#include "GenericPak.h"


std::shared_ptr<Pak> PakManager::loadPak(std::string pakFilename) {

	PakHeader header;

	inputPAKFILE.open(pakFilename, std::ios::binary);
	if (inputPAKFILE.is_open()) {
		std::cout << "File successfully opened.\n";

		//Prepopulate the Header. All Pak files will need to do this.
		inputPAKFILE.read(reinterpret_cast<char*>(&header.numOfFiles), sizeof(header.numOfFiles));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.version), sizeof(header.version));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.junk), sizeof(header.junk));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.junk2), sizeof(header.junk2));
	}
	else {
		std::cout << "File could not be opened. Please press a button to exit.\n";
		std::cin.get();
		return nullptr;
	}
	
	std::shared_ptr<Pak> pak;

	/*std::uint32_t pakMagicID;
	inputPAKFILE.read(reinterpret_cast<char*>(&pakMagicID), sizeof(pakMagicID));
	inputPAKFILE.seekg(-static_cast<std::int32_t>(sizeof(pakMagicID)), std::ios::cur);
	switch (pakMagicID) {
	case NCGRTYPE: pak = std::make_shared<NCGRPak>(); break;
	case NFTRTYPE: pak = std::make_shared<NFTRPak>(); break;
	default: pak = std::make_shared<MsgPak>(); break;
	}*/
	if (pakFilename.size() >= 12 && pakFilename.substr(pakFilename.size() - 12, 6) == "MESPAK") {
		pak = std::make_shared<MsgPak>();
	}
	else {
		pak = std::make_unique<GenericPak>();
		if (pakFilename.size() >= 8) {
			pak->setExtension(pakFilename.substr(pakFilename.size() - 8, 4));
		}
	}
	/*else if (pakFilename.substr(pakFilename.size() - 8, 4) == "NCGR") {
		pak = std::make_shared<NCGRPak>();
	}
	else if (pakFilename.substr(pakFilename.size() - 8, 4) == "NFTR") {
		pak = std::make_shared<NFTRPak>();
	}
	else if (pakFilename.substr(pakFilename.size() - 8, 4) == "NCLR") {
		pak = std::make_shared<NCLRPak>();
	}
	else if (pakFilename.substr(pakFilename.size() - 8, 4) == "NSCR") {
		pak = std::make_shared<NSCRPak>();
	}
	else {
		std::cout << "Could not find correct PAK format!" << std::endl;
		return nullptr;
	}*/

	pak->copyHeader(header);
	pak->populate(inputPAKFILE);
	
	return pak;
}