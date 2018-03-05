import sys
from ROOT import gSystem
gSystem.Load("libCCpi0Analysis_Filters")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Filters..."

sys.exit(0)

