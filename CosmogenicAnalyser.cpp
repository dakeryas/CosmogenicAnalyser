#include <fstream>
#include "boost/program_options.hpp"
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
  double cosmogenicProbability;
  double timeBinWidth;
  CosmogenicAnalyser::TimeWindow onTimeWindow, offTimeWindow;
  CosmogenicHunter::MuonDefinition<float> muonDefinition;
  int neutronMultiplicityThreshold;
  CosmogenicHunter::Bounds<float> promptEnergyBounds;
  CosmogenicHunter::InnerVetoThreshold<float> promptInnerVetoThreshold;
  CosmogenicHunter::ReconstructionCutParameters<float> reconstructionCutParameters;
  CosmogenicHunter::BufferMuonCutParameters<float> bufferMuonCutParameters;
  float minCosmogenicLikelihood;
  float minChimneyInconsistencyRatio; //CPS_prompt + CPS_delayed
  
  bpo::options_description optionDescription("CosmogenicAnalyser usage");
  optionDescription.add_options()
  ("help,h", "Display this help message")
  ("target,t", bpo::value<boost::filesystem::path>(&targetPath)->required(), "Path of the candidate tree to analyse")
  ("output,o", bpo::value<boost::filesystem::path>(&outputPath)->required(), "Output file where to save the distributions")
  ("cosmogenic-densities,d", bpo::value<boost::filesystem::path>(&densitiesPath)->required(), "Path of the file containing the cosmogenic probability densities")
  ("cosmogenic-probability,p", bpo::value<double>(&cosmogenicProbability)->required(), "Probability to be a cosmogenic event")
  ("time-bin-width", bpo::value<double>(&timeBinWidth)->default_value(50), "Time bin width [ms]")
  ("ontime-window", bpo::value<CosmogenicAnalyser::TimeWindow>(&onTimeWindow)->required(), "On-time window (start : end) [ms]")
  ("offtime-window", bpo::value<CosmogenicAnalyser::TimeWindow>(&offTimeWindow)->required(), "Off-time window (start : end) [ms]")
  ("muon-definition", bpo::value<CosmogenicHunter::MuonDefinition<float>>(&muonDefinition)->required(), "Muon definition parameters (Inner Veto charge threshold [DUQ] : visible energy threshold [MeV] : visible energy to Inner Detector charge conversion factor [DUQ/MeV])")
  ("neutron-min-multiplicity,n", bpo::value<int>(&neutronMultiplicityThreshold)->default_value(0), "Threshold for the number of neutrons following a muon")
  ("prompt-energy-bounds", bpo::value<CosmogenicHunter::Bounds<float>>(&promptEnergyBounds)->default_value(CosmogenicHunter::Bounds<float>{0, 1e2}), "Bounds (':' separator) on the prompt's energy [MeV]")
  ("prompt-IV-cuts", bpo::value<CosmogenicHunter::InnerVetoThreshold<float>>(&promptInnerVetoThreshold)->required(), "Inner Veto rejection parameters (max charge [DUQ] : max number of hit PMTs : time bounds [ns] : space correlation [mm])")
  ("prompt-buffer-cuts", bpo::value<CosmogenicHunter::BufferMuonCutParameters<float>>(&bufferMuonCutParameters)->required(), "Buffer stopping muon rejection parameters (constant [MeV^exponent] : exponent)")
  ("prompt-min-cosmogenic-likelihood,l", bpo::value<float>(&minCosmogenicLikelihood)->default_value(0), "Threshold for the cosmogenic likelihood of a prompt")
  ("delayed-reconstruction-cuts", bpo::value<CosmogenicHunter::ReconstructionCutParameters<float>>(&reconstructionCutParameters)->required(), "Poor reconstruction rejection parameters (min energy [MeV] : characteristic inconsistency)")
  ("pair-min-chimney-inconsistency", bpo::value<float>(&minChimneyInconsistencyRatio)->default_value(2), "Threshold for the chimney inconsistency ratio of a pair");
  
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
      
      CosmogenicAnalyser::PairSelector<float> pairSelector(promptEnergyBounds, promptInnerVetoThreshold, bufferMuonCutParameters, reconstructionCutParameters, minChimneyInconsistencyRatio, minCosmogenicLikelihood);
      CosmogenicAnalyser::MuonShowerSelector<float> muonShowerSelector(muonDefinition, neutronMultiplicityThreshold);
      CosmogenicAnalyser::LikelihoodComputer likelihoodComputer(densitiesPath, cosmogenicProbability);
      CosmogenicAnalyser::TimeDivision timeDivision{timeBinWidth, onTimeWindow, offTimeWindow};
      CosmogenicAnalyser::CandidateMuonPairAnalyser<float> candidateMuonPairAnalyser(likelihoodComputer, minCosmogenicLikelihood, timeDivision, {20, 0, 4e3}, {50, 0, 50}, {14, 0, 14});
      CosmogenicAnalyser::MuonShowerAnalyser muonShowerAnalyser({25, 0, 4000}, {100, 0, 10});
      CosmogenicAnalyser::CandidateTreeAnalyser<float,float> candidateTreeAnalyser(pairSelector, muonShowerSelector, candidateMuonPairAnalyser, muonShowerAnalyser);
      
      CosmogenicAnalyser::process(targetPath, outputPath, candidateTreeAnalyser);
      
    }
    catch(std::exception& error){
      
      std::cerr<<"Error: "<<error.what()<<std::endl;
      return 1;
      
    }
    
  }
  
}
