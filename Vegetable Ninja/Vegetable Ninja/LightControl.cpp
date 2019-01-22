#include "LightControl.h"

LightControl * LightControl::m_pLightControl = nullptr;
ID3DX11Effect * LightControl ::m_pLightEffect = nullptr;

void LightControl::SetFX(ID3DX11Effect * fx)
{
	m_pLightEffect = fx;
}

LightControl * LightControl::GetLightContal()
{
	if (m_pLightEffect == nullptr)
	{
		return nullptr;
	}
	else
	{
		if (m_pLightControl == nullptr)
		{
			m_pLightControl = new LightControl;
		}
	}
	return m_pLightControl;
}

void LightControl::ReleaseLight(LightControl * pLights)
{
	delete pLights;
}

bool LightControl::OnLight(char * name, int lightType)
{
	int index = FindIndex(name, lightType);
	if (index != -1)
	{
		m_LightState[lightType][index] = ON;
		switch (lightType)
		{
		case DIRECTIONAL_LIGHT:
		{
			m_DirectionalLights[index].State = ON;
			break;
		}
		case POINT_LIGHT:
		{
			m_PointLights[index].State = ON;
			break;
		}
		case SPOT_LIGHT:
		{
			m_SpotLights[index].State = ON;
			break;
		}
		default:
			break;
		}
		return true;
	}
	return false;
}

bool LightControl::OffLight(char * name, int lightType)
{
	int index = FindIndex(name, lightType);
	if (index != -1)
	{
		m_LightState[lightType][index] = OFF;
		switch (lightType)
		{
		case DIRECTIONAL_LIGHT:
		{
			m_DirectionalLights[index].State = OFF;
			break;
		}
		case POINT_LIGHT:
		{
			m_PointLights[index].State = OFF;
			break;
		}
		case SPOT_LIGHT:
		{
			m_SpotLights[index].State = OFF;
			break;
		}
		default:
			break;
		}
		return true;
	}
	return false;
}

bool LightControl::DeleteLight(char * name, int lightType)
{
	int index = FindIndex(name, lightType);
	if (index != -1)
	{
		delete m_LightName[lightType][index];				//清空名称储存
		m_LightName[lightType][index] = nullptr;			//重置指针
		m_LightState[lightType][index] = UNUSED;		//重置状态
		switch (lightType)
		{
		case  DIRECTIONAL_LIGHT:
		{
			m_DirectionalLights[index].State = UNUSED;
			break;
		}
		case POINT_LIGHT:
		{
			m_PointLights[index].State = UNUSED;
			break;
		}
		case SPOT_LIGHT:
		{
			m_SpotLights[index].State = UNUSED;
			break;
		}
		default:
			break;
		}
	}
	return false;
}

void LightControl::SetValue()
{
	for (int i = 0; i < 16; i++)
	{
		m_FXDirLight->GetElement(i)->SetRawValue(&m_DirectionalLights[i], 0, sizeof(m_DirectionalLights[i]));
		m_FXPointLight->GetElement(i)->SetRawValue(&m_PointLights[i], 0, sizeof(m_PointLights[i]));
		m_FXSpotLight->GetElement(i)->SetRawValue(&m_SpotLights[i], 0, sizeof(m_SpotLights[i]));
	}
}

LightControl::LightControl()
{
	//初始化数组
	for (int (&uses)[16] : m_LightState)		
	{
		for (int&use : uses)
		{
			use = UNUSED;
		}
	}
	for (auto&namePlace : m_LightName)
	{
		for (auto&name : namePlace)
		{
			name = nullptr;
		}
	}
	m_FXDirLight = m_pLightEffect->GetVariableByName("gDirLight");
	m_FXPointLight = m_pLightEffect->GetVariableByName("gPointLight");
	m_FXSpotLight = m_pLightEffect->GetVariableByName("gSpotLight");
}

LightControl::~LightControl()
{
	for (auto &namePlace : m_LightName)
	{
		for (auto &name : namePlace)
		{
			if (name != nullptr)
			{
				delete name;
			}
		}
	}
}

int LightControl::FindIndex(char * name, int type)//寻找同名灯光
{
	for (int i = 0; i < 16; i++)
	{
		if (m_LightName[type][i] != nullptr&&strcmp(name, m_LightName[type][i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

template<>
bool LightControl::SetLight<DirectionalLight>(char * name, DirectionalLight light)//设置平行光
{
	int index = FindIndex(name, 0);
	if (index == -1)//没找到对应的灯的名字
	{
		for (int i = 0; i < 16; i++)//查找空位
		{
			if (m_LightState[0][i] == UNUSED)//翻查0行的空位,如果有空位就写入
			{
				unsigned nameLength = strlen(name) + 1;
				m_LightName[0][i] = new char[nameLength];
				ZeroMemory(m_LightName[0][i], nameLength);
				strcpy(m_LightName[0][i], name);
				m_DirectionalLights[i].ambient = light.ambient;
				m_DirectionalLights[i].diffuse = light.diffuse;
				m_DirectionalLights[i].specular = light.specular;
				m_DirectionalLights[i].Direction = light.Direction;
				m_DirectionalLights[i].State = ON;
				m_LightState[0][i] = ON;
				return true;
			}
		}
		return false;//灯光满了
	}
	else
	{
		m_DirectionalLights[index].ambient = light.ambient;
		m_DirectionalLights[index].diffuse = light.diffuse;
		m_DirectionalLights[index].specular = light.specular;
		m_DirectionalLights[index].Direction = light.Direction;
		m_DirectionalLights[index].State = ON;
		m_LightState[0][index] = ON;
		return true;
	}
}

template<>
bool LightControl::SetLight<PointLight>(char * name, PointLight light)//设置点光
{
	int index = FindIndex(name, 1);
	if (index == -1)//没找到对应的灯的名字
	{
		for (int i = 0; i < 16; i++)//查找空位
		{
			if (m_LightState[1][i] == UNUSED)//翻查1行的空位,如果有空位就写入
			{
				unsigned nameLength = strlen(name) + 1;
				m_LightName[1][i] = new char[nameLength];
				ZeroMemory(m_LightName[1][i], nameLength);
				strcpy(m_LightName[1][i], name);
				m_PointLights[i].ambient = light.ambient;
				m_PointLights[i].diffuse = light.diffuse;
				m_PointLights[i].specular = light.specular;
				m_PointLights[i].Position = light.Position;
				m_PointLights[i].Range = light.Range;
				m_PointLights[i].Att = light.Att;
				m_PointLights[i].State = ON;
				return true;
			}
		}
		return false;//灯光满了
	}
	else
	{
		m_PointLights[index].ambient = light.ambient;
		m_PointLights[index].diffuse = light.diffuse;
		m_PointLights[index].specular = light.specular;
		m_PointLights[index].Position = light.Position;
		m_PointLights[index].Range = light.Range;
		m_PointLights[index].Att = light.Att;
		m_PointLights[index].State = ON;
		return true;
	}
}

template<>
bool LightControl::SetLight<SpotLight>(char * name, SpotLight light)//设置聚光
{
	int index = FindIndex(name, 2);
	if (index == -1)//没找到对应的灯的名字
	{
		for (int i = 0; i < 16; i++)//查找空位
		{
			if (m_LightState[2][i] == UNUSED)//翻查1行的空位,如果有空位就写入
			{
				unsigned nameLength = strlen(name) + 1;
				m_LightName[2][i] = new char[nameLength];
				ZeroMemory(m_LightName[2][i], nameLength);
				strcpy(m_LightName[2][i], name);
				m_SpotLights[i].ambient = light.ambient;
				m_SpotLights[i].diffuse = light.diffuse;
				m_SpotLights[i].specular = light.specular;
				m_SpotLights[i].Position = light.Position;
				m_SpotLights[i].Direction = light.Direction;
				m_SpotLights[i].Range = light.Range;
				m_SpotLights[i].Att = light.Att;
				m_SpotLights[i].Spot = light.Spot;
				m_SpotLights[i].State = ON;
				return true;
			}
		}
		return false;//灯光满了
	}
	else
	{
		m_SpotLights[index].ambient = light.ambient;
		m_SpotLights[index].diffuse = light.diffuse;
		m_SpotLights[index].specular = light.specular;
		m_SpotLights[index].Position = light.Position;
		m_SpotLights[index].Direction = light.Direction;
		m_SpotLights[index].Range = light.Range;
		m_SpotLights[index].Att = light.Att;
		m_SpotLights[index].Spot = light.Spot;
		m_SpotLights[index].State = ON;
		return true;
	}
}