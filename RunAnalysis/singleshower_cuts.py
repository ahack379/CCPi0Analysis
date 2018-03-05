import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from larlite import larlite as fmwk

my_proc = fmwk.ana_processor()

for x in xrange(len(sys.argv)-1): 
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

my_proc.enable_filter(True)

# Specify data output root file name
my_proc.set_output_file("singleshower_out.root") 
my_proc.set_ana_output_file("singleshower_ana.root")

pi0 = fmwk.SingleShowerCuts()
my_proc.add_process(pi0,True)

# Output we save
# HitRemoval Stuff
#my_proc.set_data_to_write(fmwk.data.kHit,         "gaushit"        )
#my_proc.set_data_to_write(fmwk.data.kCluster,     "pandoraCosmic"  )
#my_proc.set_data_to_write(fmwk.data.kAssociation, "pandoraCosmic" )
#
## OpenCV Stuff
#my_proc.set_data_to_write(fmwk.data.kCluster,     "ImageClusterHit"  )
#my_proc.set_data_to_write(fmwk.data.kAssociation, "ImageClusterHit"  )
#my_proc.set_data_to_write(fmwk.data.kPFParticle,  "ImageClusterHit"  )
#
#my_proc.set_data_to_write(fmwk.data.kCluster,     "mccluster"  )
#my_proc.set_data_to_write(fmwk.data.kAssociation, "mccluster"  )
#
## Showerreco Stuff
#my_proc.set_data_to_write(fmwk.data.kShower,      "showerreco")
#my_proc.set_data_to_write(fmwk.data.kAssociation, "showerreco")
#
## Pi0Reco Stuff
#my_proc.set_data_to_write(fmwk.data.kShower,      "pi0_1gamma_candidate_showers")
#my_proc.set_data_to_write(fmwk.data.kAssociation, "pi0_1gamma_candidate_showers")
#
## Other Things 
#my_proc.set_data_to_write(fmwk.data.kVertex,  "numuCC_vertex"  )
#my_proc.set_data_to_write(fmwk.data.kTrack,   "numuCC_track"  )
#my_proc.set_data_to_write(fmwk.data.kTrack,   "pandoraNu"  )
#
## MC Stuff 
#my_proc.set_data_to_write(fmwk.data.kMCShower,    "mcreco"         )
#my_proc.set_data_to_write(fmwk.data.kMCTrack,     "mcreco"         )
#my_proc.set_data_to_write(fmwk.data.kMCTruth,     "generator"      )

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run() 

sys.exit()

