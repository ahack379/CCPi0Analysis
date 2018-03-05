# CCpi0Analysis

This repository contains all modules needed to re-run ccpi0 analysis

Filters
0) TwoShowerCuts -> Produces a sample with only events that pass two shower cuts
1) SingleShowerCuts -> Produces a sample with only events that pass single shower cuts

MCOnly
0) MCEfficiency -> Used to produces efficiency plots for all stages of selection as shown in Chapter 7 of technote
1) HowManyPi0s -> Used for POT counting and to understand how many signals exist is full starting sample
2) CreateMCClusters -> Build truth level information based on the hit - mcparticle associations stored during selection II run. The output of this module are "mccluster"s that track back to the mctrack/shower that produced them.  This is necessary module to do backtracking for signal + background assignment later
3) ShowerRecoEffWithMCClusters -> Use the mcclusters produced by 2) to backtrack and calculate shower reconstruction efficiency

BackgroundStacks
0) BackgroundStacks -> Fills massive trees per event depending on user configurations to make data-MC comparisons