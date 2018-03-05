#ifndef LARLITE_HOWMANYPI0S_CXX
#define LARLITE_HOWMANYPI0S_CXX

#include "HowManyPi0s.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/potsummary.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/LArProperties.h"
#include "LArUtil/TimeService.h"

namespace larlite {

  bool HowManyPi0s::initialize() {    

    _event = 0; 
    _signal = 0;
    _event_list.clear();

    _n_other = 0;    // 0 
    _n_cosmic = 0;   // 1
    _n_cc1pi0 = 0;   // 2 
    _n_cc0pi0 = 0;   // 3
    _n_nc1pi0 = 0;   // 4 
    _n_nc0pi0 = 0;   // 5

    _n_nc_1gamma = 0;
    _n_cc_1gamma = 0;

    _tot_pot = 0;

    return true;
  }
  
  bool HowManyPi0s::analyze(storage_manager* storage) {

    //std::cout<<"\nEvent is : "<<_event <<", "<<storage->event_id()<<", "<<storage->subrun_id()<<std::endl ;

   auto it = _map.find(storage->subrun_id());
   bool foundit = false;

   if( it != _map.end() ){
    while ( it->first == storage->subrun_id() ){  
      auto temp_event = it->second ; 
      if( temp_event == storage->event_id() )
        foundit = true;

      it++; 
      }   
     if ( !foundit)
      _map.emplace(storage->subrun_id(), storage->event_id() );

     else return false ;
    }   
   else 
      _map.emplace(storage->subrun_id(), storage->event_id() );


    _event++ ;

    auto ev_mctruth= storage->get_data<event_mctruth>("generator"); 
    if(!ev_mctruth || !ev_mctruth->size() ) {
      std::cout<<"Event has no mctruth info "<<std::endl;
      return false;
      }

    auto & truth = ev_mctruth->at(0);
    auto & nu  = truth.GetNeutrino();

    double xyz[3] = {0.};
    auto traj = nu.Nu().Trajectory();
    xyz[0] = traj.at(traj.size() - 1).X();
    xyz[1] = traj.at(traj.size() - 1).Y();
    xyz[2] = traj.at(traj.size() - 1).Z();
    auto e = traj.at(traj.size() - 1).E();

    bool infv = true;

    if( xyz[0] < 20 || xyz[0] > 236.35 || xyz[1] > 96.5 || xyz[1] < -96.5 || xyz[2] < 10 || xyz[2] > 1026.8 )
        infv = false;

    auto parts = ev_mctruth->at(0).GetParticles();
    int n_pi0 = 0;
    int n_mu = 0;
    int n_gamma = 0;
    
    for ( auto const & p : parts ){
     
        if( p.StatusCode() == 1 && p.PdgCode() == 111 )
          n_pi0 ++;
        if( p.StatusCode() == 1 && p.PdgCode() == 13 )
          n_mu ++;

        if( p.StatusCode() == 1 && p.PdgCode() == 22 )
          n_gamma ++;
     }

     if(n_pi0 == 0 && n_gamma == 1 && nu.CCNC() == 0 && nu.Nu().PdgCode() == 14 )
       _n_cc_1gamma++;
     if( n_gamma == 1 && nu.CCNC() == 1 )
       _n_nc_1gamma++;

     if( nu.Nu().PdgCode() == 14 && n_pi0 == 1 && nu.CCNC() == 0 && infv){ 
       _event_list.emplace_back(_event-1);
       _n_cc1pi0 ++; 
     }
     else if( nu.CCNC() == 0 && n_pi0 == 0 ) 
       _n_cc0pi0++;
      else if( nu.CCNC() == 1 && n_pi0 > 0 ) 
        _n_nc1pi0 ++; 
      else if( nu.CCNC() == 1 && n_pi0 == 0 ) 
        _n_nc0pi0++;
      else 
        _n_other ++;   
    

    // Now calculate the total POT + total numu neutrinos 
    //auto ev_pot = storage->get_subrundata<potsummary>("generator"); 

    //if( storage->subrun_id() != storage->last_subrun_id() )
    //  _tot_pot += ev_pot->totgoodpot ;

    return true;
  }

  bool HowManyPi0s::finalize() {

   // std::cout<<"CCpi0 are "<<float(_event_list.size())/(_event)*100<<"\% of BNB ("<<_event_list.size()<<"/"<<_event<<")"<<std::endl ;

   // std::cout<<"\n\n"<<_event_list.size()<<" in Event list :" <<std::endl ;
   // for( auto const & e : _event_list) std::cout<<e<<", ";

    std::cout<<"Signals: "<<std::endl ;
    std::cout<<"Total CCpi0 : "<<_n_cc1pi0<<std::endl; 

    // Note that cc other includes secondary pi0s.
    std::cout<<"\nBackgrounds: "<<std::endl;
    std::cout<<"1) Cosmic : "<<_n_cosmic<< std::endl;
    std::cout<<"2) CC 1pi0 : "<<_n_cc1pi0<<std::endl;
    std::cout<<"3) CC 0pi0 : "<<_n_cc0pi0<<std::endl;
    std::cout<<"4) NC 1pi0 : "<<_n_nc1pi0<<std::endl;
    std::cout<<"5) NC 0pi0 : "<<_n_nc0pi0<<std::endl;
    std::cout<<"6) Other   : "<<_n_other<<std::endl; 

    std::cout<<"Total accounted backgrounds: "<< _n_other + _n_cosmic + _n_nc1pi0 + _n_nc0pi0 + _n_cc0pi0 <<std::endl ;
    std::cout<<"Total POT: "<<_tot_pot <<std::endl ;

    std::cout<<"CC single gamma: "<<_n_cc_1gamma<<std::endl;
    std::cout<<"NC single gamma: "<<_n_nc_1gamma<<std::endl;



    return true;
  }

}
#endif
