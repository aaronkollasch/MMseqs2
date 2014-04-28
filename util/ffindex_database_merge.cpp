#include "DBReader.h"
#include "DBWriter.h"
#include "Debug.h"
#include <vector>
#include <utility>      // std::pair

void printUsage(){
    std::string usage("\nMerge multiple ffindex files based on simular id into one file. \n");
    usage.append("Written by Martin Steinegger (Martin.Steinegger@campus.lmu.de) & Maria Hauser (mhauser@genzentrum.lmu.de).\n\n");
    usage.append("USAGE: ffindex_database_merge ffindexQueryDB ffindexOutDB ffindexFILES*\n");
    Debug(Debug::ERROR) << usage;
}

void parseArgs(int argc, const char** argv, 
	       std::string* ffindexSeqDB, 
	       std::string* ffindexOutDB, 
	       std::vector<std::pair<std::string,std::string> > * files){
    if (argc < 3){
        printUsage();
        exit(EXIT_FAILURE);
    }
    ffindexSeqDB->assign(argv[1]);
    ffindexOutDB->assign(argv[2]);

    int i = 3;
    while (i < argc){
    	files->push_back( std::make_pair<std::string, std::string>(std::string(argv[i]), std::string(argv[i])+".index" ) );
    	i++;
    }
}



int main (int argc, const char * argv[])
{

    std::string seqDB = "";
    std::string outDB = ""; 
    std::vector<std::pair<std::string, std::string> > filenames;
    parseArgs(argc, argv, &seqDB, &outDB, &filenames); 

    DBReader qdbr(seqDB.c_str(), std::string(seqDB+".index").c_str());
    qdbr.open(DBReader::NOSORT);
    DBWriter writer(outDB.c_str(), std::string( outDB +".index").c_str());
    writer.open();
    writer.mergeFiles(&qdbr, filenames, 1000000);
    writer.close();
    qdbr.close();
    return 0;
}
