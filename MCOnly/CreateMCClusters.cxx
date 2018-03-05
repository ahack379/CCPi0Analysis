#ifndef LARLITE_CREATEMCCLUSTERS_CXX
#define LARLITE_CREATEMCCLUSTERS_CXX

#include "CreateMCClusters.h"
#include "DataFormat/mcpart.h"
#include "DataFormat/hit.h"
#include "DataFormat/event_ass.h"
#include "DataFormat/cluster.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

namespace larlite {

  bool CreateMCClusters::initialize() {

    return true;
  }
  
  bool CreateMCClusters::analyze(storage_manager* storage) {

    std::cout<<"\n\nNew event : "<<std::endl ;

    auto ev_ass = storage->get_data<larlite::event_ass>("gaushitTruthMatch");

    //std::cout<<"Hit association tree size: "<<ev_ass->size()<<std::endl ;
     if ( !ev_ass || ev_ass->size() == 0 ) {
       std::cout << "No such association! " << std::endl;
       return false;
     }

    //auto const& ass_hit_v = ev_ass->association(0);
    //auto const& ass_mcpart_v = ev_ass->association(1);

    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
    auto ev_mct = storage->get_data<event_mctrack>("mcreco");

    auto ev_hit = storage->get_data<event_hit>("gaushit");

    auto ev_mcpart = storage->get_data<event_mcpart>("largeant");

    // grab associations by product id
    auto const& ass_hit_mcpart_v = ev_ass->association(ev_hit->id(),ev_mcpart->id());

    // Retrieve cluster data product (output)
    auto ev_mccluster = storage->get_data<event_cluster>("mccluster");
    auto cluster_ass_v = storage->get_data<event_ass>(ev_mccluster->name());

    storage->set_id(ev_hit->run(),ev_hit->subrun(),ev_hit->event_id());

    //std::map<int,std::vector<int>> mcpart_to_hit_map ;
    std::vector<std::vector<unsigned int> > cluster_hit_v;
    cluster_hit_v.resize( 3 *( ev_mcshower->size() + ev_mct->size()) );
    std::vector<larlite::geo::View_t> cluster_plane_v(cluster_hit_v.size(), larlite::geo::View_t::kUnknown);

    std::vector<float> cluster_mcst_id_v(cluster_hit_v.size(), 0);

    // mcparticle (TrackId) to mcshower map
    std::map<int,int> mcp_to_mcs ;

    std::map<int,int> mcp_to_mcst ;

    for (int jj=0; jj < ev_mcshower->size(); jj++){

      auto mcs = ev_mcshower->at(jj); 
        
      for ( auto const & p : mcs.DaughterTrackID() ) mcp_to_mcst[p] = jj ; 

    }

    auto ev_truth = storage->get_data<event_mctruth>("generator");
    auto t = ev_truth->at(0);

    // mcparticle to mctrack map
    std::map<int,int> mcp_to_mct ;

    for (int jj=0; jj < ev_mct->size(); jj++){
      
      auto mct = ev_mct->at(jj); 

      if ( mcp_to_mcst.find(mct.TrackID() ) != mcp_to_mcst.end() ){
         //std::cout<<"So...we can have overlap in trackIDs? "<<mct.TrackID()<<std::endl ;
         continue;
      }
      if (mct.size() < 2 or (mct.Origin() != 1 and mct.Origin() != 2) ){ continue; }

      mcp_to_mcst[mct.TrackID()] = (jj + ev_mcshower->size() ) ;
    }

    std::vector<float> cluster_ts_v(cluster_hit_v.size(), 0);
    std::vector<float> cluster_idx_v(cluster_hit_v.size(), 0);

    // loop over hits
    for (size_t kk=0; kk < ass_hit_mcpart_v.size(); kk++) {

      int hit_idx = kk ;
      auto const& hit    = ev_hit->at(hit_idx);
      auto const& pl     = hit.View();
      auto const& mcpart_id = ass_hit_mcpart_v.at(kk);

      if ( mcpart_id.size() == 0 ) continue; 
      int mcp_trackid = mcpart_id.at(0);

      if ( mcp_to_mcst.find(mcp_trackid) != mcp_to_mcst.end() ){

        int idx = mcp_to_mcst[mcp_trackid] ;
        cluster_hit_v[ pl * (ev_mcshower->size() + ev_mct->size() ) + idx  ].push_back( hit_idx );
        cluster_plane_v[ pl *(ev_mcshower->size() + ev_mct->size() ) + idx ] = pl;
	if ( idx >= ev_mcshower->size() ){
	  cluster_ts_v[ pl * (ev_mcshower->size() + ev_mct->size() ) + idx ] = 0 ; // track
	  cluster_idx_v[ pl * (ev_mcshower->size() + ev_mct->size() ) + idx ] = idx - ev_mcshower->size() ; 
	}
	else{
	  cluster_ts_v[ pl * (ev_mcshower->size() + ev_mct->size() ) + idx ] = 1 ; // shower
	  cluster_idx_v[ pl * (ev_mcshower->size() + ev_mct->size() ) + idx ] = idx ; 
	}

      }
      else continue;
    }

    //std::cout<<" CLusters: "<<cluster_hit_v.size() <<std::endl;

    std::vector<std::vector<unsigned int> > cluster_hit_ass_v;
    for (size_t it=0; it < cluster_hit_v.size(); it++){
      if (cluster_hit_v[it].size() < 4) 
        continue;

      // create a new cluster
      cluster clus;
      clus.set_n_hits(cluster_hit_v[it].size());
      clus.set_view(cluster_plane_v[it]);
      // Indicate whether this index came from mctrack or mcshower
      clus.set_start_opening(cluster_ts_v[it]);
      clus.set_width(cluster_idx_v[it]);
      ev_mccluster->push_back(clus);
      cluster_hit_ass_v.push_back(cluster_hit_v[it]);

    }// for all clusters created

    std::cout<<"Setting association !" <<std::endl ;


    // now save the associations for the cluster
    cluster_ass_v->set_association(ev_mccluster->id(),product_id(data::kHit,ev_hit->name()),
                                   cluster_hit_ass_v);


    return true;
  }

  bool CreateMCClusters::finalize() {

    return true;
  }

}
#endif
