//
// Porting of dNdEta analysis used for the charged particle multiplicity study
// <AliPhysics_source_dir>/PWGUD/selectors/dNdEta
//
//
class AliAnalysisGrid;

//______________________________________________________________________________
void run(Int_t runWhat = 0, Int_t requiredData = 2, const Char_t* file = "files.txt", const char* option = "")
{
  // runWhat options: 0 = AlidNdEtaTask
  //                  1 = AlidNdEtaCorrectionTask
  //                  2 = both
  //
  // requiredData option: 0 = only ESD
  //                      1 = ESD+MC
  //                      2 = RAW (ESD+check on event type)
  //
  // option is passed to the task(s)
  //

  // add aliroot include path
  gROOT->ProcessLine(Form(".include %s/include", gSystem->ExpandPathName("$ALICE_ROOT")));
  gROOT->SetStyle("Plain");
  // analysis manager
  AliAnalysisManager* mgr = new AliAnalysisManager("dNdEta");
  // create the alien handler and attach it to the manager
  AliAnalysisGrid *plugin = CreateAlienHandler(file);
  mgr->SetGridHandler(plugin);
  // set input handler as standard input handler and deactivate unnecessary detector parts
  AliESDInputHandler* iH = new AliESDInputHandlerRP();
  iH->SetInactiveBranches("FMD AliRawDataErrorLogs CaloClusters Cascades EMCALCells EMCALTrigger ESDfriend Kinks MuonTracks TrdTracks");
  mgr->SetInputEventHandler(iH);

  // The following part selects the analysis mode as well as the trigger, which is used
  // Via the bitwise OR "|" the different options are combined to one.
  // First of all for the analysis mode, the B-field is always turned on: kFieldOn
  // Secondly, there are two different modes for the detector, kSPD and TPCITS
  // - kSPD: the Silicon Pixel Detector is used for the analysis
  // - kTPCITS: use Time Projection Chamber as well as the Inner Tracking System (SPD, SSD, SDD)
  // AliPWG0Helper::AnalysisMode analysisMode = AliPWG0Helper::kSPD | AliPWG0Helper::kFieldOn;
  AliPWG0Helper::AnalysisMode analysisMode = AliPWG0Helper::kTPCITS | AliPWG0Helper::kFieldOn;
  // For the trigger, there are different options availiable:
  // - kOfflineFlag: this analysis is offline, so this flag is used everywhere
  // - kAcceptAll: all events are used
  // - kOneParticle: a minimum of one particle in the SPD
  AliTriggerAnalysis::Trigger trigger      = AliTriggerAnalysis::kAcceptAll | AliTriggerAnalysis::kOfflineFlag;
  // AliTriggerAnalysis::Trigger trigger      = AliTriggerAnalysis::kAcceptAll | AliTriggerAnalysis::kOfflineFlag | AliTriggerAnalysis::kOneParticle;
  // The following lines are special settings for Monte Carlo data
  AliPWG0Helper::DiffTreatment diffTreatment = AliPWG0Helper::kMCFlags;
  // AliPWG0Helper::DiffTreatment diffTreatment = AliPWG0Helper::kE710Cuts;
  // print the whole configuration
  AliPWG0Helper::PrintConf(analysisMode, trigger, diffTreatment);

  // Create the cuts, which are defined above. If the analysis mode is not kSPD, the macro
  // CreateStandardCuts.C is used (see there for further information). If the analysis method is
  // kTPCITS, the standard ITS and TPC cuts from 2010 without any additional cuts on pt
  // or eta are returned by the macro.
  AliESDtrackCuts* esdTrackCuts = 0;
  if (!(analysisMode & AliPWG0Helper::kSPD))
  {
    // execute macro CreateStandardCuts.C
    gROOT->ProcessLine(".L CreateStandardCuts.C");
    esdTrackCuts = CreateTrackCuts(analysisMode);
    // print if an error occured
    if (!esdTrackCuts)
    {
      printf("ERROR: esdTrackCuts could not be created\n");
      return;
    }
    // Enable histograms for cuts in order to controll them after the analysis
    esdTrackCuts->SetHistogramsOn(kTRUE);
  }

  // In the next lines, the first task is added to the analysis chain, the physics selection task.
  // It differs for MC and for data, which is given to the task as a bool.
  gROOT->ProcessLine(".L $ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  physicsSelectionTask = AddTaskPhysicsSelection((requiredData == 2) ? kFALSE : kTRUE);

  // The next two if clauses define the main tasks, which are performed.
  // Firstly, the analysis is defined, which is not corrected due to detector performance.
  // It can be run either with data or with MC. The C++ file for AlidNdEtaTask can be found
  // at the alipysics source code: PWGUD/selectors/AlidNdEtaTask
  if (runWhat == 0 || runWhat == 2)
  {
    task = new AlidNdEtaTask(option);

    if (requiredData == 1) task->SetReadMC();
    // physicsSelectionTask->GetPhysicsSelection()->SetBin0Callback("AlidNdEtaTask");

    // INEL>0 definition
    if (trigger & AliTriggerAnalysis::kOneParticle) task->SetMultAxisEta1();
    task->SetTrigger(trigger);
    task->SetAnalysisMode(analysisMode);
    task->SetTrackCuts(esdTrackCuts);
    // task->SetDeltaPhiCut(0.064);
    task->SetDiffTreatment(diffTreatment);

    // add task to analysis chain
    mgr->AddTask(task);
    // set output root file name for different analysis
    TString outfilename = "results.root";
    // Attach input
    mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
    // Attach output
    cOutput = mgr->CreateContainer("cOutput", TList::Class(), AliAnalysisManager::kOutputContainer);
    mgr->ConnectOutput(task, 1, cOutput);
  }

  // Secondly, if wanted, the MC data is used to get the correction map from raw
  // to final data. The source code of AlidNdEtaCorrectionTask can be founde at
  // PWGUD/selectors/AlidNdEtaCorrectionTask
  if (runWhat == 1 || runWhat == 2)
  {
    task2 = new AlidNdEtaCorrectionTask(option);

    // to account for gaps in real life SPD geometry
    task2->SetSkipParticles();

    // INEL>0 definition
    if (trigger & AliTriggerAnalysis::kOneParticle) task2->SetMultAxisEta1();
    task2->SetTrigger(trigger);
    task2->SetAnalysisMode(analysisMode);
    task2->SetTrackCuts(esdTrackCuts);
    //task2->SetDeltaPhiCut(0.064);
    task2->SetDiffTreatment(diffTreatment);

    // add task to analysis chain
    mgr->AddTask(task2);
    // Attach input
    mgr->ConnectInput(task2, 0, mgr->GetCommonInputContainer());
    // Attach output
    cOutput = mgr->CreateContainer("cOutput2", TList::Class(), AliAnalysisManager::kOutputContainer);
    mgr->ConnectOutput(task2, 0, cOutput);
  }

  // some last options for MC are set, before the analysis can start
  if (requiredData == 1)
  {
    // Enable MC event handler
    AliMCEventHandler* handler = new AliMCEventHandler;
    handler->SetReadTR(kFALSE);
    mgr->SetMCtruthEventHandler(handler);
  }

  // Load the magnatic field from the OCDB file of the run
  AliCDBManager *cdb = AliCDBManager::Instance();
  cdb->SetDefaultStorage("local://");
  cdb->SetRun(0);
  cdb->InitFromSnapshot("OCDB.root");
  AliGRPManager *grp= new AliGRPManager();
  printf("#### Loading GRP to init B-field...\n");
  if(!grp->ReadGRPEntry()) AliFatal("Cannot get GRP entry"); 
  if(!grp->SetMagField())  AliFatal("Problem with magnetic field setup"); 

  // Initialize the analysis
  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  // And start it!
  Printf("Starting Analysis....");
  mgr->StartAnalysis("local");
}

// The AlienHandler is used in order to define, where the analysis should run
AliAnalysisGrid* CreateAlienHandler(const Char_t* file)
{
  AliAnalysisAlien *plugin = new AliAnalysisAlien();
  // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
  // "test" let the analysis run on the local machine
  plugin->SetRunMode("test");

  // Plugin test mode works only providing a file containing test file locations.
  // The file should contain paths to local files like "*ESDs.root" etc.
  plugin->SetFileForTestMode(file);
  Int_t nFiles = gSystem->GetFromPipe(Form("grep -c \".\" %s", file));
  plugin->SetNtestFiles(nFiles);
  return plugin;
}

