/**
 * \file BackgroundStacks.h
 *
 * \ingroup HitDensity
 * 
 * \brief Class def header for a class BackgroundStacks
 *
 * @author ah673
 */

/** \addtogroup HitDensity

    @{*/

#ifndef LARLITE_BACKGROUNDSTACKS_H
#define LARLITE_BACKGROUNDSTACKS_H

#include "Analysis/ana_base.h"
#include "GeoAlgo/GeoAlgo.h"
#include "LArUtil/SpaceChargeMicroBooNE.h"
#include "DataFormat/track.h"


namespace larlite {
  /**
     \class BackgroundStacks
     User custom analysis class made by SHELL_USER_NAME
   */
  class BackgroundStacks : public ana_base{
  
  public:

    /// Default constructor
    BackgroundStacks(){ _name="BackgroundStacks"; _fout=0; _tree=0; _shower_tree = 0; _univ=0; _get_genie_info=false; 
			      _eventweight_producer = ""; _N = 0; _N_xsec = 0; _beam_min = 3.2; _beam_max = 4.8; _gain = 196.97874008458268; 
			      _recomb = 0.572; _lifetime_corr = 1e20; }

    /// Default destructor
    virtual ~BackgroundStacks(){}

    /** IMPLEMENT in BackgroundStacks.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in BackgroundStacks.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in BackgroundStacks.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void UseMCSample ( bool useit=false ) { _mc_sample = useit; }

    void GetPi0Info  ( bool getit=false ) { _get_pi0_info = getit; }

    void GetSingleShowerInfo  ( bool getit=false ) { _get_single_shower_info = getit; }

    void clear() ;

    void shower_clear() ;

    // The rest is for uncertainty re-weighting
    // Tell the module to calculate uncertainties 
    void GetUncertaintyInfo  ( bool getit=false ) { _get_genie_info = getit; }

    // Pick the uncertainty to vary (genie or flux)
    void SetEWProducer( std::string producer ) { _eventweight_producer = producer ; }

    // This is for the flux uncertainty portion of the analysis
    void SetNominal ( float nom ) { _N = nom ; }

    void SetNominalXsec ( float nom ) { _N_xsec = nom ; }

    void SetBeamWindow(float beam_min, float beam_max) { _beam_min = beam_min; _beam_max = beam_max ; }

    void SetGainRecomLifetime(float gain, float recomb, float lt) { _gain = gain; _recomb = recomb; _lifetime_corr = lt; }

    double Median(std::vector<double> in) ;

    double TrunMean(std::vector<double> in) ;

    double MaxDeflection(larlite::track t );

  protected:

    larutil::SpaceChargeMicroBooNE *_SCE;
    ::geoalgo::GeoAlgo _geoAlgo ;
    double _time2cm;

    // For external study -- identify events with particular kind of background 
    std::vector<int> _event_list ;

    // Counts for final cout
    std::vector<std::string> _bkgd_v ;
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

    int _n_signals;

    // Check osc group question
    int _1gamma ;
 

    // MCCluster hit stuff
    std::map<int,int> _mc_hit_map ;

    // Setter variables
    float _beam_min ;
    float _beam_max ;
    bool _mc_sample ;
    bool _get_pi0_info ;
    bool _get_single_shower_info ;
    bool _get_genie_info ;
    float _N ;
    float _N_xsec ;
    std::string _eventweight_producer ;
    float _gain ;
    float _recomb ;
    float _lifetime_corr ;

    // Flux + GENIE variables
    std::vector<std::string> _genie_label_v ;
    std::vector<std::string> _unisim_label_v ;
    std::vector<std::vector<float>>  _s_weights_by_universe ;
    std::vector<std::vector<float>>  _b_weights_by_universe ;
    std::vector<std::vector<float>>  _t_weights_by_universe ;
    std::vector<std::vector<float>>  _flux_by_universe ; 
    std::map<std::string,int> _label_map ;

    TTree * _tree ;
    int _event ;

    int _event_id ;
    int _subrun_id ;
    int _run_id ;
    int _bkgd_id ;
    int _nu_mode ;
    int _nshrs;
    // Variables related to vertex
    float _vtx_x ;
    float _vtx_y ;
    float _vtx_z ;
    float _mc_vtx_x ;
    float _mc_vtx_y ;
    float _mc_vtx_z ;
    float _mc_scecorr_vtx_x ;
    float _mc_scecorr_vtx_y ;
    float _mc_scecorr_vtx_z ;
    // Variables related to flashes
    float _flash_time ;
    int _flash_pe ;
    float _flash_y_center ;
    float _flash_z_center ;
    float _flash_y_width;
    float _flash_z_width ;
    // Variables related to candidate muon
    float _mu_true_angle ;
    float _mu_true_phi ;
    float _mu_true_mom ;
    float _mu_true_startx ;
    float _mu_true_starty ;
    float _mu_true_startz ;
    float _mu_true_endx ;
    float _mu_true_endy ;
    float _mu_true_endz ;

    float _mu_angle ;
    float _mu_len ;
    float _mu_startx ;
    float _mu_starty ;
    float _mu_startz ;
    float _mu_endx ;
    float _mu_endy ;
    float _mu_endz ;
    float _mu_phi ;
    float _mu_mom;
    float _mult;
    float _mu_purity ;
    float _mu_complete ;
    float _mu_cw_purity ;
    float _mu_cw_complete ;
    float _mu_origin ; // is this mccluster due to nu(1), cosmic(2)
    float _mu_type ;   // is this mccluster due to track(0) or shower(1)
    int _mu_mother_pdg ;
    int _mu_pdg ;

    float _mu_deviation ;
    float _mu_trun_mean_dqdx ;

    // Variables about showers
    int _n_mcs_at_vtx ;
    int _n_reco_at_vtx ;
    // Variables related to pi0 selection
    //float _mc_clus_e_0 ;
    //float _mc_clus_e_1 ;
    float _pi0_mass ;
    float _pi0_mom ;
    float _pi0_oangle ;
    float _pi0_true_oangle ;
    float _pi0_IP;
    //float _pi0_low_shrE;
    float _pi0_low_radL;
    float _pi0_low_IP_w_vtx;
    float _pi0_low_purity;
    float _pi0_low_complete;
    float _pi0_low_cw_purity;
    float _pi0_low_cw_complete;
    float _pi0_low_true_gammaE;
    float _pi0_low_true_detProf_gammaE;
    float _pi0_low_perfect_clustering_E ;
    float _pi0_low_reco_gammaE;
    // Also add some other truth info from the mccluster
    float _pi0_low_origin ;   // is the corresponding mccluster due to nu(1), cosmic(2)
    float _pi0_low_type ;     // is this mccluster due to track(0) or shower(1)
    bool _pi0_low_from_pi0;   // is this mccluster from a pi0? yes(1) no(0)
    int _pi0_low_mother_pdg ; // Mother Pdg of true matched particle
    int _pi0_low_pdg ;        // Pdg of true matched particle
    float _pi0_low_st_x ;
    float _pi0_low_st_y ;
    float _pi0_low_st_z ;
    float _pi0_low_true_st_x ;
    float _pi0_low_true_st_y ;
    float _pi0_low_true_st_z ;
    float _pi0_low_true_scecorr_st_x ;
    float _pi0_low_true_scecorr_st_y ;
    float _pi0_low_true_scecorr_st_z ;
    float _pi0_low_true_detProf_st_x ;
    float _pi0_low_true_detProf_st_y ;
    float _pi0_low_true_detProf_st_z ;
    float _pi0_low_dist_to_nearest_trk ;
    //float _pi0_high_shrE;
    float _pi0_high_radL;
    float _pi0_high_IP_w_vtx;
    float _pi0_high_purity;
    float _pi0_high_complete;
    float _pi0_high_cw_purity;
    float _pi0_high_cw_complete;
    float _pi0_high_true_gammaE;
    float _pi0_high_true_detProf_gammaE;
    float _pi0_high_perfect_clustering_E ;
    float _pi0_high_reco_gammaE;
    float _pi0_high_origin ;   
    float _pi0_high_type ;     
    bool _pi0_high_from_pi0;   
    int _pi0_high_mother_pdg ; 
    int _pi0_high_pdg ;       
    float _pi0_high_st_x ;
    float _pi0_high_st_y ;
    float _pi0_high_st_z ;
    float _pi0_high_true_st_x ;
    float _pi0_high_true_st_y ;
    float _pi0_high_true_st_z ;
    float _pi0_high_true_scecorr_st_x ;
    float _pi0_high_true_scecorr_st_y ;
    float _pi0_high_true_scecorr_st_z ;
    float _pi0_high_true_detProf_st_x ;
    float _pi0_high_true_detProf_st_y ;
    float _pi0_high_true_detProf_st_z ;
    float _pi0_high_dist_to_nearest_trk ;
    // Variables related to single shower selection
    float _gamma_startx;
    float _gamma_starty;
    float _gamma_startz;
    float _gamma_true_startx;
    float _gamma_true_starty;
    float _gamma_true_startz;
    float _gamma_true_detProf_startx;
    float _gamma_true_detProf_starty;
    float _gamma_true_detProf_startz;
    float _gamma_E;
    float _gamma_RL ;
    float _gamma_IP_w_vtx;
    float _gamma_purity; 
    float _gamma_complete; 
    float _gamma_cw_purity; 
    float _gamma_cw_complete; 
    float _gamma_trueE;
    float _gamma_trueE_detProf;
    float _gamma_perfect_clustering_E;
    float _gamma_origin ; 
    float _gamma_type ;  
    bool _gamma_from_pi0;   
    int _gamma_mother_pdg ;
    int _gamma_pdg ;
    // Post technote version v0.9 additions
    int _n_track_hits_0 ;
    int _n_track_hits_1 ;
    int _n_track_hits_2 ;
    int _n_shower_hits_0 ;
    int _n_shower_hits_1 ;
    int _n_shower_hits_2 ;
    int _n_shr_pi0 ;
    int _n_shr_nushr ;
    int _n_shr_nutrk ;
    int _n_shr_cosmic ;
    int _n_shr_noise ;
    bool _signal ;
    std::vector<float> _sel_evts_m1;
    std::vector<float> _sel_evts_p1;

    // One entry per shower
    TTree * _shower_tree ;
    float _shr_startx;
    float _shr_starty;
    float _shr_startz;
    float _shr_true_startx;
    float _shr_true_starty;
    float _shr_true_startz;
    float _shr_true_detProf_startx;
    float _shr_true_detProf_starty;
    float _shr_true_detProf_startz;
    float _shr_startw;
    float _shr_startt;
    float _shr_dirx;
    float _shr_diry;
    float _shr_dirz;
    float _shr_energy;
    float _shr_oangle;
    float _shr_dedx;
    float _shr_vtx_dist;
    float _shr_trk_delta_theta ;
    float _shr_trk_delta_phi ;
    float _shr_purity ;
    float _shr_complete ;
    float _shr_origin ;
    float _shr_type;
    bool _shr_from_pi0 ;
    float _shr_trueE_detProf ;
    float _shr_trueE ;
    float _shr_perfect_clustering_E ;
    float _shr_ip ;
    float _shr_rl;
    int _shr_mother_pdg ;
    int _shr_pdg ;

    int _shr_n_true;
    int _shr_n_reco ;

    // For flux variations  
    TTree * _univ;
    std::vector<std::vector<float>> _xsec_v ;
    std::vector<std::vector<float>> _perc_v;


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
