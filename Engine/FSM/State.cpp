#include "State.h"
#include "../Manager/LuaManager.h"
#include "../LOG/LogManager.h"

//TODO@IC - please test

ID_IMPLEMENTATION(State);

State::State(const std::string& i_szName)
	: m_szName(i_szName)
	, m_oId(i_szName.c_str())
{
	//TODO -> Studente MGD
}


State::~State(void)
{
	//TODO -> Studente MGD
}

void State::AddTransition(Transition* i_pTransition)
{
	//TODO -> Studente MGD

	m_TransitionMap.insert(std::pair<TransitionID, Transition*> (i_pTransition->GetName(), i_pTransition));

}

void State::OnEnter()
{
	if(!m_szScriptOnEnter.empty())
	{
		LuaManager::GetSingleton().ExecuteFile(m_szScriptOnEnter.c_str());
	}
}

void State::OnUpdate()
{
	if(!m_szScriptOnUpdate.empty())
	{
		LuaManager::GetSingleton().ExecuteFile(m_szScriptOnUpdate.c_str());
	}
}

void State::OnExit()
{
	if(!m_szScriptOnExit.empty())
	{
		LuaManager::GetSingleton().ExecuteFile(m_szScriptOnExit.c_str());
	}
}

const StateID& State::DoTransition( const char* i_szTransition )
{
	//TODO -> Studente MGD
	StateID retStateID = (ObjectId) nullptr; // TODO@IC please verify

	m_viTransitionMap it = m_TransitionMap.find(ObjectId(i_szTransition));

	if (it != m_TransitionMap.end()) {
		retStateID = (it->second)->GetStateTargetID();
	}

	return ObjectId(retStateID);
}

void State::SetScriptOnEnter( const char* i_szFilename )
{
	if(i_szFilename && strlen(i_szFilename) > 0)
	{
		m_szScriptOnEnter.assign(i_szFilename);
	}
}

void State::SetScriptOnUpdate( const char* i_szFilename )
{
	if(i_szFilename && strlen(i_szFilename) > 0)
	{
		m_szScriptOnUpdate.assign(i_szFilename);
	}
}

void State::SetScriptOnExit( const char* i_szFilename )
{
	if(i_szFilename && strlen(i_szFilename) > 0)
	{
		m_szScriptOnExit.assign(i_szFilename);
	}
}
