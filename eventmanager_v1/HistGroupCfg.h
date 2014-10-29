// -*- c++ -*-
#ifndef HISTGROUPCFG_H
#define HISTGROUPCFG_H
#include <string>
#include <cmath>

/*! @brief Histogram configuration data */
namespace HistGroupCfg
{
  /*! @name Numerical constants */
  /*@{*/
  static const double x_pi(2.*std::asin(1));/*!< @brief @f$\pi@f$ */
  static const double x_twoPi(2.*x_pi);     /*!< @brief @f$2 \pi@f$ */
  /*! @brief Tower @f$\Delta\phi@f$ */
  static const double x_towerDeltaPhi(0.1);
  /*! @brief Tower @f$\Delta y@f$ */ 
  static const double x_towerDeltaRap(0.1);
  /*! @brief Tower @f$\Delta\eta@f$ */
  static const double x_towerDeltaEta(x_towerDeltaRap);
  /*! @brief Tower @f$y_{\rm min}@f$ */
  static const double x_towerRapMin(-5.);
  /*! @brief Tower @f$y_{\rm max}@f$ */
  static const double x_towerRapMax(5.);
  /*! @brief Tower @f$\eta_{\rm min}@f$ */
  static const double x_towerEtaMin(x_towerRapMin);
  /*! @brief Tower @f$\eta_{\rm max}@f$ */
  static const double x_towerEtaMax(x_towerRapMax);
  /*@}*/

  /*! @name Spatial bin descriptors */
  /*@{*/
  static const int    i_etaBins(130); /*!< @brief @f$N_{\rm bin}(\eta)@f$ */
  static const double x_etaMin(-6.5); /*!< @brief @f$\eta_{\rm min}@f$    */
  static const double x_etaMax(6.5);  /*!< @brief @f$\eta_{\rm max}@f$    */
  /*! @brief x-axis label (@f$\eta@f$) */
  static const std::string s_etaTitleX("#eta");
  /*! @brief y-axis label (@f$\eta@f$) */
  static const std::string s_etaTitleY("dN/d#eta [1/0.1]");
  /*! @brief @f$N_{\rm bin}(y)@f$ */
  static const int    i_rapBins(i_etaBins); 
  /*! @brief @f$y_{\rm min}@f$ */
  static const double x_rapMin(x_etaMin);
  /*! @brief @f$y_{\rm max}@f$ */
  static const double x_rapMax(x_etaMax);
  /*! @brief x-axis label (@f$y@f$) */
  static const std::string s_rapTitleX("Rapidity y");
  /*! @brief y-axis label (@f$y@f$) */
  static const std::string s_rapTitleY("dN/dy [1/0.1]");
  static const int    i_phiBins(64);  /*!< @brief @f$N_{\rm bin}(\phi)@f$ */
  static const double x_phiMin(-3.2); /*!< @brief @f$\phi_{\rm min}@f$ */
  static const double x_phiMax(3.2);  /*!< @brief @f$\phi_{\rm max}@f$ */
  /*! @brief x-axis label (@f$\phi@f$) */
  static const std::string s_phiTitleX("#phi");
  /*! @brief y-axis label (@f$\phi@f$) */
  static const std::string s_phiTitleY("dN/d#phi [1/0.1]");
  /*@}*/

  /*! @name Kinematic bin descriptors for particles */
  /*@{*/
  /*! @brief @f$N_{\rm bin}(p_{\rm T}) @f$*/
  static const int    i_ptpBins(100);
  /*! @brief @f$p_{\rm T,min}@f$ */
  static const double x_ptpMin(0.);
  /*! @brief @f$p_{\rm T,max}@f$ */
  static const double x_ptpMax(100.);
  /*! @brief x-axis label (particle @f$p_{\rm T}@f$) */
  static const std::string s_ptpTitleX("Particle p_{T} [GeV]");
  /*! @brief y-axis label (particle @f$p_{\rm T}@f$) */
  static const std::string s_ptpTitleY("dN/dp_{T} [1/GeV]");
  /*@}*/

  /*! @name Kinematic bin descriptors for jets */
  /*@{*/
  /*! @brief @f$N_{\rm bin}(p_{\rm T}^{\rm jet})@f$ */ 
  static const int    i_ptjBins(500); 
  /*! @brief @f$p_{\rm T,min}^{\rm jet}@f$ */ 
  static const double x_ptjMin(0.);
  /*! @brief @f$p_{\rm T,max}^{\rm jet}@f$ */ 
  static const double x_ptjMax(1000.);
  /*! @brief x-axis label (jet @f$p_{\rm T}^{\rm jet}@f$) */
  static const std::string s_ptjTitleX("Jet p_{T}^{jet} [GeV]");
  /*! @brief y-axis label (jet @f$p_{\rm T}^{\rm jet}@f$) */
  static const std::string s_ptjTitleY("dN/dp_{T}^{jet} [1/(5 GeV)]");
  //
  static const int i_jmlBins(501);
  static const double x_jmlMin(0.5);
  static const double x_jmlMax(500.5);
  static const std::string s_jmlTitleX("N_{jets}");
  static const std::string s_jmlTitleY("dN/dN_{jets}");
  /*@}*/

  /*! @name Event and properties */
  /*@{*/
  static const int    i_muBins(301);  /*!< @brief @f$N_{\rm bin}(\mu)@f$ */
  static const double x_muMin(-0.5);  /*!< @brief @f$\mu_{min}@f$ */
  static const double x_muMax(300.5); /*!< @brief @f$\mu_{max}@f$ */
  /*! @brief x-axis label (@f$\mu@f$) */
  static std::string s_muTitleX("Number of interactions #mu");
  /*! @brief y-axis label (@f$\mu@f$) */
  static std::string s_muTitleY("dN/d#mu");
  /*! @brief @f$N_{\rm bin}(N_{\rm vtx})@f$ */
  static const int    i_vtxBins(301); 
  /*! @brief @f$N_{\rm vtx,min}@f$ */
  static const double x_vtxMin(-0.5);
  /*! @brief @f$N_{\rm vtx,max}@f$ */
  static const double x_vtxMax(300.5);
  /*! @brief x-axis label (@f$N_{\rm vtx}@f$) */
  static std::string  s_vtxTitleX("Number of vertices N_{vtx}");
  /*! @brief y-axis label (@f$N_{\rm vtx}@f$) */
  static std::string  s_vtxTitleY("dN/dN_{vtx}");
  /*! @brief @f$N_{\rm bin}(N_{\rm part}^{\rm evt})@f$*/
  static const int    i_nptBins(5000);
  /*! @brief @f$N_{\rm part,min}^{\rm evt}@f$*/
  static const double x_nptMin(0.);
  /*! @brief @f$N_{\rm part,max}^{\rm evt}@f$*/
  static const double x_nptMax(40000.);
  /*! @brief x-axis label (@f$N_{\rm part}^{\rm evt}@f$) */
  static const std::string   
  s_nptTitleX("Number of particles per event N_{part}^{evt}");
  /*! @brief y-axis label (@f$N_{\rm part}^{\rm evt}@f$) */
  static const std::string   s_nptTitleY("dN/dN_{part}^{evt} [1/8 partciles]");
  /*! @brief @f$N_{\rm bin}(N_{\rm part}^{\rm vtx})@f$*/
  static const int    i_vpnBins(250);
  /*! @brief @f$N_{\rm part,min}^{\rm vtx}@f$*/
  static const double x_vpnMin(0.);
  /*! @brief @f$N_{\rm part,max}^{\rm vtx}@f$*/
  static const double x_vpnMax(1000.);
  /*! @brief x-axis label (@f$N_{\rm part}^{\rm vtx}@f$) */
  static const std::string   
  s_vpnTitleX("Number of particles per vertex N_{part}^{vtx}");
  /*! @brief y-axis label (@f$N_{\rm part}^{\rm evt}@f$) */
  static const std::string   s_vpnTitleY("dN/dN_{part}^{vtx} [1/4]");
  /*! @brief @f$N_{\rm bin}(E_{\rm T}^{\rm miss})@f$ */
  static const int    i_metBins(100);
  /*! @brief @f$E_{\rm T,min}^{\rm miss}@f$ */
  static const double x_metMin(0.);
  /*! @brief @f$E_{\rm T,max}^{\rm miss}@f$ */
  static const double x_metMax(100.);
  /*! @brief x-axis label (@f$E_{\rm T}^{\rm miss}@f$) */
  static const std::string s_metTitleX("E_{T}^{miss} [GeV]");
  /*! @brief y-axis label (@f$E_{\rm T}^{\rm miss}@f$) */
  static const std::string s_metTitleY("dN/dE_{T}^{miss} [1/GeV]");
  /*! @brief @f$N_{\rm bin}(\sigma E_{\rm T}^)@f$ */
  static const int    i_setBins(1000);
  /*! @brief @f$\sigma E_{\rm T,min}@f$ */
  static const double x_setMin(0.);
  /*! @brief @f$\sigma E_{\rm T,max}@f$ */
  static const double x_setMax(10000.);
  /*! @brief x-axis label (@f$\sigma E_{\rm T}@f$) */
  static const std::string s_setTitleX("#Sigma E_{T} [GeV]");
  /*! @brief y-axis label (@f$\sigma E_{\rm T}@f$) */
  static const std::string s_setTitleY("dN/d#Sigma E_{T} [1/(10 GeV)]");
  /*! @brief @f$N_{\rm bin}(f_{\rm vtx}}@f$, with 
    @f$f_{\rm vtx} = \frac{N_{\rm vtx}}{\mu}@f$ */
  static const int    i_fvmBins(101);
  /*! @brief @f$f_{\rm vtx,min}@f$ */
  static const double x_fvmMin(-0.5);
  /*! @brief @f$f_{\rm vtx,max}@f$ */
  static const double x_fvmMax(100.5);
  /*! @brief x-axis label (@f$f_{\rm vtx}@f$) */
  static const std::string s_fvmTitleX("f_{vtx} = N_{vtx}/#mu");
  /*! @brief y-axis label (@f$f_{\rm vtx}@f$) */
  static const std::string s_fvmTitleY("dN/df_{vtx}");
  /*@}*/

  /*! @name Jet properties */
  /*!@{*/
  /*! @brief @f$N_{\rm bin}({\rm JVF})@f$ */
  static const int    i_jvfBins(101);
  /*! @brief @f${\rm JVF}_{\rm min}@f$ */
  static const double x_jvfMin(-0.005);
  /*! @brief @f${\rm JVF}_{\rm max}@f$ */
  static const double x_jvfMax(1.005);
  static const std::string s_jvfTitleX("JVF");
  static const std::string s_jvfTitleY("dN/dJVF"); 
  /*! @brief @f$N_{\rm bin}(\Psi)@f$, with 
    @f$\Psi = E_{\rm jet}(R<R_{\rm core})/E_{\rm jet}@f$ */
  static const int    i_psiBins(101);
  /*! @brief @f$\Psi_{\rm min}@f$ */
  static const double x_psiMin(-0.005);
  /*! @brief @f$\Psi_{\rm max}@f$ */
  static const double x_psiMax(1.005);
  static const std::string s_psiTitleX("#Psi");
  static const std::string s_psiTitleY("dN/d#Psi");
  /*!@}*/

  /*! @name Miscellaneous bin descriptors */
  /*@{*/
  /*! @brief @f$N_{\rm bins}(R)@f$, with @f$R@f$ being any ratio 
    @f$R\in [0,1]@f$ */
  static const int    i_frcBins(101);
  /*! @brief @f$R_{min}@f$ */
  static const double x_frcMin(-0.5);
  /*! @brief @f$R_{max}@f$ */
  static const double x_frcMax(1.05);
  /*! @brief x-axis label (@f$R@f$) */
  static const std::string s_frcTitleX("Ratio R");
  /*! @brief y-axis label (@f$R@f$) */
  static const std::string s_frcTitleY("dN/dR");
  /*@}*/
//   static const int    i_numBins(101);
//   static const double x_numMin(-0.5);
//   static const double x_numMax(100.5);

//   static const int    i_frcBins(210);
//   static const double x_frcMin(-1.05);
//   static const double x_frcMax(1.05);

//   static const int    i_nmuBins(500);
//   static const double x_nmuMin(0.);
//   static const double x_nmuMax(500.);

//   static const int    i_nptBins(1000);
//   static const double x_nptMin(0.);
//   static const double x_nptMax(10000.);

//   static const std::string 
//     s_rapRhoTitleY("d^{2}N/(dyd#phi) [1/(0.1 #times 0.1)]");
//   static const std::string 
//     s_etaRhoTitleY("d^{2}N/(d#etad#phi) [1/(0.1 #times 0.1)]");

//   static const std::string s_rhoTitleX("#rho [GeV/(0.1 #times 0.1)]");
//   static const std::string s_rhoTitleY("dN/d#rho [1/(GeV/(0.1 #times 0.1))]");
  
//   static const std::string s_numTitleX("Number of jets N_{jet}");
//   static const std::string s_numTitleY("dN/dN_{jets}");
  
//   static const std::string s_frcTitleX("Fraction of jets f_{jet}");
//   static const std::string s_frcTitleY("dN/df_{jet} [1/0.01]");

//   static const std::string s_jvfTitleX("JVF");
//   static const std::string s_jvfTitleY("dN/dJVF");

//   static const std::string s_muTitleX("#mu");
//   static const std::string s_muTitleY("dN/d#mu");

//   static const std::string s_nptTitleX("N_{part}");
//   static const std::string s_nptTitleY("dN/dN_{part}");

//   static const std::string s_vtxTitleX("Number vertices/jet N_{vtx}^{jet}");
//   static const std::string s_vtxTitleY("dN/dN_{vtx}^{jet}");

//   static const std::string s_shpTitleX("f_{core}^{jet} = E(R<R_{core})/E");
//   static const std::string s_shpTitleY("dN/df_{core} [1./0.01]");

//   static const std::string s_nmuTitleX("#rho_{vtx} = N_{part}/#mu");
//   static const std::string s_nmuTitleY("dN/d#rho_{vtx}");
  
//   static const double x_phiBin(x_twoPi);
//   static const double x_etaBin((x_etaMax-x_etaMin)/((double)i_etaBins));
//   static const double x_rapBin((x_rapMax-x_rapMin)/((double)i_rapBins));

//   static const double x_phiNormInv(x_twoPi/x_towerDeltaPhi);
//   static const double x_rapNormInv(x_rapBin/x_towerDeltaRap);
//   static const double x_etaNormInv(x_etaBin/x_towerDeltaEta);

//   static const double x_rapAreaNormInv(x_rapNormInv*x_phiNormInv);
//   static const double x_etaAreaNormInv(x_etaNormInv*x_phiNormInv);

//   static const double x_phiNorm(1./x_phiNormInv);
//   static const double x_rapNorm(1./x_rapNormInv);
//   static const double x_etaNorm(1./x_etaNormInv);

//   static const double x_rapAreaNorm(1./x_rapAreaNormInv);
//   static const double x_etaAreaNorm(1./x_etaAreaNormInv);

//   static const int    i_rhoBins(100);
//   static const double x_rhoMin(0.);
//   static const double x_rhoMax(x_ptMax*x_rapAreaNormInv);
}
#endif
