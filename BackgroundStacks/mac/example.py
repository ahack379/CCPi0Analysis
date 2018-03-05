import sys
from ROOT import gSystem
gSystem.Load("libCCpi0Analysis_BackgroundStacks")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing BackgroundStacks..."

sys.exit(0)

