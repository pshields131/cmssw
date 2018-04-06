############################################################
# define basic process
############################################################

import FWCore.ParameterSet.Config as cms
import os
process = cms.Process("L1TrackNtuple")

GEOMETRY = "D17"

 
############################################################
# import standard configurations
############################################################

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')

if GEOMETRY == "D10": 
    print "using geometry " + GEOMETRY + " (flat)"
    process.load('Configuration.Geometry.GeometryExtended2023D10Reco_cff')
    process.load('Configuration.Geometry.GeometryExtended2023D10_cff')
elif GEOMETRY == "D17":
    print "using geometry " + GEOMETRY + " (tilted)"
    process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
    process.load('Configuration.Geometry.GeometryExtended2023D17_cff')
elif GEOMETRY == "TkOnly": 
    print "using standalone tilted (T5) tracker geometry" 
    process.load('L1Trigger.TrackTrigger.TkOnlyTiltedGeom_cff')
else:
    print "this is not a valid geometry!!!"

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')


############################################################
# input and output
############################################################


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10000))

if GEOMETRY == "D17":
    #D17 (tilted barrel -- latest and greatest with T5 tracker, see: https://github.com/cms-sw/cmssw/blob/CMSSW_9_3_0_pre2/Configuration/Geometry/README.md)
    Source_Files = cms.untracked.vstring(
    # ttbar
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/002E1FCB-8168-E711-BD97-0CC47A4C8EA8.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/48041B1F-8268-E711-B399-0CC47A4C8EB6.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/60B5CE52-8168-E711-9440-0025905A6118.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/8635D976-7F68-E711-A61C-0CC47A4D765E.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/AC12EC5C-7E68-E711-B924-0025905A613C.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValSingleMuPt10Extended/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/406F81DA-9D68-E711-B026-0CC47A7C3434.root"
    #"/store/relval/CMSSW_9_3_0_pre2/RelValSingleMuPt1Extended/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/26FEADC4-9E68-E711-B172-0025905B8594.root"
    # ttbar PU200
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_92X_upgrade2023_realistic_v1_2023D17PU200-v4/00000/028AC021-016B-E711-B2D0-24BE05C4F8D2.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_92X_upgrade2023_realistic_v1_2023D17PU200-v4/00000/06A77FC1-036B-E711-88BA-E0071B7AA7D0.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_92X_upgrade2023_realistic_v1_2023D17PU200-v4/00000/06ADDEEB-006B-E711-9175-E0071B6B26F0.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_92X_upgrade2023_realistic_v1_2023D17PU200-v4/00000/0C68A723-036B-E711-95C5-E0071B7AE7D0.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_92X_upgrade2023_realistic_v1_2023D17PU200-v4/00000/109672A6-006B-E711-8628-80000025FE80.root",
    #"/store/relval/CMSSW_9_3_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_92X_upgrade2023_realistic_v1_2023D17PU200-v4/00000/12791FD3-006B-E711-97D4-80000025FE80.root",

    #"file:SingMuMinus/step2_90.root",
    #"file:SingMuMinus/step2_91.root",
    #"file:SingMuMinus/step2_92.root",
    #"file:SingMuMinus/step2_100.root",
    #"file:SingMuMinus/step2_82.root",
    "file:SingMuMinus/step2_83.root",
    "file:SingMuMinus/step2_85.root",
    "file:SingMuMinus/step2_87.root",
    "file:SingMuMinus/step2_89.root",  
    "file:SingMuMinus/step2_94.root"

    #"file:SingMuPlus/step2_95.root",
    #"file:SingMuPlus/step2_96.root",
    #"file:SingMuPlus/step2_97.root",
    #"file:SingMuPlus/step2_101.root",
    #"file:SingMuPlus/step2_84.root",
    #"file:SingMuPlus/step2_86.root",
    #"file:SingMuPlus/step2_88.root",
    #"file:SingMuPlus/step2_93.root",
    #"file:SingMuPlus/step2_98.root",
    #"file:SingMuPlus/step2_99.root"
    
)
elif GEOMETRY == "tilted":
    Source_Files = cms.untracked.vstring(
    #"file:Electron_1to50_TkOnly.root",
    #"file:Positron_1to50_TkOnly.root",
    "file:MuPlus_1to50_TkOnly.root",
    "file:MuMinus_1to50_TkOnly.root",
)
process.source = cms.Source("PoolSource", fileNames = Source_Files)

#process.TFileService = cms.Service("TFileService", fileName = cms.string('TTbar_'+GEOMETRY+'_PU0_EM_1000_trunc_conloose.root'), closeFileFast = cms.untracked.bool(True))
process.TFileService = cms.Service("TFileService", fileName = cms.string('SingleMuMinus2to10_'+GEOMETRY+'_PU0_EM_10000_2.root'), closeFileFast = cms.untracked.bool(True))

############################################################
# L1 tracking
############################################################

# remake stubs ?
process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
from L1Trigger.TrackTrigger.TTStubAlgorithmRegister_cfi import *
process.load("SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff")

if GEOMETRY == "D10": 
    TTStubAlgorithm_official_Phase2TrackerDigi_.zMatchingPS = cms.bool(False)

if GEOMETRY != "TkOnly":
    from SimTracker.TrackTriggerAssociation.TTClusterAssociation_cfi import *
    TTClusterAssociatorFromPixelDigis.digiSimLinks = cms.InputTag("simSiPixelDigis","Tracker")

process.TTClusterStub = cms.Path(process.TrackTriggerClustersStubs)
process.TTClusterStubTruth = cms.Path(process.TrackTriggerAssociatorClustersStubs)

from L1Trigger.TrackFindingTracklet.Tracklet_cfi import *

#### floating-point version

#process.load("L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff")
#if GEOMETRY == "D10": 
#    TTTracksFromTracklet.trackerGeometry = cms.untracked.string("flat")

#TTTracksFromTracklet.asciiFileName = cms.untracked.string("evlist.txt")

## run only the tracking (no MC truth associators)
#process.TTTracks = cms.Path(process.L1TrackletTracks)

## run the tracking AND MC truth associators)
#process.TTTracksWithTruth = cms.Path(process.L1TrackletTracksWithAssociators)


### emulation instead 
process.load("L1Trigger.TrackFindingTracklet.L1TrackletEmulationTracks_cff")
process.TTTracksEmulation = cms.Path(process.L1TrackletEmulationTracks)
process.TTTracksEmulationWithTruth = cms.Path(process.L1TrackletEmulationTracksWithAssociators)


#TTTracksFromTrackletEmulation.asciiFileName = cms.untracked.string("evlist.txt")


############################################################
# Define the track ntuple process, MyProcess is the (unsigned) PDGID corresponding to the process which is run
# e.g. single electron/positron = 11
#      single pion+/pion- = 211
#      single muon+/muon- = 13 
#      pions in jets = 6
#      taus = 15
#      all TPs = 1
############################################################

process.L1TrackNtuple = cms.EDAnalyzer('L1TrackNtupleMaker',
                                       MyProcess = cms.int32(13),
                                       DebugMode = cms.bool(False),      # printout lots of debug statements
                                       SaveAllTracks = cms.bool(True),   # save *all* L1 tracks, not just truth matched to primary particle
                                       SaveStubs = cms.bool(True),       # save some info for *all* stubs
                                       L1Tk_nPar = cms.int32(4),         # use 4 or 5-parameter L1 track fit ??
                                       L1Tk_minNStub = cms.int32(4),     # L1 tracks with >= 4 stubs
                                       TP_minNStub = cms.int32(4),       # require TP to have >= X number of stubs associated with it
                                       TP_minNStubLayer = cms.int32(4),  # require TP to have stubs in >= X layers/disks
                                       TP_minPt = cms.double(2.0),       # only save TPs with pt > X GeV
                                       TP_maxEta = cms.double(2.4),      # only save TPs with |eta| < X
                                       TP_maxZ0 = cms.double(30.0),      # only save TPs with |z0| < X cm
                                       #L1TrackInputTag = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks"),               ## TTTrack input
                                       L1TrackInputTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),               ## TTTrack input
                                       MCTruthTrackInputTag = cms.InputTag("TTTrackAssociatorFromPixelDigis", "Level1TTTracks"), ## MCTruth input 
                                       # other input collections
                                       L1StubInputTag = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
                                       MCTruthClusterInputTag = cms.InputTag("TTClusterAssociatorFromPixelDigis", "ClusterAccepted"),
                                       MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
                                       TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                       TrackingVertexInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                       tiltedGeometryFile = cms.FileInPath('L1Trigger/TrackFindingTracklet/test/allCoordinates.csv'),
                                       ## tracking in jets stuff (--> requires AK4 genjet collection present!)
                                       TrackingInJets = cms.bool(True),
                                       GenJetInputTag = cms.InputTag("ak4GenJets", ""),
                                       )
process.ana = cms.Path(process.L1TrackNtuple)

# use this if you want to re-run the stub making
#process.schedule = cms.Schedule(process.TTClusterStub,process.TTClusterStubTruth,process.TTTracksEmulationWithTruth,process.ana)

# use this if cluster/stub associators not available 
#process.schedule = cms.Schedule(process.TTClusterStubTruth,process.TTTracksEmulationWithTruth,process.ana)

# use this to only run tracking + track associator
process.schedule = cms.Schedule(process.TTTracksEmulationWithTruth,process.ana)

