void dNdEta()
{
    gROOT->ProcessLine(".L run.C");
    gROOT->ProcessLine(".L correct.C");
    gROOT->ProcessLine(".L drawPlots.C");
    gROOT->SetBatch(kTRUE);

    // Firstly, the analysis on the MC data is performed. This means, the
    // MC data is analysed in the same way as the data later but furthermore
    // a correction map is created to correct the data for detector effects
    // MC: perugia0
    gSystem->mkdir("perugia0");
    run(2, 1, "filesPerugia0.txt");
    gSystem->Exec("rm analysis_esd_raw.root");
    gSystem->Rename("analysis_mc.root", "perugia0/analysis_mc.root");
    gSystem->Rename("correction_map.root", "perugia0/correction_map.root");
    gSystem->Rename("event_stat.root", "perugia0/event_stat.root");
    gSystem->Rename("EventStat_temp.root", "perugia0/EventStat_temp.root");
    gSystem->Rename("histogramsCheck.root", "perugia0/histogramsCheck.root");
    // MC: perugia2011
    gSystem->mkdir("perugia2011");
    run(2, 1, "filesPerugia2011.txt");
    gSystem->Exec("rm analysis_esd_raw.root");
    gSystem->Rename("analysis_mc.root", "perugia2011/analysis_mc.root");
    gSystem->Rename("correction_map.root", "perugia2011/correction_map.root");
    gSystem->Rename("event_stat.root", "perugia2011/event_stat.root");
    gSystem->Rename("EventStat_temp.root", "perugia2011/EventStat_temp.root");
    gSystem->Rename("histogramsCheck.root", "perugia2011/histogramsCheck.root");
    // MC: phojet
    gSystem->mkdir("phojet");
    run(2, 1, "filesPhojet.txt");
    gSystem->Exec("rm analysis_esd_raw.root");
    gSystem->Rename("analysis_mc.root", "phojet/analysis_mc.root");
    gSystem->Rename("correction_map.root", "phojet/correction_map.root");
    gSystem->Rename("event_stat.root", "phojet/event_stat.root");
    gSystem->Rename("EventStat_temp.root", "phojet/EventStat_temp.root");
    gSystem->Rename("histogramsCheck.root", "phojet/histogramsCheck.root");
    // Now the data itself is analysed
    gSystem->mkdir("data");
    run(0, 2, "files.txt");
    gSystem->Rename("analysis_esd_raw.root", "data/analysis_esd_raw.root");
    gSystem->Rename("event_stat.root", "data/event_stat.root");
    gSystem->Rename("EventStat_temp.root", "data/EventStat_temp.root");
    gSystem->Rename("histogramsCheck.root", "data/histogramsCheck.root");
    // Afterwards, the data can be corrected using one of the MC correction maps
    // corretion with perugia0
    gSystem->CopyFile("perugia0/analysis_mc.root", "analysis_mc.root");
    gSystem->CopyFile("perugia0/correction_map.root", "correction_map.root");
    gSystem->CopyFile("data/analysis_esd_raw.root", "analysis_esd_raw.root");
    correct();
    gSystem->Rename("analysis_esd.root", "perugia0/analysis_esd.root");
    gSystem->Exec("rm analysis_mc.root");
    gSystem->Exec("rm correction_map.root");
    gSystem->Exec("rm analysis_esd_raw.root");
    // corretion with perugia2011
    gSystem->CopyFile("perugia2011/analysis_mc.root", "analysis_mc.root");
    gSystem->CopyFile("perugia2011/correction_map.root", "correction_map.root");
    gSystem->CopyFile("data/analysis_esd_raw.root", "analysis_esd_raw.root");
    correct();
    gSystem->Rename("analysis_esd.root", "perugia2011/analysis_esd.root");
    gSystem->Exec("rm analysis_mc.root");
    gSystem->Exec("rm correction_map.root");
    gSystem->Exec("rm analysis_esd_raw.root");
    // correction with phojet
    gSystem->CopyFile("phojet/analysis_mc.root", "analysis_mc.root");
    gSystem->CopyFile("phojet/correction_map.root", "correction_map.root");
    gSystem->CopyFile("data/analysis_esd_raw.root", "analysis_esd_raw.root");
    correct();
    gSystem->Rename("analysis_esd.root", "phojet/analysis_esd.root");
    gSystem->Exec("rm analysis_mc.root");
    gSystem->Exec("rm correction_map.root");
    gSystem->Exec("rm analysis_esd_raw.root");

    // The analysis is finished now. The following lines plot the comparison of
    // the measured data and the expected data from the MC
    // Close all opened files
    gROOT->GetListOfFiles()->Delete();
    // get files
    gROOT->SetBatch(kFALSE);
    TFile* file_perugia0_esd = TFile::Open("perugia0/analysis_esd.root");
    TFile* file_perugia0_mc = TFile::Open("perugia0/analysis_mc.root");
    TFile* file_perugia2011_esd = TFile::Open("perugia2011/analysis_esd.root");
    TFile* file_perugia2011_mc = TFile::Open("perugia2011/analysis_mc.root");
    TFile* file_phojet_esd = TFile::Open("phojet/analysis_esd.root");
    TFile* file_phojet_mc = TFile::Open("phojet/analysis_mc.root");
    // get raw histograms
    TH1* hist_ESD_perugia0 = (TH1*) file_perugia0_esd->Get("dndeta/dNdEta_corrected");
    TH1* hist_MC_perugia0 = (TH1*) file_perugia0_mc->Get("dndeta/dNdEta_corrected");
    TH1* hist_ESD_perugia2011 = (TH1*) file_perugia2011_esd->Get("dndeta/dNdEta_corrected");
    TH1* hist_MC_perugia2011 = (TH1*) file_perugia2011_mc->Get("dndeta/dNdEta_corrected");
    TH1* hist_ESD_phojet = (TH1*) file_phojet_esd->Get("dndeta/dNdEta_corrected");
    TH1* hist_MC_phojet = (TH1*) file_phojet_mc->Get("dndeta/dNdEta_corrected");
    Prepare1DPlot(hist_ESD_perugia0);
    Prepare1DPlot(hist_MC_perugia0);
    Prepare1DPlot(hist_ESD_perugia2011);
    Prepare1DPlot(hist_MC_perugia2011);
    Prepare1DPlot(hist_ESD_phojet);
    Prepare1DPlot(hist_MC_phojet);
    // Combine data to one histo with systematic errors, set their draw properties
    float sys_error = 0.041;
    TH1* hist_ESD = hist_ESD_perugia0->Clone("hist_ESD");
    TH1* hist_ESD_sys_error = hist_ESD->Clone("hist_ESD_sys_error");
    for (int i = 1; i <= hist_ESD->GetNbinsX(); i++)
    {
        hist_ESD->SetBinContent(i, hist_ESD_perugia0->GetBinContent(i)/3. + hist_ESD_perugia2011->GetBinContent(i)/3. + hist_ESD_phojet->GetBinContent(i)/3.);
        hist_ESD->SetBinError(i, hist_ESD_perugia0->GetBinError(i)/3. + hist_ESD_perugia2011->GetBinError(i)/3. + hist_ESD_phojet->GetBinError(i)/3.);
        hist_ESD_sys_error->SetBinContent(i, hist_ESD_perugia0->GetBinContent(i)/3. + hist_ESD_perugia2011->GetBinContent(i)/3. + hist_ESD_phojet->GetBinContent(i)/3.);
        hist_ESD_sys_error->SetBinError(i, sys_error * hist_ESD_sys_error->GetBinContent(i));
    }
    hist_ESD->SetLineColor(kBlack);
    hist_ESD->SetMarkerStyle(20);
    hist_ESD->SetMarkerColor(kBlack);
    hist_ESD_sys_error->SetMarkerStyle(0);
    hist_ESD_sys_error->SetFillColor(15);
    // MC histograms
    hist_MC_perugia0->SetLineColor(kCyan);
    hist_MC_perugia0->SetLineStyle(8);
    hist_MC_perugia2011->SetLineColor(kOrange);
    hist_MC_perugia2011->SetLineStyle(7);
    hist_MC_phojet->SetLineColor(kMagenta);
    hist_MC_phojet->SetLineStyle(5);
    // data/MC ratios
    TH1* ratio_perugia0 = hist_ESD->Clone("ratio_perugia0");
    ratio_perugia0->Divide(hist_MC_perugia0);
    ratio_perugia0->SetLineColor(kCyan);
    ratio_perugia0->SetLineStyle(8);
    TH1* ratio_perugia2011 = hist_ESD->Clone("ratio_perugia2011");
    ratio_perugia2011->Divide(hist_MC_perugia2011);
    ratio_perugia2011->SetLineColor(kOrange);
    ratio_perugia2011->SetLineStyle(7);
    TH1* ratio_phojet = hist_ESD->Clone("ratio_phojet");
    ratio_phojet->Divide(hist_MC_phojet);
    ratio_phojet->SetLineColor(kMagenta);
    ratio_phojet->SetLineStyle(5);
    // legend
    TLegend* legend = new TLegend(0.12, 0.05, 0.96, 0.15);
    legend->SetNColumns(2);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);
    legend->AddEntry(hist_ESD, "Data", "p");
    legend->AddEntry(hist_ESD_sys_error, "systematic error", "f");
    legend->AddEntry(hist_MC_perugia0, "Perugia0", "l");
    legend->AddEntry(hist_MC_perugia2011, "Perugia2011", "l");
    legend->AddEntry(hist_MC_phojet, "Phojet", "l");
    // creating canvas and pads
    TCanvas* canvas = new TCanvas("dNdEtaFinal", "dNdEtaFinal", 800, 600);
    canvas->Range(0, 0, 1, 1);
    TPad* pad1 = new TPad("dNdEta_1", "", 0, 0.3, 1, 1);
    pad1->Draw();
    pad1->SetGridx();
    pad1->SetGridy();
    pad1->SetTopMargin(0.05);
    pad1->SetRightMargin(0.02);
    pad1->SetBottomMargin(0.0001);
    pad1->SetLeftMargin(0.08);
    TPad* pad2 = new TPad("dNdEta_2", "", 0, 0, 1, 0.3);
    pad2->Draw();
    pad2->SetGridx();
    pad2->SetGridy();
    pad2->SetTopMargin(0.0001);
    pad2->SetRightMargin(0.02);
    pad2->SetBottomMargin(0.2);
    pad2->SetLeftMargin(0.08);
    // dummy histograms for ranges, axis etc
    dummy1 = new TH2F("dummy1", "; Pseudorapidity #eta; dN_{ch}/d#eta", 100, -0.8, 0.8, 100, 3, 6);
    dummy1->SetStats(0);
    dummy2 = new TH2F("dummy2", "; Pseudorapidity #eta; Data/MC", 100, -0.8, 0.8, 100, 0.9, 1.5);
    dummy2->SetStats(0);
    dummy2->GetYaxis()->SetTitleSize(0.08);
    dummy2->GetXaxis()->SetTitleSize(0.08);
    dummy2->GetYaxis()->SetTitleOffset(0.5);
    dummy2->GetYaxis()->SetLabelSize(0.08);
    dummy2->GetXaxis()->SetLabelSize(0.08);
    // finally draw them
    pad1->cd();
    dummy1->DrawCopy();
    legend->DrawClone("SAME");
    hist_ESD_sys_error->DrawCopy("SAME E2 ][");
    hist_ESD->DrawCopy("SAME");
    hist_MC_perugia0->DrawCopy("SAME HIST C");
    hist_MC_perugia2011->DrawCopy("SAME HIST C");
    hist_MC_phojet->DrawCopy("SAME HIST C");
    pad2->cd();
    pad2->SetTickx();
    dummy2->DrawCopy();
    ratio_perugia0->DrawCopy("SAME HIST C");
    ratio_perugia2011->DrawCopy("SAME HIST C");
    ratio_phojet->DrawCopy("SAME HIST C");
    // close and (re)open files, set plotting style and open TBrowser to inspect data
    file_perugia0_esd->Close();
    file_perugia0_mc->Close();
    file_perugia2011_esd->Close();
    file_perugia2011_mc->Close();
    file_phojet_esd->Close();
    file_phojet_mc->Close();
    TFile::Open("perugia0/analysis_esd.root");
    TFile::Open("perugia0/analysis_mc.root");
    TFile::Open("perugia2011/analysis_esd.root");
    TFile::Open("perugia2011/analysis_mc.root");
    TFile::Open("phojet/analysis_esd.root");
    TFile::Open("phojet/analysis_mc.root");
    TFile::Open("data/analysis_esd_raw.root");
    TFile::Open("perugia0/correction_map.root");
    TFile::Open("perugia2011/correction_map.root");
    TFile::Open("phojet/correction_map.root");
    gStyle->SetOptFit(111111);
    new TBrowser();
}