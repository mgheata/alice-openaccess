#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TList.h>

class MasterClassFrame : public TGMainFrame {

private:

   TGTextButton		*fExample;
   TGTextButton		*fStudent;
   TGTextButton		*fTeacher;
   TGTextButton		*fExit;
   Int_t 		 fChoice;
   Int_t 		 fMode;
   TGComboBox		*fDataset;

public:

   MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~MasterClassFrame();
   void Start();
   void StartExample();
   void StartStudent();
   void StartTeacher();

   ClassDef(MasterClassFrame, 0)
};

void MasterClassFrame::Start()
{

   #if defined(__CINT__) && !defined(__MAKECINT__)
   {
      Info("alice_vsd.C", "Has to be run in compiled mode ... doing this for you.");
      gSystem->CompileMacro("alice_vsd.C");
      alice_vsd(fChoice, fMode, fDataset->GetSelected());
   }
   #else
      alice_vsd(fChoice, fMode, fDataset->GetSelected());
   #endif

   UnmapWindow();

}

void MasterClassFrame::StartExample()
{

   fChoice = 1;
   fMode = 1;

   Start();

}

void MasterClassFrame::StartStudent()
{

   fChoice = 2;
   fMode = 2;

   Start();

}

void MasterClassFrame::StartTeacher()
{

   fChoice = 2;
   fMode = 3;

   Start();

}

MasterClassFrame::MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h) :
   TGMainFrame(p, w, h)
{

   TGVerticalFrame* hf = new TGVerticalFrame(this);
   {

      TGLabel* label = new TGLabel(hf,"MasterClass");
      hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

      TGPictureButton* b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/ALICE_logo.png"));
      hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

      fExample = new TGTextButton(hf,"Demo");
      hf->AddFrame(fExample, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fExample->Connect("Clicked()", "MasterClassFrame", this, "StartExample()");

      fStudent = new TGTextButton(hf,"Student");
      hf->AddFrame(fStudent, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fStudent->Connect("Clicked()", "MasterClassFrame", this, "StartStudent()");

      fTeacher = new TGTextButton(hf,"Teacher");
      hf->AddFrame(fTeacher, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
      fTeacher->Connect("Clicked()", "MasterClassFrame", this, "StartTeacher()");
      
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
      fDataset->AddEntry("dataset 11", 11);
      fDataset->AddEntry("dataset 12", 12);
      fDataset->AddEntry("dataset 13", 13);
      fDataset->AddEntry("dataset 14", 14);
      fDataset->AddEntry("dataset 15", 15);
      fDataset->AddEntry("dataset 16", 16);
      fDataset->AddEntry("dataset 17", 17);
      fDataset->AddEntry("dataset 18", 18);
      fDataset->AddEntry("dataset 19", 19); 
      fDataset->AddEntry("dataset 20", 20);
      fDataset->AddEntry("dataset 21", 21);
   
      fDataset->Resize(100,20);
      fDataset->Select(1,kFALSE);
      hf->AddFrame(fDataset, new TGLayoutHints(kLHintsExpandX));

      fExit = new TGTextButton(hf,"Exit");
      hf->AddFrame(fExit, new TGLayoutHints(kLHintsExpandX, 5, 5, 10, 10));
      fExit->Connect("Clicked()", "TApplication", gApplication, "Terminate()");

   }

   AddFrame(hf);

   SetWindowName("MasterClass");
   MapSubwindows();

   Resize(GetDefaultSize());

   MapWindow();

}

MasterClassFrame::~MasterClassFrame()
{

   Cleanup();

}

void masterclass()
{

   gROOT->pwd();

   new MasterClassFrame(gClient->GetRoot(), 400, 200);

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
   NOTE: after you click <Demo> <Student> or <Teacher>, it takes about 30 seconds to start\n\
         the application\n\n\
\
");

   instructions->Popup();

}
