/**
 * \file ShowerRecoEffWithMCClusters.h
 *
 * \ingroup HitDensity
 * 
 * \brief Class def header for a class ShowerRecoEffWithMCClusters
 *
 * @author ah673
 */

/** \addtogroup HitDensity

    @{*/

#ifndef LARLITE_SHOWERRECOEFFWITHMCCLUSTERS_H
#define LARLITE_SHOWERRECOEFFWITHMCCLUSTERS_H

#include "Analysis/ana_base.h"
#include "GeoAlgo/GeoAlgo.h"

#include "LArUtil/SpaceChargeMicroBooNE.h"

namespace larlite {
  /**
     \class ShowerRecoEffWithMCClusters
     User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerRecoEffWithMCClusters : public ana_base{
  
  public:

    /// Default constructor
    ShowerRecoEffWithMCClusters(){ _name="ShowerRecoEffWithMCClusters"; _fout=0; _tree=0; }

    /// Default destructor
    virtual ~ShowerRecoEffWithMCClusters(){}

    /** IMPLEMENT in ShowerRecoEffWithMCClusters.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerRecoEffWithMCClusters.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerRecoEffWithMCClusters.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void clear() ;

  protected:

  std::vector<int> _event_list ;

  int _n_noise;

  int _n_cosmic ;
  int _n_cc1pi0 ; // This is the signal
  int _n_nc1pi0 ; 

  int _n_cc1pi0_outFV ; // This is the signal
  int _n_multpi0 ;
  int _n_cccex ;
  int _n_nccex ;
  int _n_nue ;
  int _n_antimu ;
  int _n_ncother;
  int _n_ccother ;

  int _n_gamma ;
  int _n_kaondecay;
  int _n_other ;

  TTree * _tree ;
  int _event ;
  int _bkgd_id ;

  std::map<int,int> _mc_hit_map ;

  float _purity;
  float _complete;
  float _cw_purity;
  float _cw_complete;
  float _mc_e;
  float _mc_detProf_e;
  float _mc_clus_e;
  float _mc_st_x ;
  float _mc_st_y ;
  float _mc_st_z ;
  float _mc_dir_x ;
  float _mc_dir_y ;
  float _mc_dir_z ;
  float _mc_detProf_x ;
  float _mc_detProf_y ;
  float _mc_detProf_z ;
  // MC Direction is NOT normalized by default; we will normalize here
  float _mc_dir_sce_corr_x ;
  float _mc_dir_sce_corr_y ;
  float _mc_dir_sce_corr_z ;
  float _mc_detProf_sce_corr_x ;
  float _mc_detProf_sce_corr_y ;
  float _mc_detProf_sce_corr_z ;


  float _origin ; // is the corresponding mccluster due to nu(1), cosmic(2), noise(3)
  float _type ;   // is this mccluster due to track(0) or shower(1)
  bool _from_pi0;   // is this mccluster from a pi0? yes(1) no(0)

  float _reco_e;
  float _st_x ;
  float _st_y ;
  float _st_z ;
  // Reco direction is already normalized
  float _dir_x ;
  float _dir_y ;
  float _dir_z ;


  ::geoalgo::GeoAlgo _geoAlgo ;

  std::vector<std::string> _bkgd_v ;

 larutil::SpaceChargeMicroBooNE *_SCE;
 double _time2cm;

 float _n_recod_true_showers ;
 float _n_true_showers ;

 float _low_shr_e ;
 float _high_shr_e ;

 int _out_of_av ;
 int _tot_shr ;

    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
