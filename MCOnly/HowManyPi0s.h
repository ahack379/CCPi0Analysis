/**
 * \file HowManyPi0s.h
 *
 * \ingroup HitDensity
 * 
 * \brief Class def header for a class HowManyPi0s
 *
 * @author ah673
 */

/** \addtogroup HitDensity

    @{*/

#ifndef LARLITE_HOWMANYPI0S_H
#define LARLITE_HOWMANYPI0S_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class HowManyPi0s
     User custom analysis class made by SHELL_USER_NAME
   */
  class HowManyPi0s : public ana_base{
  
  public:

    /// Default constructor
    HowManyPi0s(){ _name="HowManyPi0s"; _fout=0;}

    /// Default destructor
    virtual ~HowManyPi0s(){}

    /** IMPLEMENT in HowManyPi0s.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in HowManyPi0s.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in HowManyPi0s.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

  int _event ;
  int _signal ;

  int _n_other ;
  int _n_cosmic ;
  int _n_cc1pi0 ; // This is the signal
  int _n_cc0pi0 ;
  int _n_nc1pi0 ; 
  int _n_nc0pi0 ;

  std::vector<int> _event_list ;
  std::multimap<float,float> _map;

  float _tot_pot ;

  int _n_nc_1gamma ;
  int _n_cc_1gamma ;

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
