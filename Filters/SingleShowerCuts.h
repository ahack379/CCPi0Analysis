/**
 * \file SingleShowerCuts.h
 *
 * \ingroup Filters
 * 
 * \brief Class def header for a class SingleShowerCuts
 *
 * @author ah673
 */

/** \addtogroup Filters

    @{*/

#ifndef LARLITE_SINGLESHOWERCUTS_H
#define LARLITE_SINGLESHOWERCUTS_H

#include "Analysis/ana_base.h"
#include "GeoAlgo/GeoAlgo.h"
#include "LArUtil/GeometryHelper.h"


namespace larlite {
  /**
     \class SingleShowerCuts
     User custom analysis class made by SHELL_USER_NAME
   */
  class SingleShowerCuts : public ana_base{
  
  public:

    /// Default constructor
    SingleShowerCuts(){ _name="SingleShowerCuts"; _fout=0; _pi0_selection=0; }

    /// Default destructor
    virtual ~SingleShowerCuts(){}

    /** IMPLEMENT in SingleShowerCuts.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in SingleShowerCuts.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in SingleShowerCuts.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void clear();

  protected:

    TTree * _pi0_selection;
    int _event;
    float _pi0_mass;
    float _pi0_mom;
    float _pi0_oangle;
    float _pi0_low_shrE;
    float _pi0_high_shrE;
    float _pi0_low_radL;
    float _pi0_high_radL;
    float _mu_mom;
    float _mu_angle;

    ::geoalgo::GeoAlgo _geoAlgo ;
    std::vector<int> _event_list ;    
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
