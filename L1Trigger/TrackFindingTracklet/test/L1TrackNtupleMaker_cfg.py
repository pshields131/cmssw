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
elif GEOMETRY == "tilted": 
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

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

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
    "/store/relval/CMSSW_9_3_0_pre2/RelValSingleMuPt1Extended/GEN-SIM-DIGI-RAW/92X_upgrade2023_realistic_v1_2023D17noPU-v1/00000/26FEADC4-9E68-E711-B172-0025905B8594.root"
)
elif GEOMETRY == "tilted":
    Source_Files = cms.untracked.vstring(
        "file:MuMinus_1to10_TkOnly.root",
        "file:MuPlus_1to10_TkOnly.root",
)
process.source = cms.Source("PoolSource", fileNames = Source_Files)

#process.TFileService = cms.Service("TFileService", fileName = cms.string('TTbar_'+GEOMETRY+'_PU0_EM_1000_trunc_conloose.root'), closeFileFast = cms.untracked.bool(True))
process.TFileService = cms.Service("TFileService", fileName = cms.string('SingMuon_pt1_'+GEOMETRY+'_PU0_EM_1000.root'), closeFileFast = cms.untracked.bool(True))


############################################################
# L1 tracking
############################################################

# remake stubs 

# ===> IMPORTANT !!! stub window tuning as is by default in CMSSW is incorrect !!! <===

process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
from L1Trigger.TrackTrigger.TTStubAlgorithmRegister_cfi import *

if GEOMETRY == "D10": 
    TTStubAlgorithm_official_Phase2TrackerDigi_.zMatchingPS = cms.bool(False)
process.TTClusterStub = cms.Path(process.TrackTriggerClustersStubs)

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
                                       MyProcess = cms.int32(1),
                                       DebugMode = cms.bool(False),      # printout lots of debug statements
                                       SaveAllTracks = cms.bool(True),   # save *all* L1 tracks, not just truth matched to primary particle
                                       SaveStubs = cms.bool(True),      # save some info for *all* stubs
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
                                       )
process.ana = cms.Path(process.L1TrackNtuple)

#process.schedule = cms.Schedule(process.TTClusterStub,process.TTTracksWithTruth,process.ana)
process.schedule = cms.Schedule(process.TTClusterStub,process.TTTracksEmulationWithTruth,process.ana)

