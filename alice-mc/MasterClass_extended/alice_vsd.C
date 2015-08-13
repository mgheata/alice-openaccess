#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <TGFileDialog.h>
#include <TRootHelpDialog.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGSplitter.h>

#include <TGLOverlayButton.h>

#include <TLatex.h>
#include <TVirtualFitter.h>

#include <TG3DLine.h>
#include <TAxis3D.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TF1.h>
#include <TEveGValuators.h>
#include <TGDoubleSlider.h>

#include <TApplication.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveWindowManager.h>
#include <TEveVSD.h>
#include <TEveVSDStructs.h>

#include <TCanvas.h>

#include <TGLEmbeddedViewer.h>
#include <TCanvas.h>
#include <TParticlePDG.h>

#include <TH1D.h>

#include <TGTextEntry.h>

#include <TGLabel.h>
#include <TGComboBox.h>

#include <TGTab.h>
#include <TGButton.h>
#include <TTimer.h>

#include <TFile.h>
#include <TKey.h>
#include <TSystem.h>
#include <TPRegexp.h>

#include <TGNumberEntry.h>
#include <TGLWidget.h>

#include <TFileMerger.h>
#include <TSystemDirectory.h>

#include <TEveManager.h>

#include <TEveBrowser.h>
#include <TEveWindow.h>

#include <TAxis3D.h>

using namespace std;

namespace
{
  const char *gMasterClassOutputData[] = {"MasterClass Output Data", "*.masterclass", 0, 0}; //for loading results
}

namespace
{
  const char *gMasterClassOutputPictue[] = {"MasterClass Histograms", "*.png", 0, 0}; //for loading results
}

TH1D* gKMinvHist = 0;
TH1D* gXMinvHist = 0;
TH1D* gLMinvHist = 0;
TH1D* gALMinvHist = 0;
TH1D* gAllMinvHist = 0;

TCanvas* pad1 = 0;
TCanvas* pad2 = 0;

Int_t globalChoice = 0;
Int_t globalMode = 0;
Int_t globalDataset = 0;
Int_t gLoads = 0;

Bool_t foundNeg = kFALSE;
Bool_t foundPos = kFALSE;

Int_t AC = 0;

class TVSDReader
{
public:
  // ----------------------------------------------------------
  // File / Event Data
  // ----------------------------------------------------------

  TFile      *fFile;


  TObjArray  *fEvDirKeys;

  TEveVSD    *fVSD;

  Int_t       fMaxEv, fCurEv, fMaxR; 

  TTimer        *fTimer;
  
  TH1D*			fMinvHisto;
  TCanvas*		fPad;
  TEveGDoubleValuator*	fFitRangeGaus;
  TEveGDoubleValuator*	fFitRangePoly;
  TF1*			fFitGaus;
  TF1*			fFitPolynomial;
TGComboBox*           fComboBoxAntiLambda;
  TGComboBox*           fComboBoxK0;
  TGComboBox*           fComboBoxLambda;

public:
  TVSDReader(const char* file_name) :
    fFile(0), fEvDirKeys(0),
    fVSD(0), fMaxEv(-1), fCurEv(-1), fMaxR(400), 
    fTimer(0), fMinvHisto(0), fPad(0), fFitRangeGaus(0), fFitRangePoly(0),
    fFitGaus(0), fFitPolynomial(0)

  {
    fFile = TFile::Open(file_name);
    if (!fFile)
      {
	Error("VSD_Reader", "Can not open file '%s' ... terminating.", file_name);
	gSystem->Exit(1);
      }

    fEvDirKeys = new TObjArray;
    TPMERegexp name_re("Event\\d+");
    TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
    while (lnk)
      {
	if (name_re.Match(lnk->GetObject()->GetName()))
	  {
            fEvDirKeys->Add(lnk->GetObject());
	  }
	lnk = lnk->Next();
      }

    fMaxEv = fEvDirKeys->GetEntriesFast();
    if (fMaxEv == 0)
      {
	Error("VSD_Reader", "No events to show ... terminating.");
	gSystem->Exit(1);
      }

    fVSD = new TEveVSD;

  }

  virtual ~TVSDReader()
  {
    // Destructor.

    delete fVSD;
    delete fEvDirKeys;

    fFile->Close();
    delete fFile;
  }

 void ToStudent(){


  
    string fileIn;
    switch(globalDataset)
    {

    case 1:
      fileIn="dataset1.txt";
      break;
      default:
     fileIn= "dataset1.txt";
      break;

    }

   fEvDirKeys = new TObjArray;
    TPMERegexp name_re("Event\\d+");
    TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
    while (lnk)
      {
	if (name_re.Match(lnk->GetObject()->GetName()))
	  {
            fEvDirKeys->Add(lnk->GetObject());
	  }
	lnk = lnk->Next();
      }

    fMaxEv = fEvDirKeys->GetEntriesFast();
    if (fMaxEv == 0)
      {
	Error("VSD_Reader", "No events to show ... terminating.");
	gSystem->Exit(1);
      }

    fVSD = new TEveVSD;
    fTimer = new TTimer();
    StudentSet();
}

  void Exit(){


    gSystem->CleanCompiledMacros();

  }

   void Instructions(){

    if(globalMode == 3)
      {

	TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "TEACHER MODE INSTRUCTIONS", 700, 600);

	instructions->SetText("\
Welcome in the <Teacher> mode, here you can see the tools to merge and analyse\n\
your students' results\n\n\
\
On the left hand side panel you can see four panels, called <Teacher Instructions>,\n\
<Teacher Controls>, <Results> and <Encyclopaedia>:\n\n\
\
   Instructions - desribes how to operate the application in the <Teacher> mode\n\
   Teacher controls - allows you to merge and analayze students' results.\n\
      Get Files - this allows to get your students' results\n\
      Save Histograms - saves histograms showing your students' results\n\
   Results\n\
      Level 1 - shows and interprets the analysis results in Level 1\n\
      Level 2 - shows and interprets the analysis results in Level 2\n\n\
Encyclopedia:\n\
   ALICE detector - description of ALICE and its main components;\n\
   V0 Decay Patterns - patterns of the particles you will be looking for;\n\
   (NOTE: click on the pictures of the detectors/decays to get further\n\
   information)\n\n\
\
In the bottom, there are main application control buttons:\n\
   Student Mode - starts the application in the Student mode,\n\
   Exit\n\n\
\
How to merge and analyze the students' results\n\n\
\
   1. Click the button <Get Files> and a window <Open> will appear.\n\
   2. Find the result (filetype *.masterclass) and click <Open>. The result\n\
      will be loaded. Repeat it for all the students' files\n\
   3. Click on the tab <Merged Invariant Mass> (right-hand side) to see\n\
      the invariant mass of the strange particles found by your students.\n\
      Update the histograms by clicking on each of them.\n\
   4. Click on the tab <Merged Rapidity> (right-hand side) to see see the\n\
      rapidity distibutions\n\
   5. Click <Save Histograms>, give the title of the pictures and <Save>\n\
   6. Click <Level 1>. A table called <Strange Particle Statistics> appears\n\
   7. Compare the pre-calculated results from Monte Carlo simulations with the\n\
      results of your students. Do the results agree? Yes? No? What Does it mean?\n\
   8. Close <Strange Particle Statistics> by clicking <Close>\n\
   9. Click <Level 2>. A table called <Particle Ratios> appears. Compare the ratios\n\
      of strange particles to pions in pre-calculated Monte Carlo data\n\
      and your analysis.Do the ratios agree? Is there a strangeness enhancement?\n\
   10. Click <Close>. The analysis is done!\n\n\
\
");
        instructions->DontCallClose();
	instructions->Popup();

      }

}

void CombineK0pp()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("pp-K0.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________

void CombineK0PpPp()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("PbPb-K0.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________

void Combine11()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset11.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________

void Combine12()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset12.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________

void Combine13()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset13.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }

void Combine14()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset14.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }


void Combine15()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset15.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }


void Combine16()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset16.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }


void Combine17()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset17.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }


void Combine18()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset18.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }


void Combine19()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset19.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }

void Combine20()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset20.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,1.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine21()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset21.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }

void Combine22()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset22.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine23()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset23.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine24()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset24.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void Combine25()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset25.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine26()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset26.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine27()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset27.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine28()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset28.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void Combine29()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset29.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine30()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset30.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void CombineLambdapp()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("pp-Lambda.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,0.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void CombineLambdaPpPp()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("PbPb-Lambda.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void Combine31()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset31.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }

void Combine32()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset32.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine33()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset33.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine34()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset34.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void Combine35()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset35.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine36()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset36.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine37()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset37.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine38()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset38.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void Combine39()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset39.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
void Combine40()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("dataset40.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void CombineAntiLambdapp()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("pp-AntiLambda.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________
void CombineAntiLambdaPpPp()
  {
	ifstream file;
	Double_t minv;
        if(AC<4) {
    	AC=AC+1;
	cout << AC << endl; }
    	else  {
    	AC=1;
	cout << AC <<endl; }	
	
    	fPad->cd(AC);

    	fFitPolynomial = NULL;
    	fFitGaus = NULL;

      
		file.open("PbPb-AntiLambda.txt");
       
      
       
 	if( file.good() == false )
	{
   		cout << "No file" << endl;
		return;
	}
    	fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,1.0,2.0);
    	fMinvHisto->SetLineColor(2);
    	fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    	fMinvHisto->GetYaxis()->SetTitle("Counts");
   	fMinvHisto->SetFillColor(0);

	while(!file.eof() )
    	{
		file>>minv;
		cout<<"Invariant Mass : "<<minv<<endl;
                fMinvHisto->Fill(minv);

       }
       fMinvHisto->Draw();
       fPad->Update();
       file.close();
  }
//____________

  void FitInvariantMass()
  {
    
    Double_t MinRange = TMath::Min(fFitRangePoly->GetMin(),fFitRangeGaus->GetMin());
    Double_t MaxRange = TMath::Max(fFitRangePoly->GetMax(),fFitRangeGaus->GetMax());

    if (!fFitGaus) fFitGaus = new TF1("fitGaus","[0]*TMath::Gaus(x,[1],[2])+[3]*x*x+[4]*x+[5]",MinRange,MaxRange);
    else fFitGaus->SetRange(MinRange,MaxRange);

    fFitGaus->SetParameter(0,80.);
    fFitGaus->SetParameter(1,0.5*(fFitRangeGaus->GetMin()+fFitRangeGaus->GetMax()));
    fFitGaus->SetParameter(2,1.);
    fFitGaus->SetParLimits(0,0.,TMath::Power(10.,9));    
    fFitGaus->SetParLimits(1,fFitRangeGaus->GetMin(),fFitRangeGaus->GetMax());    
    fFitGaus->SetParLimits(2,0.,(fFitRangeGaus->GetMax()-fFitRangeGaus->GetMin())/2.);

    fFitGaus->SetLineColor(kGreen+1);
    fMinvHisto->Fit(fFitGaus,"rme");

    TLatex *labels2 = new TLatex();
    labels2->SetTextSize(0.045);
    labels2->SetTextColor(kBlack);
    labels2->SetNDC(kTRUE);
    Double_t LabelX = 0.5;
    Double_t LabelY = 0.8;
    Double_t LabelYstep = 0.05;
      
    Int_t Total_Fit = (Int_t)(fFitGaus->Integral(fFitRangeGaus->GetMin(),fFitRangeGaus->GetMax())/fMinvHisto->GetBinWidth(1));
    Int_t Bck_Fit = (Int_t)(Background_Integral(fFitRangeGaus->GetMin(),fFitRangeGaus->GetMax())/fMinvHisto->GetBinWidth(1));

    labels2->DrawLatex(LabelX, LabelY,Form("Total: %d",Total_Fit));
    labels2->DrawLatex(LabelX, LabelY-LabelYstep,Form("Background: %d",Bck_Fit));
    labels2->DrawLatex(LabelX, LabelY-2.*LabelYstep,Form("Signal: %d",Total_Fit-Bck_Fit));
    labels2->DrawLatex(LabelX, LabelY-3.*LabelYstep,Form("Mean: %f#pm%f",fFitGaus->GetParameter(1)*1000., fFitGaus->GetParError(1)*1000.));
    labels2->DrawLatex(LabelX, LabelY-4.*LabelYstep,Form("#sigma: %f#pm%f",fFitGaus->GetParameter(2)*1000., fFitGaus->GetParError(2)*1000.));
    
    if(!fFitPolynomial) fFitPolynomial = new TF1("fitPoly","[0]*x*x+[1]*x+[2]",MinRange,MaxRange);
    else fFitPolynomial->SetRange(MinRange,MaxRange);
    fFitPolynomial->SetParameter(0,fFitGaus->GetParameter(3));
    fFitPolynomial->SetParameter(1,fFitGaus->GetParameter(4));
    fFitPolynomial->SetParameter(2,fFitGaus->GetParameter(5));
    fFitPolynomial->SetLineColor(kBlue+1);
    fFitPolynomial->Draw("SAME");
      
    fPad->Update();
  }
    
    Double_t Background_Integral(Double_t xmin, Double_t xmax) {
    
        return (xmax-xmin)*(fFitGaus->GetParameter(5)+fFitGaus->GetParameter(4)*(xmax+xmin)/2.)+(fFitGaus->GetParameter(3)*(TMath::Power(xmax,3) - TMath::Power(xmin,3))/3.);
        
    }


 void AutomaticMenu()
  {

    TEveBrowser* browser = gEve->GetBrowser();

    browser->GetMainFrame()->SetWindowName(Form("ALICE MasterClass - STUDENT MODE - "));
    browser->CloseTab(0);
    browser->StartEmbedding(TRootBrowser::kLeft);

    TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
    frmMain->SetWindowName("XX GUI");
    frmMain->SetCleanup(kDeepCleanup);

    TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
    TGTextButton* b = 0;
    TGGroupFrame* gf = 0;
    TGVerticalFrame* vf = 0;
    TGHorizontalFrame* hf = 0;
    ULong_t buttonRedColor;

//-------------------New GUI--------------------------
  // tab widget
   TGTab *fTab1590 = new TGTab(frmMain,128,280);

   // container of "Tab1"
   TGCompositeFrame *fCompositeFrame1593;
   fCompositeFrame1593 = fTab1590->AddTab("K0");
   fCompositeFrame1593->SetLayoutManager(new TGVerticalLayout(fCompositeFrame1593));
   TGCheckButton *fTextButton2153 = new TGCheckButton(fCompositeFrame1593,"pp");
 fTextButton2153->Connect("Clicked()", "TVSDReader", this, "CombineK0pp()");
   fTextButton2153->SetTextJustify(36);
   fTextButton2153->SetMargins(0,0,0,0);
   fTextButton2153->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2153, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2190 = new TGCheckButton(fCompositeFrame1593,"PbPb");
 fTextButton2190->Connect("Clicked()", "TVSDReader", this, "CombineK0PpPp()");
   fTextButton2190->SetTextJustify(36);
   fTextButton2190->SetMargins(0,0,0,0);
   fTextButton2190->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2190, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2197 = new TGCheckButton(fCompositeFrame1593,"PbPb-0-10%");
 fTextButton2197->Connect("Clicked()", "TVSDReader", this, "Combine11()");
   fTextButton2197->SetTextJustify(36);
   fTextButton2197->SetMargins(0,0,0,0);
   fTextButton2197->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2197, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2204 = new TGCheckButton(fCompositeFrame1593,"PbPb-10-20%");
 fTextButton2204->Connect("Clicked()", "TVSDReader", this, "Combine12()");
   fTextButton2204->SetTextJustify(36);
   fTextButton2204->SetMargins(0,0,0,0);
   fTextButton2204->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2204, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2211 = new TGCheckButton(fCompositeFrame1593,"PbPb-20-30%");
  fTextButton2211->Connect("Clicked()", "TVSDReader", this, "Combine13()");
   fTextButton2211->SetTextJustify(36);
   fTextButton2211->SetMargins(0,0,0,0);
   fTextButton2211->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2211, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2218 = new TGCheckButton(fCompositeFrame1593,"PbPb-30-40%");
  fTextButton2218->Connect("Clicked()", "TVSDReader", this, "Combine14()");
   fTextButton2218->SetTextJustify(36);
   fTextButton2218->SetMargins(0,0,0,0);
   fTextButton2218->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2218, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2225 = new TGCheckButton(fCompositeFrame1593,"PbPb-40-50%");
  fTextButton2225->Connect("Clicked()", "TVSDReader", this, "Combine15()");
   fTextButton2225->SetTextJustify(36);
   fTextButton2225->SetMargins(0,0,0,0);
   fTextButton2225->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2225, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2232 = new TGCheckButton(fCompositeFrame1593,"PbPb-50-60%");
 fTextButton2232->Connect("Clicked()", "TVSDReader", this, "Combine16()");
   fTextButton2232->SetTextJustify(36);
   fTextButton2232->SetMargins(0,0,0,0);
   fTextButton2232->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2232, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2239 = new TGCheckButton(fCompositeFrame1593,"PbPb-60-70%");
fTextButton2239->Connect("Clicked()", "TVSDReader", this, "Combine17()");
   fTextButton2239->SetTextJustify(36);
   fTextButton2239->SetMargins(0,0,0,0);
   fTextButton2239->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2239, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2246 = new TGCheckButton(fCompositeFrame1593,"PbPb-70-80%");
fTextButton2246->Connect("Clicked()", "TVSDReader", this, "Combine18()");
   fTextButton2246->SetTextJustify(36);
   fTextButton2246->SetMargins(0,0,0,0);
   fTextButton2246->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2246, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2255 = new TGCheckButton(fCompositeFrame1593,"PbPb-80-90%");
fTextButton2255->Connect("Clicked()", "TVSDReader", this, "Combine19()");
   fTextButton2255->SetTextJustify(36);
   fTextButton2255->SetMargins(0,0,0,0);
   fTextButton2255->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2255, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2262 = new TGCheckButton(fCompositeFrame1593,"PbPb-90-100%");
fTextButton2262->Connect("Clicked()", "TVSDReader", this, "Combine20()");
   fTextButton2262->SetTextJustify(36);
   fTextButton2262->SetMargins(0,0,0,0);
   fTextButton2262->SetWrapLength(-1);
   fCompositeFrame1593->AddFrame(fTextButton2262, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

 //char* file1="PbPb-K0.txt";
   // container of "Tab2"
   TGCompositeFrame *fCompositeFrame1595;
   fCompositeFrame1595 = fTab1590->AddTab("Lambda");
   fCompositeFrame1595->SetLayoutManager(new TGVerticalLayout(fCompositeFrame1595));
   TGCheckButton *fTextButton2269 = new TGCheckButton(fCompositeFrame1595,"pp");
  fTextButton2269->Connect("Clicked()", "TVSDReader", this, "CombineLambdapp()");
   fTextButton2269->SetTextJustify(36);
   fTextButton2269->SetMargins(0,0,0,0);
   fTextButton2269->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2269, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2276 = new TGCheckButton(fCompositeFrame1595,"PbPb");
fTextButton2276->Connect("Clicked()", "TVSDReader", this, "CombineLambdaPpPp()");
   fTextButton2276->SetTextJustify(36);
   fTextButton2276->SetMargins(0,0,0,0);
   fTextButton2276->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2276, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2283 = new TGCheckButton(fCompositeFrame1595,"PbPb-0-10%");
fTextButton2283->Connect("Clicked()", "TVSDReader", this, "Combine21()");
   fTextButton2283->SetTextJustify(36);
   fTextButton2283->SetMargins(0,0,0,0);
   fTextButton2283->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2283, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2290 = new TGCheckButton(fCompositeFrame1595,"PbPb-10-20%");
fTextButton2290->Connect("Clicked()", "TVSDReader", this, "Combine22()");
   fTextButton2290->SetTextJustify(36);
   fTextButton2290->SetMargins(0,0,0,0);
   fTextButton2290->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2290, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2297 = new TGCheckButton(fCompositeFrame1595,"PbPb-20-30%");
fTextButton2297->Connect("Clicked()", "TVSDReader", this, "Combine23()");
   fTextButton2297->SetTextJustify(36);
   fTextButton2297->SetMargins(0,0,0,0);
   fTextButton2297->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2297, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2304 = new TGCheckButton(fCompositeFrame1595,"PbPb-30-40%");
fTextButton2304->Connect("Clicked()", "TVSDReader", this, "Combine24()");
   fTextButton2304->SetTextJustify(36);
   fTextButton2304->SetMargins(0,0,0,0);
   fTextButton2304->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2304, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2311 = new TGCheckButton(fCompositeFrame1595,"PbPb-40-50%");
fTextButton2311->Connect("Clicked()", "TVSDReader", this, "Combine25()");
   fTextButton2311->SetTextJustify(36);
   fTextButton2311->SetMargins(0,0,0,0);
   fTextButton2311->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2311, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2318 = new TGCheckButton(fCompositeFrame1595,"PbPb-50-60%");
   fTextButton2318->Connect("Clicked()", "TVSDReader", this, "Combine26()");
   fTextButton2318->SetTextJustify(36);
   fTextButton2318->SetMargins(0,0,0,0);
   fTextButton2318->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2318, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2325 = new TGCheckButton(fCompositeFrame1595,"PbPb-60-70%");
  fTextButton2325->Connect("Clicked()", "TVSDReader", this, "Combine27()");
   fTextButton2325->SetTextJustify(36);
   fTextButton2325->SetMargins(0,0,0,0);
   fTextButton2325->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2325, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2349 = new TGCheckButton(fCompositeFrame1595,"PbPb-70-80%");
   fTextButton2349->Connect("Clicked()", "TVSDReader", this, "Combine28()");
   fTextButton2349->SetTextJustify(36);
   fTextButton2349->SetMargins(0,0,0,0);
   fTextButton2349->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2349, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2356 = new TGCheckButton(fCompositeFrame1595,"PbPb-80-90%");
 fTextButton2356->Connect("Clicked()", "TVSDReader", this, "Combine29()");
   fTextButton2356->SetTextJustify(36);
   fTextButton2356->SetMargins(0,0,0,0);
   fTextButton2356->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2356, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton2363 = new TGCheckButton(fCompositeFrame1595,"PbPb-90-100%");
fTextButton2363->Connect("Clicked()", "TVSDReader", this, "Combine30()");
   fTextButton2363->SetTextJustify(36);
   fTextButton2363->SetMargins(0,0,0,0);
   fTextButton2363->SetWrapLength(-1);
   fCompositeFrame1595->AddFrame(fTextButton2363, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

//char* file1="PbPb-K0.txt";
   // container of "Tab2"
   TGCompositeFrame *fCompositeFrameAntiLambda;
   fCompositeFrameAntiLambda = fTab1590->AddTab("AntiLambda");
   fCompositeFrameAntiLambda->SetLayoutManager(new TGVerticalLayout(fCompositeFrameAntiLambda));
   TGCheckButton *fTextButtonALpp = new TGCheckButton(fCompositeFrameAntiLambda,"pp");
   fTextButtonALpp->Connect("Clicked()", "TVSDReader", this, "CombineAntiLambdapp()");
   fTextButtonALpp->SetTextJustify(36);
   fTextButtonALpp->SetMargins(0,0,0,0);
   fTextButtonALpp->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALpp, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb");
   fTextButtonALPbPb->Connect("Clicked()", "TVSDReader", this, "CombineAntiLambdaPpPp()");
   fTextButtonALPbPb->SetTextJustify(36);
   fTextButtonALPbPb->SetMargins(0,0,0,0);
   fTextButtonALPbPb->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb0_10 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-0-10%");
   fTextButtonALPbPb0_10->Connect("Clicked()", "TVSDReader", this, "Combine31()");
   fTextButtonALPbPb0_10->SetTextJustify(36);
   fTextButtonALPbPb0_10->SetMargins(0,0,0,0);
   fTextButtonALPbPb0_10->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb0_10, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb10_20 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-10-20%");
   fTextButtonALPbPb10_20->Connect("Clicked()", "TVSDReader", this, "Combine32()");
   fTextButtonALPbPb10_20->SetTextJustify(36);
   fTextButtonALPbPb10_20->SetMargins(0,0,0,0);
   fTextButtonALPbPb10_20->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb10_20, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb20_30 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-20-30%");
   fTextButtonALPbPb20_30->Connect("Clicked()", "TVSDReader", this, "Combine33()");
   fTextButtonALPbPb20_30->SetTextJustify(36);
   fTextButtonALPbPb20_30->SetMargins(0,0,0,0);
   fTextButtonALPbPb20_30->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb20_30, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb30_40 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-30-40%");
   fTextButtonALPbPb30_40->Connect("Clicked()", "TVSDReader", this, "Combine34()");
   fTextButtonALPbPb30_40->SetTextJustify(36);
   fTextButtonALPbPb30_40->SetMargins(0,0,0,0);
   fTextButtonALPbPb30_40->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb30_40, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb40_50 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-40-50%");
   fTextButtonALPbPb40_50->Connect("Clicked()", "TVSDReader", this, "Combine35()");
   fTextButtonALPbPb40_50->SetTextJustify(36);
   fTextButtonALPbPb40_50->SetMargins(0,0,0,0);
   fTextButtonALPbPb40_50->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb40_50, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb50_60 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-50-60%");
   fTextButtonALPbPb50_60->Connect("Clicked()", "TVSDReader", this, "Combine36()");
   fTextButtonALPbPb50_60->SetTextJustify(36);
   fTextButtonALPbPb50_60->SetMargins(0,0,0,0);
   fTextButtonALPbPb50_60->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb50_60, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb60_70 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-60-70%");
   fTextButtonALPbPb60_70->Connect("Clicked()", "TVSDReader", this, "Combine37()");
   fTextButtonALPbPb60_70->SetTextJustify(36);
   fTextButtonALPbPb60_70->SetMargins(0,0,0,0);
   fTextButtonALPbPb60_70->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb60_70, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb70_80 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-70-80%");
   fTextButtonALPbPb70_80 ->Connect("Clicked()", "TVSDReader", this, "Combine38()");
   fTextButtonALPbPb70_80 ->SetTextJustify(36);
   fTextButtonALPbPb70_80 ->SetMargins(0,0,0,0);
   fTextButtonALPbPb70_80 ->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb70_80 , new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb80_90 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-80-90%");
   fTextButtonALPbPb80_90->Connect("Clicked()", "TVSDReader", this, "Combine39()");
   fTextButtonALPbPb80_90->SetTextJustify(36);
   fTextButtonALPbPb80_90->SetMargins(0,0,0,0);
   fTextButtonALPbPb80_90->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb80_90, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButtonALPbPb90_100 = new TGCheckButton(fCompositeFrameAntiLambda,"PbPb-90-100%");
   fTextButtonALPbPb90_100->Connect("Clicked()", "TVSDReader", this, "Combine40()");
   fTextButtonALPbPb90_100->SetTextJustify(36);
   fTextButtonALPbPb90_100->SetMargins(0,0,0,0);
   fTextButtonALPbPb90_100->SetWrapLength(-1);
   fCompositeFrameAntiLambda->AddFrame(fTextButtonALPbPb90_100, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   fTab1590->SetTab(1);

   fTab1590->Resize(fTab1590->GetDefaultSize());
   frmMain->AddFrame(fTab1590, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTab1590->MoveResize(8,8,128,280);

   frmMain->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   frmMain->MapSubwindows();

   frmMain->Resize(frmMain->GetDefaultSize());
   frmMain->MapWindow();
   //frmMain->Resize(490,372);

	
    gf = new TGGroupFrame(frmMain, "Fit");

    vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
    {

      hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);

      vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));

      hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
      {

        fFitRangePoly = new TEveGDoubleValuator(hf,"Bkg range:", 40, 0);
        fFitRangePoly->SetNELength(5);
        fFitRangePoly->SetLabelWidth(70);
        fFitRangePoly->Build();
        fFitRangePoly->GetSlider()->SetWidth(190);
        fFitRangePoly->SetLimits(0.0, 2.0, TGNumberFormat::kNESRealThree);
        fFitRangePoly->SetValues(0.0, 2.0, TGNumberFormat::kNESRealThree);

        hf->AddFrame(fFitRangePoly, new TGLayoutHints(kLHintsExpandX));

      }

      vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));

      hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
      {

        fFitRangeGaus = new TEveGDoubleValuator(hf,"Sig range:", 40, 0);
        fFitRangeGaus->SetNELength(5);
        fFitRangeGaus->SetLabelWidth(70);
        fFitRangeGaus->Build();
        fFitRangeGaus->GetSlider()->SetWidth(190);
        fFitRangeGaus->SetLimits(0.0, 2.0, TGNumberFormat::kNESRealThree);
        fFitRangeGaus->SetValues(0.0, 2.0, TGNumberFormat::kNESRealThree);

        hf->AddFrame(fFitRangeGaus, new TGLayoutHints(kLHintsExpandX));

      }

      vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));

        hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
        {
            b = new TGTextButton(hf, "Fit signal+background");
            b->Connect("Clicked()", "TVSDReader", this, "FitInvariantMass()");
            hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
        }
        
        vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));
        
    }

    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    frmMain->AddFrame(gf);

    frmMain->MapSubwindows();
    frmMain->Resize();
    frmMain->MapWindow();

    browser->StopEmbedding(" ");

    browser->StartEmbedding(TRootBrowser::kBottom);

    frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600, kHorizontalFrame);
    frmMain->SetWindowName("XX GUI");
    frmMain->SetCleanup(kDeepCleanup);

    const TGFont *font = gClient->GetFont("-*-helvetica-bold-r-normal-*-20-*-*-*-*-*-*-*");
    FontStruct_t buttonFont = font->GetFontStruct();

    gClient->GetColorByName("red", buttonRedColor);
    b = new TGTextButton(frmMain,"Exit");
    b->SetTextColor(buttonRedColor);
    b->SetFont(buttonFont);
    
    b->Connect("Clicked()", "TApplication", gApplication, "Terminate()");

    frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));

    frmMain->MapSubwindows();
    frmMain->Resize();
    frmMain->MapWindow();

    browser->StopEmbedding(" ");

    browser->StartEmbedding(TRootBrowser::kRight);

    fPad = new TCanvas();
    fPad->Divide(2, 2);
    fPad->cd(1);

    fMinvHisto = new TH1D("Invariant Mass","Invariant Mass",400,0.0,2.0);

    if(AC<=4) {
    	AC=AC+1; 
	cout << AC << endl;}

	
    else {
    	AC=1; 
	cout << AC << endl;}

    fPad->cd(AC);
    fMinvHisto->SetLineColor(2);
    fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    fMinvHisto->GetYaxis()->SetTitle("Counts");
    fMinvHisto->SetFillColor(0);

    browser->StopEmbedding("Invariant Mass Distibution");

    browser->GetTabRight()->SetTab(2);

  }
 void StudentSet(){

	AutomaticMenu();

  }

  ClassDef(TVSDReader, 0);
};

TVSDReader* gVSDReader = 0;


//______________________________________________________________________________
void alice_vsd(Int_t dataset)
{

  const char* vsd_file_name = NULL;


    switch(dataset)
    {

    case 1:
      vsd_file_name = "eve_files/data/AliVSD_MasterClass_5.root";
      break;

   }


  TEveManager::Create(kTRUE,"FV");

  gVSDReader = new TVSDReader(vsd_file_name);

  gVSDReader->fTimer = new TTimer();
 
  TEveBrowser* browser = gEve->GetBrowser();

  browser->GetTabLeft()->RemoveTab(1);
  browser->GetTabLeft()->RemoveTab(0);
  gStyle->SetOptStat("e");
//  gStyle->SetOptFit(1111);
  gStyle->SetOptFit(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPalette(1);

  gKMinvHist = new TH1D("Statistics","Kaons",50,0.0,2.0);
  gKMinvHist->SetLineColor(2);
  gKMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gKMinvHist->GetYaxis()->SetTitle("Counts");
  gKMinvHist->SetFillColor(0);
  
  gXMinvHist = new TH1D("Statistics","Xis",50,0.0,2.0);
  gXMinvHist->SetLineColor(3);
  gXMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gXMinvHist->GetYaxis()->SetTitle("Counts");
  gXMinvHist->SetFillColor(0);
 
  gLMinvHist = new TH1D("Statistics","Lambdas",50,0.0,2.0);
  gLMinvHist->SetLineColor(4);
  gLMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gLMinvHist->GetYaxis()->SetTitle("Counts");
  gLMinvHist->SetFillColor(0);

  gALMinvHist = new TH1D("Statistics","Anti-Lambdas",50,0.0,2.0);
  gALMinvHist->SetLineColor(9);
  gALMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gALMinvHist->GetYaxis()->SetTitle("Counts");
  gALMinvHist->SetFillColor(0);

  gAllMinvHist = new TH1D("Statistics","Invariant Mass - final result",20,0.0,2.0);
  gAllMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gAllMinvHist->GetYaxis()->SetTitle("Counts");
  gAllMinvHist->SetFillColor(0);

  gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event"));

  gVSDReader->ToStudent();
   
}
