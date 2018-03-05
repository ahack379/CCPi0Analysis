from algoconfig import loadAlgo
from algoconfig_nu import loadAlgo as loadAlgo_nu
from showerconfig import getShowerRecoAlgModular 

import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from larlite import larlite as fmwk
from ROOT import protoshower

my_proc = fmwk.ana_processor()

for x in xrange(len(sys.argv) - 1 ): 
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify data output root file name
my_proc.set_output_file("showerreco_out.root") 

my_proc.set_ana_output_file("showerreco_ana.root");

# 0) Translate Selection II associations into data products
search = fmwk.SaveSel2VtxTrk()
search.SetVerbose(False)
my_proc.add_process(search)

# 1) Create MCClusters
mcpart = fmwk.CreateMCClusters()
my_proc.add_process(mcpart)

my_proc.enable_event_alignment(False)

my_proc.enable_filter(False)

# 2) Prepare the various hit removal stages
my_proc.add_process( loadAlgo("ROIRemoval") )
my_proc.add_process( loadAlgo("VertexSlopeCorrelation") )
my_proc.add_process( loadAlgo("VertexAngleCorrelation") )
my_proc.add_process( loadAlgo("RemoveDeltaRays") )

my_proc.add_process(loadAlgo_nu("VertexProximityRemoval") )
my_proc.add_process(loadAlgo_nu("PandoraLinearRemoval") )
my_proc.add_process(loadAlgo_nu("TrackDeltaRayRemoval") )
my_proc.add_process(loadAlgo_nu("SimpleClusterer") )
my_proc.add_process(loadAlgo_nu("VertexTrackRemoval") )
my_proc.add_process(loadAlgo_nu("LinearRemoval") )
my_proc.add_process(loadAlgo_nu("RemoveHitsNearVtx") )

cfg  = 'MCBNB_cosmics_BBox.fcl'

def DefaultShowerReco3D():

    ana_unit = fmwk.ShowerReco3D()
    ana_unit.SetRequirePDG11(False)
    sralg = getShowerRecoAlgModular()
    ana_unit.AddShowerAlgo(sralg)

    return ana_unit

# 3) After hit removal, run OpenCV clustering
myunit = fmwk.LArImageHit()
myunit.set_config(cfg)
my_proc.add_process(myunit,False)

# 4) Finally, reconstruct showers!
protoshoweralg = protoshower.ProtoShowerAlgOpenCV()
shr_unit = DefaultShowerReco3D()
shr_unit.GetProtoShowerHelper().setProtoShowerAlg( protoshoweralg )
shr_unit.SetInputProducer("ImageClusterHit")
shr_unit.SetOutputProducer("showerreco")
my_proc.add_process(shr_unit)

# Output we save
# Selection II associations translated to data products 
my_proc.set_data_to_write(fmwk.data.kVertex,  "numuCC_vertex"  )
my_proc.set_data_to_write(fmwk.data.kTrack,   "numuCC_track"  )

# Backtracker
my_proc.set_data_to_write(fmwk.data.kCluster,     "mccluster")
my_proc.set_data_to_write(fmwk.data.kAssociation, "mccluster")
my_proc.set_data_to_write(fmwk.data.kAssociation,   "gaushitTruthMatch"  )

# HitRemoval Stuff
my_proc.set_data_to_write(fmwk.data.kHit,         "gaushit"        )
my_proc.set_data_to_write(fmwk.data.kHit,         "pandoraCosmicHitRemoval"        )
my_proc.set_data_to_write(fmwk.data.kCluster,     "pandoraCosmic"  )
my_proc.set_data_to_write(fmwk.data.kAssociation, "pandoraCosmic" )

# OpenCV Stuff
my_proc.set_data_to_write(fmwk.data.kCluster,     "ImageClusterHit"  )
my_proc.set_data_to_write(fmwk.data.kAssociation, "ImageClusterHit"  )
my_proc.set_data_to_write(fmwk.data.kPFParticle,  "ImageClusterHit"  )

# Showerreco Stuff
my_proc.set_data_to_write(fmwk.data.kShower,      "showerreco")
my_proc.set_data_to_write(fmwk.data.kAssociation, "showerreco")

# Other Things 
my_proc.set_data_to_write(fmwk.data.kTrack,   "pandoraNu"  )
my_proc.set_data_to_write(fmwk.data.kAssociation,   "pandoraNu"  )
my_proc.set_data_to_write(fmwk.data.kCalorimetry,   "pandoraNucalo"  )
my_proc.set_data_to_write(fmwk.data.kAssociation,   "pandoraNucalo"  )
my_proc.set_data_to_write(fmwk.data.kOpFlash,   "simpleFlashBeam"  )

# MC Stuff 
my_proc.set_data_to_write(fmwk.data.kMCShower,    "mcreco"         )
my_proc.set_data_to_write(fmwk.data.kMCTrack,     "mcreco"         )
my_proc.set_data_to_write(fmwk.data.kVertex,     "mcvertex"         )
my_proc.set_data_to_write(fmwk.data.kMCTruth,     "generator"      )

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run() 

sys.exit()
