#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <TGFileDialog.h>
#include <TRootHelpDialog.h>

#include "TImage.h"
#include "TRecorder.h"

#include <TGClient.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGSplitter.h>
#include <TGComboBox.h>

#include <TGLOverlayButton.h>

#include <TG3DLine.h>
#include <TAxis3D.h>
#include <TStyle.h>
#include <TLegend.h>

#include <TApplication.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveWindowManager.h>
#include <TEveVSD.h>
#include <TEveVSDStructs.h>

#include <TCanvas.h>

#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveGeoShape.h>

#include <TEveSelection.h>

#include <TGLEmbeddedViewer.h>
#include <TCanvas.h>
#include <TParticlePDG.h>

#include <TH1D.h>

#include <TGTextEntry.h>

#include <TGLabel.h>

#include <TGTab.h>
#include <TGButton.h>
#include <TTimer.h>

#include <TFile.h>
#include <TKey.h>
#include <TSystem.h>
#include <TPRegexp.h>

#include <TGNumberEntry.h>
#include <TGLWidget.h>

#include "MultiView.C"

using namespace std;

namespace
{
  const char *gMasterClassOutputData[] = {"MasterClass Output Data", "*.masterclass", 0, 0}; //for loading results
}

namespace
{
  const char *gMasterClassOutputPictue[] = {"MasterClass Histograms", "*.png", 0, 0}; //for loading results
}

MultiView* gMultiView = 0;

TH1D* gMultHist = 0;
TH1D* gMultHistMinPt = 0;
TH1D* gPtHist = 0;
TH1D* gChargeHist = 0;
TH1D* gAllMinvHist = 0;

TCanvas* pad1 = 0;
TCanvas* pad2 = 0;
TCanvas* pad = 0;

TGCheckButton* gCheckVertex = 0;
TGCheckButton* gCheckClusters = 0;
TGCheckButton* gCheckITSClusters = 0;
TGCheckButton* gCheckTPCClusters = 0;
TGCheckButton* gCheckTRDClusters = 0;
TGCheckButton* gCheckTOFClusters = 0;
TGCheckButton* gCheckESDTracks = 0;
TGCheckButton* gCheckPrimaryTracks = 0;

TGLabel* gDataset= 0;
TGLabel* gEventNumber = 0;
TGLabel* gEventAnalysed = 0;
TGLabel* gAutomaticCount = 0;
TGLabel* gAutomaticCountMinPt = 0;

TEveLine* pointingLine[10] = {0};
TEveLine* pointingLine1[10] = {0};
TEveLine* pointingLine2[10] = {0};
TEveLine* guideX[12] = {0};
TEveLine* guideY[14] = {0};
TEveLine* guideZ[14] = {0};

Int_t globalChoice = 0;
Int_t globalMode = 0;
Int_t globalDataset = 0;
Int_t nPrim = 0;
Int_t nPrimMinPt = 0;

Int_t detector = 0;
Int_t geometry = 0;

Int_t fCount = 0;
Int_t globalCollSys = 0;
Bool_t change = kTRUE;

//--------------------------------------------------------------
//additional classes
//--------------------------------------------------------------


//----------------KASIA-------------------


class AliceDetectorInfoWindow : public TGMainFrame
{

  TGPictureButton *b1;

	public:

	AliceDetectorInfoWindow(const TGWindow *p, UInt_t w, UInt_t h, Int_t detectorSel) :
	TGMainFrame(p, w, h) {

		DontCallClose();

		b1 = 0;
		TGGroupFrame* gf = 0;
		TGHorizontalFrame *hf = 0;

		switch (detectorSel){
			case 0:
				gf = new TGGroupFrame(this, "ALICE Detector");
				hf = new TGHorizontalFrame(gf, 250, 250);
				b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/ALICE.png"));
				SetWindowName("ALICE Detector Info");
				break;
			case 1:
				gf = new TGGroupFrame(this, "ITS Detector");
				hf = new TGHorizontalFrame(gf, 250, 250);
				b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/ITS.png"));
				SetWindowName("ITS Detector Info");
				break;
			case 2:
				gf = new TGGroupFrame(this, "TPC Detector");
				hf = new TGHorizontalFrame(gf, 250, 250);
				b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/TPC.png"));
				SetWindowName("TPC Detector Info");
				break;
			case 3:
				gf = new TGGroupFrame(this, "TRD and TOF Detectors");
				hf = new TGHorizontalFrame(gf, 250, 250);
				b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/TRDTOF.png"));
				SetWindowName("TRD and TOF Detectors Info");
				break;
			case 4:
				gf = new TGGroupFrame(this, "PHOS Detector");
				hf = new TGHorizontalFrame(gf, 250, 250);
				b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/PHOS.png"));
				SetWindowName("PHOS Detector Info");
				break;
			case 5:
				gf = new TGGroupFrame(this, "HMPID Detector");
				hf = new TGHorizontalFrame(gf, 250, 250);
				b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/HMPID.png"));
				SetWindowName("HMPID Detector Info");
				break;
			default:
				gf = new TGGroupFrame(this, "ALICE Detector");
				hf = new TGHorizontalFrame(gf, 250, 250);
				b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/ALICE.png"));
				SetWindowName("ALICE Detector Info");
				break;
		}

		b1->Connect("Clicked()", "AliceDetectorInfoWindow", this, "UnmapWindow()");

		hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		gf->AddFrame(hf);

		AddFrame(gf);
		MapSubwindows();
		Resize(GetDefaultSize());
		MapWindow();
	}

	ClassDef(AliceDetectorInfoWindow, 0);

};

AliceDetectorInfoWindow* gAliceDetectorInfoWindow = 0;

class AliceDetectorInfo : public TGMainFrame
{

	protected:

	TGPictureButton *b1;

	public:

	AliceDetectorInfo(const TGWindow *p, UInt_t w, UInt_t h) :
	TGMainFrame(p, w, h){

		DontCallClose();

		b1 = 0;
		TGTextButton *b2 = 0;
		TGGroupFrame* gf = new TGGroupFrame(this, "ALICE Detector");
		TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 250, 250);

		b1 = new TGPictureButton(hf, gClient->GetPicture("eve_files/ALICE_small2.png"));
		b1->Connect("Clicked()", "AliceDetectorInfo", this, "MakeBigger()");

		hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		gf->AddFrame(hf);

		hf = new TGHorizontalFrame(gf);
		{
			b2 = new TGTextButton(hf, "ALICE");
			b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowALICE()");
			hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			b2 = new TGTextButton(hf, "ITS");
			b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowITS()");
			hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			b2 = new TGTextButton(hf, "TPC");
			b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowTPC()");
			hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			b2 = new TGTextButton(hf, "TRD+TOF");
			b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowTRDTOF()");
			hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			b2 = new TGTextButton(hf, "PHOS");
			b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowPHOS()");
			hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			b2 = new TGTextButton(hf, "HMPID");
			b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowHMPID()");
			hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		}

		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		hf = new TGHorizontalFrame(gf);
		{
			b2 = new TGTextButton(hf, "Close");
			b2->Connect("Clicked()", "AliceDetectorInfo", this, "UnmapWindow()");
			hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		}

		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		AddFrame(gf);

		SetWindowName("ALICE Detector Info");
		MapSubwindows();

		// Initialize the layout algorithm via Resize()
		Resize(GetDefaultSize());

		// Map main frame
		MapWindow();
	}

	void ShowALICE(){
		b1->SetPicture(gClient->GetPicture("eve_files/ALICE_small2.png"));
		b1->DoRedraw();
		detector = 0;
	}

	void ShowITS(){
		b1->SetPicture(gClient->GetPicture("eve_files/ITS_small.png"));
		b1->DoRedraw();
		detector = 1;
	}

	void ShowTPC(){
		b1->SetPicture(gClient->GetPicture("eve_files/TPC_small.png"));
		b1->DoRedraw();
		detector = 2;
	}

	void ShowTRDTOF(){
		b1->SetPicture(gClient->GetPicture("eve_files/TRDTOF_small.png"));
		b1->DoRedraw();
		detector = 3;
	}

	void ShowPHOS(){
		b1->SetPicture(gClient->GetPicture("eve_files/PHOS_small.png"));
		b1->DoRedraw();
		detector = 4;
	}

	void ShowHMPID(){
		b1->SetPicture(gClient->GetPicture("eve_files/HMPID_small.png"));
		b1->DoRedraw();
		detector = 5;
	}

	void MakeBigger()	{
		gAliceDetectorInfoWindow = new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, detector);
	}

	ClassDef(AliceDetectorInfo, 0);
};

AliceDetectorInfo* gAliceDetectorInfo = 0;


//_______________________________________________________________________________________
//__________________________________ Including Calculator _______________________________
class AliceCounter : public TGMainFrame
{

	protected:

	TGNumberEntryField *fpx;
	TGNumberEntryField *fpy;
	TGNumberEntryField *fpz;
	TGNumberEntryField *fpt;
	TGNumberEntryField *fcharge;
	TGNumberEntryField *fmult;
	TGNumberEntryField *fmultminpt;
	TGNumberEntryField *frapiditycalc;
	TGNumberEntryField *finvariant_masscalc;
	public:

	AliceCounter(const TGWindow *p, UInt_t w, UInt_t h, Int_t collSys) :
	TGMainFrame(p, w, h){
		// Create main frame

 		DontCallClose();
		if (collSys == 2){
			TGGroupFrame* gf = new TGGroupFrame(this, "Counter Instructions");
			TGHorizontalFrame* hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);
			{
				const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
				FontStruct_t buttonFont = font->GetFontStruct();
				ULong_t buttonRedColor;
				gClient->GetColorByName("red", buttonRedColor);
				TGTextButton *b = new TGTextButton(hf,"Instructions");
				b->SetTextColor(buttonRedColor);
				b->SetFont(buttonFont);
				hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				b->Connect("Clicked()", "AliceCounter", this, "Instructions()");
			}

			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			TGLabel *val1 = 0;
			gf = new TGGroupFrame(this, "Particle Properties");
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			val1 = new TGLabel(hf, "Properties");
			val1->SetTextJustify(kTextCenterX);
			hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			val1 = new TGLabel(hf, "px (GeV/c)");
			fpx = new TGNumberEntryField(hf);
			hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			hf->AddFrame(fpx, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			val1 = new TGLabel(hf, "py (GeV/c)");
			fpy = new TGNumberEntryField(hf);
			hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			hf->AddFrame(fpy, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			val1 = new TGLabel(hf, "pz (GeV/c)");
			fpz = new TGNumberEntryField(hf);
			hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			hf->AddFrame(fpz, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			val1 = new TGLabel(hf, "pt (GeV/c)");
			fpt = new TGNumberEntryField(hf);
			hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			hf->AddFrame(fpt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//----------------------------------------------------------------------------
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			val1 = new TGLabel(hf, "charge");
			fcharge = new TGNumberEntryField(hf);
			hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			hf->AddFrame(fcharge, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

			//---------------------------------------------------------------------------
			gf = new TGGroupFrame(this, "Multiplicity");
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			fmult= new TGNumberEntryField(hf);
			fmult->SetEnabled(kFALSE);
			hf->AddFrame(fmult, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			
			gf = new TGGroupFrame(this, "Multiplicity for pt > 1.0 GeV/c");
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			fmultminpt= new TGNumberEntryField(hf);
			fmultminpt->SetEnabled(kFALSE);
			hf->AddFrame(fmultminpt, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			
			//*****************************************************************************
			gf = new TGGroupFrame(this, "Options");
			TGTextButton* b = 0;
			b = new TGTextButton(gf, "Clear");
			b->Connect("Pressed()", "AliceCounter", this, "SetZero()");
			gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//--------------------------------------------
			b = new TGTextButton(gf, "Publish to Mult Histogram");
			b->Connect("Pressed()", "AliceCounter", this, "PublishMult()");
			gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			b = new TGTextButton(gf, "Close");
			b->Connect("Pressed()", "AliceCounter", this, "UnmapWindow()");
			gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			SetWindowName("Counter pp");
			
		} else if (collSys == 3 || collSys == 4 || collSys == 5){
			TGGroupFrame* gf = new TGGroupFrame(this, "Counter Instructions");
			TGHorizontalFrame* hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);
			{
				const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
				FontStruct_t buttonFont = font->GetFontStruct();
				ULong_t buttonRedColor;
				gClient->GetColorByName("red", buttonRedColor);
				TGTextButton *b = new TGTextButton(hf,"Instructions");
				b->SetTextColor(buttonRedColor);
				b->SetFont(buttonFont);
				hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				b->Connect("Clicked()", "AliceCounter", this, "Instructions()");
			}

			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

			TGTextButton* b = 0;
			//---------------------------------------------------------------------------
			gf = new TGGroupFrame(this, "Multiplicity");
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			fmult= new TGNumberEntryField(hf);
			fmult->SetEnabled(kFALSE);
			hf->AddFrame(fmult, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			gf = new TGGroupFrame(this, "Multiplicity for pt > 1.0 GeV/c");
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
			fmultminpt= new TGNumberEntryField(hf);
			fmultminpt->SetEnabled(kFALSE);
			hf->AddFrame(fmultminpt, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			
			// Automatic counting
			gf = new TGGroupFrame(this, "Options");
			b = new TGTextButton(gf, "Count all primary tracks");
			b->Connect("Pressed()", "AliceCounter", this, "CountAutomatic()");
			gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
				TGLabel* explain = new TGLabel(hf);	
				explain->SetText(" Multiplicity:  ");
				hf->AddFrame(explain, new TGLayoutHints(kLHintsExpandX));
				gAutomaticCount = new TGLabel(hf);
				gAutomaticCount->SetText("");
				hf->AddFrame(gAutomaticCount, new TGLayoutHints(kLHintsExpandX));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);	
				TGLabel* explainMinPt = new TGLabel(hf);	
				explainMinPt->SetText(" Mult. pt > 1 GeV/c:  ");
				hf->AddFrame(explainMinPt, new TGLayoutHints(kLHintsExpandX));
				gAutomaticCountMinPt = new TGLabel(hf);
				gAutomaticCountMinPt->SetText("");
				hf->AddFrame(gAutomaticCountMinPt, new TGLayoutHints(kLHintsExpandX));
			gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//*****************************************************************************
			gf = new TGGroupFrame(this, "Options");
			b = new TGTextButton(gf, "Clear");
			b->Connect("Pressed()", "AliceCounter", this, "SetZero2()");
			gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			//---------------------------------------------------------------------------
			b = new TGTextButton(gf, "Close");
			b->Connect("Pressed()", "AliceCounter", this, "UnmapWindow()");
			gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
			SetWindowName("Counter Pb-Pb");
		}
		// Set a name to the main frame      
		
		MapSubwindows();

		// Initialize the layout algorithm via Resize()
		Resize(GetDefaultSize());

		// Map main frame
		MapWindow();
	}

	void DoExit() {
		this->UnmapWindow();
	}

	void CountAutomatic(){
		gAutomaticCount->SetText(TString::Format("%i",nPrim));
		gAutomaticCountMinPt->SetText(TString::Format("%i",nPrimMinPt));
	}
	
	void SetZero() {
		fcharge->SetNumber(0);
		fpx->SetNumber(0);
		fpy->SetNumber(0);
		fpz->SetNumber(0);
		fpt->SetNumber(0);
		fmult->SetNumber(0);
		fmultminpt->SetNumber(0);
	}
	
	void SetZero2() {
		if (gAutomaticCount){
			gAutomaticCount->SetText("");
		}
		if (gAutomaticCount){
			gAutomaticCountMinPt->SetText("");
		}
		fmult->SetNumber(0);
		fmultminpt->SetNumber(0);
	}
	
	void Instructions(){
		TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "CALCULATOR INSTRUCTIONS", 700, 400);
		instructions->SetText("\
Counter Instructions \n\n\
\
The Counter is the tool to help you count the primary tracks in the event-display. By clicking on the tracks the multiplicity is increased by 1 and the properties of the track are displayed. The following properties will be evaluated:\n\
px \t- momentum of the particle in x direction\n\
py \t- momentum of the particle in y direction\n\
pz \t- momentum of the particle in z direction\n\
pt \t- momentum of the particle in the transverse plane (sqrt(px^2 + py^2))\n\
charge \t - sign of the charge of the particle\n\
The transverse momentum (pt) and the charge will be automatically published to a histogram in the header \"Events Characteristics\".\n\n\
The two fields below the particle properties will give you the multiplicity in the event (with or without a minimum pt)\n\n \
The \"Clear\"-button - this button will set all the entries to 0, however the entries in the histograms remain, as they need to be accumulated over several events.\n\n\
The button \"Publish Multiplicity\"- will publish the multiplicity to the corresponding histograms. \n\n\
With the \"Close\"-button you can close the Counter\n\
      ");
		instructions->Popup();
	}

	void Set1(Double_t px1, Double_t py1, Double_t pz1, Int_t charge1){
		fpx->SetNumber(px1);
		fpy->SetNumber(py1);
		fpz->SetNumber(pz1);
		fcharge->SetNumber(charge1);
		Double_t pt1 = TMath::Sqrt(px1*px1+py1*py1);
		fpt->SetNumber(pt1);
		change = kTRUE;
		gPtHist->Fill(pt1);
		gChargeHist->Fill(charge1);
		pad->cd(3);
		gPtHist->Draw();
		pad->cd(3)->Update();
		pad->cd(4);
		gChargeHist->Draw();
		pad->cd(4)->Update();
	}

	void Set2(Double_t px1, Double_t py1, Double_t pz1){
		fpx->SetNumber(px1);
		fpy->SetNumber(py1);
		fpz->SetNumber(pz1);
		change = kTRUE;
	}

	
	void IncreaseMult(){
		Int_t actualMult = fmult->GetNumber();
		Int_t newMult = actualMult+1;
		fmult->SetNumber(newMult);
		if (fpt->GetNumber() > 1.0){
			Int_t actualMultMinPt = fmultminpt->GetNumber();
			Int_t newMultMinPt = actualMultMinPt+1;
			fmultminpt->SetNumber(newMultMinPt);
		}
		
		change = kTRUE;
	}
	
	void IncreaseMult2(Double_t px1, Double_t py1){
		Int_t actualMult = fmult->GetNumber();
		Int_t newMult = actualMult+1;
		fmult->SetNumber(newMult);
		change = kTRUE;
		Double_t pt1 = TMath::Sqrt(px1*px1+py1*py1);
		if (pt1 > 1.0){
			Int_t actualMultMinPt = fmultminpt->GetNumber();
			Int_t newMultMinPt = actualMultMinPt+1;
			fmultminpt->SetNumber(newMultMinPt);
		}
	}

	void PublishMult(){
		gMultHist->Fill(fmult->GetNumber());
		pad->cd(1);
		gMultHist->Draw();
		pad->cd(1)->Update();
		
		gMultHistMinPt->Fill(fmultminpt->GetNumber());
		pad->cd(2);
		gMultHistMinPt->Draw();
		pad->cd(2)->Update();
	
		SetZero();
	}

	ClassDef(AliceCounter, 0);
	
};

AliceCounter* gAliceCounter = 0;


//---------------------------------------------------------------------------

class GroupBox : public TGGroupFrame {
	private:
		TGNumberEntryField *fEntry;

	public:
		GroupBox(const TGWindow *p, const char *name, const char *title);
		TGNumberEntryField *GetEntry() const { return fEntry; }
		Double_t GetNumber() const { return fEntry->GetNumber(); }

		ClassDef(GroupBox, 0)
	};

	GroupBox::GroupBox(const TGWindow *p, const char *name, const char *title) :
	TGGroupFrame(p, name) {

	TGHorizontalFrame *hf = new TGHorizontalFrame(this);

	TGLabel *label = new TGLabel(hf, title);
	hf->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

	AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	fEntry = new TGNumberEntryField(this);
	AddFrame(fEntry, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

}

//------------------------------------------------------

class SigTestSpitter
{
	TEveSelection *fSel;
	TString        fPrefix;

	public:
	SigTestSpitter(TEveSelection* sel, const TString& prefix) :
		fSel(sel), fPrefix(prefix)
		{
		fSel->Connect("SelectionAdded(TEveElement*)", "SigTestSpitter", this, "Added(TEveElement*)");
			}

	// ----------------------------------------------------------------

	void Added(TEveElement* el)
	{

		if(el->GetElementName() == TString::Format("ITS")) {
			new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, 1);
			return;
		}

		if(el->GetElementName() == TString::Format("TPC")) {
			new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, 2);
			return;
		}

		if(el->GetElementName() == TString::Format("TRD") || el->GetElementName() == TString::Format("TOF")){
			new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, 3);
			return;
		}

		if(el->GetElementName() == TString::Format("ESD_Track")){
			TEveTrack *track = (dynamic_cast<TEveTrack*>(el));
			if(gAliceCounter){
				if (track->GetLineColor()==kRed){
					TRootHelpDialog* startMessage = new TRootHelpDialog(gClient->GetRoot(), "warning", 250, 100);
					startMessage->SetText("This tracks has already been counted");
					startMessage->Popup();
				} else {
					track->SetLineColor(kRed);
					if (globalCollSys == 2){
						gAliceCounter->Set1(track->GetMomentum().fX, track->GetMomentum().fY, track->GetMomentum().fZ, track->GetCharge() );
						gAliceCounter->IncreaseMult();
					} else if (globalCollSys == 3 || globalCollSys == 4 || globalCollSys == 5 ){
						gAliceCounter->IncreaseMult2(track->GetMomentum().fX, track->GetMomentum().fY);
					}	else { 
						gAliceCounter->Set2(track->GetMomentum().fX, track->GetMomentum().fY, track->GetMomentum().fZ);
					}	
				}
			} else {
				TRootHelpDialog* startMessage = new TRootHelpDialog(gClient->GetRoot(), "warning", 250, 100);
				startMessage->SetText("Start the Counter first!");
				startMessage->Popup();
		}
			return;
		}
	}
};

SigTestSpitter* gSigTestSpitter = 0;

//---------------------------------------------------------------------------

class TVSDReader
{
	public:
	// ----------------------------------------------------------
	// File / Event Data
	// ----------------------------------------------------------

	TFile      *fFile;
	TDirectory *fDirectory;

	TObjArray  *fEvDirKeys;

	TEveVSD    *fVSD;

	Int_t       fMaxEv, fCurEv, fMaxR, fRAnim;

	Bool_t      fVertexLoaded, fITSClustersLoaded, fTPCClustersLoaded, fTRDClustersLoaded, fTOFClustersLoaded,  fClustersLoaded, fEsdTracksLoaded, fPrimaryTracksLoaded, fGuidesLoaded, fGeometrySet, fInDemoMode;

	// ----------------------------------------------------------
	// Event visualization structures
	// ----------------------------------------------------------

	TEveTrackList *fTrackList;
	TEveTrackList *fTrackListPrim;
	
	TEvePointSet  *fPsVertex;
	TEvePointSet  *fITSClusters;
	TEvePointSet  *fTPCClusters;
	TEvePointSet  *fTRDClusters;
	TEvePointSet  *fTOFClusters;

	TTimer        *fTimer;
	
	
	TGLOverlayButton*	obutton1;
	TGLOverlayButton*	obutton2;
	TGLOverlayButton*	obutton3;
	TGLOverlayButton*	overlayButton;

	public:
	TVSDReader(const char* file_name) :
		fFile(0), fDirectory(0), fEvDirKeys(0), 
		fVSD(0),

		fMaxEv(-1), fCurEv(-1), fMaxR(600), fRAnim(0), 

		fVertexLoaded(kFALSE), fITSClustersLoaded(kFALSE), fTPCClustersLoaded(kFALSE), fTRDClustersLoaded(kFALSE),
		fTOFClustersLoaded(kFALSE), fClustersLoaded(kFALSE), fEsdTracksLoaded(kFALSE), fPrimaryTracksLoaded(kFALSE), fGuidesLoaded(kFALSE),
		fGeometrySet(kTRUE), fInDemoMode(kFALSE),

		fTrackList(0), fTrackListPrim(0),
		fPsVertex(0), fITSClusters(0), fTPCClusters(0), fTRDClusters(0), fTOFClusters(0), fTimer(0),
		obutton1(0), obutton2(0), obutton3(0), overlayButton(0)

	{
		fFile = TFile::Open(file_name);
		if (!fFile){
			Error("VSD_Reader", "Can not open file '%s' ... terminating.", file_name);
			gSystem->Exit(1);
		}

		fEvDirKeys = new TObjArray;
		TPMERegexp name_re("Event\\d+");
		TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
		while (lnk){
			if (name_re.Match(lnk->GetObject()->GetName())){
					fEvDirKeys->Add(lnk->GetObject());
			}
			lnk = lnk->Next();
		}

		fMaxEv = fEvDirKeys->GetEntriesFast();
		if (fMaxEv == 0) {
			Error("VSD_Reader", "No events to show ... terminating.");
			gSystem->Exit(1);
		}

		fVSD = new TEveVSD;

		if(gMultiView) {
			obutton1 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "use ROLLER to zoom in/out", 0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-20, 250, 20);
			obutton2 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "click LEFT button to rotate", 0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-40, 250, 20);
			obutton3 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "press ROLLER to move", 0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-60, 250, 20);
		} else {
			obutton1 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "use ROLLER to zoom in/out", 0, 0, 250, 20);
			obutton2 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "click LEFT button to rotate", 0, 0, 250, 20);
			obutton3 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "press ROLLER to move", 0, 0, 250, 20);
		}

		obutton1->SetAlphaValues(0.05, 0.8);
		obutton2->SetAlphaValues(0.05, 0.8);
		obutton3->SetAlphaValues(0.05, 0.8);
	}

	virtual ~TVSDReader()
	{
		// Destructor.

		DropEvent();

		delete fVSD;
		delete fEvDirKeys;

		fFile->Close();
		delete fFile;
	}

	void ToStudent(){
		DropEvent();

		fInDemoMode = kFALSE;

		globalChoice = 2;
		globalMode = 2;

		delete fVSD;
		delete fEvDirKeys;

		fFile->Close();
		delete fFile;


		fFile = TFile::Open(Form("eve_files/data/AliVSD_Masterclass_%i.root",globalDataset));
		cout << "and here is second"<< endl;
		
	//    fFile = TFile::Open("AliVSD.root");

		if (!fFile) {
			Error("VSD_Reader", "Can not open the demo file ... terminating.");
			gSystem->Exit(1);
		}

		fEvDirKeys = new TObjArray;
		TPMERegexp name_re("Event\\d+");
		TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
		while (lnk) {
			if (name_re.Match(lnk->GetObject()->GetName())){
				fEvDirKeys->Add(lnk->GetObject());
			}
			lnk = lnk->Next();
		}

		fMaxEv = fEvDirKeys->GetEntriesFast();
		if (fMaxEv == 0) {
			Error("VSD_Reader", "No events to show ... terminating.");
			gSystem->Exit(1);
		}

		fVertexLoaded = kFALSE;
		fITSClustersLoaded = kFALSE;
		fTPCClustersLoaded = kFALSE;
		fTRDClustersLoaded = kFALSE;
		fTOFClustersLoaded = kFALSE;
		fClustersLoaded = kFALSE;
		fEsdTracksLoaded = kTRUE;
		fPrimaryTracksLoaded = kFALSE;
		fGuidesLoaded = kFALSE;
		fGeometrySet = kTRUE;

		fVSD = new TEveVSD;
		fTimer = new TTimer();
		fTimer->Connect("Timeout()", "TVSDReader", this, "AnimateEsdTracks()");
		StudentSet();
	//      GotoEvent(0);
		gEve->FullRedraw3D();
	}

	void Exit(){
		gEve->GetBrowser()->UnmapWindow();
		gSystem->CleanCompiledMacros();
	}

	void Counter(){
		if (globalCollSys == 1){
			TRootHelpDialog* startMessage = new TRootHelpDialog(gClient->GetRoot(), "warning", 250, 100);
			startMessage->SetText("There is no counter, \nas you do not need to count tracks.");
			startMessage->Popup();
		} else {
			gAliceCounter = new AliceCounter(gClient->GetRoot(), 400, 200, globalCollSys);
		}
	}

	void DetectorInfo(){
		detector = 0;
		gAliceDetectorInfo = new AliceDetectorInfo(gClient->GetRoot(), 100, 100);
	}

	void Instructions(){

			TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "INSTRUCTIONS", 750, 500);

			instructions->SetText("\
Welcome to the ALICE event-diplay, here you see all the tools you need to analyse\n\
the ALICE Events.\n\n\
\
On the right hand side you can see the events in 3 views.\n\
On the left hand side there is your steering board which is divided into segments:\n\
Event Navigation:\n\
   Click on the arrows to go to the previous or to the next event.\n\
   The current event number and number of analysed events are also displayed.\n\n\
\
\
Counter:\n\
   Counter - allows you to count the charged tracks in the event and publishes it to histograms\n\
   in addition it will display the properties of the track like:\n\
   Momentum in x,y,z as well as the charge and the transverse momentum \n\
  \n\
\
Display - this allows you to display or hide elements like:\n\
   Vertex: \t\t\tthe point where the collision took place\n\
   Clusters: \t\t\treconstucted points in the detectors showing where the particle was\n\
   Tracks: \t\t\treconstructed trajectories of the particles traversing the detectors\n\
   Show primary tracks only: \twill reduce the number of tracks shown to the ones originating\n\
   \t\t\t\tin the primary vertex\n\
   Geometry: \t\t\tdrawing of ALICE detectors\n\
   Background: \t\t\tchoose the colour of the display panel\n\
   Axes: \t\t\treference system of ALICE Experiment\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Encyclopedia:\n\
   ALICE detector - description of ALICE and its main components;\n\
   (NOTE: click on the pictures of the detectors to get further\n\
   information)\n\n\
\
In the bottom, there are main application control buttons:\n\
   Exit\n\n\n\
YOUR TASK:  \n\
  In this part of the master class programm you are supposed to evaluate the mean multiplicity\n\
  in pp (first 30 events), peripheral (31st event), semicentral (32nd event) and central (33rd \n\
  event) Pb-Pb-collisions and determine the integrated nuclear modification factor for these 3 \n\
  Pb-Pb events.\n \n\
  But first you need to make yourself familiar with this tool. \n\
  In the event-display you have the possibilty to display the clusters (reconstructed space points)\n\
  in the different detectors. From these clusters the paths on which a particles went through the \n\
  detector are reconstructed - so called tracks. Based on the tracks one can reconstruct the \n\
  collision point (primary vertex) from which most of the tracks should come. However as the \n\
  some of the detectors are very large one might see several points in which a lot of tracks \n\
  originate. The primary collision however should have taken place close to the center of our \n\
  coordinate system (X~0, Y~0, Z~0), as then all detectors are able to see the collision and \n\
  the tracks can be much better reconstructed. The tracks originating in the other vertices are \n\
  called pile-up and do not need to be counted for the multiplicity of the event. Therefore you \n\
  can just show the primary tracks (originating in the primary vertex) by clicking on the button \n\
  \"Show primary tracks only\". If you click it again you will see the full event again.\n\n\
  To achieve the final goal you should use the \"Counter\". This tool will help you count the \n\
  number of primary tracks. It will increase the multiplicity in this event by one if you click on \n\
  a track, which will then turn red to avoid double counting.\n\
  After having clicked the button \"Event analysed\" the multiplicity of this event will be published\n\
  to the corresponding histogram in the header \"Event Characteristics\". \n\
  In the header \"Event Characteristics\" the histograms are aranged as follows: \n \
  top left:\t\t multiplicity per event or as one could also say number of tracks\n \
  top right:\t\t multiplicity per event, for tracks with a transverse momentum (pt)\n \
  \t\t\t bigger 1.0 GeV/c \n \
  bottom left:\t\t transverse momentum distribution for every analysed track \n \
  bottom right:\t sign of the charge of every analysed track\n\
  The mean multiplicity can be extracted either with a pt-threshold or without and can be seen in the \n\
  statistics box of the corresponding histogram in the top row.\
   ");
// 			instructions->DontCallClose();
			instructions->Popup();
	}


	void StudentSet(){

		if(globalMode == 3){
			globalMode = 2;
			globalChoice = 2;
		}

// 		if(globalMode == 1 && !fInDemoMode) {
// 			ToDemo();
// 			return;
// 		}

		Instructions();

		TEveBrowser* browser = gEve->GetBrowser();

		/*if(globalChoice == 1)
			browser->GetMainFrame()->SetWindowName("ALICE MasterClass - DEMO MODE");
		else
		*/	
		browser->GetMainFrame()->SetWindowName("ALICE MasterClass - STUDENT MODE");

		if(browser->GetTabRight()->GetTabTab(3))
			browser->GetTabRight()->RemoveTab(3);

		if(browser->GetTabRight()->GetTabTab(2))
			browser->GetTabRight()->RemoveTab(2);

		if(browser->GetTabLeft()->GetTabTab(0))
			browser->GetTabLeft()->RemoveTab(0);

		if(browser->GetTabBottom()->GetTabTab(0))
			browser->GetTabBottom()->RemoveTab(0);

		browser->StartEmbedding(TRootBrowser::kLeft);

		TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
		frmMain->SetWindowName("XX GUI");
		frmMain->SetCleanup(kDeepCleanup);

		TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
		TGPictureButton* b1 = 0;
		TGTextButton* b = 0;
		TGLabel* label = 0;
		TGGroupFrame* gf = 0;
		TGVerticalFrame* vf = 0;
		TGHorizontalFrame* hf = 0;
		ULong_t buttonRedColor;

		gf = new TGGroupFrame(frmMain, "Student Instructions");

		vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
		{
			const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
			FontStruct_t buttonFont = font->GetFontStruct();
			gClient->GetColorByName("red", buttonRedColor);
			b = new TGTextButton(vf,"Instructions");
			b->SetTextColor(buttonRedColor);
			b->SetFont(buttonFont);
			vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			b->Connect("Clicked()", "TVSDReader", this, "Instructions()");
		}

		gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		frmMain->AddFrame(gf);

		//------------------------------------------------------

		gf = new TGGroupFrame(frmMain, "Event Navigation");

		vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
		{
			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{
				label = new TGLabel(hf, "Previous");
				hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				label = new TGLabel(hf, "Current");
				hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				label = new TGLabel(hf, "Next");
				hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			}

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{
				b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoBack.gif"));
				hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				b1->Connect("Clicked()", "TVSDReader", this, "PrevEvent()");
				gEventNumber = new TGLabel(hf);
				gEventNumber->SetText(1);
				hf->AddFrame(gEventNumber, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoForward.gif"));
				hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				b1->Connect("Clicked()", "TVSDReader", this, "NextEvent()");
			}
			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{				
				b = new TGTextButton(hf, "Event analysed!");
				hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				b->Connect("Clicked()", "TVSDReader", this, "CountEvents()");
			}
			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{
				label = new TGLabel(hf, "Events done: ");
				hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));	
				gEventAnalysed = new TGLabel(hf);
				gEventAnalysed->SetText(0);
				hf->AddFrame(gEventAnalysed, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			}
			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
		}

		gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		frmMain->AddFrame(gf);

		gf = new TGGroupFrame(frmMain, "Analysis Tool");

		vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
		{
			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{
				
				gDataset = new TGLabel(hf);
				gDataset->SetText("pp, 7 TeV, B=0 T");
				hf->AddFrame(gDataset, new TGLayoutHints(kLHintsExpandX));
			}
			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{			
				b = new TGTextButton(hf, "Counter");
				hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
				b->Connect("Clicked()", "TVSDReader", this, "Counter()");
			}
			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
		}

		gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		frmMain->AddFrame(gf);

		gf = new TGGroupFrame(frmMain, "Display");

		vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
		{
			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			b = new TGTextButton(hf,"Vertex");
			b->Connect("Clicked()", "TVSDReader", this, "DisplayVertex()");
			gCheckVertex = new TGCheckButton(hf, "", 10);
			gCheckVertex->SetEnabled(kFALSE);

			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
			hf->AddFrame(gCheckVertex);

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);

			b = new TGTextButton(hf,"Clusters");
			b->Connect("Clicked()", "TVSDReader", this, "DisplayClusters()");
			gCheckClusters = new TGCheckButton(hf, "", 10);
			gCheckClusters->SetEnabled(kFALSE);

			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
			hf->AddFrame(gCheckClusters);

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);
			
			b = new TGTextButton(hf, "Tracks");
			b->Connect("Clicked()", "TVSDReader", this, "DisplayEsdTracks()");
			gCheckESDTracks = new TGCheckButton(hf, "", 10);
			gCheckESDTracks->SetEnabled(kFALSE);

			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
			hf->AddFrame(gCheckESDTracks);

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);
			
			b = new TGTextButton(hf, "Show primary tracks only");
			b->Connect("Clicked()", "TVSDReader", this, "DisplayPrimaryTracks()");
			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			
			hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);

			b = new TGTextButton(hf, "Geometry");
			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			b->Connect("Clicked()", "TVSDReader", this, "ChangeGeometry()");

			b = new TGTextButton(hf, "Axes");
			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			b->Connect("Clicked()", "TVSDReader", this, "DisplayGuides()");

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			b = new TGTextButton(vf, "Background");
			vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			b->Connect("Clicked()", "TVSDReader", this, "ChangeBackgroundColor()");

		}

		gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		frmMain->AddFrame(gf);
	
		gf = new TGGroupFrame(frmMain, "Encyclopaedia");
		vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
		{
			b = new TGTextButton(vf, "ALICE Detector");
			b->Connect("Clicked()", "TVSDReader", this, "DetectorInfo()");
			vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		}

		gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
		frmMain->AddFrame(gf);

		vf = new TGVerticalFrame(frmMain, 200, 150, kFixedWidth);
		{
			b1 = new TGPictureButton(vf, gClient->GetPicture("eve_files/2011-Nov-24-ALICE_logo_scaled.eps"));
		}

		vf->AddFrame(b1);
		frmMain->AddFrame(vf);

		frmMain->MapSubwindows();
		frmMain->Resize();
		frmMain->MapWindow();

		browser->StopEmbedding("Student");

		browser->StartEmbedding(TRootBrowser::kBottom);

		frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600, kHorizontalFrame);
		frmMain->SetWindowName("XX GUI");
		frmMain->SetCleanup(kDeepCleanup);

		//   hf = new TGHorizontalFrame(frmMain);

		const TGFont *font = gClient->GetFont("-*-helvetica-bold-r-normal-*-20-*-*-*-*-*-*-*");
		FontStruct_t buttonFont = font->GetFontStruct();


		gClient->GetColorByName("red", buttonRedColor);
		b = new TGTextButton(frmMain,"Exit");
		b->SetTextColor(buttonRedColor);
		b->SetFont(buttonFont);
		//   hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));
		b->Connect("Clicked()", "TApplication", gApplication, "Terminate()");

		frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));
		frmMain->MapSubwindows();
		frmMain->Resize();
		frmMain->MapWindow();

		browser->StopEmbedding(" ");
		browser->StartEmbedding(TRootBrowser::kRight);

		pad = new TCanvas();
		pad->Divide(2, 2);

		pad->cd(1);
		gMultHist->Draw();

		pad->cd(2);
		gMultHistMinPt->Draw();

		pad->cd(3);
		gPtHist->Draw();

		pad->cd(4);
		gChargeHist->Draw();

		browser->StopEmbedding("Event Characteristics");
		GotoEvent(0);
		browser->GetTabRight()->SetTab(1);

	}


	void DestroyGeometry(){
		TEveSceneList* sceneList1=gEve->GetScenes();
		TEveElement::List_i i = sceneList1->BeginChildren();
		TEveSceneList* sceneList2 = (TEveSceneList*) *i;
		sceneList2->RemoveElements();
		i++;
		i++;
		TEveSceneList* sceneList3 = (TEveSceneList*) *i;
		sceneList3->RemoveElements();
		i++;
		TEveSceneList* sceneList4 = (TEveSceneList*) *i;
		sceneList4->RemoveElements();

		gMultiView->SetDepth(-10);
		gMultiView->DestroyGeomRPhi();
		gMultiView->DestroyGeomRhoZ();
		gMultiView->SetDepth(0);
		gMultiView->SetRPhiAxes();
		gMultiView->SetRhoZAxes();
	}

	void GeometryDefault(){

		TEveSceneList* sceneList1=gEve->GetScenes();
		TEveElement::List_i i = sceneList1->BeginChildren();
		TEveSceneList* sceneList2 = (TEveSceneList*) *i;
		sceneList2->RemoveElements();
		i++;
		i++;
		TEveSceneList* sceneList3 = (TEveSceneList*) *i;
		sceneList3->RemoveElements();
		i++;
		TEveSceneList* sceneList4 = (TEveSceneList*) *i;
		sceneList4->RemoveElements();

		TEveGeoShape *gentle_geom = 0;

		TFile* geom = TFile::Open("alice_mini_geom.root");
		if (!geom)
			return;
		TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
		gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
		geom->Close();
		delete geom;

		gentle_geom->SetRnrSelf(kFALSE);

		//ITS

		i = gentle_geom->BeginChildren();

		TEveGeoShape* lvl1 = (TEveGeoShape*) *i;

		//TPC

		i++;

		//TRD+TOF

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		TEveElement::List_i j = lvl1->BeginChildren();

		TEveGeoShape* trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		j++;
		j++;
		j++;
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		j++;
		j++;
		j++;
		j++;
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainTransparency(80);
		j++;

		gEve->AddGlobalElement(gentle_geom);

		gMultiView->SetDepth(-10);
		gMultiView->DestroyGeomRPhi();
		gMultiView->ImportGeomRPhi(gentle_geom);
		gMultiView->DestroyGeomRhoZ();
		gMultiView->ImportGeomRhoZ(gentle_geom);
		gMultiView->SetDepth(0);
		gMultiView->SetRPhiAxes();
		gMultiView->SetRhoZAxes();
	}

	void Geometry2(){

		TEveSceneList* sceneList1=gEve->GetScenes();
		TEveElement::List_i i = sceneList1->BeginChildren();
		TEveSceneList* sceneList2 = (TEveSceneList*) *i;
		sceneList2->RemoveElements();
		i++;
		i++;
		TEveSceneList* sceneList3 = (TEveSceneList*) *i;
		sceneList3->RemoveElements();
		i++;
		TEveSceneList* sceneList4 = (TEveSceneList*) *i;
		sceneList4->RemoveElements();

		TEveGeoShape *gentle_geom = 0;

		TFile* geom = TFile::Open("alice_mini_geom.root");
		if (!geom)
			return;
		TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
		gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
		geom->Close();
		delete geom;

		gentle_geom->SetRnrSelf(kFALSE);
		i = gentle_geom->BeginChildren();

		//ITS

		TEveGeoShape* lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		TEveElement::List_i j = lvl1->BeginChildren();

		TEveGeoShape* lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kFALSE);
		TEveElement::List_i k = lvl2->BeginChildren();

		TEveGeoShape* its1 = (TEveGeoShape*) *k;
		its1->SetRnrSelf(kTRUE);
		its1->SetMainColor(kYellow-4);
		its1->SetMainTransparency(50);
		k++;

		TEveGeoShape* its2 = (TEveGeoShape*) *k;
		its2->SetRnrSelf(kTRUE);
		its2->SetMainColor(kYellow-7);
		its2->SetMainTransparency(50);
		k++;

		TEveGeoShape* its3 = (TEveGeoShape*) *k;
		its3->SetRnrSelf(kTRUE);
		its3->SetMainColor(kYellow-9);
		its3->SetMainTransparency(50);

		//TPC

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		j = lvl1->BeginChildren();

		lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kFALSE);
		k = lvl2->BeginChildren();

		TEveGeoShape* lvl3 = (TEveGeoShape*) *k;
		lvl3->SetRnrSelf(kTRUE);
		lvl3->SetMainColor(kGray);
		lvl3->SetMainTransparency(80);
		TEveElement::List_i l = lvl3->BeginChildren();

		TEveGeoShape* lvl4 = (TEveGeoShape*) *l;
		lvl4->SetRnrSelf(kFALSE);
		TEveElement::List_i m = lvl4->BeginChildren();

		TEveGeoShape* tpc1 = (TEveGeoShape*) *m;
		tpc1->SetRnrSelf(kTRUE);
		tpc1->SetMainColor(kGray+2);
		tpc1->SetMainTransparency(80);
		m++;

		TEveGeoShape* tpc2 = (TEveGeoShape*) *m;
		tpc2->SetMainColor(kGray);
		tpc2->SetMainColor(kGray+2);
		tpc2->SetMainTransparency(80);
		m++;

		TEveGeoShape* tpc3 = (TEveGeoShape*) *m;
		tpc3->SetRnrSelf(kTRUE);
		tpc3->SetMainColor(kGray+2);
		tpc3->SetMainTransparency(80);
		m++;

		//TRD+TOF

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);

		for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j){
			lvl2 = (TEveGeoShape*) *j;
			lvl2->SetRnrSelf(kFALSE);
			lvl2->SetMainColor(0);
			lvl2->SetMainTransparency(80);
		}

		//PHOS

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);

		for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j){
			lvl2 = (TEveGeoShape*) *j;
			lvl2->SetRnrSelf(kTRUE);
			lvl2->SetMainTransparency(30);
		}
		
		gEve->AddGlobalElement(gentle_geom);
		gMultiView->SetDepth(-10);
		gMultiView->DestroyGeomRPhi();
		gMultiView->ImportGeomRPhi(gentle_geom);
		gMultiView->DestroyGeomRhoZ();
		gMultiView->ImportGeomRhoZ(gentle_geom);
		gMultiView->SetDepth(0);
		gMultiView->SetRPhiAxes();
		gMultiView->SetRhoZAxes();

	}


	void Geometry3(){

		TEveSceneList* sceneList1=gEve->GetScenes();
		TEveElement::List_i i = sceneList1->BeginChildren();
		TEveSceneList* sceneList2 = (TEveSceneList*) *i;
		sceneList2->RemoveElements();
		i++;
		i++;
		TEveSceneList* sceneList3 = (TEveSceneList*) *i;
		sceneList3->RemoveElements();
		i++;
		TEveSceneList* sceneList4 = (TEveSceneList*) *i;
		sceneList4->RemoveElements();

		TEveGeoShape *gentle_geom = 0;

		TFile* geom = TFile::Open("alice_mini_geom.root");
		if (!geom)
			return;
		TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
		gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
		geom->Close();
		delete geom;

		gentle_geom->SetRnrSelf(kFALSE);

		i = gentle_geom->BeginChildren();
		//ITS

		TEveGeoShape* lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		TEveElement::List_i j = lvl1->BeginChildren();

		TEveGeoShape* lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kFALSE);
		TEveElement::List_i k = lvl2->BeginChildren();

		TEveGeoShape* its1 = (TEveGeoShape*) *k;
		its1->SetRnrSelf(kTRUE);
		its1->SetMainColor(0);
		its1->SetMainTransparency(80);
		k++;

		TEveGeoShape* its2 = (TEveGeoShape*) *k;
		its2->SetRnrSelf(kTRUE);
		its2->SetMainColor(0);
		its2->SetMainTransparency(80);
		k++;

		TEveGeoShape* its3 = (TEveGeoShape*) *k;
		its3->SetRnrSelf(kTRUE);
		its3->SetMainColor(0);
		its3->SetMainTransparency(80);

		//TPC

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		j = lvl1->BeginChildren();

		lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kFALSE);
		k = lvl2->BeginChildren();

		TEveGeoShape* lvl3 = (TEveGeoShape*) *k;
		lvl3->SetRnrSelf(kTRUE);
		lvl3->SetMainColor(0);
		lvl3->SetMainTransparency(80);
		TEveElement::List_i l = lvl3->BeginChildren();

		TEveGeoShape* lvl4 = (TEveGeoShape*) *l;
		lvl4->SetRnrSelf(kFALSE);
		TEveElement::List_i m = lvl4->BeginChildren();

		TEveGeoShape* tpc1 = (TEveGeoShape*) *m;
		tpc1->SetRnrSelf(kTRUE);
		tpc1->SetMainColor(0);
		tpc1->SetMainTransparency(80);
		m++;

		TEveGeoShape* tpc2 = (TEveGeoShape*) *m;
		tpc2->SetMainColor(kGray);
		tpc2->SetMainColor(0);
		tpc2->SetMainTransparency(80);
		m++;

		TEveGeoShape* tpc3 = (TEveGeoShape*) *m;
		tpc3->SetRnrSelf(kTRUE);
		tpc3->SetMainColor(0);
		tpc3->SetMainTransparency(80);
		m++;

		//TRD+TOF

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		j = lvl1->BeginChildren();

		lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kTRUE);
		lvl2->SetMainColor(0);
		lvl2->SetMainTransparency(80);
		j++;

		lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kTRUE);
		lvl2->SetMainColor(0);
		lvl2->SetMainTransparency(80);

		//PHOS

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);

		for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j) {
			lvl2 = (TEveGeoShape*) *j;
			lvl2->SetRnrSelf(kTRUE);
			lvl2->SetMainColor(0);
			lvl2->SetMainTransparency(80);
		}

		//HMPID

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);

		for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j) {
			lvl2 = (TEveGeoShape*) *j;
			lvl2->SetRnrSelf(kTRUE);
			lvl2->SetMainColor(0);
			lvl2->SetMainTransparency(80);
		}

		gEve->AddGlobalElement(gentle_geom);
		gMultiView->SetDepth(-10);
		gMultiView->DestroyGeomRPhi();
		gMultiView->ImportGeomRPhi(gentle_geom);
		gMultiView->DestroyGeomRhoZ();
		gMultiView->ImportGeomRhoZ(gentle_geom);
		gMultiView->SetDepth(0);
		gMultiView->SetRPhiAxes();
		gMultiView->SetRhoZAxes();
	}

	void Geometry4(){

		TEveSceneList* sceneList1=gEve->GetScenes();
		TEveElement::List_i i = sceneList1->BeginChildren();
		TEveSceneList* sceneList2 = (TEveSceneList*) *i;
		sceneList2->RemoveElements();
		i++;
		i++;
		TEveSceneList* sceneList3 = (TEveSceneList*) *i;
		sceneList3->RemoveElements();
		i++;
		TEveSceneList* sceneList4 = (TEveSceneList*) *i;
		sceneList4->RemoveElements();

		TEveGeoShape *gentle_geom = 0;

		TFile* geom = TFile::Open("alice_mini_geom.root");
		if (!geom)
			return;
		TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
		gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
		geom->Close();
		delete geom;

		gentle_geom->SetRnrSelf(kFALSE);

		i = gentle_geom->BeginChildren();

		//ITS

		TEveGeoShape* lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		TEveElement::List_i j = lvl1->BeginChildren();

		TEveGeoShape* lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kFALSE);
		TEveElement::List_i k = lvl2->BeginChildren();

		TEveGeoShape* its1 = (TEveGeoShape*) *k;
		its1->SetRnrSelf(kTRUE);
		its1->SetMainColor(1);
		its1->SetMainTransparency(80);
		k++;

		TEveGeoShape* its2 = (TEveGeoShape*) *k;
		its2->SetRnrSelf(kTRUE);
		its2->SetMainColor(1);
		its2->SetMainTransparency(80);
		k++;

		TEveGeoShape* its3 = (TEveGeoShape*) *k;
		its3->SetRnrSelf(kTRUE);
		its3->SetMainColor(1);
		its3->SetMainTransparency(80);

		//TPC

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		j = lvl1->BeginChildren();

		lvl2 = (TEveGeoShape*) *j;
		lvl2->SetRnrSelf(kFALSE);
		k = lvl2->BeginChildren();

		TEveGeoShape* lvl3 = (TEveGeoShape*) *k;
		lvl3->SetRnrSelf(kFALSE);
		lvl3->SetMainColor(1);
		lvl3->SetMainTransparency(80);

		TEveElement::List_i l = lvl3->BeginChildren();

		TEveGeoShape* lvl4 = (TEveGeoShape*) *l;
		lvl4->SetRnrSelf(kFALSE);
		TEveElement::List_i m = lvl4->BeginChildren();

		TEveGeoShape* tpc1 = (TEveGeoShape*) *m;
		tpc1->SetRnrSelf(kTRUE);
		tpc1->SetMainColor(1);
		tpc1->SetMainTransparency(80);
		m++;

		TEveGeoShape* tpc2 = (TEveGeoShape*) *m;
		tpc2->SetRnrSelf(kTRUE);
		tpc2->SetMainColor(1);
		tpc2->SetMainTransparency(80);
		m++;

		TEveGeoShape* tpc3 = (TEveGeoShape*) *m;
		tpc3->SetRnrSelf(kTRUE);
		tpc3->SetMainColor(1);
		tpc3->SetMainTransparency(80);
		m++;

		//TRD+TOF

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);
		j = lvl1->BeginChildren();

		TEveGeoShape* trd1 = (TEveGeoShape*) *j;
		trd1->SetRnrSelf(kTRUE);
		trd1->SetMainColor(1);
		trd1->SetMainTransparency(80);
		j++;

		TEveGeoShape* tof1 = (TEveGeoShape*) *j;
		tof1->SetRnrSelf(kTRUE);
		tof1->SetMainColor(1);
		tof1->SetMainTransparency(80);

		//PHOS

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);

		for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j) {
			lvl2 = (TEveGeoShape*) *j;
			lvl2->SetRnrSelf(kTRUE);
			lvl2->SetMainColor(1);
			lvl2->SetMainTransparency(80);
		}

		//HMPID

		i++;

		lvl1 = (TEveGeoShape*) *i;
		lvl1->SetRnrSelf(kFALSE);

		for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j) {
			lvl2 = (TEveGeoShape*) *j;
			lvl2->SetRnrSelf(kTRUE);
			lvl2->SetMainColor(1);
			lvl2->SetMainTransparency(20);
		}
		gEve->AddGlobalElement(gentle_geom);
		gMultiView->SetDepth(-10);
		gMultiView->DestroyGeomRPhi();
		gMultiView->ImportGeomRPhi(gentle_geom);
		gMultiView->DestroyGeomRhoZ();
		gMultiView->ImportGeomRhoZ(gentle_geom);
		gMultiView->SetDepth(0);
		gMultiView->SetRPhiAxes();
		gMultiView->SetRhoZAxes();

	}

	void ChangeGeometry(){

		if(fGeometrySet) {
			DestroyGeometry();
			fGeometrySet = kFALSE;
		} else {
			GeometryDefault();
			fGeometrySet = kTRUE;
		}
	}

	void ChangeBackgroundColor(){
		gEve->GetViewers()->SwitchColorSet();

	}

	void AttachEvent(){
		// Attach event data from current directory.
		fVSD->LoadTrees();
		fVSD->SetBranchAddresses();
	}

	void DropEvent(){
		// Drop currently held event data, release current directory.

		// Drop old visualization structures.
		gEve->GetViewers()->DeleteAnnotations();
		gEve->GetCurrentEvent()->DestroyElements();

		// Drop old event-data.
		fVSD->DeleteTrees();
		delete fDirectory;
		fDirectory = 0;
	}

  //---------------------------------------------------------------------------
  // Event navigation
  //---------------------------------------------------------------------------

	void NextEvent(){
		fPrimaryTracksLoaded =kFALSE;
		if(fCurEv == fMaxEv-1)
			GotoEvent(0);
		else
			GotoEvent(fCurEv + 1);
		if(gAliceCounter){
			if (globalCollSys == 2){
				gAliceCounter->SetZero();
			} else if (globalCollSys == 3 || globalCollSys == 4 || globalCollSys == 5){
				gAliceCounter->SetZero2();
			}
		}
		if (gAutomaticCount){
			gAutomaticCount->SetText("");
		}
		if (gAutomaticCountMinPt){
			gAutomaticCountMinPt->SetText("");
		}
	}

	void PrevEvent(){
		fPrimaryTracksLoaded =kFALSE;
		if(fCurEv == 0)
			GotoEvent(fMaxEv-1);
		else
			GotoEvent(fCurEv - 1);
		if(gAliceCounter){
			if (globalCollSys == 2){
				gAliceCounter->SetZero();
			} else if (globalCollSys == 3 || globalCollSys == 4 || globalCollSys == 5){
				gAliceCounter->SetZero2();
			}
		}
		if (gAutomaticCount){
			gAutomaticCount->SetText("");
		}
		if (gAutomaticCountMinPt){
			gAutomaticCountMinPt->SetText("");
		}
	}

	Bool_t GotoEvent(Int_t ev){
			
		change = kTRUE;

		if (ev < 0 || ev >= fMaxEv){
			Warning("GotoEvent", "fMaxEv = fEvDirKeys->GetEntriesFast();Invalid event id %d.", ev);
			return kFALSE;
		}

		if(fTimer)
			fTimer->Stop();

		if(fRAnim)
			fRAnim=0;

		DropEvent();
	
		fCurEv = ev;
		if (fCurEv == 31){
			gDataset->SetText("PbPb, peripheral");	
			globalCollSys = 3;		
		} else if (fCurEv == 32){
			gDataset->SetText("PbPb, semicentral");	
			globalCollSys = 4;		
		} else if (fCurEv == 33){
			gDataset->SetText("PbPb, central");	
			globalCollSys = 5;		
		} else if (fCurEv == 0){
			gDataset->SetText("pp, 7 TeV, B=0 T");	
			globalCollSys = 1;		
		} else {
			gDataset->SetText("pp, 2.76 TeV, B=0.5 T");	
			globalCollSys = 2;		
		}
		
		fDirectory = (TDirectory*) ((TKey*) fEvDirKeys->At(fCurEv))->ReadObj();

		//      if(fVSD && fDirectory)
		fVSD->SetDirectory(fDirectory);

		AttachEvent();

		//      if(gEventNumber)
		gEventNumber->SetText(TString::Format("%i / %i",fCurEv+1,fMaxEv));

		// Load event data into visualization structures.

		if(fVertexLoaded)
			LoadVertex();

		if(fGuidesLoaded)
			LoadGuides();

		if(fITSClustersLoaded)
			LoadClusters(fITSClusters, "ITS", 0);

		if(fTPCClustersLoaded)
			LoadClusters(fTPCClusters, "TPC", 1);

		if(fTRDClustersLoaded)
			LoadClusters(fTRDClusters, "TRD", 2);

		if(fTOFClustersLoaded)
			LoadClusters(fTOFClusters, "TOF", 3);

		if(fEsdTracksLoaded)
			LoadEsdTracks(fMaxR);

		if(fPrimaryTracksLoaded)
			LoadPrimaryTracks(fMaxR);

		
		if(overlayButton && gMultiView)
			gMultiView->Get3DView()->GetGLViewer()->RemoveOverlayElement((TGLOverlayElement*)overlayButton);

		if(gMultiView) {
			obutton1->SetPosition(0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-25);
			obutton2->SetPosition(0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-45);
			obutton3->SetPosition(0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-65);
		}

		TEveElement* top = gEve->GetCurrentEvent();

		if(gMultiView) {
			gMultiView->DestroyEventRPhi();
			gMultiView->ImportEventRPhi(top);
			gMultiView->DestroyEventRhoZ();
			gMultiView->ImportEventRhoZ(top);
			gEve->Redraw3D(kFALSE, kTRUE);
		}
		return kTRUE;
	}


//____________________________ZLiczanie Eventow!!!!!!!!!!!!!!!

	void CountEvents(){
		if (globalCollSys == 2){
			gAliceCounter->PublishMult();
		}
		gEventAnalysed->SetText(TString::Format("%i",fCount+1));
		fCount++;		
	}



  //---------------------------------------------------------------------------
  // Display Clusters and Tracks
  //---------------------------------------------------------------------------

	void Message() {
		printf("done.\n");
	}

	void DisplayGuides(){
		if(fGuidesLoaded){
			fGuidesLoaded = kFALSE;
			GotoEvent(fCurEv);
		} else {
			fGuidesLoaded = kTRUE;
			GotoEvent(fCurEv);
		}
	}

	void DisplayVertex(){
		if(fVertexLoaded){
			fVertexLoaded=kFALSE;
			GotoEvent(fCurEv);
			gCheckVertex->SetDisabledAndSelected(kFALSE);
		}else {
			fVertexLoaded=kTRUE;
			printf("Loading Vertex...\n");
			GotoEvent(fCurEv);
			printf("Vertex loaded.\n");
			gCheckVertex->SetDisabledAndSelected(kTRUE);	
		}
	}

	void DisplayClusters(){
		if(fClustersLoaded){
			fClustersLoaded = kFALSE;
			fITSClustersLoaded=kFALSE;
			fTPCClustersLoaded=kFALSE;
			fTRDClustersLoaded=kFALSE;
			fTOFClustersLoaded=kFALSE;
			GotoEvent(fCurEv);
			gCheckClusters->SetDisabledAndSelected(kFALSE);
		} else {
			fClustersLoaded = kTRUE;
			fITSClustersLoaded=kTRUE;
			fTPCClustersLoaded=kTRUE;
			fTRDClustersLoaded=kTRUE;
			fTOFClustersLoaded=kTRUE;
			printf("Loading Clusters...\n");
			GotoEvent(fCurEv);
			printf("Clusters loaded.\n");
			gCheckClusters->SetDisabledAndSelected(kTRUE);
		}
	}

	void DisplayITSClusters(){
		if(fITSClustersLoaded) {
			fITSClustersLoaded=kFALSE;
			GotoEvent(fCurEv);
			gCheckITSClusters->SetDisabledAndSelected(kFALSE);
		} else {
			fITSClustersLoaded=kTRUE;
			printf("Loading ITS Clusters...\n");
			GotoEvent(fCurEv);
			printf("ITS Clusters loaded.\n");
			gCheckITSClusters->SetDisabledAndSelected(kTRUE);
		}
	}

	void DisplayTPCClusters() {
		if(fTPCClustersLoaded){
			fTPCClustersLoaded=kFALSE;
			GotoEvent(fCurEv);
			gCheckTPCClusters->SetDisabledAndSelected(kFALSE);
		} else {
			fTPCClustersLoaded=kTRUE;
			printf("Loading TPC Clusters...\n");
			GotoEvent(fCurEv);
			printf("TPC Clusters loaded.\n");
			gCheckTPCClusters->SetDisabledAndSelected(kTRUE);	
		}
	}

	void DisplayTRDClusters() {

		if(fTRDClustersLoaded) {
			fTRDClustersLoaded=kFALSE;
			GotoEvent(fCurEv);
			gCheckTRDClusters->SetDisabledAndSelected(kFALSE);
		} else {
			fTRDClustersLoaded=kTRUE;
			printf("Loading TRD Clusters...\n");
			GotoEvent(fCurEv);
			printf("TRD Clusters loaded.\n");
			gCheckTRDClusters->SetDisabledAndSelected(kTRUE);         
		}
	}

	void DisplayTOFClusters() {

		if(fTOFClustersLoaded){
			fTOFClustersLoaded=kFALSE;
			GotoEvent(fCurEv);
			gCheckTOFClusters->SetDisabledAndSelected(kFALSE);
		} else {
			fTOFClustersLoaded=kTRUE;
			printf("Loading TOF Clusters...\n");
			GotoEvent(fCurEv);
			printf("TOF Clusters loaded.\n");			
			gCheckTOFClusters->SetDisabledAndSelected(kTRUE);
		}
	}

	void DisplayEsdTracks() {

		if(fEsdTracksLoaded) {
			fEsdTracksLoaded=kFALSE;
			GotoEvent(fCurEv);
			if(gCheckESDTracks)
			gCheckESDTracks->SetDisabledAndSelected(kFALSE);
		} else {
			fEsdTracksLoaded=kTRUE;
			printf("Loading Esd Tracks...\n");
			GotoEvent(fCurEv);
			printf("Esd Tracks loaded.\n");
			gCheckESDTracks->SetDisabledAndSelected(kTRUE);
		}
	}

	void DisplayPrimaryTracks() {

		if(fPrimaryTracksLoaded) {
			fPrimaryTracksLoaded=kFALSE;
			GotoEvent(fCurEv);
		} else {
			fPrimaryTracksLoaded=kTRUE;
			printf("Loading Esd Tracks...\n");
			GotoEvent(fCurEv);
			printf("Esd Tracks loaded.\n");
		}
	}


  //---------------------------------------------------------------------------
  // Tracks animating
  //---------------------------------------------------------------------------

	void AnimateEsdTracks(){
		
		TEveTrackPropagator* trkProp = fTrackList->GetPropagator();
		trkProp->SetMaxR(fRAnim); //R[cm] 

		fRAnim+=10;

		gEve->AddElement(fTrackList);

		TEveElement* top = gEve->GetCurrentEvent();

		gMultiView->DestroyEventRPhi();
		gMultiView->ImportEventRPhi(top);

		gMultiView->DestroyEventRhoZ();
		gMultiView->ImportEventRhoZ(top);

		gEve->Redraw3D(kFALSE, kTRUE);

		if(fRAnim>fMaxR)
			fTimer->Stop();
	}

	
	
	void StartAnimation(){
		
		if(fVertexLoaded){
			fVertexLoaded=kFALSE;
			gCheckVertex->SetDisabledAndSelected(kFALSE);
		}

		if(fClustersLoaded){
			fClustersLoaded=kFALSE;
			gCheckClusters->SetDisabledAndSelected(kFALSE);
		}

		if(fITSClustersLoaded){
			fITSClustersLoaded=kFALSE;
		}

		if(fTPCClustersLoaded){
			fTPCClustersLoaded=kFALSE;
		}

		if(fTRDClustersLoaded){
			fTRDClustersLoaded=kFALSE;
		}

		if(fTOFClustersLoaded){
			fTOFClustersLoaded=kFALSE;
		}

		if(fEsdTracksLoaded){
			fEsdTracksLoaded=kFALSE;
			gCheckESDTracks->SetDisabledAndSelected(kFALSE);
		}

		if(fGeometrySet){
			fGeometrySet=kFALSE;
			DestroyGeometry();
		}

		GotoEvent(fCurEv);

		if(fEsdTracksLoaded){
			fEsdTracksLoaded=kFALSE;
			GotoEvent(fCurEv);
		} else {
			LoadEsdTracks(fMaxR);
			fEsdTracksLoaded=kTRUE;
		}  

		fTimer->Start(5,kFALSE);
	}

  void StopAnimation(){
    fTimer->Stop();
  }

  //---------------------------------------------------------------------------
  // Guides
  //---------------------------------------------------------------------------

	void LoadGuides(){

		for(Int_t i=0; i<12; i++){
			guideX[i] = new TEveLine(" ");
			guideX[i]->SetLineColor(kRed);
			guideX[i]->SetLineWidth(1);
			guideX[i]->SetLineStyle(1);
		}

		for(Int_t i=0; i<14; i++) {
			guideY[i] = new TEveLine(" ");
			guideY[i]->SetLineColor(kGreen);
			guideY[i]->SetLineWidth(1);
			guideY[i]->SetLineStyle(1);

			guideZ[i] = new TEveLine(" ");
			guideZ[i]->SetLineColor(kBlue);
			guideZ[i]->SetLineWidth(1);
			guideZ[i]->SetLineStyle(1);
		}

		//line X

		guideX[0]->SetPoint(0, -100,  0,  0);
		guideX[0]->SetPoint(1,  100,  0,  0);

		//arrows X
		guideX[1]->SetPoint(0, -100,  0,  0);
		guideX[1]->SetPoint(1, -95,  5,  0);

		guideX[2]->SetPoint(0, -100,  0,  0);
		guideX[2]->SetPoint(1, -95, -5,  0);

		guideX[3]->SetPoint(0,  100,  0,  0);
		guideX[3]->SetPoint(1,  95,  5,  0);

		guideX[4]->SetPoint(0,  100,  0,  0);
		guideX[4]->SetPoint(1,  95, -5,  0);

		//+X
		guideX[5]->SetPoint(0, 95,  10,  0);
		guideX[5]->SetPoint(1, 90,  10,  0);

		guideX[6]->SetPoint(0, 92.5,  7.5,  0);
		guideX[6]->SetPoint(1, 92.5, 12.5,  0);

		guideX[7]->SetPoint(0,  90,  6,  0);
		guideX[7]->SetPoint(1,  86,  14,  0);

		guideX[8]->SetPoint(0,  90,  14,  0);
		guideX[8]->SetPoint(1,  86, 6,  0);

		//-X
		guideX[9]->SetPoint(0, -85,  10,  0);
		guideX[9]->SetPoint(1, -90,  10,  0);

		guideX[10]->SetPoint(0,  -95,  6,  0);
		guideX[10]->SetPoint(1,  -91,  14,  0);

		guideX[11]->SetPoint(0,  -95,  14,  0);
		guideX[11]->SetPoint(1,  -91, 6,  0);

		//line Y
		guideY[0]->SetPoint(0,  0, -100,  0);
		guideY[0]->SetPoint(1,  0,  100,  0);

		//arrows Y
		guideY[1]->SetPoint(0,  0, -100,  0);
		guideY[1]->SetPoint(1,  0, -95,  5);

		guideY[2]->SetPoint(0,  0, -100,  0);
		guideY[2]->SetPoint(1,  0, -95, -5);

		guideY[3]->SetPoint(0,  0,  100,  0);
		guideY[3]->SetPoint(1,  0,  95,  5);

		guideY[4]->SetPoint(0,  0,  100,  0);
		guideY[4]->SetPoint(1,  0,  95, -5);

		//+Y
		guideY[5]->SetPoint(0, 0, 95,  10);
		guideY[5]->SetPoint(1, 0, 90,  10);

		guideY[6]->SetPoint(0, 0, 92.5,  7.5);
		guideY[6]->SetPoint(1, 0, 92.5, 12.5);

		guideY[7]->SetPoint(0,  0, 96.5,  12.5);
		guideY[7]->SetPoint(1,  0, 92.5,  14.5);

		guideY[8]->SetPoint(0,  0, 96.5,  16.5);
		guideY[8]->SetPoint(1,  0, 92.5, 14.5);

		guideY[9]->SetPoint(0,  0, 92.5,  14.5);
		guideY[9]->SetPoint(1,  0, 88.5, 14.5);


		//-Y
		guideY[10]->SetPoint(0, 0, -92.5,  7.5);
		guideY[10]->SetPoint(1, 0, -92.5, 12.5);

		guideY[11]->SetPoint(0,  0, -88.5,  12.5);
		guideY[11]->SetPoint(1,  0, -92.5,  14.5);

		guideY[12]->SetPoint(0,  0, -88.5,  16.5);
		guideY[12]->SetPoint(1,  0, -92.5, 14.5);

		guideY[13]->SetPoint(0,  0, -92.5,  14.5);
		guideY[13]->SetPoint(1,  0, -96.5, 14.5);

		//line Z
		guideZ[0]->SetPoint(0,  0,  0, -100);
		guideZ[0]->SetPoint(1,  0,  0,  100);

		//Z arrows
		guideZ[1]->SetPoint(0,  0,  0, -100);
		guideZ[1]->SetPoint(1,  8,  0, -92);

		guideZ[2]->SetPoint(0,  0,  0, -100);
		guideZ[2]->SetPoint(1, -8,  0, -92);

		guideZ[3]->SetPoint(0,  0,  0,  100);
		guideZ[3]->SetPoint(1,  8,  0,  92);

		guideZ[4]->SetPoint(0,  0,  0,  100);
		guideZ[4]->SetPoint(1, -8,  0,  92);

		//+Z
		guideZ[5]->SetPoint(0, 0, 10, 90);
		guideZ[5]->SetPoint(1, 0, 10, 85);

		guideZ[6]->SetPoint(0, 0, 7.5, 87.5);
		guideZ[6]->SetPoint(1, 0, 12.5, 87.5);

		guideZ[7]->SetPoint(0,  0, 14, 95);
		guideZ[7]->SetPoint(1,  0, 14, 90);

		guideZ[8]->SetPoint(0,  0, 14, 95);
		guideZ[8]->SetPoint(1,  0, 6, 90);

		guideZ[9]->SetPoint(0,  0, 6, 90);
		guideZ[9]->SetPoint(1,  0, 6, 95);

		//-Z
		guideZ[10]->SetPoint(0, 0, 10, -90);
		guideZ[10]->SetPoint(1, 0, 10, -95);

		guideZ[11]->SetPoint(0,  0, 14, -90);
		guideZ[11]->SetPoint(1,  0, 14, -85);

		guideZ[12]->SetPoint(0,  0, 14, -85);
		guideZ[12]->SetPoint(1,  0, 6, -90);

		guideZ[13]->SetPoint(0,  0, 6, -85);
		guideZ[13]->SetPoint(1,  0, 6, -90);

		for(Int_t i=0; i<12; i++){
			gEve->AddElement(guideX[i]);
		}

		for(Int_t i=0; i<14; i++) {
			gEve->AddElement(guideY[i]);
			gEve->AddElement(guideZ[i]);
		}


	}

  //---------------------------------------------------------------------------
  // Primary Vertex
  //---------------------------------------------------------------------------

	void LoadVertex(){
		if (fPsVertex == 0) {
			fPsVertex = new TEvePointSet("Vertex");
			fPsVertex->SetMainColor(kBlue);
			fPsVertex->SetMarkerSize(2);
			fPsVertex->SetMarkerStyle(2);
			fPsVertex->IncDenyDestroy();
		} else {
			fPsVertex->Reset();
		}

		TEvePointSelector ss(fVSD->fTreeR, fPsVertex, "fPVX:fPVY:fPVZ");
		ss.Select();

		fPsVertex->SetTitle("Primary Vertex");

		gEve->AddElement(fPsVertex);
		TEveLine* pointingLine3 = new TEveLine(TString::Format(" "));
		pointingLine3->SetPoint(0, 0, 0, 0);
		pointingLine3->SetPoint(1, 0, 0, 0);
		pointingLine3->SetLineColor(kBlue);
		pointingLine3->SetLineWidth(0);
		pointingLine3->SetLineStyle(0);
		gEve->AddElement(pointingLine3);
	}

  //---------------------------------------------------------------------------
  // Cluster loading
  //---------------------------------------------------------------------------

	void LoadClusters(TEvePointSet*& ps, const TString& det_name, Int_t det_id){
		if (ps == 0) {
			ps = new TEvePointSet(det_name);
			ps->SetMainColor((Color_t)(det_id + 2));
			ps->SetMarkerSize(0.5);
			ps->SetMarkerStyle(2);
			ps->IncDenyDestroy();
		} else {
			ps->Reset();
		}

		TEvePointSelector ss(fVSD->fTreeC, ps, "fV.fX:fV.fY:fV.fZ",
				TString::Format("fDetId==%d", det_id));
		ss.Select();
		ps->SetTitle(det_name + " Clusters");

		gEve->AddElement(ps);
	}

  //---------------------------------------------------------------------------
  // Track loading
  //---------------------------------------------------------------------------

  enum ESDTrackFlags {
      kITSin=0x0001,kITSout=0x0002,kITSrefit=0x0004,kITSpid=0x0008,
      kTPCin=0x0010,kTPCout=0x0020,kTPCrefit=0x0040,kTPCpid=0x0080,
      kTRDin=0x0100,kTRDout=0x0200,kTRDrefit=0x0400,kTRDpid=0x0800,
      kTOFin=0x1000,kTOFout=0x2000,kTOFrefit=0x4000,kTOFpid=0x8000,
      kHMPIDpid=0x20000,
      kEMCALmatch=0x40000,
      kTRDbackup=0x80000,
      kTRDStop=0x20000000,
      kESDpid=0x40000000,
      kTIME=0x80000000
    };

  Bool_t trackIsOn(TEveTrack* t, Int_t mask){
    // Check is track-flag specified by mask are set.
    return (t->GetStatus() & mask) > 0;
  }

	void LoadEsdTracks(Int_t maxR) {
		// Read reconstructed tracks from current event.

		if (fTrackList == 0) {
			fTrackList = new TEveTrackList("ESD Tracks"); 
			fTrackList->SetMainColor(40);
			fTrackList->SetMarkerColor(kYellow);
			fTrackList->SetMarkerStyle(4);
			fTrackList->SetMarkerSize(0.5);
			fTrackList->IncDenyDestroy();
		} else {
			fTrackList->DestroyElements();
		}

		TEveTrackPropagator* trkProp = fTrackList->GetPropagator();
		if (globalCollSys == 1 ){
			trkProp->SetMagField(0.);
		} else {
			trkProp->SetMagField(0.5);
		}
		trkProp->SetMaxR(maxR); //R[cm]

		Int_t nTracks = fVSD->fTreeR->GetEntries();

		fVSD->fTreeR->GetEntry(0);
		Int_t nTracksInList = fVSD->fR.fIndex;

		Int_t n = 0;

		for (Int_t i = nTracksInList; i < nTracks; ++i){
			fVSD->fTreeR->GetEntry(n);
			TEveTrack* track = new TEveTrack(&fVSD->fR, trkProp);
			track->SetName("ESD_Track");
			track->SetTitle("ESD_Track");
			track->SetAttLineAttMarker(fTrackList);
			fTrackList->AddElement(track);
			n++;
		}
		fTrackList->MakeTracks();
		gEve->AddElement(fTrackList);
	}
 
	void LoadPrimaryTracks(Int_t maxR) {
		// Read reconstructed tracks from current event.

		if (fTrackList == 0) {
			fTrackList = new TEveTrackList("ESD Tracks"); 
			fTrackList->SetMainColor(kBlue);
			fTrackList->SetMarkerColor(kYellow);
			fTrackList->SetMarkerStyle(4);
			fTrackList->SetMarkerSize(0.5);
			fTrackList->IncDenyDestroy();
		}  else {
			fTrackList->DestroyElements();
		}
		
		TEveTrackPropagator* trkProp = fTrackList->GetPropagator();
		if (globalCollSys == 1 ){
			trkProp->SetMagField(0.);
		} else {
			trkProp->SetMagField(0.5);
		}
		trkProp->SetMaxR(maxR); //R[cm]

		Int_t nTracks = fVSD->fTreeR->GetEntries();

		fVSD->fTreeR->GetEntry(0);
		Int_t nTracksInList = fVSD->fR.fIndex;

		Int_t n = 0;
		nPrim = 0;
		nPrimMinPt = 0;
		for (Int_t i = nTracksInList; i < nTracks; ++i){
			fVSD->fTreeR->GetEntry(n);
			TEveTrack *track = new TEveTrack(&fVSD->fR, trkProp);
			Double_t dcaXY = fVSD->fR.fDcaXY;
			Double_t dcaZ = fVSD->fR.fDcaZ;
			if (TMath::Abs(dcaXY) < 0.5 && TMath::Abs(dcaZ) < 1.){
				track->SetLineColor(kBlue);
				track->SetName("ESD_Track");
				track->SetTitle("ESD_Track");
				track->SetAttLineAttMarker(fTrackList);
				fTrackList->AddElement(track);
				nPrim++; 
				Double_t ptTrack = TMath::Sqrt(track->GetMomentum().fX*track->GetMomentum().fX + track->GetMomentum().fY* track->GetMomentum().fY);
				if (ptTrack > 1.0 ) {
					nPrimMinPt++; 
				}
			}
			n++;
		}
 		fTrackList->MakeTracks();
		gEve->Redraw3D();
	}
 
	ClassDef(TVSDReader, 0);
};

TVSDReader* gVSDReader = 0;


//______________________________________________________________________________
void alice_vsd(Int_t choice, Int_t mode, Int_t dataset){

	TString vsd_file_name;


	vsd_file_name = Form("eve_files/data/AliVSD_Masterclass_%i.root",dataset);
   // Download dataset from portal
   TString alice_portal = gSystem->Getenv("ALICE_PORTAL");
   if (alice_portal.IsNull()) {
      printf("ALICE_PORTAL not defined. Exiting.");
      exit(1);
   }
   if (gSystem->AccessPathName(vsd_file_name)) {
   // Download file from the portal
      TString sfile = Form("%s/%s", alice_portal.Data(), vsd_file_name.Data());
      printf("Downloading %s ...\n", sfile.Data());
      if (gSystem->Exec(Form("wget %s -O %s 2>/dev/null", sfile.Data(),vsd_file_name.Data()))) {
//      if (!TFile::Cp(sfile,vsd_file_name, kTRUE)) {     // very slow on VM
         printf("Dataset %d not available on the ALICE portal", dataset);
         exit(1);
      }   
   }
    
	cout << vsd_file_name.Data() << endl;
	
	globalChoice = choice;
	globalMode = mode;
	globalDataset = dataset;

	// Main function, initializes the application.
	//
	// 1. Load the auto-generated library holding ESD classes and ESD dictionaries.
	// 2. Open ESD data-files.
	// 3. Load cartoon geometry.
	// 4. Spawn simple GUI.
	// 5. Load first event.

	TFile::SetCacheFileDir(".");

	TEveVSD::DisableTObjectStreamersForVSDStruct();

	TEveManager::Create(kTRUE,"FV");

	TEveGeoShape *gentle_geom = 0;
	
	TFile* geom = TFile::Open("alice_mini_geom.root");
	if (!geom) return;
	TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
	gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
	geom->Close();
	delete geom;

	gentle_geom->SetRnrSelf(kFALSE);

	//ITS

	TEveElement::List_i i = gentle_geom->BeginChildren();
	TEveGeoShape* lvl1 = (TEveGeoShape*) *i;

	//TPC
	i++;

	//TRD+TOF
	i++;

	lvl1 = (TEveGeoShape*) *i;
	lvl1->SetRnrSelf(kFALSE);
	TEveElement::List_i j = lvl1->BeginChildren();

	TEveGeoShape* trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	j++;
	j++;
	j++;
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	j++;
	j++;
	j++;
	j++;
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

	trd1 = (TEveGeoShape*) *j;
	trd1->SetRnrSelf(kTRUE);
	trd1->SetMainTransparency(80);
	j++;

 	gEve->AddGlobalElement(gentle_geom);

	// Standard multi-view
	//=====================

	gMultiView = new MultiView();
	gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);
	gMultiView->f3DView->GetGLViewer()->SetStyle(0);
	gMultiView->SetDepth(-10);
	gMultiView->ImportGeomRPhi(gentle_geom);
	gMultiView->ImportGeomRhoZ(gentle_geom);
	gMultiView->SetDepth(0);

	// Final stuff
	//=============

	gEve->GetViewers()->SwitchColorSet();
	gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);

	//   make_gui(mode);
	gVSDReader = new TVSDReader(vsd_file_name);

	gVSDReader->fTimer = new TTimer();
	gVSDReader->fTimer->Connect("Timeout()", "TVSDReader", gVSDReader, "AnimateEsdTracks()");

	TEveBrowser* browser = gEve->GetBrowser();

	browser->GetTabLeft()->RemoveTab(1);
	browser->GetTabLeft()->RemoveTab(0);
	
	//   browser->StartEmbedding(TRootBrowser::kRight);
	gStyle->SetOptStat(110);
	gStyle->SetPalette(1,0);
	gStyle->SetFrameBorderMode(0);
	gStyle->SetFrameFillColor(0);
	gStyle->SetTitleFillColor(0);
	gStyle->SetTextSize(0.5);
	gStyle->SetLabelSize(0.03,"xyz");
	gStyle->SetLabelOffset(0.002,"xyz");
	gStyle->SetTitleFontSize(0.04);
	gStyle->SetTitleOffset(1,"y");
	gStyle->SetTitleOffset(0.7,"x");		
	gStyle->SetCanvasColor(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	
	
	gMultHist = new TH1D("Statistics Mult","Multiplicity distribution",10,0.,50.);
	gMultHist->SetLineColor(2);
	gMultHist->GetXaxis()->SetTitle("Multiplicity (# TPC tracks)");
	gMultHist->GetYaxis()->SetTitle("Counts");
	gMultHist->SetFillColor(0);
// 	gMultHist->Draw();

	gMultHistMinPt = new TH1D("Statistics Mult pt > 1.0","Multiplicity distribution pt > 1.0 GeV/c",10,0.,50.);
	gMultHistMinPt->SetLineColor(2);
	gMultHistMinPt->GetXaxis()->SetTitle("Multiplicity (# TPC tracks)");
	gMultHistMinPt->GetYaxis()->SetTitle("Counts");
	gMultHistMinPt->SetFillColor(0);
// 	gMultHistMinPt->Draw();

	
	gPtHist = new TH1D("Statistics pt","p_{t} distribution",50,0.,20.);
	gPtHist->SetLineColor(3);
	gPtHist->GetXaxis()->SetTitle("p_{t} (GeV/c)");
	gPtHist->GetYaxis()->SetTitle("Counts");
	gPtHist->SetFillColor(0);
// 	gPtHist->Draw();

	gChargeHist = new TH1D("Statistics charge","Charge distribution",5,-2.5,2.5);
	gChargeHist->SetLineColor(4);
	gChargeHist->GetXaxis()->SetTitle("Charge ");
	gChargeHist->GetYaxis()->SetTitle("Counts");
	gChargeHist->SetFillColor(0);
// 	gChargeHist->Draw();

	
	gSigTestSpitter = new SigTestSpitter(gEve->GetSelection(), "Selection");

	gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event"));

	gVSDReader->StudentSet();
	gVSDReader->GotoEvent(0);

	gEve->GetWindowManager()->HideAllEveDecorations();
	gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.

}
