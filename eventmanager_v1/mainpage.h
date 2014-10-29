// -*- c++ -*-

/*! @mainpage Analysis Package Documentation
 *
 * The documentation for basic modules for a simple analysis toolkit based on 
 * a input data format in @c [ROOT](http://root.cern.ch) and an internal
 * event data model employing [Fastjet](http://www.fastjet.fr) is available
 * in this help system. 
 * 
 * The toolkit features event analysis for (1) individual (single interaction) 
 * signal events and  (2) pile-up events with many interactions. All software
 * supporting the merging with client defined pile-up conditions is included. 
 *   
 *
 * @section data_format Supported data format
 *
 * Both signal and pile-up events are expected to be in a simple @c ROOT tuple
 * structure providing the following variables:
 * @code
 * #define _FLOAT_T_ float 
 * int Nentry;                  // total # entries
 * int Npartons;                // # partons
 * int Nparticles;              // # particles
 * int ID[Nentry];              // PDG Id
 * int Stat[Nentry];            // internal status word (-1,-2 for partons, 2 for particles)
 * _FLOAT_T_ Charge[Nentry];    // charge
 * _FLOAT_T_ Px[Nentry];        // momentum Px
 * _FLOAT_T_ Py[Nentry];        // momentum Py
 * _FLOAT_T_ Pz[Nentry];        // momentum Pz
 * _FLOAT_T_ P0[Nentry];        // energy E
 * _FLOAT_T_ Pm[Nentry];        // mass m
 * _FLOAT_T_ Pt[Nentry];        // transverse momentum
 * _FLOAT_T_ Rap[Nentry];       // rapidity y
 * _FLOAT_T_ Phi[Nentry];       // azimuth phi
 * _FLOAT_T_ Eta[Nentry];       // pseudo-rapidity eta
 * @endcode
 *
 * Presently the toolkit does not support dynamic allocation of branches 
 * (flexible branch names). Parton kinematics is stored in the first 
 * @c Npartons entries (array indices @c 0...@c Npartons-1), while stable 
 * particle kinematics is stored at indices @c Npartons...@c Nentry-1. Thus,
 * @c Nparticles=Nentry-Npartons.
 *   
 * @section event_data_model Event data model
 *
 * The event data model is based on @c fastjet::PseudoJet (Fastjet versions 
 * 3.x required). Each @c PseudoJet represents a particle or parton. A user 
 * info object @c ParticleInfo is attached which stores particle ID, charge, 
 * vertex association, and other useful characteristics not accommodated by
 * @c PseudoJet directly. 
 * 
 * The @c Event container typically holds all partons, particles, and vertices
 * characterizing a given event. Helper functions are provided which fill the
 * @c Event structure.
 *
 * @note @c Event does not control its data content. It needs to be reset by
 *       the client before a new event is loaded. Otherwise it will add final
 *       state partons and particles to the previous event each time the 
 *       corresponding data handler (@c DataHandler) method is invoked.   
 * 
 * @section merging_strategy Event merging    
 *
 * @todo There is no unpacking of partons available right now. Events are 
 *       only available in the stable particle representation.
 */
