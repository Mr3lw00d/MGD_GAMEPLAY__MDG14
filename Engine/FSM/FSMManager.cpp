#include "FSMManager.h"
#include "FSM.h"
#include "../Manager/LuaManager.h"
#include "../LOG/LogManager.h"

#include <iostream> //TODOIC <<<<<<<<<<<<<<<<

void FSMManager::CreateInstance()
{
	if(GetSingletonPtr() == NULL)
		new FSMManager();
}

FSMManager::FSMManager()
{
	std::cerr << "----------------TRY TO START FSM -------------" << std::endl;

	RegisterLuaFunctions();
}

FSMManager::~FSMManager()
{
	//TODO -> Studente MGD
	//TODO - > Maybe i shoud delete all the Map Elements


}

void FSMManager::RegisterLuaFunctions()
{
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("StartFSM", &FSMManager::StartFSM);
}

void FSMManager::StartFSM(const char* i_szFSMName)
{
	//TODO -> Studente MGD

	FSMManager::GetSingleton().m_vFSMMap.find(ObjectId(i_szFSMName))->second->Start(); //Run the current FSM
}

void FSMManager::AddFSM( FSM* i_pFSM )
{
	//TODO -> Studente MGD
	FSMManager::GetSingleton().m_vFSMMap.insert(std::pair<FSMID, FSM*>((i_pFSM)->GetName(), i_pFSM)); //Add Key
}

void FSMManager::RemoveFSM( const ObjectId& i_oName )
{
	//TODO -> Studente MGD
	FSMManager::GetSingleton().m_vFSMMap.erase(i_oName); //Remove the target object
}

void FSMManager::Update(real i_fFrametime, real i_fTimestep)
{
	//TODO -> Studente MGD

	//for each map element call update

	

	for (MGDMap<FSMID, FSM*>::iterator i = FSMManager::GetSingleton().m_vFSMMap.begin(); i != FSMManager::GetSingleton().m_vFSMMap.end(); ++i) {
		(*(*i).second).Update(i_fFrametime, i_fTimestep);
	
	}

}