import FWCore.ParameterSet.Config as cms

process = cms.Process('AMFITBASE')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtendedPhase2TkBE5DPixel10DReco_cff')
process.load('Configuration.Geometry.GeometryExtendedPhase2TkBE5DPixel10D_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('L1Trigger.TrackFindingAM.L1AMTrack_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
process.load("Extractors.RecoExtractor.MIB_extractor_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(NEVTS)
)

# Input source
#
# You can use as input file the result of the script AMPR_test.py of part 5.2.2 of the tutorial
#
# Any other EDM file containing patterns and produced with CMSSW 620_SLHC13 should also work
#

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('INPUTFILENAME'),     
                            duplicateCheckMode = cms.untracked.string( 'noDuplicateCheck' )
)

# Additional output definition
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'MYGLOBALTAG', '')

process.TFileService = cms.Service("TFileService", fileName = cms.string('OFFFILENAME'), closeFileFast = cms.untracked.bool(True))

# The name of the stub container over which the association is done, please note that the filtered cluster container is
# not associated due to the lack of simPixelDigis in official samples

process.TTStubAssociatorFromPixelDigis.TTStubs        = cms.VInputTag( cms.InputTag("MergeFITOutput", "StubInTrack"))
process.TTStubAssociatorFromPixelDigis.TTClusterTruth = cms.VInputTag( cms.InputTag("TTClusterAssociatorFromPixelDigis","ClusterAccepted"))
process.TTTrackAssociatorFromPixelDigis.TTTracks      = cms.VInputTag( cms.InputTag("MergeFITOutput", "AML1Tracks"))


process.TTTracksTAMUFromTC.ConstantsDir                = cms.FileInPath("L1Trigger/TrackFindingAM/data/PreEstimate_Transverse/matrixVD_2016.txt")

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('OUTPUTFILENAME'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    )
)

# For the moment need to explicitely keep the following containers
# (not yet in the customizing scripts)

process.MIBextraction.doMatch          = True
process.MIBextraction.doMC             = True
process.MIBextraction.doSTUB           = True
process.MIBextraction.doL1TRK          = True

process.MIBextraction.L1pattern_tag    = cms.InputTag( "MergePROutput", "AML1Patterns")
process.MIBextraction.L1tc_tag         = cms.InputTag( "MergeTCOutputb", "AML1BinTCs")
process.MIBextraction.L1track_tag      = cms.InputTag( "MergeFITOutput", "AML1Tracks")
process.MIBextraction.CLUS_container   = cms.string( "TTStubsFromPixelDigis")
process.MIBextraction.CLUS_name        = cms.string( "ClusterAccepted" )
process.MIBextraction.extractedRootFile= cms.string('EXTRFILENAME')

# Keep the PR and TC outputs
process.RAWSIMoutput.outputCommands.append('keep  *_*_*_AMPR*')
process.RAWSIMoutput.outputCommands.append('keep  *_*_*_AMTC*')

# Keep the FIT output
process.RAWSIMoutput.outputCommands.append('keep  *_*_*_AMFIT*')
process.RAWSIMoutput.outputCommands.append('drop *_TTTracks*From*_*_*')
process.RAWSIMoutput.outputCommands.append('keep  *_*_MergedTrackTruth_*')

process.L1TrackNtuple = cms.EDAnalyzer('L1TrackNtupleMaker',
                                       MyProcess = cms.int32(1),
                                       NTrkPar   = cms.int32(5),
                                       DebugMode = cms.bool(False),
                                       SaveAllTracks = cms.bool(False),
                                       DoPixelTrack = cms.bool(False),
                                       SaveStubs = cms.bool(False),
                                       L1TrackInputTag = cms.InputTag("MergeFITOutput", "AML1Tracks"),               ## TTTrack input
                                       MCTruthTrackInputTag = cms.InputTag("TTTrackAssociatorFromPixelDigis", "AML1Tracks") ## MCTruth input 

                                       )

# Path and EndPath definitions
process.L1AMFIT_step         = cms.Path(process.TTTracksFromTCswStubs)
process.p                    = cms.Path(process.MIBextraction)
process.ana                  = cms.Path(process.L1TrackNtuple)
process.endjob_step          = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step    = cms.EndPath(process.RAWSIMoutput)

process.schedule = cms.Schedule(process.L1AMFIT_step,process.p,process.ana,process.endjob_step,process.RAWSIMoutput_step)

#process.schedule = cms.Schedule(process.L1AMFIT_step,process.p,process.endjob_step,process.RAWSIMoutput_step)


# Automatic addition of the customisation function

from SLHCUpgradeSimulations.Configuration.combinedCustoms import customiseBE5DPixel10D
from SLHCUpgradeSimulations.Configuration.combinedCustoms import customise_ev_BE5DPixel10D

process=customiseBE5DPixel10D(process)
process=customise_ev_BE5DPixel10D(process)

# End of customisation functions
