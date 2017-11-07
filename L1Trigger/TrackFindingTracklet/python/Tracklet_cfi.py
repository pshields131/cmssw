import FWCore.ParameterSet.Config as cms

TTTracksFromTracklet = cms.EDProducer("L1TrackProducer",
                                      SimTrackSource = cms.InputTag("g4SimHits"),
                                      SimVertexSource = cms.InputTag("g4SimHits"),
                                      TTStubSource = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
                                      BeamSpotSource = cms.InputTag("offlineBeamSpot"),
                                      asciiFileName = cms.untracked.string(""),
                                      trackerGeometryType  = cms.untracked.string("")  #tilted barrel is assumed, use "flat" if running on flat
    )

TTTracksFromTrackletEmulation = cms.EDProducer("L1FPGATrackProducer",
                                               # general L1 tracking inputs
                                               SimTrackSource = cms.InputTag("g4SimHits"),
                                               SimVertexSource = cms.InputTag("g4SimHits"),
                                               TTStubSource = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
                                               BeamSpotSource = cms.InputTag("offlineBeamSpot"),
                                               trackerGeometryType  = cms.untracked.string(""),  #tilted barrel is assumed, use "flat" if running on flat
                                               tiltedGeometryFile = cms.FileInPath('L1Trigger/TrackFindingTracklet/test/allCoordinates.csv'),
                                               # specific emulation inputs
                                               fitPatternFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/test/fitpattern.txt'),
                                               memoryModulesFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/test/memorymodules_new.dat'),
                                               processingModulesFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/test/processingmodules_new.dat'),
                                               wiresFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/test/wires_new.dat')
    )
