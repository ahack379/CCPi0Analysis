#ifndef LARLITE_TWOSHOWERCUTS_CXX
#define LARLITE_TWOSHOWERCUTS_CXX

#include "TwoShowerCuts.h"
#include "DataFormat/shower.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/cluster.h"
#include "DataFormat/pfpart.h"

namespace larlite {

  bool TwoShowerCuts::initialize() {
    
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
      _pi0_selection->Branch("_mu_len",&_mu_len,"mu_len/F");
      _pi0_selection->Branch("_mu_angle",&_mu_angle,"mu_angle/F");
      _pi0_selection->Branch("_mu_phi",&_mu_phi,"mu_phi/F");
      _pi0_selection->Branch("_mult",&_mult,"mult/F");
      }

    _event = -1;
    _more_than_one = 0;

    return true;
  }

  void TwoShowerCuts::clear(){
  
    _pi0_mass      = -10;
    _pi0_mom       = -10;
    _pi0_oangle    = -10;
    _pi0_low_shrE  = -10;
    _pi0_high_shrE = -10;
    _pi0_low_radL  = -10;
    _pi0_high_radL = -10;
    _mu_len        = -10;
    _mu_mom        = -10;
    _mu_angle      = -10;
    _mu_phi        = -10;
    _mult          = 0;
  
  }
  
  bool TwoShowerCuts::analyze(storage_manager* storage) {

    _event++;

    auto ev_s = storage->get_data<event_shower>("showerreco");

    if( !ev_s || !ev_s->size() || ev_s->size() < 2 ){

      if( ev_s->size() == 1)
        _one_shower_list.emplace_back(_event) ;

      if( _chain_modules ){
        auto ev_c = storage->get_data<event_cluster>("ImageClusterHit");
        auto ev_p = storage->get_data<event_pfpart>("ImageClusterHit");
        auto ev_v = storage->get_data<event_vertex>(_vtx_producer);
        auto ev_t = storage->get_data<event_track>("numuCC_track");
        ev_c->clear();
        ev_p->clear();
        ev_v->clear();
        ev_t->clear();
      }
      
      std::cout<<"Not enough reco'd showers..." <<ev_s->size()<<std::endl;
      return false;
     }

    storage->set_id( ev_s->run(), ev_s->subrun(), ev_s->event_id() );
    auto new_shower_v = storage->get_data<larlite::event_shower>("pi0_candidate_showers");

    std::cout<<"\nEvent : "<<_event <<std::endl;

    clear();

    std::vector<std::pair<int,int>> candidate_pairs;
    std::vector<int> cand_ids;

    for ( int s1 = 0; s1 < ev_s->size(); s1++ ){

        auto const& shr1 = ev_s->at(s1);

        if ( shr1.Energy(2) < 1e-30 ) continue;

        for ( int s2 = 0; s2 < ev_s->size(); s2++ ){

            if (s2 <= s1) continue;

            auto const& shr2 = ev_s->at(s2);

            geoalgo::Vector_t rev_shr1(-1.*shr1.Direction()) ;
            geoalgo::Vector_t rev_shr2(-1.*shr2.Direction()) ;

            // Make the backwards projection for the showers
            auto shr1_bkwrd_hl = ::geoalgo::HalfLine_t(shr1.ShowerStart(),rev_shr1);
            auto shr2_bkwrd_hl = ::geoalgo::HalfLine_t(shr2.ShowerStart(),rev_shr2);

            // Calc the Opening angle of the showers
            double oangle = acos( shr1.Direction().Dot(shr2.Direction())) ;

            // Calc the vertex point of the two showers. the true designated backwards project
            geoalgo::Point_t vertex(3);

            auto st1 = shr1.ShowerStart();
            auto st2 = shr2.ShowerStart();
            auto dir1 = shr1.Direction();
            auto dir2 = shr2.Direction();
            geoalgo::HalfLine_t shr1_hl(st1.X(),st1.Y(),st1.Z(),dir1.X(),dir1.Y(), dir1.Z() );
            geoalgo::HalfLine_t shr2_hl(st2.X(),st2.Y(),st2.Z(),dir2.X(),dir2.Y(), dir2.Z() );

            _geoAlgo.commonOrigin(shr1_hl, shr2_hl, vertex, true);

            // Calc Diretion of two correlated shower
            geoalgo::Vector_t momentum(3);// need to fill out
            geoalgo::Vector_t mom_vect(shr2.Direction()*shr1.Energy(2) +shr1.Direction()*shr2.Energy(2)) ;

            auto tot_pi0_mom = sqrt(pow(mom_vect[0],2) + pow(mom_vect[1],2) + pow(mom_vect[2],2) );

            //mom_vect.Normalize();
            //momentum = mom_vect * sqrt(pow(shr1.Energy(2),2)+pow(shr2.Energy(2),2)+2*shr2.Energy(2)*shr1.Energy(2)*cos(oangle));
            //===========================================
            auto IP = pow(_geoAlgo.SqDist(shr1_bkwrd_hl,shr2_bkwrd_hl),0.5);
            auto radL_shr1 = vertex.Dist(shr1.ShowerStart());
            auto radL_shr2 = vertex.Dist(shr2.ShowerStart());

            if( oangle < 0.35 ){
              //std::cout<<"Bad Angle: "<< oangle<<std::endl ; 
              continue;
             }

            if( pow( _geoAlgo.SqDist(shr1_bkwrd_hl, shr2_bkwrd_hl), 0.5 ) > 4.){
               //std::cout<<"Ip? "<<pow(_geoAlgo.SqDist(shr1_bkwrd_hl,shr2_bkwrd_hl),0.5)<<std::endl ; 
               continue;
            }


           // if( radL_shr1 > 62 || radL_shr2 > 62 ){
           //    //std::cout<<"Rad Length : "<<radL_shr1<<", "<<radL_shr2<<std::endl ;
           //    continue;
           //     }



            _pi0_mass      = sqrt(2 * shr1.Energy(2) * shr2.Energy(2) *(1.-cos(oangle))); 
            _pi0_mom       = tot_pi0_mom;
            _pi0_oangle    = oangle;
            _pi0_low_shrE  = shr1.Energy(2) < shr2.Energy(2) ? shr1.Energy(2) : shr2.Energy(2) ;
            _pi0_high_shrE = shr1.Energy(2) < shr2.Energy(2) ? shr2.Energy(2) : shr1.Energy(2) ;
            _pi0_low_radL  = shr1.Energy(2) < shr2.Energy(2) ? radL_shr1 : radL_shr2 ;
            _pi0_high_radL = shr1.Energy(2) < shr2.Energy(2) ? radL_shr2 : radL_shr1 ;

	    if( _pi0_low_radL > 100 || _pi0_high_radL > 80 ) 
	      continue;
	    
         if (_pi0_low_shrE < 40 && _pi0_high_shrE < 40)
	      continue;

         candidate_pairs.push_back(std::make_pair(s1,s2));
         cand_ids.emplace_back(s1);
         cand_ids.emplace_back(s2);
        }// shower ID 2 
      }// shower ID 1 
      if ( candidate_pairs.size() > 1 ) _more_than_one ++ ;

      if( candidate_pairs.size() != 1 || cand_ids.size() != 2 ){

        if( _chain_modules ){
          auto ev_c = storage->get_data<event_cluster>("ImageClusterHit");
          auto ev_p = storage->get_data<event_pfpart>("ImageClusterHit");
          auto ev_v = storage->get_data<event_vertex>(_vtx_producer);
          auto ev_t = storage->get_data<event_track>("numuCC_track");

          ev_c->clear();
          ev_p->clear();
	  ev_v->clear();
	  ev_t->clear();
	}
        return false;
      }

      //std::cout<<"TwoShowerCuts - Found a candidate! "<<std::endl ;

      // Store the new shower data product
      new_shower_v->emplace_back(ev_s->at(cand_ids[0]));
      new_shower_v->emplace_back(ev_s->at(cand_ids[1]));

      // Now also store the associations
      event_cluster *ev_cluster = nullptr;
      auto ass_cluster_v = storage->find_one_ass(ev_s->id(), ev_cluster, ev_s->name());

      std::vector<std::vector<unsigned int> > shower_cluster_v ; 
      shower_cluster_v.reserve(2) ;

      for( int i = 0; i < ass_cluster_v.size(); i ++ ){
         if ( i == cand_ids[0] || i == cand_ids[1] ){
            shower_cluster_v.emplace_back(ass_cluster_v[i] ) ;
            //std::cout<<"shower : "<<shower_cluster_v[i].size()<<std::endl ;
            }
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

  bool TwoShowerCuts::finalize() {

    if(_fout) { 
      _fout->cd(); 
      _pi0_selection->Write(); 
    }

   std::cout<<"\n\n****** "<<_event_list.size()<<" events found by 2 Shower Pi0Reco Module! ******"<<std::endl; 
   for ( auto const & e : _event_list )
     std::cout<<e <<", "; 

   //std::cout<<"\n\n****** "<<_one_shower_list.size()<<" events found by 2 Shower Pi0Reco Module! ******"<<std::endl; 
   //for ( auto const & e : _one_shower_list)
   //  std::cout<<e <<", "; 

  std::cout<<"\n\n\n";

  std::cout<<_more_than_one<<" events renmoved because more than one pair foudn! "<<std::endl ;
  
    return true;
  }

}
#endif
