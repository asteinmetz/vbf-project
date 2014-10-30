{
  // open input files

  TFile* f0 = new TFile("vbf_7_000_pt_cut20.root","READ");
  TFile* f1 = new TFile("vbf_14_000_pt_cut20.root","READ");

  // get histograms
  TH1D* hjetseta0 = (TH1D*)f0->FindObjectAny("VBF_EtaJet");
  TH1D* hjetseta1 = (TH1D*)f1->FindObjectAny("VBF_EtaJet");
  TH2D* ddiffeta0 = (TH2D*)f0->FindObjectAny("VBF_lead_eta_diff_eta");
  TH2D* ddiffeta1 = (TH2D*)f1->FindObjectAny("VBF_lead_eta_diff_eta");

  TH2D* dleadsub0 = (TH2D*)f0->FindObjectAny("VBF_lead_sub");
  TH2D* dleadsub1 = (TH2D*)f1->FindObjectAny("VBF_lead_sub");

  //  if ( hmass0 == 0 || hmass1 == 0 )
  //   { cout << "Problem allocating histograms" << endl; }

  // get number of entries
  TH1D*    hnjet   = (TH1D*)f0->FindObjectAny("VBF_Njet");
  Double_t nentry0 = hnjet->GetEntries();
  if ( nentry == 0. )
    {
      cout << "histogram " << hnjet->GetName() << " empty!" << endl;
    }
  // normalize to number of events
  Double_t norm0 = 1./nentry0;
  // example only!!!
  hnjet->Scale(norm0);




  TCanvas* a = new TCanvas("JetsEta","JetsEta");

  hjetseta0->SetTitle("");
  hjetseta0->SetLineColor(kBlue);
  hjetseta0->SetLineWidth(2);

  hjetseta1->SetLineColor(kRed);
  hjetseta1->SetLineWidth(2);
  
  gStyle->SetOptStat(0);
  hjetseta0->GetXaxis()->SetRangeUser(-5,5);

  hjetseta0->Draw();
  hjetseta1->Draw("same");

  hjetseta0->GetXaxis()->SetTitle("Rapidity #eta");
  hjetseta0->GetYaxis()->SetTitle("Number of Jets");
  hjetseta0->GetXaxis()->CenterTitle();
  hjetseta0->GetYaxis()->CenterTitle();
  hjetseta0->GetYaxis()->SetLabelSize(0);
  hjetseta0->GetXaxis()->SetLabelSize(0.04);

  hjetseta0->GetXaxis()->SetTitleSize(0.04);
  hjetseta0->GetYaxis()->SetTitleSize(0.04);


  TLatex* txt = new TLatex();
  txt->SetTextAlign(11);
  txt->SetTextSize(0.04);
  txt->DrawLatex(-1.4,3000,"#sqrt{s} = 7 TeV");
  //  txt->SetTextAlign(11);
  txt->DrawLatex(-1.4,5000,"#sqrt{s} = 14 TeV");

  TLine* line0 = new TLine(-2,3000,-1.5,3000);
  line0->SetLineColor(kBlue);
  line0->SetLineWidth(2);
  line0->Draw();

  TLine* line1 = new TLine(-2,5000,-1.5,5000);
  line1->SetLineColor(kRed);
  line1->SetLineWidth(2);
  line1->Draw();




  TCanvas* b = new TCanvas("diffeta","diffeta");
  TH1D* hdiffeta0 = ddiffeta0->ProjectionY("hdiffeta0");

  hdiffeta0->SetTitle(" ");
  hdiffeta0->SetLineColor(kBlue);
  hdiffeta0->SetLineWidth(2);

  TH1D* hdiffeta1 = ddiffeta1->ProjectionY("hdiffeta1");
  hdiffeta1->SetLineColor(kRed);
  hdiffeta1->SetLineWidth(2);

  hdiffeta0->Draw();
  hdiffeta1->Draw("same");
 
  hdiffeta0->GetXaxis()->SetTitle("Rapidity Difference in Leading Jets #Delta#eta");
  hdiffeta0->GetXaxis()->CenterTitle();
  hdiffeta0->GetXaxis()->SetLabelSize(0.04);
  hdiffeta0->GetXaxis()->SetTitleSize(0.04);

  hdiffeta0->GetYaxis()->SetTitle("Number of Jet Pairs");
  hdiffeta0->GetYaxis()->CenterTitle();
  hdiffeta0->GetYaxis()->SetLabelSize(0);
  hdiffeta0->GetYaxis()->SetTitleSize(0.04);


  TLatex* txt = new TLatex();
  txt->SetTextAlign(11);
  txt->SetTextSize(0.04);
  txt->DrawLatex(-1.4,1000,"#sqrt{s} = 7 TeV");
  //  txt->SetTextAlign(11);
  txt->DrawLatex(-1.4,2000,"#sqrt{s} = 14 TeV");

  TLine* line0 = new TLine(-2,1000,-1.5,1000);
  line0->SetLineColor(kBlue);
  line0->SetLineWidth(2);
  line0->Draw();

  TLine* line1 = new TLine(-2,2000,-1.5,2000);
  line1->SetLineColor(kRed);
  line1->SetLineWidth(2);
  line1->Draw();






  TCanvas* c = new TCanvas("leadsub","leadsub");
  TH1D* hleadsub0 = dleadsub0->ProjectionY("leadsub0",30,90,"o");

  hleadsub0->SetTitle(" ");
  hleadsub0->SetLineColor(kBlue);
  hleadsub0->SetLineWidth(2);

  TH1D* hleadsub1 = dleadsub1->ProjectionY("leadsub1",30,90,"o");
  hleadsub1->SetLineColor(kRed);
  hleadsub1->SetLineWidth(2);

  hleadsub0->Draw();
  hleadsub1->Draw("same");
 
  hleadsub0->GetXaxis()->SetTitle("Sub #eta when Lead #eta in [-3,3]");
  hleadsub0->GetXaxis()->CenterTitle();
  hleadsub0->GetXaxis()->SetLabelSize(0.04);
  hleadsub0->GetXaxis()->SetTitleSize(0.04);

  hleadsub0->GetYaxis()->SetTitle("Number of Sub Jets");
  hleadsub0->GetYaxis()->CenterTitle();
  hleadsub0->GetYaxis()->SetLabelSize(0);
  hleadsub0->GetYaxis()->SetTitleSize(0.04);


  TLatex* txt = new TLatex();
  txt->SetTextAlign(11);
  txt->SetTextSize(0.04);
  txt->DrawLatex(-0.9,500,"#sqrt{s} = 7 TeV");
  //  txt->SetTextAlign(11);
  txt->DrawLatex(-0.9,600,"#sqrt{s} = 14 TeV");

  TLine* line0 = new TLine(-1.5,500,-1,500);
  line0->SetLineColor(kBlue);
  line0->SetLineWidth(2);
  line0->Draw();

  TLine* line1 = new TLine(-1.5,600,-1,600);
  line1->SetLineColor(kRed);
  line1->SetLineWidth(2);
  line1->Draw();





 
  







  // TH1D* dmass0 = new TH1D(*hmass0);
  // dmass0->SetName("Diff");
  // dmass0->SetTitle("Difference");
  
  // dmass0->Add(hmass1,1);

  //  TCanvas* b = new TCanvas("Differences","Differences");

  // dmass0->SetLineColor(kBlack);
  // dmass0->SetLineWidth(2);
  // dmass0->Draw();
  // hmass0->Draw("same");
  // hmass1->Draw("same");

  // TBrowser k;

}
