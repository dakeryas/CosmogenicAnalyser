#include <fstream>
#include "boost/program_options.hpp"
#include "Cosmogenic/InnerVeto.hpp"
#include "Cosmogenic/BufferMuonVeto.hpp"
#include "Cosmogenic/ReconstructionVeto.hpp"
#include "Cosmogenic/ChimneyVeto.hpp"
#include "CandidateTreeAnalyser.hpp"
#include "InputIterator.hpp"

namespace bpo = boost::program_options;
namespace CsHt = CosmogenicHunter;

namespace CosmogenicAnalyser{
  
  template <class T, class K>
  void analyseFile(const boost::filesystem::path& targetPath, CosmogenicAnalyser::CandidateTreeAnalyser<T,K>& candidateTreeAnalyser){
    
    std::ifstream inputStream(targetPath.string(), std::ios::binary);
    cereal::BinaryInputArchive inputArchive(inputStream);
    
    candidateTreeAnalyser.analyse(cereal::Archive::InputIterator<CosmogenicHunter::CandidateTree<T,K>>{inputArchive}, cereal::Archive::InputIterator<CosmogenicHunter::CandidateTree<T,K>>{});
    
  }
  
  template <class T, class K>
  void process(const boost::filesystem::path& targetPath, const boost::filesystem::path& outputPath, CosmogenicAnalyser::CandidateTreeAnalyser<T,K> candidateTreeAnalyser){
    
    analyseFile(targetPath, candidateTreeAnalyser);
    std::cout<<candidateTreeAnalyser<<std::endl;
    
    auto fitResults = candidateTreeAnalyser.fitTimeIntervals(257); //257ms 9Li lifetime
    std::cout<<fitResults<<std::endl;
    
    TFile outputFile(outputPath.c_str(), "recreate");
    for(const auto& distribution : candidateTreeAnalyser.getCandidateMuonPairDistributions()) distribution.Write(distribution.GetName());
    candidateTreeAnalyser.getMuonSpectrum().Write();
    for(const auto& distribution : candidateTreeAnalyser.getNeutronDistributions()) distribution.Write(distribution.GetName());
    for(const auto& distribution : candidateTreeAnalyser.getBackgroundSubtractedCandidateMuonPairDistributions()) distribution.Write(distribution.GetName());
    
  }
  
}

int main(int argc, char* argv[]){
 
  boost::filesystem::path targetPath, outputPath, densitiesPath;
  double priorRatio;
  double timeBinWidth;
  CosmogenicAnalyser::TimeWindow onTimeWindow, offTimeWindow;
  CosmogenicHunter::MuonDefinition<float> muonDefinition;
  int neutronMultiplicityThreshold;
  CosmogenicHunter::Bounds<float> promptEnergyBounds;
  CosmogenicHunter::InnerVeto<float> promptInnerVeto;
  CosmogenicHunter::ReconstructionVeto<float> reconstructionVeto;
  CosmogenicHunter::BufferMuonVeto<float> bufferMuonVeto;
  CosmogenicHunter::ChimneyVeto<float> chimneyVeto; //CPS_prompt + CPS_delayed
  float minCosmogenicLikelihood;
  
  bpo::options_description optionDescription("CosmogenicAnalyser usage");
  optionDescription.add_options()
  ("help,h", "Display this help message")
  ("target", bpo::value<boost::filesystem::path>(&targetPath)->required(), "Path of the candidate tree to analyse")
  ("output,o", bpo::value<boost::filesystem::path>(&outputPath)->required(), "Output file where to save the distributions")
  ("cosmogenic-densities,d", bpo::value<boost::filesystem::path>(&densitiesPath)->required(), "Path of the file containing the cosmogenic probability densities")
  ("prior-ratio,p", bpo::value<double>(&priorRatio)->required(), "Ratio of the cosmogenic prior to the accidental's")
  ("time-bin-width", bpo::value<double>(&timeBinWidth)->default_value(50), "Time bin width [ms]")
  ("ontime-window", bpo::value<CosmogenicAnalyser::TimeWindow>(&onTimeWindow)->required(), "On-time window (start : end) [ms]")
  ("offtime-window", bpo::value<CosmogenicAnalyser::TimeWindow>(&offTimeWindow)->required(), "Off-time window (start : end) [ms]")
  ("muon-definition", bpo::value<CosmogenicHunter::MuonDefinition<float>>(&muonDefinition)->required(), "Muon definition parameters (Inner Veto charge threshold [DUQ] : visible energy threshold [MeV] : visible energy to Inner Detector charge conversion factor [DUQ/MeV])")
  ("neutron-min-multiplicity,n", bpo::value<int>(&neutronMultiplicityThreshold)->default_value(0), "Threshold for the number of neutrons following a muon")
  ("prompt-energy-bounds", bpo::value<CosmogenicHunter::Bounds<float>>(&promptEnergyBounds)->default_value(CosmogenicHunter::Bounds<float>{0, 1e2}), "Bounds (':' separator) on the prompt's energy [MeV]")
  ("prompt-IV-cuts", bpo::value<CosmogenicHunter::InnerVeto<float>>(&promptInnerVeto), "Inner Veto rejection parameters (max charge [DUQ] : max number of hit PMTs : time bounds [ns] : space correlation [mm])")
  ("prompt-buffer-cuts", bpo::value<CosmogenicHunter::BufferMuonVeto<float>>(&bufferMuonVeto), "Buffer stopping muon rejection parameters (constant [MeV^exponent] : exponent)")
  ("prompt-min-cosmogenic-likelihood,l", bpo::value<float>(&minCosmogenicLikelihood)->default_value(0), "Threshold for the cosmogenic likelihood of a prompt")
  ("delayed-reconstruction-cuts", bpo::value<CosmogenicHunter::ReconstructionVeto<float>>(&reconstructionVeto), "Poor reconstruction rejection parameters (min energy [MeV] : characteristic inconsistency)")
  ("pair-min-chimney-inconsistency", bpo::value<CosmogenicHunter::ChimneyVeto<float>>(&chimneyVeto), "Threshold for the chimney inconsistency ratio of a pair");
  
  bpo::positional_options_description positionalOptions;//to use arguments without "--"
  positionalOptions.add("target", 1);
  
  bpo::variables_map arguments;
  try{
    
    bpo::store(bpo::command_line_parser(argc, argv).options(optionDescription).positional(positionalOptions).run(), arguments);
    
    if(arguments.count("help") || argc == 1){
      
      std::cerr<<optionDescription<<std::endl;
      return 0;
      
    }
      
    bpo::notify(arguments);//the arguments are ready to be used
    
  }
  catch(bpo::error& error){
    
    std::cerr<<error.what()<<std::endl;
    return 1;
    
  }
  catch(std::invalid_argument& error){
    
    std::cerr<<"Error: "<<error.what()<<std::endl;
    return 1;
    
  }
  
  if(!boost::filesystem::is_regular_file(targetPath)){
    
    std::cerr<<"Error: "<<targetPath<<" is not a regular file"<<std::endl;
    return 1;
    
  }
  else if(!boost::filesystem::is_regular_file(densitiesPath)){
    
    std::cerr<<"Error: "<<densitiesPath<<" is not a regular file"<<std::endl;
    return 1;
    
  }
  else{
    
    try{
      
      CosmogenicAnalyser::PairSelector<float> pairSelector(promptEnergyBounds);
      if(arguments.count("prompt-IV-cuts")) pairSelector.emplaceVeto<CosmogenicHunter::InnerVeto<float>>(std::move(promptInnerVeto));
      if(arguments.count("prompt-buffer-cuts")) pairSelector.emplaceVeto<CosmogenicHunter::BufferMuonVeto<float>>(std::move(bufferMuonVeto));
      if(arguments.count("delayed-reconstruction-cuts")) pairSelector.emplaceVeto<CosmogenicHunter::ReconstructionVeto<float>>(std::move(reconstructionVeto));
      if(arguments.count("pair-min-chimney-inconsistency")) pairSelector.emplaceVeto<CosmogenicHunter::ChimneyVeto<float>>(std::move(chimneyVeto));
      
      CosmogenicAnalyser::MuonShowerSelector<float> muonShowerSelector(muonDefinition, neutronMultiplicityThreshold);
      CosmogenicAnalyser::LikelihoodComputer likelihoodComputer(priorRatio, densitiesPath);
      CosmogenicAnalyser::TimeDivision timeDivision{timeBinWidth, onTimeWindow, offTimeWindow};
      CosmogenicAnalyser::CandidateMuonPairAnalyser<float> candidateMuonPairAnalyser(likelihoodComputer, minCosmogenicLikelihood, timeDivision, {20, 0, 4e3}, {50, 0, 50}, {28, 0, 14});
      CosmogenicAnalyser::MuonShowerAnalyser muonShowerAnalyser({25, 0, 4000}, {100, 0, 10});
      CosmogenicAnalyser::CandidateTreeAnalyser<float,float> candidateTreeAnalyser(std::move(pairSelector), std::move(muonShowerSelector), std::move(candidateMuonPairAnalyser), std::move(muonShowerAnalyser));
      
      CosmogenicAnalyser::process(targetPath, outputPath, candidateTreeAnalyser);
      
    }
    catch(std::exception& error){
      
      std::cerr<<"Error: "<<error.what()<<std::endl;
      return 1;
      
    }
    
  }
  
}
