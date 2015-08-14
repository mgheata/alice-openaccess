/* $Id: CreateCuts.C,v 1.5 2008/01/11 08:28:52 jgrosseo Exp $ */

// this macro creates the track cuts used in this analysis

AliESDtrackCuts* CreateTrackCuts(AliPWG0Helper::AnalysisMode analysisMode,  Bool_t hists = kTRUE,  Float_t ptMin = 0,  Float_t etacut = 1e10)
{
  AliESDtrackCuts* esdTrackCuts = 0;

  // The standard cuts for the different detector options are created. The different options are:
  // 17: kTPC - Time Projection Chamber only
  // 30: kTPCITS - Time Projection Chamber and whole Inner Tracking System
  // 38: kTPCSPD - Time Projection Chamber and Silicon Pixel Detecor
  // 44: kFiledOn - Is magnetic B-field off?
  // For further information see
  // https://twiki.cern.ch/twiki/bin/view/ALICE/SelectionOfPrimaryTracksForPpDataAnalysis

  if (analysisMode & AliPWG0Helper::kTPC)
  {
    esdTrackCuts = new AliESDtrackCuts("AliESDtrackCuts");
    esdTrackCuts->SetMaxDCAToVertexZ(3.2);
    esdTrackCuts->SetMaxDCAToVertexXY(2.4);
    esdTrackCuts->SetDCAToVertex2D(kTRUE);
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetAcceptKinkDaughters(kFALSE);
    esdTrackCuts->SetMinNClustersTPC(70);
    esdTrackCuts->SetMaxChi2PerClusterTPC(4);
    TString tag("TPC-only tracking");
  }

  if (analysisMode & AliPWG0Helper::kTPCITS)
  {
    esdTrackCuts = AliESDtrackCuts::GetStandardITSTPCTrackCuts2010(kTRUE);
    esdTrackCuts->SetPtRange(ptMin); // adding pt cut
    esdTrackCuts->SetEtaRange(-etacut, etacut); // adding eta cut
    TString tag("Global tracking");
  }

  if ( analysisMode & AliPWG0Helper::kTPCSPD)
  {
    esdTrackCuts = AliESDtrackCuts::GetStandardITSTPCTrackCuts2010(kFALSE);
    TString tag("Global tracking+tracklets");
  }

  if (!(analysisMode & AliPWG0Helper::kFieldOn)) tag += " without field";

  if (hists) esdTrackCuts->DefineHistograms(1);

  Printf("Created track cuts for: %s", tag.Data());

  return esdTrackCuts;
}

