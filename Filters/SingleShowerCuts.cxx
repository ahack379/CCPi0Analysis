#ifndef LARLITE_SINGLESHOWERCUTS_CXX
#define LARLITE_SINGLESHOWERCUTS_CXX

#include "SingleShowerCuts.h"
#include "DataFormat/shower.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/cluster.h"

namespace larlite {

  bool SingleShowerCuts::initialize() {
    
    if( !_pi0_selection ){
      _pi0_selection = new TTree("pi0_selection","");
      _pi0_selection->Branch("_event",&_event,"event/I");
      _pi0_selection->Branch("_pi0_mass",&_pi0_mass,"pi0_mass/F");
      _pi0_selection->Branch("_pi0_mom",&_pi0_mom,"pi0_mom/F");
      _pi0_selection->Branch("_pi0_oangle",&_pi0_oangle,"pi0_oangle/F");
      _pi0_selection->Branch("_pi0_low_shrE",&_pi0_low_shrE,"pi0_low_shrE/F");
      _pi0_selection->Branch("_pi0_high_shrE",&_pi0_high_shrE,"pi0_high_shrE/F");
      _pi0_selection->Branch("_pi0_low_radL",&_pi0_low_radL,"pi0_low_radL/F");
      _pi0_selection->Branch("_pi0_high_radL",&_pi0_high_radL,"pi0_high_radL/F");
      _pi0_selection->Branch("_mu_mom",&_mu_mom,"mu_mom/F");
      _pi0_selection->Branch("_mu_angle",&_mu_angle,"mu_angle/F");
      }

    _event = -1;

    return true;
  }

  void SingleShowerCuts::clear(){
  
    _pi0_mass      = -10;
    _pi0_mom       = -10;
    _pi0_oangle    = -10;
    _pi0_low_shrE  = -10;
    _pi0_high_shrE = -10;
    _pi0_low_radL  = -10;
    _pi0_high_radL = -10;
    _mu_mom        = -10;
    _mu_angle      = -10;
  
  }
  
  bool SingleShowerCuts::analyze(storage_manager* storage) {

    _event++;

    auto ev_s = storage->get_data<event_shower>("showerreco");
    auto ev_v = storage->get_data<event_vertex>("numuCC_vertex");
    auto ev_t = storage->get_data<event_track>("numuCC_track");

    storage->set_id( ev_s->run(), ev_s->subrun(), ev_s->event_id() );
    auto new_shower_v = storage->get_data<larlite::event_shower>("pi0_1gamma_candidate_showers");

    if( !ev_s || !ev_s->size() ){
      std::cout<<"Not enough reco'd showers..." <<ev_s->size()<<std::endl; return false; }

    if( !ev_t || !ev_t->size() ){
      std::cout<<"No tagged track; what??" <<std::endl; return false; }

    std::cout<<"\nEvent : "<<_event <<std::endl ; 

    clear();

    std::multimap<float,float> cand_map ;

    for ( int s1 = 0; s1 < ev_s->size(); s1++ ){

        auto const& shr1 = ev_s->at(s1);
        _pi0_high_shrE = shr1.Energy(2) ;

	 auto vtx = ev_v->at(0);
         ::geoalgo::Point_t vertex(vtx.X(),vtx.Y(),vtx.Z());

         // Make the backwards projection for the showers
         geoalgo::Vector_t rev_shr1(-1.*shr1.Direction()) ;
         auto shr1_bkwrd_hl = ::geoalgo::HalfLine_t(shr1.ShowerStart(),rev_shr1);

	 auto dist = _geoAlgo.SqDist(vertex, shr1_bkwrd_hl) ;

         auto shr_st = shr1.ShowerStart(); 
         ::geoalgo::Point_t shr_start(shr1.ShowerStart().X(),shr1.ShowerStart().Y(),shr1.ShowerStart().Z());
	 auto dist_to_vtx = sqrt( pow( vtx.X() - shr_st.X(),2) +
                                  pow( vtx.Y() - shr_st.Y(),2) + 
				  pow( vtx.Z() - shr_st.Z(),2)); 

         auto radL_shr1 = vertex.Dist(shr1.ShowerStart());
        _pi0_high_radL = radL_shr1 ;

	if ( shr1.Energy(2) < 1e-30 ) continue;

	if( dist > 4 ){ 
	   std::cout<<"Impact Parameter: "<<dist<<std::endl ;
           continue;
	}

        if( radL_shr1 > 62 ) { 
	   std::cout<<"Radiation length : "<<radL_shr1<<std::endl ;
           continue;
	 }

     	cand_map.emplace(1./shr1.Energy(2),s1);

        //std::cout<<" Dist to vtx : "<<dist_to_vtx<<", IP: "<<dist <<", energy: "<<_pi0_high_shrE<<std::endl ;

      }// shower ID 1 

      if( cand_map.size() < 1 ) return false;

      auto tag_trk = ev_t->at(0) ;
      _mu_angle      = cos(tag_trk.Theta());

      // Store the new shower data product
      new_shower_v->emplace_back(ev_s->at(cand_map.begin()->second));

      // Now also store the associations
      event_cluster *ev_cluster = nullptr;
      auto ass_cluster_v = storage->find_one_ass(ev_s->id(), ev_cluster, ev_s->name());

      std::vector<std::vector<unsigned int> > shower_cluster_v ; 
      shower_cluster_v.reserve(2) ;

      for( int i = 0; i < ass_cluster_v.size(); i ++ ){
         if ( i == cand_map.begin()->second )
            shower_cluster_v.emplace_back(ass_cluster_v[i] ) ;
      } 

      ::larlite::event_ass * shower_cluster_ass_v = 0;

      // if associated clusters not found -> quit and exit
      if ( !ev_cluster or (ev_cluster->size() == 0) ) 
        print(msg::kERROR, __FUNCTION__, Form("No clusters found associated to shower" ));
      else 
        shower_cluster_ass_v = storage->get_data<event_ass>(new_shower_v->name());

      if ( shower_cluster_ass_v ) {
        shower_cluster_ass_v->set_association(new_shower_v->id(),
                                              product_id(data::kCluster, ev_cluster->name()),
                                              shower_cluster_v);
      }
      
      _pi0_selection->Fill();
      _event_list.emplace_back(_event); 

    return true;
  }

  bool SingleShowerCuts::finalize() {

    if(_fout) { 
      _fout->cd(); 
      _pi0_selection->Write(); 
    }

   std::cout<<_event_list.size()<<" events found! "<<std::endl; 

   for ( auto const & e : _event_list )
     std::cout<<e <<", "; 
  
   std::cout<<std::endl ;
    return true;
  }

}
#endif
