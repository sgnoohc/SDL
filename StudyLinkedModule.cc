# include "StudyLinkedModule.h"

StudyLinkedModule::StudyLinkedModule(const char* studyName)
{
  studyname = studyName;
}

void StudyLinkedModule::bookStudy()
{

    ana.histograms.addVecHistogram(TString::Format("Linked_MD_occupancy_in_barrel"),100,0,100,[&](){return BarrelLinkedModuleOccupancy;});
    ana.histograms.addHistogram(TString::Format("Average_number_of_Linked_modules_in_barrel"),100,0,100,[&](){return nBarrelLinkedModules;});
    ana.histograms.addHistogram(TString::Format("Average_number_of_Linked_modules_in_endcap"),100,0,100,[&](){return nEndcapLinkedModules;});

    ana.histograms.addHistogram(TString::Format("average_Linked_MD_occupancy_in_barrel"),1000,0,100,[&](){return averageBarrelLinkedModuleOccupancy;});

    ana.histograms.addVecHistogram(TString::Format("Linked_MD_occupancy_in_endcap"),100,0,100,[&](){return EndcapLinkedModuleOccupancy;});

    ana.histograms.addHistogram(TString::Format("average_Linked_MD_occupancy_in_endcap"),1000,0,100,[&](){return averageEndcapLinkedModuleOccupancy;});

    for(int i = 0; i<6; i++)
    {
      ana.histograms.addHistogram(TString::Format("average_Linked_MD_occupancy_in_barrel_for_layer_%d",i+1),1000,0,100,[&,i](){return averageLayerBarrelLinkedModuleOccupancy[i];});

      ana.histograms.addVecHistogram(TString::Format("Linked_MD_occupancy_in_barrel_for_layer_%d",i+1),100,0,100,[&,i](){return LayerBarrelLinkedModuleOccupancy.at(i);});

      ana.histograms.addHistogram(TString::Format("average_Linked_MD_occupancy_in_endcap_for_layer_%d",i+1),1000,0,100,[&,i](){return averageLayerEndcapLinkedModuleOccupancy[i];});

      ana.histograms.addVecHistogram(TString::Format("Linked_MD_occupancy_in_endcap_for_layer_%d",i+1),1000,0,100,[&,i](){return LayerEndcapLinkedModuleOccupancy.at(i);});

      ana.histograms.addHistogram(TString::Format("Average_number_of_Linked_Modules_in_Barrel_for_layer_%d",i+1),100,0,100,[&,i](){return nLayerBarrelLinkedModules.at(i);});
      ana.histograms.addHistogram(TString::Format("Average_number_of_Linked_Modules_in_endcap_for_layer_%d",i+1),100,0,100,[&,i](){return nLayerEndcapLinkedModules.at(i);});
    }


    for(int i=0;i<15;i++)
    {
      ana.histograms.addHistogram(TString::Format("average_Linked_MD_occupancy_in_endcap_for_ring_%d",i+1),1000,0,100,[&,i](){return averageEndcapRingLinkedModuleOccupancy[i];});

      ana.histograms.addVecHistogram(TString::Format("Linked_MD_occupancy_in_endcap_for_ring_%d",i+1),100,0,100,[&,i](){return RingEndcapLinkedModuleOccupancy.at(i);});

      ana.histograms.addHistogram(TString::Format("Average_number_of_Linked_Modules_in_endcap_for_ring_%d",i+1),100,0,100,[&,i](){return nRingEndcapLinkedModules.at(i);});
    }
}


void StudyLinkedModule::doStudy(SDL::Event &event,std::vector<std::tuple<unsigned int, SDL::Event*>> simtrkevents)
{

    averageBarrelLinkedModuleOccupancy = 0;
    averageEndcapLinkedModuleOccupancy = 0;
    int nBarrelModules = 0,nEndcapModules = 0;
    nBarrelLinkedModules = 0;
    nEndcapLinkedModules = 0;

    BarrelLinkedModuleOccupancy.clear();
    EndcapLinkedModuleOccupancy.clear();

    averageLayerBarrelLinkedModuleOccupancy.clear();
    averageLayerBarrelLinkedModuleOccupancy = {0,0,0,0,0,0};    
    std::vector<int> nBarrelLayerModules(6,0);
    std::vector<int> nEndcapLayerModules(6,0);
    nLayerBarrelLinkedModules.clear();
    nLayerBarrelLinkedModules = {0,0,0,0,0,0};
    averageLayerEndcapLinkedModuleOccupancy.clear();
    averageLayerEndcapLinkedModuleOccupancy = {0,0,0,0,0,0};
    nLayerEndcapLinkedModules.clear();
    nLayerEndcapLinkedModules = {0,0,0,0,0,0};

    averageEndcapRingLinkedModuleOccupancy.clear();
    nRingEndcapLinkedModules.clear();
    for(int i=0;i<15;i++)
    {
      averageEndcapRingLinkedModuleOccupancy.push_back(0);
      nRingEndcapLinkedModules.push_back(0);
    }
    std::vector<int> nEndcapRingModules(15,0);

    LayerBarrelLinkedModuleOccupancy.clear();
    LayerEndcapLinkedModuleOccupancy.clear();
    RingEndcapLinkedModuleOccupancy.clear();

    for(int i=0;i<6;i++)
    {
      LayerBarrelLinkedModuleOccupancy.push_back(std::vector<float>());
      LayerEndcapLinkedModuleOccupancy.push_back(std::vector<float>());
    }

    for(int i=0;i<15;i++)
    {
      RingEndcapLinkedModuleOccupancy.push_back(std::vector<float>());
    }



    std::vector<SDL::Module*> moduleList = event.getModulePtrs();

    for(auto &module:moduleList)
    {
      std::vector<unsigned int> connectedModuleDetIds = SDL::moduleConnectionMap.getConnectedModuleDetIds(module->detId());
      int nConnectedModules = 0, connectedModuleOccupancy = 0;
      for(auto &connectedModuleId:connectedModuleDetIds)
      {
        SDL::Module& connectedModule = event.getModule(connectedModuleId);
        //Now start the occupancy thing
        connectedModuleOccupancy += connectedModule.getMiniDoubletPtrs().size();
        nConnectedModules ++;
      }

      if(module->subdet() == SDL::Module::Barrel)
      {
        if((module->getMiniDoubletPtrs()).size() != 0)
        {
          nBarrelLinkedModules += nConnectedModules;
          nLayerBarrelLinkedModules.at(module->layer()-1) += nConnectedModules;

          BarrelLinkedModuleOccupancy.push_back(connectedModuleOccupancy);
          averageBarrelLinkedModuleOccupancy += connectedModuleOccupancy;
          nBarrelModules++;

        LayerBarrelLinkedModuleOccupancy.at(module->layer() -1).push_back(connectedModuleOccupancy);
        averageLayerBarrelLinkedModuleOccupancy.at(module->layer()-1) += connectedModuleOccupancy;
        nBarrelLayerModules.at(module->layer()-1)++;
        }
      }
      else if(module->subdet() == SDL::Module::Endcap)
      {
        if((module->getMiniDoubletPtrs()).size() != 0)
        {
          nEndcapLinkedModules += nConnectedModules;
          nLayerEndcapLinkedModules.at(module->layer()-1) += nConnectedModules;
          nRingEndcapLinkedModules.at(module->ring()-1) += nConnectedModules;

          EndcapLinkedModuleOccupancy.push_back(connectedModuleOccupancy);
          averageEndcapLinkedModuleOccupancy += connectedModuleOccupancy;
          nEndcapModules++;

          LayerEndcapLinkedModuleOccupancy.at(module->layer()-1).push_back(connectedModuleOccupancy);
          averageLayerEndcapLinkedModuleOccupancy.at(module->layer()-1) += connectedModuleOccupancy;
          nEndcapLayerModules.at(module->layer()-1)++;

          RingEndcapLinkedModuleOccupancy.at(module->ring()-1).push_back(connectedModuleOccupancy);
          averageEndcapRingLinkedModuleOccupancy.at(module->ring()-1) += connectedModuleOccupancy;
          nEndcapRingModules.at(module->ring()-1) ++;
        }
      }
    }

    averageBarrelLinkedModuleOccupancy = (nBarrelModules != 0) ? averageBarrelLinkedModuleOccupancy / nBarrelModules : 0;
    averageEndcapLinkedModuleOccupancy = (nEndcapModules != 0) ? averageEndcapLinkedModuleOccupancy / nEndcapModules : 0;

    nBarrelLinkedModules = (nBarrelModules != 0) ? nBarrelLinkedModules/nBarrelModules : 0;
    nEndcapLinkedModules = (nEndcapModules != 0) ? nEndcapLinkedModules/nEndcapModules : 0;

    for(int i=0;i<6;i++)
    {
      averageLayerBarrelLinkedModuleOccupancy[i] = (nBarrelLayerModules[i] != 0) ? averageLayerBarrelLinkedModuleOccupancy[i]/nBarrelLayerModules[i] : 0;

      averageLayerEndcapLinkedModuleOccupancy[i] = (nEndcapLayerModules[i] != 0) ? averageLayerEndcapLinkedModuleOccupancy[i]/nEndcapLayerModules[i] : 0;

      nLayerBarrelLinkedModules[i] = (nBarrelLayerModules[i] != 0) ? nLayerBarrelLinkedModules[i]/nBarrelLayerModules[i] : 0;
      nLayerEndcapLinkedModules[i] = (nEndcapLayerModules[i] != 0) ? nLayerEndcapLinkedModules[i]/nEndcapLayerModules[i] : 0;

    }

    for(int i=0;i<15;i++)
    {
      averageEndcapRingLinkedModuleOccupancy.at(i) = (nEndcapRingModules.at(i) !=0) ? averageEndcapRingLinkedModuleOccupancy.at(i)/nEndcapRingModules.at(i) : 0;

      nRingEndcapLinkedModules[i] = (nEndcapRingModules.at(i) != 0) ? nRingEndcapLinkedModules[i]/nEndcapRingModules.at(i) : 0;
    }
}
