#include <Riostream.h>
#include <TApplication.h>
#include <TBrowser.h>
#include <TFile.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGTab.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TList.h>
#include <TRootHelpDialog.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TG3DLine.h>
#include <TSystem.h>
#include <TROOT.h>

class MasterClassFrame : public TGMainFrame
{

private:
  TGTab             *fTab;
  TRootHelpDialog   *fInstructions;
  TGTextButton      *fExample;
  TGPictureButton   *fLaunchMC;
  TGPictureButton   *fLaunchRAA;
  TGPictureButton   *fLaunchS;
  TGPictureButton   *fLaunchPt;
  TGPictureButton   *fLaunchQA;
  TGPictureButton   *fLaunchdNdEta;
  TGTextButton      *fStudentS;
  TGTextButton      *fTeacherS;
  TGTextButton      *fExit;
  TGTextButton      *fInfoMasterClass;
  TGTextButton      *fInfoRAA;
  TGTextButton      *fInfoS;
  TGTextButton      *fInfoPt;
  TGTextButton      *fInfoQA;
  TGTextButton      *fInfodNdEta;
  Int_t              fChoice;
  Int_t              fMode;
  TGComboBox        *fDataset;
  TGComboBox        *fDatasetS;
  TGComboBox        *fDatasetPt;
  TGComboBox        *fDatasetQA;
  TGComboBox        *fDatasetdNdEta;
  TGNumberEntry     *fCentMin;
  TGNumberEntry     *fCentMax;
  TGNumberEntry     *fPtNfiles;
  TGNumberEntry     *fQANfiles;
  TGNumberEntry     *fdNdEtaNfiles;
  TString            fDSpath;
  TString            fDSname;
  TString            fDSdescription;
  Int_t              fDSnfiles;
  TString            datasets[15];
  TString            datasetNames[15];
  Int_t              datasetOffsets[15];

public:

  MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~MasterClassFrame();
  Bool_t DownloadAndUnpack(const char *module);
  Bool_t DownloadData(const TString dataset, const Int_t nfiles, const Int_t offset, const TString filename);
  void InfoMasterClass();
  void InfoRAA();
  void InfoS();
  void InfoPt();
  void InfoQA();
  void InfodNdEta();
  void MakeTitle(const char *title, TGCompositeFrame *parent);
  void ParseDataset(const char *dataset);
  const char *DecodeValue(TString &line);
  void StartDocView(const char *document);
  void RAAdocumentation();
  void StartEvDisplay();
  void StartStr();
  void StartStr1();
  void StartStr2();
  void StartStr3();
  void StartRAA();
  void StartEvd1();
  void StartPt();
  void StartQA();
  void StartdNdEta();

  ClassDef(MasterClassFrame, 0)
};

void MasterClassFrame::RAAdocumentation()
{
  // Display RAA documentation
  if (!DownloadAndUnpack("alice-raa.tgz")) return;
  StartDocView("RAA/Documentation.pdf");
}

void MasterClassFrame::StartDocView(const char *document)
{
  // Start a viewer for the pdf document
  if (!gSystem->Exec("command -v xpdf >/dev/null 2>&1"))
  {
    gSystem->Exec(Form("xpdf %s & 2>&1", document));
    return;
  }
  if (!gSystem->Exec("command -v okular >/dev/null 2>&1"))
  {
    gSystem->Exec(Form("okular %s & 2>&1", document));
    return;
  }
  if (!gSystem->Exec("command -v acroread >/dev/null 2>&1"))
  {
    gSystem->Exec(Form("acroread %s & 2>&1", document));
    return;
  }
  if (!gSystem->Exec("command -v evince >/dev/null 2>&1"))
  {
    gSystem->Exec(Form("evince %s & 2>&1", document));
    return;
  }
}

Bool_t MasterClassFrame::DownloadAndUnpack(const char *module)
{
  // Download and unpack the given analysis module from the portal
  if (gSystem->AccessPathName(module))
  {
    // Download the RAA module
    TString alice_portal = gSystem->Getenv("ALICE_PORTAL");
    if (alice_portal.IsNull())
    {
      printf("ALICE_PORTAL not defined. Exiting.\n");
      return kFALSE;
    }
    alice_portal += "//eos/opendata/alice/modules";
    printf("Downloading masterclass module...\n");
    if (gSystem->Exec(Form("xrdcp -fsP %s/%s %s > /dev/null 2>&1", alice_portal.Data(), module, module)))
    {
      if (gSystem->Exec(Form("wget %s/%s > /dev/null 2>&1", alice_portal.Data(), module)))
      {
        printf("Module %s not available on portal %s\n", module, alice_portal.Data());
        if (!gSystem->AccessPathName(module)) gSystem->Exec(Form("rm -f %s", module));
        return kFALSE;
      }
    }
    if (gSystem->Exec(Form("tar xvzf %s > /dev/null", module)))
    {
      printf("Cannot unpack module %s\n", module);
      return kFALSE;
    }
    return kTRUE;
  }
  return kTRUE;
}

Bool_t MasterClassFrame::DownloadData(const TString dataset, Int_t nfiles, const Int_t offset = 0, const TString filename = "files.txt")
{
  // defining variables
  TString alice_portal = gSystem->Getenv("ALICE_PORTAL");
  alice_portal += "//eos/opendata/alice";
  TString dir, sfile;
  // Download selected dataset if not already downloaded
  TString spath = Form("../data/%s.json", dataset.Data());
  if (gSystem->AccessPathName(spath.Data()))
  {
    if (alice_portal.IsNull())
    {
      printf("ALICE_PORTAL not defined. Exiting.\n");
      return kFALSE;
    }
    sfile = Form("%s/%s.json", alice_portal.Data(), dataset.Data());
    printf("Downloading %s ...\n", sfile.Data());
    if (gSystem->Exec(Form("xrdcp -fsP %s %s > /dev/null 2>&1", sfile.Data(), spath.Data())))
    {
      // if (gSystem->Exec(Form("wget %s -O %s 2>/dev/null", sfile.Data(),spath.Data()))) {
      printf("Dataset %s not available on the ALICE portal\n", sfile.Data());
      return kFALSE;
    }
  }
  // Parse the dataset file
  printf("Creating local dataset.\n");
  ParseDataset(spath);
  // download the requested number of files
  if (nfiles > fDSnfiles)
  {
    nfiles = fDSnfiles;
    printf("This dataset has only %d files\n", nfiles);
  }
  alice_portal = gSystem->Getenv("ALICE_PORTAL");
  for (Int_t i = offset; i < nfiles + offset; i++)
  {
    if (alice_portal.IsNull())
    {
      printf("ALICE_PORTAL not defined. Exiting.\n");
      return kFALSE;
    }
    dir = TString::Format("../data/%s/%04d", dataset.Data(), i);
    sfile = Form("%s/%s/%04d", alice_portal.Data(), fDSpath.Data(), i);
    // download only missing files
    if (gSystem->AccessPathName(Form("%s/AliESDs.root", dir.Data())))
    {
      printf("Downloading %s ...\n", Form("%s/AliESD.root", sfile.Data()));
      // Create data directories if not existing
      gSystem->mkdir(dir, true);
      // Download file
      // if (gSystem->Exec(Form("wget %s/AliESDs.root -O %s/AliESDs.root 2>/dev/null", sfile.Data(),dir.Data()))) {
      // if (gSystem->Exec(Form("xrdcp -fsP %s/AliESDs.root %s > /dev/null 2>&1", sfile.Data(), dir.Data())))
      if (gSystem->Exec(Form("xrdcp -RsP %s/ %s > /dev/null 2>&1", sfile.Data(), dir.Data())))
      {
        printf("Data file %s/AliESDs.root not available on the ALICE portal\n", sfile.Data());
        return kFALSE;
      }
    }
    if (i == offset) gSystem->Exec(Form("echo \"%s/AliESDs.root\" > %s", dir.Data(), filename.Data()));
    else           gSystem->Exec(Form("echo \"%s/AliESDs.root\" >> %s", dir.Data(), filename.Data()));
  }
  return kTRUE;
}

void MasterClassFrame::MakeTitle(const char *title, TGCompositeFrame *parent)
{
  // Create attribute frame title.
  TGCompositeFrame *f1 = new TGCompositeFrame(parent, 180, 10, kHorizontalFrame |
      kFitWidth |
      kFixedWidth |
      kOwnBackground);
  f1->AddFrame(new TGLabel(f1, title),
               new TGLayoutHints(kLHintsLeft, 1, 1, 0, 0));
  f1->AddFrame(new TGHorizontal3DLine(f1),
               new TGLayoutHints(kLHintsExpandX, 5, 5, 7, 7));
  parent->AddFrame(f1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));
}

void MasterClassFrame::StartEvDisplay()
{
  if (!DownloadAndUnpack("alice-raa.tgz")) return;
#if defined(__CINT__) && !defined(__MAKECINT__)
  {
    Info("alice_vsd.C", "Has to be run in compiled mode ... doing this for you.");
    gSystem->cd("RAA/Part1");
    Int_t dataset = fDataset->GetSelected();
    TString vsd_file_name = Form("eve_files/data/AliVSD_Masterclass_%i.root", dataset);
    // Download dataset from portal
    TString alice_portal = gSystem->Getenv("ALICE_PORTAL");
    TString spath = "eve_files/data/LHC2010h_PbPb_VSD_139036.json";
    TString sfile;
    if (gSystem->AccessPathName(spath.Data()))
    {
      if (alice_portal.IsNull())
      {
        printf("ALICE_PORTAL not defined. Exiting.\n");
        return;
      }
      sfile = Form("%s//eos/opendata/alice/LHC2010h_PbPb_VSD_139036.json", alice_portal.Data());
      printf("Downloading %s ...\n", sfile.Data());
      if (gSystem->Exec(Form("xrdcp -fsP %s %s > /dev/null 2>&1", sfile.Data(), spath.Data())))
      {
        printf("Dataset %s not available on the ALICE portal\n", sfile.Data());
        return;
      }
    }
    // Parse the dataset file
    ParseDataset(spath);
    // download the requested file
    sfile = Form("%s/%s/AliVSD_Masterclass_%i.root", alice_portal.Data(), fDSpath.Data(), dataset);
    // download only missing files
    if (gSystem->AccessPathName(vsd_file_name))
    {
      printf("Downloading %s ...\n", sfile.Data());
      if (gSystem->Exec(Form("xrdcp -fsP %s %s > /dev/null 2>&1", sfile.Data(), vsd_file_name.Data())))
      {
        printf("Data file %s not available on the ALICE portal\n", sfile.Data());
        return;
      }
    }

    gROOT->LoadMacro("alice_vsd.C+");
    // gSystem->CompileMacro("alice_vsd.C");
    printf("Module alice-raa compiled\n");
    alice_vsd(fChoice, fMode, fDataset->GetSelected());
  }
#else
  alice_vsd(fChoice, fMode, fDataset->GetSelected());
#endif

  UnmapWindow();
}

void MasterClassFrame::StartStr()
{
  if (!DownloadAndUnpack("alice-mc.tgz")) return;
#if defined(__CINT__) && !defined(__MAKECINT__)
  {
    Info("alice_vsd.C", "Has to be run in compiled mode ... doing this for you.");
    gSystem->cd("alice-mc");
    gROOT->LoadMacro("alice_vsd.C+");
    // gSystem->CompileMacro("alice_vsd.C");
    printf("Module alice-mc compiled\n");
    alice_vsd(fChoice, fMode, fDatasetS->GetSelected());
  }
#else
  alice_vsd(fChoice, fMode, fDataset->GetSelected());
#endif
  UnmapWindow();
}

void MasterClassFrame::StartRAA()
{
  if (!DownloadAndUnpack("alice-raa.tgz")) return;
  TString alice_portal = gSystem->Getenv("ALICE_PORTAL");
  if (alice_portal.IsNull())
  {
    printf("ALICE_PORTAL not defined. Exiting.\n");
    exit(1);
  }
  alice_portal += "//eos/opendata/alice/masterclass";
  gSystem->cd("RAA/Part2");
  TString filename = "MasterClassesTree_LHC10h_Run139036.root";
  if (gSystem->AccessPathName(filename))
  {
    // Download file from the portal
    TString sfile = Form("%s/%s", alice_portal.Data(), filename.Data());
    if (gSystem->Exec(Form("xrdcp -fsP %s %s > /dev/null 2>&1", sfile.Data(), filename.Data())))
    {
      // if (!TFile::Cp(sfile,filename, kFALSE)) {
      printf("File %s not available on the ALICE portal\n", filename.Data());
      exit(1);
    }
  }
  Double_t centmin = fCentMin->GetNumber();
  Double_t centmax = fCentMax->GetNumber();
  UnmapWindow();
  gROOT->LoadMacro("AnalyseTreeForRAAStudents.C+");
  AnalyseTreeForRAAStudents(filename, "PbPb", "kFALSE", centmin, centmax);
}

void MasterClassFrame::StartPt()
{
  if (!DownloadAndUnpack("PtAnalysis.tgz")) return;
  gSystem->cd("PtAnalysis");
  Int_t nfiles = fPtNfiles->GetNumber();
  Int_t dsind = fDatasetPt->GetSelected();
  if (!DownloadData(datasets[dsind], nfiles, datasetOffsets[dsind])) return;
  printf("Creating local dataset.\n");
  gROOT->ProcessLine(".x runEx01.C");
  UnmapWindow();
}

void MasterClassFrame::StartQA()
{
  printf("Starting QA train...\n");
  // if (!DownloadAndUnpack("QAtrain.tgz")) return;
  gSystem->cd("QAtrain");
  Int_t nfiles = fQANfiles->GetNumber();
  Int_t dsind = fDatasetQA->GetSelected();
  if (!DownloadData(datasets[dsind], nfiles, datasetOffsets[dsind])) return;
  gROOT->ProcessLine(".x QAtrain.C");
  UnmapWindow();

  TBrowser *browser;
  TFile *QAresults = new TFile("QAresults.root");
  TList *vlist;
  if (!QAresults->IsOpen()) return;
  QAresults->cd("Vertex_Performance");
  gDirectory->GetObject("cOutputVtxESD", vlist);
  browser = new TBrowser("browser", vlist, "Browsing QAtrain results");
}

void MasterClassFrame::StartdNdEta()
{
  printf("Starting dNdEta analysis...\n");
  // if (!DownloadAndUnpack("dNdEta.tgz")) return;
  gSystem->cd("dNdEta");
  Int_t nfiles = fdNdEtaNfiles->GetNumber();
  Int_t dsind = fDatasetdNdEta->GetSelected();
  if (!DownloadData(datasets[dsind], nfiles, datasetOffsets[dsind])) return;
  /* MC
  TString mcdatasets1[7] = {"LHC10c_pp_MC_Perugia0_120076", "LHC10c_pp_MC_Perugia0_120244", "LHC10c_pp_MC_Perugia0_120505",
                            "LHC10c_pp_MC_Perugia0_120616", "LHC10c_pp_MC_Perugia0_120822", "LHC10c_pp_MC_Perugia0_120824",
                            "LHC10c_pp_MC_Perugia0_120829"};
  if (!DownloadData(mcdatasets1[dsind-1], nfiles, 1, "filesPerugia0.txt")) return;
  TString mcdatasets2[7] = {"LHC10c_pp_MC_Perugia2011_120076", "LHC10c_pp_MC_Perugia2011_120244", "LHC10c_pp_MC_Perugia2011_120505",
                            "LHC10c_pp_MC_Perugia2011_120616", "LHC10c_pp_MC_Perugia2011_120822", "LHC10c_pp_MC_Perugia2011_120824",
                            "LHC10c_pp_MC_Perugia2011_120829"};
  if (!DownloadData(mcdatasets2[dsind-1], nfiles, 1, "filesPerugia2011.txt")) return;
  TString mcdatasets3[7] = {"LHC10c_pp_MC_Phojet_120076", "LHC10c_pp_MC_Phojet_120244", "LHC10c_pp_MC_Phojet_120505",
                            "LHC10c_pp_MC_Phojet_120616", "LHC10c_pp_MC_Phojet_120822", "LHC10c_pp_MC_Phojet_120824",
                            "LHC10c_pp_MC_Phojet_120829"};
  if (!DownloadData(mcdatasets3[dsind-1], nfiles, 1, "filesPhojet.txt")) return;
  */

  gROOT->ProcessLine(".x dNdEta.C");
  UnmapWindow();
}

void MasterClassFrame::StartEvd1()
{
  fChoice = 2;
  fMode = 2;

  StartEvDisplay();
}

void MasterClassFrame::StartStr1()
{
  fChoice = 1;
  fMode = 1;

  StartStr();
}

void MasterClassFrame::StartStr2()
{
  fChoice = 2;
  fMode = 2;

  StartStr();
}

void MasterClassFrame::StartStr3()
{
  fChoice = 2;
  fMode = 3;

  StartStr();
}

MasterClassFrame::MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h) :
  TGMainFrame(p, w, h)
{
  // The datasets
  datasets[0]       = "LHC2010b_pp_ESD_117222";
  datasets[1]       = "LHC10c_pp_ESD_120076";
  datasets[2]       = "LHC10c_pp_ESD_120244";
  datasets[3]       = "LHC10c_pp_ESD_120505";
  datasets[4]       = "LHC10c_pp_ESD_120616";
  datasets[5]       = "LHC10c_pp_ESD_120822";
  datasets[6]       = "LHC10c_pp_ESD_120824";
  datasets[7]       = "LHC10c_pp_ESD_120829";
  datasets[8]       = "LHC2010h_PbPb_ESD_138275";
  datasets[9]       = "LHC2010h_PbPb_ESD_139036";
  datasets[10]       = "LHC10h_PbPb_ESD_139038";
  datasets[11]       = "LHC10h_PbPb_ESD_139173";
  datasets[12]       = "LHC10h_PbPb_ESD_139437";
  datasets[13]       = "LHC10h_PbPb_ESD_139438";
  datasets[14]       = "LHC10h_PbPb_ESD_139465";
  datasetNames[0]       = "pp 3.5TeV, run 117222";
  datasetNames[1]       = "pp 3.5TeV, run 120076";
  datasetNames[2]       = "pp 3.5TeV, run 120244";
  datasetNames[3]       = "pp 3.5TeV, run 120505";
  datasetNames[4]       = "pp 3.5TeV, run 120616";
  datasetNames[5]       = "pp 3.5TeV, run 120822";
  datasetNames[6]       = "pp 3.5TeV, run 120824";
  datasetNames[7]       = "pp 3.5TeV, run 120829";
  datasetNames[8]       = "PbPb 2.76TeV, run 138275";
  datasetNames[9]       = "PbPb 2.76TeV, run 139036";
  datasetNames[10]      = "PbPb 2.76TeV, run 139038";
  datasetNames[11]      = "PbPb 2.76TeV, run 139173";
  datasetNames[12]      = "PbPb 2.76TeV, run 139437";
  datasetNames[13]      = "PbPb 2.76TeV, run 139438";
  datasetNames[14]      = "PbPb 2.76TeV, run 139465";
  datasetOffsets[0] = 0;
  datasetOffsets[1] = 1;
  datasetOffsets[2] = 1;
  datasetOffsets[3] = 1;
  datasetOffsets[4] = 1;
  datasetOffsets[5] = 1;
  datasetOffsets[6] = 1;
  datasetOffsets[7] = 1;
  datasetOffsets[8] = 0;
  datasetOffsets[9] = 0;
  datasetOffsets[10] = 1;
  datasetOffsets[11] = 1;
  datasetOffsets[12] = 1;
  datasetOffsets[13] = 1;
  datasetOffsets[14] = 1;

  // The big frame
  TGHorizontalFrame* allall = new TGHorizontalFrame(this);
  TGVerticalFrame *all = new TGVerticalFrame(allall);
  TGVerticalFrame *all2 = new TGVerticalFrame(allall);
  {
    TGLabel* labelall = new TGLabel(all, "ALICE analysis modules");
    all->AddFrame(labelall, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
    TGPictureButton* b1 = new TGPictureButton(all, gClient->GetPicture("pict/2011-Nov-24-ALICE_logo_small.eps"));
    b1->Connect("Clicked()", "MasterClassFrame", this, "RAAdocumentation()");
    b1->SetToolTipText("Documentation for the RAA analysis");
    all->AddFrame(b1, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));

    fTab = new TGTab(all2, 300, 300);
    // The event display frame
    TGCompositeFrame *tf = fTab->AddTab("RAA_1");
    TGCompositeFrame *hf = new TGCompositeFrame(tf, 1, 1, kVerticalFrame);
    MakeTitle("Event display exercise", tf);
    // TGVerticalFrame* hf = new TGVerticalFrame(this,1,1,kSunkenFrame);
    {
      fInfoMasterClass = new TGTextButton(hf, "Info");
      fInfoMasterClass->SetToolTipText("Info for using the event display GUI");
      hf->AddFrame(fInfoMasterClass, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fInfoMasterClass->Connect("Clicked()", "MasterClassFrame", this, "InfoMasterClass()");
      fLaunchMC = new TGPictureButton(hf, gClient->GetPicture("pict/ev_display_small.png"));
      fLaunchMC->SetToolTipText("Launch the event display interface");
      fLaunchMC->Connect("Clicked()", "MasterClassFrame", this, "StartEvd1()");
      hf->AddFrame(fLaunchMC, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));

      fDataset = new TGComboBox(hf);
      fDataset->AddEntry("dataset 1", 1);
      fDataset->AddEntry("dataset 2", 2);
      fDataset->AddEntry("dataset 3", 3);
      fDataset->AddEntry("dataset 4", 4);
      fDataset->AddEntry("dataset 5", 5);
      fDataset->AddEntry("dataset 6", 6);
      fDataset->AddEntry("dataset 7", 7);
      fDataset->AddEntry("dataset 8", 8);
      fDataset->AddEntry("dataset 9", 9);
      fDataset->AddEntry("dataset 10", 10);
      fDataset->Resize(200, 20);
      fDataset->Select(1, kFALSE);
      hf->AddFrame(fDataset, new TGLayoutHints(kLHintsExpandX));
    }
    tf->AddFrame(hf);

    // The RAA frame
    tf = fTab->AddTab("RAA_2");
    MakeTitle("RAA analysis", tf);
    TGCompositeFrame* hf1 = new TGCompositeFrame(tf, 1, 1, kVerticalFrame);
    {
      fInfoRAA = new TGTextButton(hf1, "Info");
      fInfoRAA->SetToolTipText("Info for using the RAA masterclass GUI");
      hf1->AddFrame(fInfoRAA, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fInfoRAA->Connect("Clicked()", "MasterClassFrame", this, "InfoRAA()");

      fLaunchRAA = new TGPictureButton(hf1, gClient->GetPicture("pict/alice_raa_small.png"));
      fLaunchRAA->SetToolTipText("Launch the RAA analysis after selecting the centrality range");
      fLaunchRAA->Connect("Clicked()", "MasterClassFrame", this, "StartRAA()");
      hf1->AddFrame(fLaunchRAA, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));
      // Number entry for min centrality
      TGCompositeFrame *f1c1 = new TGCompositeFrame(hf1, 200, 10, kHorizontalFrame |
          kLHintsExpandX | kFixedWidth | kOwnBackground);
      f1c1->AddFrame(new TGLabel(f1c1, "Minimum centrality"), new TGLayoutHints(kLHintsLeft, 1, 1, 6, 0));
      fCentMin = new TGNumberEntry(f1c1, 0., 3);
      fCentMin->SetNumAttr(TGNumberFormat::kNEAPositive);
      fCentMin->SetLimits(TGNumberFormat::kNELLimitMinMax, 0., 100.);
      TGTextEntry *nef = (TGTextEntry*)fCentMin->GetNumberEntry();
      nef->SetToolTipText("Enter the minimum centrality value");
      fCentMin->Associate(this);
      f1c1->AddFrame(fCentMin, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
      hf1->AddFrame(f1c1, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
      // Number entry for max centrality
      TGCompositeFrame *f1c2 = new TGCompositeFrame(hf1, 200, 10, kHorizontalFrame |
          kLHintsExpandX | kFixedWidth | kOwnBackground);
      f1c2->AddFrame(new TGLabel(f1c2, "Maximum centrality"), new TGLayoutHints(kLHintsLeft, 1, 1, 6, 0));
      fCentMax = new TGNumberEntry(f1c2, 5., 3);
      fCentMax->SetNumAttr(TGNumberFormat::kNEAPositive);
      fCentMax->SetLimits(TGNumberFormat::kNELLimitMinMax, 0., 100.);
      nef = (TGTextEntry*)fCentMax->GetNumberEntry();
      nef->SetToolTipText("Enter the maximum centrality value");
      fCentMax->Associate(this);
      f1c2->AddFrame(fCentMax, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
      hf1->AddFrame(f1c2, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
    }
    tf->AddFrame(hf1);
    // The strangeness masterclass
    tf = fTab->AddTab("Strangeness");
    MakeTitle("Strangeness masterclass", tf);
    TGCompositeFrame *hf2 = new TGCompositeFrame(tf, 1, 1, kVerticalFrame);
    {
      fInfoS = new TGTextButton(hf2, "Info");
      fInfoS->SetToolTipText("Info for using the strangeness masterclass GUI");
      hf2->AddFrame(fInfoS, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fInfoS->Connect("Clicked()", "MasterClassFrame", this, "InfoS()");
      // Launch demo strangeness button
      fLaunchS = new TGPictureButton(hf2, gClient->GetPicture("pict/alice_str_small.png"));
      fLaunchS->SetToolTipText("Start the strangeness masterclass demo");
      fLaunchS->Connect("Clicked()", "MasterClassFrame", this, "StartStr1()");
      hf2->AddFrame(fLaunchS, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));

      fStudentS = new TGTextButton(hf2, "Student");
      fStudentS->SetToolTipText("Start the student exercise");
      hf2->AddFrame(fStudentS, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fStudentS->Connect("Clicked()", "MasterClassFrame", this, "StartStr2()");

      fTeacherS = new TGTextButton(hf2, "Teacher");
      fTeacherS->SetToolTipText("Start the teacher exercise");
      hf2->AddFrame(fTeacherS, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fTeacherS->Connect("Clicked()", "MasterClassFrame", this, "StartStr3()");

      fDatasetS = new TGComboBox(hf2);
      fDatasetS->AddEntry("dataset 1", 1);
      fDatasetS->AddEntry("dataset 2", 2);
      fDatasetS->AddEntry("dataset 3", 3);
      fDatasetS->AddEntry("dataset 4", 4);
      fDatasetS->AddEntry("dataset 5", 5);
      fDatasetS->AddEntry("dataset 6", 6);
      fDatasetS->AddEntry("dataset 7", 7);
      fDatasetS->AddEntry("dataset 8", 8);
      fDatasetS->AddEntry("dataset 9", 9);
      fDatasetS->AddEntry("dataset 10", 10);
      fDatasetS->AddEntry("dataset 11", 11);
      fDatasetS->AddEntry("dataset 12", 12);
      fDatasetS->AddEntry("dataset 13", 13);
      fDatasetS->AddEntry("dataset 14", 14);
      fDatasetS->AddEntry("dataset 15", 15);
      fDatasetS->AddEntry("dataset 16", 16);
      fDatasetS->AddEntry("dataset 17", 17);
      fDatasetS->AddEntry("dataset 18", 18);
      fDatasetS->AddEntry("dataset 19", 19);
      fDatasetS->AddEntry("dataset 20", 20);
      fDatasetS->AddEntry("dataset 21", 21);

      fDatasetS->Resize(200, 20);
      fDatasetS->Select(1, kFALSE);
      hf2->AddFrame(fDatasetS, new TGLayoutHints(kLHintsExpandX));
    }
    tf->AddFrame(hf2);
    // The Pt tutorial analysis
    tf = fTab->AddTab("Pt tutorial");
    MakeTitle("Pt tutorial", tf);
    TGCompositeFrame *hf3 = new TGCompositeFrame(tf, 1, 1, kVerticalFrame);
    {
      fInfoPt = new TGTextButton(hf3, "Info");
      fInfoPt->SetToolTipText("Info for using the Pt tutorial GUI");
      hf3->AddFrame(fInfoPt, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fInfoPt->Connect("Clicked()", "MasterClassFrame", this, "InfoPt()");
      // Launch Pt analysis button
      fLaunchPt = new TGPictureButton(hf3, gClient->GetPicture("pict/alice_pt_small.png"));
      fLaunchPt->SetToolTipText("Start the Pt analysis");
      fLaunchPt->Connect("Clicked()", "MasterClassFrame", this, "StartPt()");
      hf3->AddFrame(fLaunchPt, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));
      // Dataset selection
      fDatasetPt = new TGComboBox(hf3);
      for (int i = 0; i < 15; i++)
      {
        fDatasetPt->AddEntry(datasetNames[i], i);
      }
      fDatasetPt->Resize(200, 20);
      fDatasetPt->Select(0, kFALSE);
      hf3->AddFrame(fDatasetPt, new TGLayoutHints(kLHintsExpandX));
      // Number entry for number of files to process
      TGCompositeFrame *f3c1 = new TGCompositeFrame(hf3, 200, 10, kHorizontalFrame |
          kLHintsExpandX | kFixedWidth | kOwnBackground);
      f3c1->AddFrame(new TGLabel(f3c1, "Number of files"), new TGLayoutHints(kLHintsLeft, 1, 1, 6, 0));
      fPtNfiles = new TGNumberEntry(f3c1, 1., 3);
      fPtNfiles->SetNumAttr(TGNumberFormat::kNEAPositive);
      fPtNfiles->SetLimits(TGNumberFormat::kNELLimitMinMax, 0., 100.);
      TGTextEntry *nef = (TGTextEntry*)fPtNfiles->GetNumberEntry();
      nef->SetToolTipText("Enter the number of files to be downloaded and processed");
      fPtNfiles->Associate(this);
      f3c1->AddFrame(fPtNfiles, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
      hf3->AddFrame(f3c1, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
    }
    tf->AddFrame(hf3);
    // The QAtrain example
    tf = fTab->AddTab("QAtrain");
    MakeTitle("QAtrain", tf);
    TGCompositeFrame *hf4 = new TGCompositeFrame(tf, 1, 1, kVerticalFrame);
    {
      // Info button
      fInfoQA = new TGTextButton(hf4, "Info");
      fInfoQA->SetToolTipText("Info for using the QAtrain example");
      fInfoQA->Connect("Clicked()", "MasterClassFrame", this, "InfoQA()");
      hf4->AddFrame(fInfoQA, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      // Launch Picture
      fLaunchQA = new TGPictureButton(hf4, gClient->GetPicture("pict/QAtrain_small.png"));
      fLaunchQA->SetToolTipText("Start the QAtrain");
      fLaunchQA->Connect("Clicked()", "MasterClassFrame", this, "StartQA()");
      hf4->AddFrame(fLaunchQA, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));
      // Dataset selection
      fDatasetQA = new TGComboBox(hf4);
      for (int i = 0; i < 8; i++)
      {
        fDatasetQA->AddEntry(datasetNames[i], i);
      }
      fDatasetQA->Resize(200, 20);
      fDatasetQA->Select(0, kFALSE);
      hf4->AddFrame(fDatasetQA, new TGLayoutHints(kLHintsExpandX));
      // Number entry for number of files to process
      TGCompositeFrame *f4c1 = new TGCompositeFrame(hf4, 200, 10, kHorizontalFrame |
          kLHintsExpandX | kFixedWidth | kOwnBackground);
      f4c1->AddFrame(new TGLabel(f4c1, "Number of files"), new TGLayoutHints(kLHintsLeft, 1, 1, 6, 0));
      fQANfiles = new TGNumberEntry(f4c1, 1., 3);
      fQANfiles->SetNumAttr(TGNumberFormat::kNEAPositive);
      fQANfiles->SetLimits(TGNumberFormat::kNELLimitMinMax, 0., 100.);
      TGTextEntry *nef = (TGTextEntry*)fQANfiles->GetNumberEntry();
      nef->SetToolTipText("Enter the number of files to be downloaded and processed");
      fQANfiles->Associate(this);
      f4c1->AddFrame(fQANfiles, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
      hf4->AddFrame(f4c1, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
    }
    tf->AddFrame(hf4);
    // The dNdEta analysis
    tf = fTab->AddTab("dNdEta Analysis");
    MakeTitle("dNdEta Analysis", tf);
    TGCompositeFrame *hf5 = new TGCompositeFrame(tf, 1, 1, kVerticalFrame);
    {
      // Info button
      fInfodNdEta = new TGTextButton(hf5, "Info");
      fInfodNdEta->SetToolTipText("Info for running the dNdEta analysis");
      fInfodNdEta->Connect("Clicked()", "MasterClassFrame", this, "InfodNdEta()");
      hf5->AddFrame(fInfodNdEta, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      // Launch Picture
      fLaunchdNdEta = new TGPictureButton(hf5, gClient->GetPicture("pict/dNdEta_small.png"));
      fLaunchdNdEta->SetToolTipText("Start the dNdEta analysis");
      fLaunchdNdEta->Connect("Clicked()", "MasterClassFrame", this, "StartdNdEta()");
      hf5->AddFrame(fLaunchdNdEta, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));
      // Dataset selection
      fDatasetdNdEta = new TGComboBox(hf5);
      for (int i = 1; i < 8; i++)
      {
        fDatasetdNdEta->AddEntry(datasetNames[i], i);
      }
      fDatasetdNdEta->Resize(200, 20);
      fDatasetdNdEta->Select(7, kFALSE);
      hf5->AddFrame(fDatasetdNdEta, new TGLayoutHints(kLHintsExpandX));
      // Number entry for number of files to process
      TGCompositeFrame *f5c1 = new TGCompositeFrame(hf5, 200, 10, kHorizontalFrame |
          kLHintsExpandX | kFixedWidth | kOwnBackground);
      f5c1->AddFrame(new TGLabel(f5c1, "Number of files"), new TGLayoutHints(kLHintsLeft, 1, 1, 6, 0));
      fdNdEtaNfiles = new TGNumberEntry(f5c1, 1., 3);
      fdNdEtaNfiles->SetNumAttr(TGNumberFormat::kNEAPositive);
      fdNdEtaNfiles->SetLimits(TGNumberFormat::kNELLimitMinMax, 0., 100.);
      TGTextEntry *nef = (TGTextEntry*)fdNdEtaNfiles->GetNumberEntry();
      nef->SetToolTipText("Enter the number of files to be downloaded and processed");
      fdNdEtaNfiles->Associate(this);
      f5c1->AddFrame(fdNdEtaNfiles, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
      hf5->AddFrame(f5c1, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 2, 2, 4, 4));
    }
    tf->AddFrame(hf5);
  }
  all2->AddFrame(fTab, new TGLayoutHints(kLHintsRight, 5, 5, 5, 5));
  fExit = new TGTextButton(all2, "Exit");
  fExit->SetToolTipText("Exit the ALICE masterclasses");
  all2->AddFrame(fExit, new TGLayoutHints(kLHintsExpandX, 5, 5, 10, 10));
  fExit->Connect("Clicked()", "TApplication", gApplication, "Terminate()");
  allall->AddFrame(all, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));
  allall->AddFrame(all2, new TGLayoutHints(kLHintsRight, 5, 5, 5, 5));
  AddFrame(allall);

  SetWindowName("ALICE analysis modules");
  MapSubwindows();

  Resize(GetDefaultSize());

  MapWindow();
}

MasterClassFrame::~MasterClassFrame()
{
  Cleanup();
}

void MasterClassFrame::InfoMasterClass()
{
  TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "MASTERCLASS MENU INSTRUCTIONS", 700, 250);

  instructions->SetText("\
Welcome to ALICE Masterclass!\n\n\
\
  In this frame you can select the dataset which you are supposed to analyse, please ask your tutor \n\
  which number it has, afterwards you just press the picture button representing a detector.\n\n\
  With the buttom \"Exit\" you can finish the exercise.\n");

  instructions->Popup();
}

void MasterClassFrame::InfoRAA()
{
  TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "RAA MENU INSTRUCTIONS", 700, 250);

  instructions->SetText("\
Welcome to ALICE RAA analysis!\n\n\
\
  In this frame you should set the centrality range for the RAA analysis, then start the\n\
  analysis using the picture button below.\n\
  With the buttom \"Exit\" you can finish the exercise.\n");

  instructions->Popup();
}

void MasterClassFrame::InfoS()
{
  TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "MASTERCLASS MENU INSTRUCTIONS", 700, 250);

  instructions->SetText("\
Welcome to ALICE Masterclass!\n\n\
\
  You see the window <MasterClass>.There are 4 buttons to your disposal\n\
  Using these buttons, one can choose the mode:\n\n\
    Demo - starts a demo of MasterClass exercise with 4 events to analyse\n\
\
    Student - starts MasterClass with 100 real events for you to analyze\n\
\
    Teacher - starts Masterclass with Teacher tools that allow to summarise\n\
    the students results and draw conclusions from the analysis\n\n\
\
    The button <Exit> quits MasterClass application\n\n\
\
  NOTE: after you click <Demo> <Student> or <Teacher>, it takes about 30 seconds\n\
  to start the application\n");

  instructions->Popup();
}

void MasterClassFrame::InfoPt()
{
  TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "PT MENU INSTRUCTIONS", 700, 250);

  instructions->SetText("\
Welcome to ALICE Pt analysis!\n\n\
\
  In this frame you should select the dataset to use (pp or PbPb) and the number of files to be processed,\n\
  then start the analysis using the picture button below.\n\
  With the buttom \"Exit\" you can finish the exercise.\n");

  instructions->Popup();
}

void MasterClassFrame::InfoQA()
{
  TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "QA MENU INSTRUCTIONS", 700, 250);

  instructions->SetText("\
Welcome to ALICE QAtrain!\n\n\
\
  Start an example of quality assurance at ALICE by clicking on the picture below.\n\
  The QA is important to control the quality of the data. Especially the reconstruction\n\
  of the interaction vertex is an important key feature, which has to be controled.\n\
  This example processes the selected data and produces a root-file, which contains\n\
  the vertex positions, which are reconstructed with different parts of the detector.\n");

  instructions->Popup();
}

void MasterClassFrame::InfodNdEta()
{
  // Display dNdEta documentation
  // if (!DownloadAndUnpack("dNdEta.tgz")) return;
  StartDocView("dNdEta/Documentation.pdf");
}

void MasterClassFrame::ParseDataset(const char *dataset)
{
  // Opens local dataset file and parses the relevant information
  TString expname = gSystem->ExpandPathName(dataset);
  if (gSystem->AccessPathName(expname))
  {
    printf("ParseDataset: Cannot open file %s", dataset);
    return;
  }
  ifstream infile;
  infile.open(expname);
  char cline[1024];
  TString line;
  TString decode;
  Bool_t ds_start = false;
  Int_t datasets = 0;
  Int_t crtind = 0;    // current search index for line
  Int_t len;
  while (infile.good())
  {
    infile.getline(cline, 1024);
    line = cline;
    line = line.Strip(TString::kLeading, ' ');
    line = line.Strip(TString::kTrailing, ' ');
    crtind = 0;
    if (!ds_start)
    {
      crtind = line.Index("\"Dataset\"");
      if (crtind < 0) continue;
      datasets++;
      ds_start = true;
      crtind += 9;
    }
    crtind = line.Index("\"name\"", crtind);
    if (crtind >= 0)
    {
      crtind = line.Index("\"", crtind + 6);
      if (crtind < 0)
      {
        printf("ParseDataset: Cannot parse value for \"name\" in %s", dataset);
        return;
      }
      len = line.Index("\"", crtind + 1) - crtind - 1;
      fDSname = line(crtind + 1, len);
      continue;
    }
    crtind = line.Index("\"description\"", crtind);
    if (crtind >= 0)
    {
      crtind = line.Index("\"", crtind + 13);
      if (crtind < 0)
      {
        printf("ParseDataset: Cannot parse value for \"description\" in %s", dataset);
        return;
      }
      len = line.Index("\"", crtind + 1) - crtind - 1;
      fDSdescription = line(crtind + 1, len);
      continue;
    }
    crtind = line.Index("\"path\"", crtind);
    if (crtind >= 0)
    {
      crtind = line.Index("\"", crtind + 6);
      if (crtind < 0)
      {
        printf("ParseDataset: Cannot parse value for \"description\" in %s", dataset);
        return;
      }
      len = line.Index("\"", crtind + 1) - crtind - 1;
      fDSpath = line(crtind + 1, len);
      continue;
    }
    crtind = line.Index("\"files\"", crtind);
    if (crtind >= 0)
    {
      crtind = line.Index(":", crtind + 6);
      if (crtind < 0)
      {
        printf("ParseDataset: Cannot parse value for \"files\" in %s", dataset);
        return;
      }
      line = line(crtind + 1, line.Length());
      line = line.Strip(TString::kLeading, ' ');
      fDSnfiles = line.Atoi();
      continue;
    }
  }
  printf("Using dataset: %s === %s (%d files)\n", fDSname.Data(), fDSdescription.Data(), fDSnfiles);
  infile.close();
}

const char *MasterClassFrame::DecodeValue(TString &line)
{
  // Decode the value string from the line
  static TString value;
  value = line(line.Index(' '), line.Length());
  value = value.Strip(TString::kLeading, ' ');
  value = value.Strip(TString::kTrailing, ' ');
  return value.Data();
}


void masterclass()
{
  // gROOT->pwd();

  new MasterClassFrame(gClient->GetRoot(), 400, 200);

  TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "MASTERCLASS MENU INSTRUCTIONS", 700, 250);

  instructions->SetText("\
Welcome to ALICE analysis examples!\n\n\
\
  Use the large picture button showing the ALICE logo to open the documentation for the event \n\
  display and RAA analysis\n\n\
  The picture buttons below will download, unpack and launch the corresponding analysis module.\n\
  Use the <Info> buttons in each frame to get a description of what each module does. \n\n\
  You can exit at any time by using the <Exit> buton of the menu. To restart te masterclass, open\n\
  a terminal by clicking the terminal emulator icon on the bottom left, then type:\n\n\
    [alice@localhost analysis] root masterclass.C\n");

  instructions->Popup();
}
