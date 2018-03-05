/**
 * \file CreateMCClusters.h
 *
 * \ingroup Misc
 * 
 * \brief Class def header for a class CreateMCClusters
 *
 * @author ah673
 */

/** \addtogroup Misc

    @{*/

#ifndef LARLITE_CREATEMCCLUSTERS_H
#define LARLITE_CREATEMCCLUSTERS_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class CreateMCClusters
     User custom analysis class made by SHELL_USER_NAME
   */
  class CreateMCClusters : public ana_base{
  
  public:

    /// Default constructor
    CreateMCClusters(){ _name="CreateMCClusters"; _fout=0;}

    /// Default destructor
    virtual ~CreateMCClusters(){}

    /** IMPLEMENT in CreateMCClusters.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CreateMCClusters.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CreateMCClusters.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
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
