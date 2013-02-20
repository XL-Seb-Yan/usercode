#include "../interface/X3872FilterPAT.h"


#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Math/interface/Error.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
X3872FilterPAT::X3872FilterPAT(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

}


X3872FilterPAT::~X3872FilterPAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
X3872FilterPAT::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using std::vector;
  using namespace edm;
  using namespace reco;
  using namespace std;

  Handle< vector<pat::Muon> > thePATMuonHandle;
  iEvent.getByLabel("cleanPatMuons", thePATMuonHandle);
  
  Handle< vector<pat::GenericParticle> > thePATTrackHandle;
  iEvent.getByLabel("cleanPatTrackCands", thePATTrackHandle);

  //if(thePATTrackHandle->size() > 2000) return false;

  if (thePATMuonHandle->size()>=2) {
    for ( std::vector<pat::Muon>::const_iterator iMuonP = thePATMuonHandle->begin();
	  iMuonP != thePATMuonHandle->end(); ++iMuonP ) {
      //check for mu+ first
      if (iMuonP->charge() != 1) continue;
      TrackRef muTrackP = iMuonP->track();
      if ( muTrackP.isNull() ) {
	//cout << "continue due to no track ref" << endl;
	continue;
      }
      const reco::Muon* mup = dynamic_cast<const reco::Muon * >(iMuonP->originalObject());      

      //next check for mu-
      for ( std::vector<pat::Muon>::const_iterator iMuonM = thePATMuonHandle->begin();
	    iMuonM != thePATMuonHandle->end(); ++iMuonM ) {
	if (iMuonM->charge() != -1) continue;
	TrackRef muTrackM = iMuonM->track();
	if ( muTrackM.isNull() ) {
	  //cout << "continue from no track ref" << endl;
	  continue;
	}
	const reco::Muon* mum = dynamic_cast<const reco::Muon * >(iMuonM->originalObject());      
	if(!muon::overlap(*mup, *mum)) {
	  //cout << "Keeping event" << endl;
	  return true;
	}
      }// 2nd loop over muons
    }//1st loop over muons

  }//if two muons
  return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
X3872FilterPAT::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
X3872FilterPAT::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(X3872FilterPAT);
