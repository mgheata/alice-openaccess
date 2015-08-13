void correct(const char* dataInput = "analysis_esd_raw.root", const char* dataOutput = "analysis_esd.root", const char* correctionMapFile = "correction_map.root", const char* correctionMapFolder = "dndeta_correction")
{
  // Open the corrention map and initialize the correction algorithm (AlidNdEtaCorrection)
  AlidNdEtaCorrection* dNdEtaCorrection = 0;
  if (correctionMapFile)
  {
    dNdEtaCorrection = new AlidNdEtaCorrection(correctionMapFolder, correctionMapFolder);
    if (!TFile::Open(correctionMapFile)) return;
    dNdEtaCorrection->LoadHistograms();
  }
  // Now open the input file, i.e. the raw data analysis, as well as the output file
  TFile* file = TFile::Open(dataInput);
  if (!file)
  {
    cout << "Error. File not found" << endl;
    return;
  }
  TFile* file2 = TFile::Open(dataOutput, "RECREATE");

  // Background ?????
  Int_t backgroundEvents = 0;
  //backgroundEvents = 1162+434; // Michele for MB1, run 104892, 15.02.10
  //backgroundEvents = 842;    // JF estimate for V0 systematic case 1
  //backgroundEvents = 6;          // Michele for V0AND, run 104892, 15.02.10
  //backgroundEvents = 1758+434; // MB1, run 104867-92
  //backgroundEvents = 4398+961;   // Michele for MB1, run 104824-52, 16.02.10
  //backgroundEvents = 19;         // Michele for V0AND, run 104824-52, 16.02.10
  backgroundEvents = -1;    // use 0 bin from MC! for 2.36 TeV
  //backgroundEvents = 900; // my estimate for 2.36 TeV
  //backgroundEvents = 918;   // V0OR for run 114786 w/o bunch intensities w/o proper 0 checking!
  //backgroundEvents = 723; // V0OR for run 114798 w/o bunch intensities w/o proper 0 checking!
  Printf("Subtracting %d background events!!!", backgroundEvents);
  gSystem->Sleep(1000);

  // Now the correction is applied to the data. The correction can be done for several
  // different normalizations and triggers.

  // NSD: No Single Diffractive events
  // file->cd();
  // dNdEtaAnalysis* fdNdEtaAnalysis = new dNdEtaAnalysis("dndetaNSD", "dndetaNSD");
  // fdNdEtaAnalysis->LoadHistograms("fdNdEtaAnalysisESD");
  // fdNdEtaAnalysis->Finish(dNdEtaCorrection, 0.151, AlidNdEtaCorrection::kNSD, "ESD -> NSD", backgroundEvents);
  // file2->cd();
  // fdNdEtaAnalysis->SaveHistograms();

  // All INELastic events
  file->cd();
  dNdEtaAnalysis* fdNdEtaAnalysis = new dNdEtaAnalysis("dndeta", "dndeta");
  fdNdEtaAnalysis->LoadHistograms("fdNdEtaAnalysisESD");
  fdNdEtaAnalysis->Finish(dNdEtaCorrection, 0.151, AlidNdEtaCorrection::kINEL, "ESD -> full inelastic", backgroundEvents);
  file2->cd();
  fdNdEtaAnalysis->SaveHistograms();

  // Minimum bias
  // file->cd();
  // fdNdEtaAnalysis = new dNdEtaAnalysis("dndetaTr", "dndetaTr");
  // fdNdEtaAnalysis->LoadHistograms("fdNdEtaAnalysisESD");
  // fdNdEtaAnalysis->Finish(dNdEtaCorrection, 0.151, AlidNdEtaCorrection::kVertexReco, "ESD -> minimum bias", backgroundEvents);
  // file2->cd();
  // fdNdEtaAnalysis->SaveHistograms();

  // Minimum bias with reconstructed vertex
  // file->cd();
  // fdNdEtaAnalysis = new dNdEtaAnalysis("dndetaTrVtx", "dndetaTrVtx");
  // fdNdEtaAnalysis->LoadHistograms("fdNdEtaAnalysisESD");
  // fdNdEtaAnalysis->Finish(dNdEtaCorrection, 0.151, AlidNdEtaCorrection::kTrack2Particle, "ESD -> MB with vertex", backgroundEvents);
  // file2->cd();
  // fdNdEtaAnalysis->SaveHistograms();

  // All events but with pt cut
  // file->cd();
  // fdNdEtaAnalysis = new dNdEtaAnalysis("dndetaTracks", "dndetaTracks");
  // fdNdEtaAnalysis->LoadHistograms("fdNdEtaAnalysisESD");
  // fdNdEtaAnalysis->Finish(0, 0.151, AlidNdEtaCorrection::kNone, "ESD raw with pt cut", backgroundEvents);
  // file2->cd();
  // fdNdEtaAnalysis->SaveHistograms();

  // All events without any cut
  // file->cd();
  // fdNdEtaAnalysis = new dNdEtaAnalysis("dndetaTracksAll", "dndetaTracksAll");
  // fdNdEtaAnalysis->LoadHistograms("fdNdEtaAnalysisESD");
  // fdNdEtaAnalysis->Finish(0, -1, AlidNdEtaCorrection::kNone, "ESD raw", backgroundEvents);
  // file2->cd();
  // fdNdEtaAnalysis->SaveHistograms();

  // One Particle trigger, i.e. at least one particle in detector
  // file->cd();
  // fdNdEtaAnalysis = new dNdEtaAnalysis("dndetaOnePart", "dndetaOnePart");
  // fdNdEtaAnalysis->LoadHistograms("fdNdEtaAnalysisESD");
  // fdNdEtaAnalysis->Finish(dNdEtaCorrection, 0.151, AlidNdEtaCorrection::kOnePart, "ESD -> OnePart", backgroundEvents);
  // file2->cd();
  // fdNdEtaAnalysis->SaveHistograms();
  
  // write results to output file
  file2->Write();
  file2->Close();
}