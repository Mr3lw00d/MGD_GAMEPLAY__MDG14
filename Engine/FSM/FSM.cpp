#include "FSM.h"
#include "State.h"
#include "../LOG/LogManager.h"

#include <iostream> //TODOIC <<<<<<<<<<<<<<<<

FSM::FSM(const std::string& i_szName)
	: m_szName(i_szName)
	, m_oId(i_szName.c_str())
	, m_pCurrentState(NULL)
	, m_bIsStarted(FALSE)
{
	m_TransitionEvent.Subscribe(this, &FSM::DoTransition, ObjectId("DoTransition"));
}

FSM::~FSM()
{
	//TODO -> Studente MGD
}

void FSM::AddState( State* i_pState )
{
	//TODO -> Studente MGD
	m_States.insert(std::pair<StateID, State*>((i_pState)->GetName(), i_pState)); //Add key
}

void FSM::RegisterTransitions(const MGDMap<TransitionID, Transition*>& m_TransitionMap)
{
	//TODO ???
}

void FSM::DoTransition(const char* i_szTransition)
{
	//TODO -> Studente MGD

	StateID RetStateId = m_pCurrentState->DoTransition(i_szTransition); //try run the transition

	if (RetStateId != ObjectId(nullptr))  //if exists the target transition
	{
		MGDMap<StateID, State*>::iterator i = m_States.find(ObjectId (RetStateId)); //get the target State

		if (i != m_States.end()) //if the target is reachable 
		{ 
			m_pCurrentState->OnExit();
			m_pCurrentState = (i->second);
			m_pCurrentState->OnEnter();
		}

	}
	
}

void FSM::Update(real i_fFrametime, real i_fTimestep)
{
	//TODO -> Studente MGD
	//if (!m_bIsStarted)
	//	m_bIsStarted = true;
	m_pCurrentState->OnUpdate(); //Run the current state update

}

void FSM::Start()
{
	//TODO -> Studente MGD

	MGDMap<StateID, State*>::iterator i = m_States.find(ObjectId(m_oInitialState));
	if (i != m_States.end()) //if the target is reachable 
	{
		m_pCurrentState = (i->second);
		m_pCurrentState->OnEnter();
		m_bIsStarted = true;
	}
	else
	{
		m_bIsStarted = false;
	}

}
