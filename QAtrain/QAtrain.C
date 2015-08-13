// Offline trigger selection
UInt_t kTriggerInt = AliVEvent::kAnyINT;
//UInt_t kTriggerMask = kTriggerInt;
UInt_t kTriggerMask = 0;

Int_t       debug_level   = 1;        // Debugging
Int_t       run_number    = 0;

// CDB connection (not supported in DP mode)
TString cdbString = "";

// Tasks in the train
Bool_t doCDBconnect   = 0;
Bool_t doEventStat    = 1;
Bool_t doVertex       = 1;

void QAtrain(Int_t run = 0, 
             const char *xmlfile   = "wn.xml",
             const char *cdb     = "local://$ALICE_ROOT/OCDB")
{
  cdbString = cdb;
  // If raw OCDB requested, connect to GRID
  if (cdbString.Contains("raw://"))
  {
    TGrid::Connect("alien://");
    if (!gGrid || !gGrid->IsConnected()) {
      ::Error("QAtrain", "No grid connection");
      return;
    }  
  }
  run_number = run;
  // Include path for local compilation if any
  gSystem->AddIncludePath("-I$ALICE_PHYSICS/include -I$ALICE_ROOT/include");
  printf("Include path: %s\n", gSystem->GetIncludePath());

  // Create manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("PilotAnalysis", "Production train");
   // create the alien handler and attach it to the manager
  AliAnalysisGrid *plugin = CreateAlienHandler(); 
  mgr->SetGridHandler(plugin);
  mgr->SetRunFromPath(run_number);
  mgr->SetCacheSize(0);
  // Input handler
  AliESDInputHandler *esdHandler = new AliESDInputHandler();
  
  esdHandler->SetReadFriends(kFALSE);
//  esdHandler->SetActiveBranches("ESDfriend");
  
  mgr->SetInputEventHandler(esdHandler);
  mgr->SetDebugLevel(debug_level);

  // AnalysisTasks
  AddAnalysisTasks(cdb);

  // Input chain
  TChain *chain = new TChain("esdTree");
  chain->Add("AliESDs.root");
  TStopwatch timer;
  timer.Start();
  if (mgr->InitAnalysis()) {                                                                                                              
    mgr->PrintStatus(); 
    // mgr->SetSkipTerminate(kTRUE);
    mgr->StartAnalysis("local" /*, chain*/);
  }
  timer.Print();
}

void AddAnalysisTasks(const char *cdb_location)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  mgr->SetCommonFileName("QAresults.root");
  mgr->AddStatisticsTask(kTriggerMask);
  //
  // CDB connection
  //
  if (doCDBconnect) {
    printf("=== CDBconnect activated ===\n");
    gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");
    AliTaskCDBconnect *taskCDB = AddTaskCDBconnect(cdb_location, run_number);    
    if (!taskCDB) return;
  }    

  // Event Statistics (needed in case any offline trigger selection requested)
  //
  if (doEventStat) {
    printf("=== EventStat activated ===\n");
      gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
      AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(kFALSE /*MC*/);
      AliAnalysisDataContainer *cstatsout = (AliAnalysisDataContainer*)mgr->GetOutputs()->FindObject("cstatsout");
      cstatsout->SetFileName("EventStat_temp.root");
  }

  // Vertexing (A. Dainese)
  if (doVertex) {
    printf("=== Vertexing activated ===\n");
    gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskVertexESD.C");
    AliAnalysisTaskVertexESD* taskvertexesd =  AddTaskVertexESD(kFALSE, kTriggerMask);
    // Mechanism to activate offline trigger selection for events
    taskvertexesd->SelectCollisionCandidates(kTriggerMask);
  }  
}

//______________________________________________________________________________
AliAnalysisGrid* CreateAlienHandler()
{
    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
    plugin->SetRunMode("test");

    // Plugin test mode works only providing a file containing test file locations, used in "local" mode also
    plugin->SetFileForTestMode("files.txt"); // file should contain path name to a local directory containg *ESDs.root etc
    return plugin;
}
  
