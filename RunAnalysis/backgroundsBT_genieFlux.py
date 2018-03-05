import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

my_proc.set_ana_output_file('BT_backgrounds_uncertainty.root')

name = 'twoshower'
useTwoShower = False
useOneShower = False

if name =='twoshower':
  useTwoShower = True
elif name =='singleshower':
  useOneShower = True

ana = fmwk.BackgroundTruthMatchBT()
ana.UseMCSample(True)
ana.GetPi0Info(useTwoShower)
ana.GetSingleShowerInfo(useOneShower)
ana.SetBeamWindow(3.2,4.8)
ana.SetGainRecombLifetime(196.97874008458268,0.572,1e20)
ana.GetUncertaintyInfo(True)
ana.SetEWProducer("genieeventweight")

nominal = 69 
nominal_xsec = 2.56e-38 

if name == 'singleshower':
  nominal = 241.8212
  nominal_xsec = 2.64e-38 

ana.SetNominalXsec(nominal_xsec)
ana.SetNominal(nominal)

my_proc.add_process(ana)

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
