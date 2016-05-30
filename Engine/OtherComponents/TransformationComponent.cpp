#include "TransformationComponent.h"
#include "../Manager/LuaManager.h"
#include "../Script/LuaUtilities.h"
#include "../Event\EventRegistry.h"

ID_IMPLEMENTATION(TransformationComponent);

TransformationComponent::TransformationComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_oOrigin(0.0f,0.0f,0.0f)	
	, m_oOrientation(Quaternion::IDENTITY)
	, m_oScale(Vec3::UNIT_SCALE)
{
	CreateLuaObject();
}
	
TransformationComponent::~TransformationComponent()
{

}

void TransformationComponent::Init()
{
	char szEventName[128];
	sprintf_s(szEventName,"%s/POSITION_CHANGED",GetOwnerID().GetDebugName().c_str());
	m_oEvent.SetPath(ObjectId(szEventName));	
}

const Vec3& TransformationComponent::GetPosition() const
{
	return m_oOrigin;
}

Vec3 TransformationComponent::GetDirection()	const
{
	return m_oOrientation.zAxis();
}

void TransformationComponent::SetDirection( Vec3& i_oDirection )
{
	if (i_oDirection == Vec3::ZERO)		
		return;

	i_oDirection.normalise();

	Vec3 oXAxe;
	Vec3 oYAxe;
	Vec3 oZAxe;

	m_oOrientation.ToAxes(oXAxe, oYAxe, oZAxe);

	Quaternion rotQuat;
	if ( (oZAxe + i_oDirection).squaredLength() <  0.0f) 
	{
		// Oops, a 180 degree turn (infinite possible rotation axes)
		// Default to yaw i.e. use current UP
		rotQuat.FromAngleAxis(Ogre::Radian(Ogre::Math::PI), oYAxe);
	}
	else
	{
		// Derive shortest arc to new direction
		rotQuat = oZAxe.getRotationTo(i_oDirection);
	}

	m_oOrientation = rotQuat * m_oOrientation;
}

Vec3	TransformationComponent::GetUp() const
{
	return m_oOrientation.yAxis();
}

Vec3	TransformationComponent::GetSide() const
{
	return m_oOrientation.xAxis();
}

Vec3	TransformationComponent::GetX()	const
{
	return m_oOrientation.xAxis();
}

Vec3	TransformationComponent::GetY() const
{
	return m_oOrientation.yAxis();
}

Vec3	TransformationComponent::GetZ() const
{
	return m_oOrientation.zAxis();
}

void TransformationComponent::SetPosition(const Vec3& i_oPosition)
{
	m_oOrigin = i_oPosition;
	m_oEvent.Raise();
}

void TransformationComponent::Translate(const Vec3& i_oTranslation)
{
	m_oOrigin += i_oTranslation;
	m_oEvent.Raise();
}

const Quaternion& TransformationComponent::GetRotation() const
{
	return m_oOrientation;
}

Matrix33 TransformationComponent::GetRotationMatrix() const
{
	Matrix33 oRotationMatrix;
	m_oOrientation.ToRotationMatrix(oRotationMatrix);

	return oRotationMatrix;
}

void TransformationComponent::SetRotation(const Quaternion& i_oRotation)
{
	m_oOrientation = i_oRotation;
}

void TransformationComponent::SetRotation(const Matrix33& i_oRotation)
{
	m_oOrientation.FromRotationMatrix(i_oRotation);
	m_oEvent.Raise();
}

bool TransformationComponent::HasScale() const
{
	return (m_oScale.x != 1.0 || m_oScale.y != 1.0 || m_oScale.z != 1.0);
}

void TransformationComponent::SetScale(const Vec3& i_oScale)
{
	m_oScale = i_oScale;
}	

const Vec3& TransformationComponent::GetScale() const
{
	return m_oScale;	
}

void TransformationComponent::SetPositionFromPhysics( const Vec3& i_oPosition )
{
	m_oOrigin = i_oPosition;
}

void TransformationComponent::SetRotationFromPhysics( const Quaternion& i_Rotation )
{
	m_oOrientation = i_Rotation;
}

void TransformationComponent::SetRotationFromPhysics( const Matrix33& i_oRotation )
{
	m_oOrientation.FromRotationMatrix(i_oRotation);
}

void TransformationComponent::RotateAround( const Vec3& i_Axis, const real i_Angle )
{
	Quaternion oRotation(Ogre::Radian(Ogre::Degree(i_Angle).valueRadians()),i_Axis);
	SetRotation(oRotation * m_oOrientation);
}

bool TransformationComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		real degree = 0.0f;
		real x = 0.0f;
		real y = 0.0f;
		real z = 0.0f;

		const tinyxml2::XMLElement* pPosition = pNode->FirstChildElement("Position");
		if (pPosition)
		{
			x = pPosition->FloatAttribute("x");
			y = pPosition->FloatAttribute("y");
			z = pPosition->FloatAttribute("z");
			SetPosition(Vec3(x,y,z));
		}

		const tinyxml2::XMLElement* pRotation = pNode->FirstChildElement("Rotation");
		if (pRotation)
		{
			degree = pRotation->FloatAttribute("degree");
			x = pRotation->FloatAttribute("x");
			y = pRotation->FloatAttribute("y");
			z = pRotation->FloatAttribute("z");
			RotateAround(Vec3(x,y,z), degree);
		}

		const tinyxml2::XMLElement* pScale = pNode->FirstChildElement("Scale");
		if (pScale)
		{
			x = pScale->FloatAttribute("x");
			y = pScale->FloatAttribute("y");
			z = pScale->FloatAttribute("z");
			SetScale(Vec3(x,y,z));
		}

		const tinyxml2::XMLElement* pLookAt = pNode->FirstChildElement("LookAt");
		if (pLookAt)
		{
			x = pLookAt->FloatAttribute("x");
			y = pLookAt->FloatAttribute("y");
			z = pLookAt->FloatAttribute("z");
			LookAt(Vec3(x,y,z));
		}
	}

	return Component::SetupFromXml(pNode);
}

void TransformationComponent::LookAt( const Vec3& i_oPoint )
{
	SetDirection(i_oPoint - m_oOrigin);
}

/******************* LUA ********************************/
/*
	Create LuaObject when I instance a TrasfomationComponent
*/
void TransformationComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName("TransformationComponentMetatable");

	m_oLuaObject.SetLightUserData("__object", const_cast<TransformationComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}
	
void TransformationComponent::SetPositionFromScript(LuaPlus::LuaObject i_oVec3LuaObject)
{
	//TODO -> Studente MGD	
}

LuaPlus::LuaObject TransformationComponent::GetPositionFromScript()
{
	//TODO -> Studente MGD	
	return LuaPlus::LuaObject();
}

void TransformationComponent::TranslateFromScript(LuaPlus::LuaObject i_oVec3LuaObject)
{
	//TODO -> Studente MGD	
}

void TransformationComponent::LookAtFromScript(LuaPlus::LuaObject i_oVec3LuaObject)
{
	//TODO -> Studente MGD	
}

void TransformationComponent::RotateFromScript(LuaPlus::LuaObject i_oVec3LuaObject, real i_fDegree)
{
	//TODO -> Studente MGD	
}

real TransformationComponent::GetRotationX() const
{
	//TODO -> Studente MGD	

	//Pitch

	return 0.0f;
}

real TransformationComponent::GetRotationY() const
{
	//TODO -> Studente MGD	

	//Yaw

	return 0.0f;
}

real TransformationComponent::GetRotationZ() const
{
	//TODO -> Studente MGD	

	//Roll
	return 0.0f;
}

void TransformationComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable("TransformationComponentMetatable");
	metaTable.SetObject("__index", metaTable); 

	//TODO -> Studente MGD	
	
	//Esempio: metaTable.RegisterObjectDirect("NomeFunzioneInLua", (NomeClass*)0, Puntatore a funzione);

	// Esporre SetPosition
	// Esporre GetPosition
	// Esporre Translate
	// Esporre LookAt
	// Esporre Rotate
	// Esporre GetRotateX
	// Esporre GetRotateY
	// Esporre GetRotateZ
}



/*******************************************************/