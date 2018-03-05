import sys
from ROOT import gSystem
gSystem.Load("libCCpi0Analysis_MCOnly")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing MCOnly..."

sys.exit(0)

