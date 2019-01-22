#include "Vegetable_Ninja.h"

DX11Game::Vegetable_Ninja::Vegetable_Ninja(HINSTANCE hInstance) :D3DApp(hInstance),
m_MainMenu(0, 2, 2), m_GameMenu(1, 6, 1), m_PauseMenu(0, 1, 2), m_GameEnd(1, 3, 1), m_Loading(0, 1, 0), m_IsFirstLoad(true),
m_CorrectAnswerTime(0.0f), m_LastTime(2.0f)
{
	m_GameStatus = MENU_MAIN;
	for (ID3D11Buffer *& buff : m_pBfruits)
	{
		buff = nullptr;
	}
	for (ID3D11ShaderResourceView *&texture : m_pTfruits)
	{
		texture = nullptr;
	}
	m_LightControl = nullptr;
	//camera
	m_Camera.SetPosition(0.0f, 0.0f, -2.0f);
	m_Camera.SetTarget(0.0f, 0.0f, 0.0f);
	m_Camera.SetUpNormal(0.0f, 1.0f, 0.0f);

	//material
	m_FruitMatrial.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_FruitMatrial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_FruitMatrial.specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);

	Random::Setup();

	m_GameControl.AddGravity(XMFLOAT3(0.0f, -0.3f, 0.0f));

	m_IsKnife = false;
}

DX11Game::Vegetable_Ninja::~Vegetable_Ninja()
{
	ReleaseCOM(m_p2DEffect);
	ReleaseCOM(m_p2DInputLayout);
	ReleaseCOM(m_p3DEffect);
	ReleaseCOM(m_p3DInputLayout);
	ReleaseCOM(m_pAlphaToCoverageBS);
	for (ID3D11Buffer *& buff : m_pBfruits)
	{
		ReleaseCOM(buff);
	}
	for (ID3D11ShaderResourceView *&texture : m_pTfruits)
	{
		ReleaseCOM(texture);
	}
	LightControl::ReleaseLight(m_LightControl);
	for (FruitPairs *&fruit : m_FruitPairs)
	{
		delete fruit;
	}
	for (wchar_t * wch : m_Questions)
	{
		delete wch;
	}
	for (wchar_t * wch : m_Answers)
	{
		delete wch;
	}
	for (Sprite2D *& xv : m_AfterKnifeShow)
	{
		delete xv;
	}
}

bool DX11Game::Vegetable_Ninja::Init()
{
	if (!D3DApp::Init())
		return false;

	//TODO :1.BuildGeometryBuffers()��㣻2.BuildFX()������ɫ����
	//3.BuildVertexLayout()�������㲼�֣�4.BuidTexture()������
	Build2DEffect("VertexShader2D.hlsl");
	Build2DVertexLayout();
	Build3DEffect("VertexShader3D.hlsl");
	Build3DVertexLayout();
	BuildBlend();
	//��ʼ��UI
	DX11UI::UI::Setup(m_D3dDevice, m_p2DEffect, m_p2DTechnique, m_pSwapChain, mhMainWnd, md3dImmediateContext, m_p2DEffectWorldViewProj, m_p2DInputLayout,m_pAlphaToCoverageBS);
	//���MainMenu
	LoadMainMenu();
	LoadGame();
	LoadPause();
	LoadGameEnd();
	LoadLight(); 
	LoadLoading();
	//��ʼ��Sprite3D
	Sprite3D::Setup3D(m_D3dDevice, md3dImmediateContext, m_p3DEffect, m_p3DInputLayout);
	//��ȡ��Ŀ
	LoadQuestionsAndAnswers();
	//��ȡˮ��ģ��
	LoadVegetables();
	return true;
}

void DX11Game::Vegetable_Ninja::OnResize()
{
	m_MainMenu.BeforeReSize();
	m_GameMenu.BeforeReSize();
	m_PauseMenu.BeforeReSize();
	m_GameEnd.BeforeReSize();
	for (FruitPairs *& fruit : m_FruitPairs)
	{
		fruit->Words.PrepareResize();
	}
	m_ExampleFruit.Words.PrepareResize();
	m_CorrectAnswer.PrepareResize();
	//
	D3DApp::OnResize();
	//
	m_MainMenu.AfterResize(mhMainWnd);
	m_GameMenu.AfterResize(mhMainWnd);
	m_PauseMenu.AfterResize(mhMainWnd);
	m_GameEnd.AfterResize(mhMainWnd);
	for (FruitPairs *& fruit : m_FruitPairs)
	{
		fruit->Words.SetDigxBuff();
	}
	m_ExampleFruit.Words.SetDigxBuff();
	m_CorrectAnswer.SetDigxBuff();
	RECT rect;
	GetClientRect(mhMainWnd, &rect);
	m_Window.x = (float)(rect.right - rect.left - 18);
	m_Window.y = (float)(rect.bottom - rect.top - 47);
	m_Camera.SetPerspectiveMatrix(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void DX11Game::Vegetable_Ninja::UpdateScene(float dt)
{
	if (m_GameStatus == MENU_LOADING)
	{
		if (m_Fut1&&m_Fut2&&m_Fut3&&m_Fut4)//�����̼߳������
		{
			if (m_IsFirstLoad)
			{
				CalculateVertices();
				m_IsFirstLoad = false;
			}
			m_LastTime = 2.0f;
			m_GameStatus = MENU_GAME;
			m_UI.clear();
			m_UI.push_back(&m_GameMenu);
			for (int i = 0; i < m_UI[0]->ImageBox_Size(); i++)
			{
				m_UI[0]->m_ImageBox[i].Visible();
			}
			m_ExampleFruit.Fruit.SetMatrial(m_FruitMatrial);
			int type	= Random::INT::Next(0, 8);
			m_ExampleFruit.Type = type;
			m_ExampleFruit.Fruit.SetReLocation(m_Translation[type].x - 0.7f, m_Translation[type].y - 0.4f, m_Translation[type].z);
			m_ExampleFruit.Fruit.SetReScale(m_ScaleRate[type], m_ScaleRate[type], m_ScaleRate[type]);
			m_ExampleFruit.Fruit.SetTexture(m_pTfruits[type]);
			m_ExampleFruit.Fruit.SetVertex(m_pBfruits[type], m_NumofVertex[type]);
			m_ExampleFruit.Fruit.ReSetSprite();
			m_ExampleFruit.QASub = Random::INT::Next(0, m_QuestionNum);
			m_CorrectSub = m_ExampleFruit.QASub * 4;
			m_ExampleFruit.Wchar = m_Questions[m_ExampleFruit.QASub];
			//ȷ���׳���ˮ������
			Random::INT::ArrayD(0, 8, m_FruitArray, 4);
			bool isN = true;
			for (int i = 0; i < 4; i++)
			{
				if (m_FruitArray[i] == m_ExampleFruit.Type)
				{
					isN = false;
					break;
				}
			}
			if (isN)
			{
				m_FruitArray[0] = m_ExampleFruit.Type;
			}
		}
	}
	else if (m_GameStatus == MENU_GAME)
	{		
		GameOn(dt);
		m_GameControl.UpData(dt);
		if (m_CorrectAnswerTime > 0)
		{
			m_CorrectAnswerTime -= dt;
		}
		int size = m_AfterKnifeShowTime.size();
		for (int i = 0; i < size; i++)
		{
			if (m_AfterKnifeShowTime[i] > 0)
			{
				m_AfterKnifeShowTime[i] -= dt;
			}
			else
			{
				delete m_AfterKnifeShow[i];
				m_AfterKnifeShow.erase(m_AfterKnifeShow.begin() + i);
				m_AfterKnifeShowTime.erase(m_AfterKnifeShowTime.begin() + i);
				i--;
				size--;
			}
		}
		if (m_IsKnife)//��ˮ��
		{
			POINT Cursor;
			GetCursorPos(&Cursor);
			ScreenToClient(mhMainWnd, &Cursor);
			XMMATRIX viewMatrix = m_Camera.GetPerspectiveMatrix();
			XMFLOAT4 ray;
			ray.x = (2.0f*Cursor.x / m_Window.x - 1.0f) / viewMatrix._11;
			ray.y = (1.0f - Cursor.y*2.0f / m_Window.y) / viewMatrix._22;
			ray.z = 1.0f;
			ray.w = 0.0f;
			XMFLOAT3 cameraPos = m_Camera.GetPosition();
			XMVECTOR origin = XMLoadFloat4(&XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f));
			XMVECTOR rayDirection = XMLoadFloat4(&ray);			
			rayDirection = XMVector4Normalize(rayDirection);
			int size = m_FruitPairs.size();
			for (int i = 0; i < size; i++)
			{  
				XMFLOAT3 pos = m_FruitPairs[i]->Fruit.GetTranslationRelative();
				XNA::AxisAlignedBox fruitBox = m_FruitBox[m_FruitPairs[i]->Type];
				fruitBox.Center.x += pos.x;
				fruitBox.Center.y += pos.y;
				fruitBox.Center.z += pos.z;
				float dist;
				if (XNA::IntersectRayAxisAlignedBox(origin,rayDirection,&fruitBox,&dist))
				{
					//����ˮ�����¹�����ӣ��ѹ��λ�ô��������
					m_AfterKnifeShow.push_back(new Sprite2D());
					m_AfterKnifeShow.back()->ImgMoveTo(Cursor.x / 631.0f - 1.0f, 1.0 - Cursor.y / 336.5f, 0.0001f);
					m_AfterKnifeShowTime.emplace_back(XSHOWINTERVAL);
					if (m_FruitPairs[i]->Type == m_ExampleFruit.Type && m_FruitPairs[i]->QASub == m_CorrectSub)
					{
						m_AfterKnifeShow.back()->LoadPicture("Texture/��.png");
						m_Goal += 10;
						m_GameMenu.m_TextBox[0].SetWords(std::to_wstring(m_Goal).c_str(), D2D1::ColorF::Orange);
					}
					else
					{
						m_CorrectNum = m_CorrectSub;
						m_AfterKnifeShow.back()->LoadPicture("Texture/��.png");
						m_Heart--;
						m_GameMenu.m_ImageBox[2 + m_Heart].Invisible();
						if (m_Heart < 1)
						{
							m_GameStatus = MENU_END;
							m_GameEnd.m_TextBox[0].SetWords(std::to_wstring(m_Goal).c_str(), D2D1::ColorF::Orange);
							m_UI.push_back(&m_GameEnd);
						}
						m_CorrectAnswerTime = XSHOWINTERVAL;
					}
					delete m_FruitPairs[i];
					m_GameControl.DelSprite3D(i);
					m_FruitPairs.erase(m_FruitPairs.begin() + i);
					i--;
					size--;
					m_AfterKnifeShow.back()->BuildGeometryBuffers(XMFLOAT2(-0.075f, 0.1f), XMFLOAT2(0.075f, -0.1f), 0.0001);
					//��������Ŀ
					int type = Random::INT::Next(0, 8);
					m_ExampleFruit.Type = type;
					int qNum = Random::INT::Next(0, m_QuestionNum);
					m_ExampleFruit.Wchar = m_Questions[qNum];
					m_ExampleFruit.QASub = qNum;
					m_CorrectSub = qNum * 4;
					m_ExampleFruit.Fruit.SetReLocation(m_Translation[type].x - 0.7f, m_Translation[type].y - 0.4f, m_Translation[type].z);
					m_ExampleFruit.Fruit.SetReScale(m_ScaleRate[type], m_ScaleRate[type], m_ScaleRate[type]);
					m_ExampleFruit.Fruit.SetTexture(m_pTfruits[type]);
					m_ExampleFruit.Fruit.SetVertex(m_pBfruits[type], m_NumofVertex[type]);
					m_ExampleFruit.Fruit.ReSetSprite();
					//ȷ���׳���ˮ������
					Random::INT::ArrayD(0, 8, m_FruitArray, 4);
					bool isN = true;
					for (int i = 0; i < 4; i++)
					{
						if (m_FruitArray[i] == m_ExampleFruit.Type)
						{
							isN = false;
							break;
						}
					}
					if (isN)
					{
						m_FruitArray[0] = m_ExampleFruit.Type;
					}
				}
			}
		}
	}
	for (DX11UI::UI * &ui : m_UI)
	{
		ui->Updata();
	}
}

void DX11Game::Vegetable_Ninja::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//TODO :ÿ�θ���һ֡ˢ�µĻ���
	XMMATRIX viewMatrix = m_Camera.GetViewMatrix();					//�۲����
	XMMATRIX perMatrix = m_Camera.GetPerspectiveMatrix();		//ͶӰ����
	for (DX11UI::UI * &ui : m_UI)
	{
		ui->Draw();
	}
	if (m_GameStatus == MENU_GAME)
	{
		//�Ȳ鿴��û��ˮ������
		for (Sprite2D *& xv : m_AfterKnifeShow)
		{
			xv->DrawScene();
		}
		if (m_CorrectAnswerTime > 0)
		{
			wchar_t * wAnswer = new wchar_t[6 + lstrlenW(m_Answers[m_CorrectNum])]{};
			lstrcpyW(wAnswer, L"��ȷ�𰸣�");
			lstrcatW(wAnswer, m_Answers[m_CorrectNum]);
			m_CorrectAnswer.Draw(wAnswer, D2D1::ColorF::GreenYellow, XMFLOAT2(0.5625f, 0.66667f), XMFLOAT2(1.0f, 0.33333f));
			delete[] wAnswer;
		}
		//�Ȼ���׼ˮ��
		m_LightControl->SetValue();
		XMFLOAT2 drawLT, drawRD;
		m_ExampleFruit.Fruit.SetWorldProj(viewMatrix, perMatrix);
		m_ExampleFruit.Fruit.Draw();
		XMVECTOR relativeMove = XMLoadFloat3(&m_ExampleFruit.Fruit.GetTranslationAbsolute());
		/*XMStoreFloat2(&drawLT, XMVector3TransformCoord(XMVector3TransformCoord(relativeMove + m_FruitWordLeftTop[m_ExampleFruit.Type], viewMatrix), perMatrix));
		XMStoreFloat2(&drawRD, XMVector3TransformCoord(XMVector3TransformCoord(relativeMove + m_FruitWordRightDown[m_ExampleFruit.Type], viewMatrix), perMatrix));*/
		m_ExampleFruit.Words.Draw(m_ExampleFruit.Wchar, D2D1::ColorF::Black, XMFLOAT2(-1.0f, -0.6f), XMFLOAT2(-0.8f, -0.8f));
		int size = m_FruitPairs.size();
		for (int i = 0; i < size; i++)
		{
			if (m_FruitPairs[i]->Fruit.m_Flags)//���ˮ���ɳ���Ļ����
			{
				delete m_FruitPairs[i];
				m_FruitPairs.erase(m_FruitPairs.begin() + i);
				i--;
				size--;
				continue;
			}
			else
			{	
				//��ˮ��
				m_LightControl->SetValue();
				m_FruitPairs[i]->Fruit.SetWorldProj(viewMatrix, perMatrix);
				m_FruitPairs[i]->Fruit.Draw();
				//������
				relativeMove =XMLoadFloat3(&m_FruitPairs[i]->Fruit.GetTranslationAbsolute());
				int sub = m_FruitPairs[i]->Type;
				XMStoreFloat2(&drawLT, XMVector3TransformCoord(XMVector3TransformCoord(relativeMove + m_FruitWordLeftTop[sub], viewMatrix), perMatrix) + m_WordsFixed[sub]);
				XMStoreFloat2(&drawRD, XMVector3TransformCoord(XMVector3TransformCoord(relativeMove + m_FruitWordRightDown[sub], viewMatrix), perMatrix) + m_WordsFixed[sub]);
				drawRD.x += 0.2f;
				drawLT.x -= 0.2f;
				m_FruitPairs[i]->Words.Draw(m_FruitPairs[i]->Wchar, D2D1::ColorF::Black, drawLT, drawRD);
			}
		}
	}
	HR(m_pSwapChain->Present(0, 0));
}

void DX11Game::Vegetable_Ninja::OnMouseLeftDown(WPARAM btnState, int x, int y)
{
	m_UI.back()->MouseLeftDown();
	m_IsKnife = true;
}

void DX11Game::Vegetable_Ninja::OnMouseLeftUp(WPARAM btnState, int x, int y)
{
	m_UI.back()->MouseLeftUp();
	ButtonClick();
	m_IsKnife = false;
}

void DX11Game::Vegetable_Ninja::OnMouseMove(WPARAM btnState, int x, int y)
{
}

void DX11Game::Vegetable_Ninja::Build2DEffect(char * path)
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFileA(path, 0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);

	// compilationMsgs can store errors or warnings.
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	// Even if there are no compilationMsgs, check to make sure there were no other errors.
	if (FAILED(hr))
	{
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, m_D3dDevice, &m_p2DEffect));

	ReleaseCOM(compiledShader);

	m_p2DTechnique = m_p2DEffect->GetTechniqueByName("TexTech");

	m_p2DEffectWorldViewProj = m_p2DEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void DX11Game::Vegetable_Ninja::Build3DEffect(char * path)
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFileA(path, 0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);

	// compilationMsgs can store errors or warnings.
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	// Even if there are no compilationMsgs, check to make sure there were no other errors.
	if (FAILED(hr))
	{
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, m_D3dDevice, &m_p3DEffect));

	ReleaseCOM(compiledShader);
	m_p3DTechnique = m_p3DEffect->GetTechniqueByName("LightTech");
	m_p3DEffectWorldViewProj = m_p3DEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_p3DWorld = m_p3DEffect->GetVariableByName("gWorld")->AsMatrix();
	m_p3DEffectWorldInvTranspose = m_p3DEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_p3DEffectEyePosW = m_p3DEffect->GetVariableByName("gEyePosW")->AsVector();

	m_p3DEffectMaterial = m_p3DEffect->GetVariableByName("gMaterial");

	m_pLightControl->SetFX(m_p3DEffect);
	m_pLightControl = LightControl::GetLightContal();
}

void DX11Game::Vegetable_Ninja::Build2DVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_p2DTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_D3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_p2DInputLayout));
}

void DX11Game::Vegetable_Ninja::Build3DVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_p3DTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_D3dDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_p3DInputLayout));
}

void DX11Game::Vegetable_Ninja::BuildBlend()
{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;//��Ϲ�ʽ�е�Դ����
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//��Ϲ�ʽ�е�Ŀ������
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;//��Ϲ�ʽ�Ĳ���
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;//��Ϲ�ʽ��Դ��ϰٷֱ�����
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//��Ϲ�ʽ�е�Ŀ���ϰٷֱ�����
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//��Ϲ�ʽ�еĻ�ϰٷֱȵĲ���
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//��Ϲ�ʽ����,һ�����
		m_D3dDevice->CreateBlendState(&blendDesc, &m_pAlphaToCoverageBS);
}

void DX11Game::Vegetable_Ninja::KeyBoardMsg()
{
}

void DX11Game::Vegetable_Ninja::LoadMainMenu()
{
	//����
	m_MainMenu.m_ImageBox[0].SetControl(XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f), 1);
	m_MainMenu.m_ImageBox[0].SetImage("Texture/����.png");
	//��Ϸ����
	m_MainMenu.m_ImageBox[1].SetControl(XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), 2);
	m_MainMenu.m_ImageBox[1].SetImage("Texture/����.png");
	//������Ϸ�İ�ť
	m_MainMenu.m_Button[0].SetControl(XMFLOAT2(-0.315f, 0.0f), XMFLOAT2(0.315f, -0.373333f), 3);
	m_MainMenu.m_Button[0].SetPicture_ButtonOut("Texture/��ʼ��Ϸ��ť-.png");
	m_MainMenu.m_Button[0].SetPicture_ButtonOn("Texture/��ʼ��Ϸ��ť.png");
	m_MainMenu.m_Button[0].SetPicture_ButtonDown("Texture/��ʼ��Ϸ��ť.png");
	m_MainMenu.m_Button[0].SetFont_All(L"��������", 36.0f);
	m_MainMenu.m_Button[0].SetWords_All(L"", D2D1::ColorF::White);
	m_MainMenu.m_Button[0].SetWords_ButtonOn(L"", D2D1::ColorF::YellowGreen);
	m_MainMenu.m_Button[0].SetWords_ButtonDown(L"", D2D1::ColorF::Orange);
	//�˳���Ϸ�İ�ť
	m_MainMenu.m_Button[1].SetControl(XMFLOAT2(-0.315f, -0.4f), XMFLOAT2(0.315f, -0.773332f), 3);
	m_MainMenu.m_Button[1].SetPicture_ButtonOut("Texture/�˳���Ϸ��ť-.png");
	m_MainMenu.m_Button[1].SetPicture_ButtonOn("Texture/�˳���Ϸ��ť.png");
	m_MainMenu.m_Button[1].SetPicture_ButtonDown("Texture/�˳���Ϸ��ť.png");
	m_MainMenu.m_Button[1].SetFont_All(L"��������", 36.0f);
	m_MainMenu.m_Button[1].SetWords_All(L"", D2D1::ColorF::White);
	m_MainMenu.m_Button[1].SetWords_ButtonOn(L"", D2D1::ColorF::YellowGreen);
	m_MainMenu.m_Button[1].SetWords_ButtonDown(L"", D2D1::ColorF::Orange);
	m_UI.push_back(&m_MainMenu);
}

void DX11Game::Vegetable_Ninja::LoadGame()
{
	//����
	m_GameMenu.m_ImageBox[0].SetControl(XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f), 1);
	m_GameMenu.m_ImageBox[0].SetImage("Texture/����.png");
	//��������
	m_GameMenu.m_ImageBox[1].SetControl(XMFLOAT2(0.5625f, 1.0f), XMFLOAT2(1.0f, 0.66667f), 99);
	m_GameMenu.m_ImageBox[1].SetImage("Texture/������.png");
	//����1
	m_GameMenu.m_ImageBox[2].SetControl(XMFLOAT2(0.5625f, -0.666667), XMFLOAT2(1.0f, -1.0f), 97);
	m_GameMenu.m_ImageBox[2].SetImage("Texture/��.png");
	//����2
	m_GameMenu.m_ImageBox[3].SetControl(XMFLOAT2(0.5625f, -0.666667), XMFLOAT2(1.0f, -1.0f), 98);
	m_GameMenu.m_ImageBox[3].SetImage("Texture/��.png");
	//����3
	m_GameMenu.m_ImageBox[4].SetControl(XMFLOAT2(0.5625f, -0.666667), XMFLOAT2(1.0f, -1.0f), 99);
	m_GameMenu.m_ImageBox[4].SetImage("Texture/��.png");
	//������
	m_GameMenu.m_ImageBox[5].SetControl(XMFLOAT2(0.5625f, -0.666667), XMFLOAT2(1.0f, -1.0f), 96);
	m_GameMenu.m_ImageBox[5].SetImage("Texture/��-4.png");
	//�˵�
	m_GameMenu.m_Button[0].SetControl(XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(-0.8125f, 0.666667f), 99);
	m_GameMenu.m_Button[0].SetPicture_ButtonOut("Texture/��ͣ-.png");
	m_GameMenu.m_Button[0].SetPicture_ButtonOn("Texture/��ͣ.png");
	m_GameMenu.m_Button[0].SetFont_All(L"��������", 48.0f);
	m_GameMenu.m_Button[0].SetWords_All(L"", D2D1::ColorF::White);
	m_GameMenu.m_Button[0].SetWords_ButtonOn(L"", D2D1::ColorF::YellowGreen);
	m_GameMenu.m_Button[0].SetWords_ButtonDown(L"", D2D1::ColorF::Orange);
	//����
	m_GameMenu.m_TextBox[0].SetControl(XMFLOAT2(0.5625f, 1.0f), XMFLOAT2(1.0f, 0.66667f), 3);
	m_GameMenu.m_TextBox[0].SetFont(L"Georgia", 48.0f);
	m_GameMenu.m_TextBox[0].SetWords(L"100", D2D1::ColorF::Orange);
	//����ˮ��
	m_ExampleFruit.Words.Initialize(m_pSwapChain, mhMainWnd);
	m_ExampleFruit.Words.SetFont(L"΢���ź�", 24.0f);
	m_ExampleFruit.Words.SetParagraphAlignment();
	m_ExampleFruit.Words.SetTextAlignment();
	m_CorrectAnswer.Initialize(m_pSwapChain, mhMainWnd);
	m_CorrectAnswer.SetFont(L"΢���ź�", 24.0f);
	m_CorrectAnswer.SetParagraphAlignment();
	m_CorrectAnswer.SetTextAlignment();
	LoadFruitChange();
}

void DX11Game::Vegetable_Ninja::LoadPause()
{
	//����
	m_PauseMenu.m_ImageBox[0].SetControl(XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f), 30);
	m_PauseMenu.m_ImageBox[0].SetImage("Texture/����.png");
	//������Ϸ
	m_PauseMenu.m_Button[0].SetControl(XMFLOAT2(-0.315f, 0.0f), XMFLOAT2(0.315f, -0.373332f), 31);
	m_PauseMenu.m_Button[0].SetPicture_ButtonOut("Texture/������Ϸ��ť-.png");
	m_PauseMenu.m_Button[0].SetPicture_ButtonOn("Texture/������Ϸ��ť.png");
	m_PauseMenu.m_Button[0].SetFont_All(L"��������", 36.0f);
	m_PauseMenu.m_Button[0].SetWords_All(L"", D2D1::ColorF::White);
	//�������˵�
	m_PauseMenu.m_Button[1].SetControl(XMFLOAT2(-0.315f, -0.4f), XMFLOAT2(0.315f, -0.773332f), 31);
	m_PauseMenu.m_Button[1].SetPicture_ButtonOut("Texture/���ذ�ť-.png");
	m_PauseMenu.m_Button[1].SetPicture_ButtonOn("Texture/���ذ�ť.png");
	m_PauseMenu.m_Button[1].SetFont_All(L"Georgia", 36.0f);
	m_PauseMenu.m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
}

void DX11Game::Vegetable_Ninja::LoadGameEnd()
{
	//����
	m_GameEnd.m_ImageBox[0].SetControl(XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f), 30);
	m_GameEnd.m_ImageBox[0].SetImage("Texture/����.png");
	//�Ʒְ屳��
	m_GameEnd.m_ImageBox[1].SetControl(XMFLOAT2(-0.314063f, 0.402778f), XMFLOAT2(0.310937f,-0.669444f), 31);
	m_GameEnd.m_ImageBox[1].SetImage("Texture/��¼��.png"); 
	//��Ϸ��������
	m_GameEnd.m_ImageBox[2].SetControl(XMFLOAT2(-0.21875f, 0.555556f), XMFLOAT2(0.21875f, 0.222222f), 32);
	m_GameEnd.m_ImageBox[2].SetImage("Texture/��Ϸ����.png");
	//����
	m_GameEnd.m_TextBox[0].SetControl(XMFLOAT2(-0.225f, -0.111111f), XMFLOAT2(0.225f, -0.413889f), 33);
	m_GameEnd.m_TextBox[0].SetFont(L"΢���ź�", 48.0f);
	wchar_t text[128] = {};
	lstrcpyW(text, std::to_wstring(m_Goal).c_str());
	m_GameEnd.m_TextBox[0].SetWords(text, D2D1::ColorF::Orange);
	//�������˵�
	m_GameEnd.m_Button[0].SetControl(XMFLOAT2(-0.225f, - 0.413889f), XMFLOAT2(0.225f, -0.680556f), 34);
	m_GameEnd.m_Button[0].SetPicture_ButtonOut("Texture/���ذ�ť-.png");
	m_GameEnd.m_Button[0].SetPicture_ButtonOn("Texture/���ذ�ť.png");
	m_GameEnd.m_Button[0].SetFont_All(L"΢���ź�", 36.0f);
	m_GameEnd.m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
}

void DX11Game::Vegetable_Ninja::LoadLoading()
{
	m_Loading.m_ImageBox[0].SetControl(XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f), 1);
	m_Loading.m_ImageBox[0].SetImage("Texture/���ػ���.jpg");
}

void DX11Game::Vegetable_Ninja::CalculateVertices()
{
	for (int i = 0; i < 8; i++)
	{
		float maxX, minX, maxY, minY, maxZ, minZ;
		maxX = minX = m_FruitVertices[i][0].Pos.x;
		maxY = minY = m_FruitVertices[i][0].Pos.y;
		maxZ = minZ = m_FruitVertices[i][0].Pos.z;
		for (int i1 = 0; i1 < m_FruitVertices[i].size(); i1++)
		{
			if (maxX < m_FruitVertices[i][i1].Pos.x)
			{
				maxX = m_FruitVertices[i][i1].Pos.x;
			}
			else if (minX > m_FruitVertices[i][i1].Pos.x)
			{
				minX = m_FruitVertices[i][i1].Pos.x;
			}
			if (maxY < m_FruitVertices[i][i1].Pos.y)
			{
				maxY = m_FruitVertices[i][i1].Pos.y;
			}
			else if (minY > m_FruitVertices[i][i1].Pos.y)
			{
				minY = m_FruitVertices[i][i1].Pos.y;
			}
			if (maxZ < m_FruitVertices[i][i1].Pos.z)
			{
				maxZ = m_FruitVertices[i][i1].Pos.z;
			}
			else if (minZ > m_FruitVertices[i][i1].Pos.z)
			{
				minZ = m_FruitVertices[i][i1].Pos.z;
			}
		}
		minX *= m_ScaleRate[i];
		maxX *= m_ScaleRate[i];
		minY *= m_ScaleRate[i];
		maxY *= m_ScaleRate[i];
		minZ *= m_ScaleRate[i];
		maxZ *= m_ScaleRate[i];
		m_FruitPort[i].Height = maxY - minY;
		m_FruitPort[i].Width = maxX - minX;
		m_FruitPort[i].TopLeftX = minX;
		m_FruitPort[i].TopLeftY = maxY;
		m_FruitPort[i].MinDepth = maxZ;
		m_FruitPort[i].MaxDepth = minZ;
		m_Translation[i].x = (maxX + minX)*-0.5f;
		m_Translation[i].y = (maxY + minY)*-0.5f;
		m_FruitBox[i].Center = XMFLOAT3(0.0f, 0.0f, m_Translation[i].z);
		m_FruitBox[i].Extents = XMFLOAT3((maxX - minX)*0.5f, (maxY - minY)*0.5f, (maxZ - minZ)*0.5f);
		m_FruitWordLeftTop[i] =XMLoadFloat3(&XMFLOAT3(minX, maxY, maxZ));
		m_FruitWordRightDown[i] = XMLoadFloat3(&XMFLOAT3(maxX, minY, maxZ));
	}
}

void DX11Game::Vegetable_Ninja::ButtonClick()
{
	switch (m_GameStatus)
	{
	case MENU_MAIN:
	{
		switch (m_UI.back()->ButtonCheck())
		{
		case 0:
		{
			m_GameStatus = MENU_LOADING;
			m_GameControl.Clear();
			for (Sprite2D *& xv : m_AfterKnifeShow)
			{
				delete xv;
			}
			m_AfterKnifeShow.clear();
			m_AfterKnifeShowTime.clear();
			for (FruitPairs * &sprite : m_FruitPairs)
			{
				delete sprite;
			}
			m_FruitPairs.clear();
			m_UI.clear();
			m_UI.push_back(&m_Loading);
			for (int i = 0; i < m_UI[0]->ImageBox_Size(); i++)
			{
				m_UI[0]->m_ImageBox[i].Visible();
			}
			m_Goal = 0;
			m_Heart = 3;
			m_GameMenu.m_TextBox[0].SetWords(std::to_wstring(m_Goal).c_str(), D2D1::ColorF::Orange);
			m_UI[0]->ResetButtonCheck();
			break;
		}
		case 1:
		{
			m_UI.back()->ResetButtonCheck();
			PostQuitMessage(0);
			break;
		}
		default:
			break;
		}
		break;
	}
	case MENU_GAME:
	{
		switch (m_UI.back()->ButtonCheck())
		{	
		case 0:
		{
			m_UI.back()->ResetButtonCheck();
			m_GameStatus = MENU_PAUSE;
			for (DX11UI::UI *& ui : m_UI)
			{
				for (int i = 0; i < ui->Button_Size(); i++)
				{
					ui->m_Button[i].Disable();
				}
			}
			m_UI.push_back(&m_PauseMenu);
	
			break;
		}
		default:
			break;
		}
		break;
	}
	case MENU_PAUSE:
	{
		switch (m_UI.back()->ButtonCheck())
		{
		case 0:
		{
			m_UI.back()->ResetButtonCheck();
			m_GameStatus = MENU_GAME;
			m_UI.pop_back();
			for (DX11UI::UI *& ui : m_UI)
			{
				for (int i = 0; i < ui->Button_Size(); i++)
				{
					ui->m_Button[i].Enable();
				}
			}
			break;
		}
		case 1:
		{
			m_UI.back()->ResetButtonCheck();
			m_GameStatus = MENU_MAIN;
			m_FruitPairs.clear();
			for (DX11UI::UI *& ui : m_UI)
			{
				for (int i = 0; i < ui->Button_Size(); i++)
				{
					ui->m_Button[i].Enable();
				}
			}
			m_UI.clear();
			m_UI.push_back(&m_MainMenu);
			break;
		}
		default:
			break;
		}
		break;
	}
	case MENU_END:
	{
		switch (m_UI[m_UI.size() - 1]->ButtonCheck())
		{
		case 0:
		{
			m_UI.back()->ResetButtonCheck();
			m_GameStatus = MENU_MAIN;
			for (DX11UI::UI *& ui : m_UI)
			{
				for (int i = 0; i < ui->Button_Size(); i++)
				{
					ui->m_Button[i].Enable();
				}
			}
			m_UI.clear();
			m_UI.push_back(&m_MainMenu);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void DX11Game::Vegetable_Ninja::LoadQuestionsAndAnswers()
{
	std::string tempString;
	std::ifstream file("Data/question.dat");
	while (std::getline(file, tempString))
	{
		m_Questions.push_back(CharToWchar(tempString.c_str(), tempString.length()));
		tempString.clear();
	}
	file.close();
	file.open("Data/answer.dat");
	while (std::getline(file, tempString))
	{
		m_Answers.push_back(CharToWchar(tempString.c_str(), tempString.length()));
		tempString.clear();
	}
	file.close();
	m_QuestionNum = m_Questions.size();
}

void DX11Game::Vegetable_Ninja::LoadVegetables()
{
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/����/Orange.jpg", nullptr, nullptr, &m_pTfruits[0], nullptr);//����full	
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/����/AM130_017_diff.jpg", nullptr, nullptr, &m_pTfruits[1], nullptr);//����
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/����/AM130_004_diff1.jpg", nullptr, nullptr, &m_pTfruits[2], nullptr);//����
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/������/all.jpg", nullptr, nullptr, &m_pTfruits[3], nullptr);//������ all
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/������/AM130_007_diff.jpg", nullptr, nullptr, &m_pTfruits[4], nullptr);//������
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/⨺���/AM130_015_diff.jpg", nullptr, nullptr, &m_pTfruits[5], nullptr);//⨺���
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/ľ��/all.jpg", nullptr, nullptr, &m_pTfruits[6], nullptr);//ľ�� all
	D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/ƻ��/Full-Apple.jpg", nullptr, nullptr, &m_pTfruits[7], nullptr);//ƻ�� all
	//D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/����/Orange_half.jpg", nullptr, nullptr, &m_pTfruits[8], nullptr);//���� half
	//D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/������/half.jpg", nullptr, nullptr, &m_pTfruits[9], nullptr);//������ half
	//D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/ľ��/half.jpg", nullptr, nullptr, &m_pTfruits[10], nullptr);//ľ�� half
	//D3DX11CreateShaderResourceViewFromFileA(m_D3dDevice, "Texture/ƻ��/Half-Apple.jpg", nullptr, nullptr, &m_pTfruits[11], nullptr);//ƻ�� half

	m_Fut1 = m_Fut2 = m_Fut3 = m_Fut4 = false;
	std::promise<bool> prom1;
	std::future<bool>fut1 = prom1.get_future();//��prom��fut����

	std::promise<bool> prom2;
	std::future<bool>fut2 = prom2.get_future();//��prom��fut����

	std::promise<bool> prom3;
	std::future<bool>fut3 = prom3.get_future();//��prom��fut����
 
	std::promise<bool> prom4;
	std::future<bool>fut4 = prom4.get_future();//��prom��fut����
	
	std::thread th1(&Vegetable_Ninja::LoadVegetables1, this, std::ref(fut1));//�½��̣߳�ִ�к��� 
	th1.detach();
	std::thread th2(&Vegetable_Ninja::LoadVegetables2, this, std::ref(fut2));//�½��̣߳�ִ�к��� 
	th2.detach();
	std::thread th3(&Vegetable_Ninja::LoadVegetables3, this, std::ref(fut3));//�½��̣߳�ִ�к���
	th3.detach();
	std::thread th4(&Vegetable_Ninja::LoadVegetables4, this, std::ref(fut4));//�½��̣߳�ִ�к��� 
	th4.detach();	
}

void DX11Game::Vegetable_Ninja::LoadVegetables1(std::future<bool>& fut)
{
	m_NumofVertex[0] = Sprite3D::LoadMesh(OBJ, "Texture/����/all_orange.obj", &m_pBfruits[0], m_FruitVertices[0]);//���� all
	//m_NumofVertex[8] = Sprite3D::LoadMesh(OBJ, "Texture/����/half_orange.obj", &m_pBfruits[8], m_FruitVertices[8]);//���� half
	m_NumofVertex[1] = Sprite3D::LoadMesh(OBJ, "Texture/����/all_tomato.obj", &m_pBfruits[1], m_FruitVertices[1]);//���� all
	//m_NumofVertex[9] = Sprite3D::LoadMesh(OBJ, "Texture/����/half_tomato.obj", &m_pBfruits[9], m_FruitVertices[9]);//���� half
	m_Fut1 =  true;
}

void DX11Game::Vegetable_Ninja::LoadVegetables2(std::future<bool>& fut)
{
	m_NumofVertex[2] = Sprite3D::LoadMesh(OBJ, "Texture/����/all_yellowpeach.obj", &m_pBfruits[2], m_FruitVertices[2]);//���� all
	//m_NumofVertex[10] = Sprite3D::LoadMesh(OBJ, "Texture/����/half_yellowpeach.obj", &m_pBfruits[10], m_FruitVertices[10]);//���� half
	m_NumofVertex[3] = Sprite3D::LoadMesh(OBJ, "Texture/������/all_firedragonball.obj", &m_pBfruits[3], m_FruitVertices[3]);//������ all
	//m_NumofVertex[11] = Sprite3D::LoadMesh(OBJ, "Texture/������/half _firedragonball.obj", &m_pBfruits[11], m_FruitVertices[11]);//������ half
	m_Fut2 = true;
}

void DX11Game::Vegetable_Ninja::LoadVegetables3(std::future<bool>& fut)
{
	m_NumofVertex[4] = Sprite3D::LoadMesh(OBJ, "Texture/������/all_patato.obj", &m_pBfruits[4], m_FruitVertices[4]);//������ all
	//m_NumofVertex[12] = Sprite3D::LoadMesh(OBJ, "Texture/������/half_patato.obj", &m_pBfruits[12], m_FruitVertices[12]); //������ half
	m_NumofVertex[5] = Sprite3D::LoadMesh(OBJ, "Texture/⨺���/all_kiwi.obj", &m_pBfruits[5], m_FruitVertices[5]);//⨺��� all
	//m_NumofVertex[13] = Sprite3D::LoadMesh(OBJ, "Texture/⨺���/half_kiwi.obj", &m_pBfruits[13], m_FruitVertices[13]);//⨺��� half
	m_Fut3 = true;
}

void DX11Game::Vegetable_Ninja::LoadVegetables4(std::future<bool>& fut)
{
	m_NumofVertex[6] = Sprite3D::LoadMesh(OBJ, "Texture/ľ��/all_pawpaw.obj", &m_pBfruits[6], m_FruitVertices[6]);//ľ�� all
	//m_NumofVertex[14] = Sprite3D::LoadMesh(OBJ, "Texture/ľ��/half_pawpaw.obj", &m_pBfruits[14], m_FruitVertices[14]);//ľ�� half
	m_NumofVertex[7] = Sprite3D::LoadMesh(OBJ, "Texture/ƻ��/Full-Apple.obj", &m_pBfruits[7], m_FruitVertices[7]);//ƻ�� all
	//m_NumofVertex[15] = Sprite3D::LoadMesh(OBJ, "Texture/ƻ��/Half-Apple.obj", &m_pBfruits[15], m_FruitVertices[15]);//ƻ�� half
	m_Fut4 = true;
}

void DX11Game::Vegetable_Ninja::LoadFruitChange()
{
	std::ifstream file;
	std::string comments;
	file.open("Texture/Scale.dat");
	int i = 0;
	while (!file.eof())
	{
		file >> m_ScaleRate[i];
		i++;
	}
	file.close();
	file.open("Texture/Translation.dat");
	i = 0;
	while (!file.eof())
	{
		file >> m_Translation[i].x >> m_Translation[i].y >> m_Translation[i].z;
		i++;
	}
	file.close();
	file.open("Texture/wordT.dat");
	i = 0;
	while (!file.eof())
	{
		float x, y;
		file >> comments >> x >> y;
		m_WordsFixed[i] = XMLoadFloat2(&XMFLOAT2(x, y));
		i++;
	}
	file.close();
}

void DX11Game::Vegetable_Ninja::LoadLight()
{
	m_LightControl = LightControl::GetLightContal();
	DirectionalLight dirLight0;
	dirLight0.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight0.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLight0.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight0.Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_LightControl->SetLight<DirectionalLight>("Only", dirLight0);
}

void DX11Game::Vegetable_Ninja::GameOn(float dt)
{
	//m_LastTime += dt;
	if (m_FruitPairs.empty())
	{
		//m_LastTime -= THROW_FRUIT_INTERVAL;
		//����´ε��׳�ˮ�������ͣ�����û����ȷˮ�����ͣ���ˮ��0�ĳ���ȷ����
		int answerArray[4]{};
		Random::INT::ArrayD(0, 4, answerArray, 4);
		for (int i = 0; i < 4; i++)
		{
			ThrowFruit(m_FruitArray[i], m_ExampleFruit.QASub * 4 + answerArray[i]);
		}
	}
}

void DX11Game::Vegetable_Ninja::ThrowFruit(int fruitType, int answerSub)
{
	float location = Random::FLOAT::Next(-0.6f, 0.6f, 4);
	FruitPairs * fruit = new FruitPairs(fruitType, answerSub, m_Answers[answerSub], m_pSwapChain, mhMainWnd);
	fruit->Fruit.SetMatrial(m_FruitMatrial);
	fruit->Fruit.SetTexture(m_pTfruits[fruitType]);
	fruit->Fruit.SetVertex(m_pBfruits[fruitType], m_NumofVertex[fruitType]);
	fruit->Fruit.SetReScale(XMFLOAT3(m_ScaleRate[fruitType], m_ScaleRate[fruitType], m_ScaleRate[fruitType]));
	fruit->Fruit.SetReLocation(m_Translation[fruitType]);
	fruit->Fruit.ReSetSprite();
	fruit->Fruit.Move(XMFLOAT3(location, -0.6f, 0.0f));
	float velocityX = Random::FLOAT::Next(0.0f, 0.4f, 4);
	if (location > 0)
	{
		velocityX *= -1.0f;
	}
	m_FruitPairs.push_back(fruit);
	m_GameControl.AddSprite3D(&fruit->Fruit, XMFLOAT3(velocityX, Random::FLOAT::Next(0.6f, 0.8f, 4), 0.0f));
}