#pragma once

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"
#include "RenderStates.h"
#include "LightControl.h"
#include "xnacollision.h"
#include "Camera.h"
#include "UI.h"
#include <future>//std::promisestd::future
#include <thread>//std::thread
#include <functional>//std::ref
#include "GameControl.h"
#include <string>
#include <fstream>
#include "FruitWords.h"

#pragma comment(lib,"winmm.lib")
#include "Random.h"

#define XSHOWINTERVAL 0.5f
#define THROW_FRUIT_INTERVAL 2.0f

namespace DX11Game
{
	class Vegetable_Ninja :public D3DApp
	{
	public:
		Vegetable_Ninja(HINSTANCE hInstance);
		~Vegetable_Ninja();

		bool Init();
		void OnResize();
		void UpdateScene(float dt);
		void DrawScene();

		void OnMouseLeftDown(WPARAM btnState, int x, int y);
		void OnMouseLeftUp(WPARAM btnState, int x, int y);
		void OnMouseMove(WPARAM btnState, int x, int y);

	private:
		enum GameStaus { MENU_MAIN, MENU_GAME, MENU_PAUSE, MENU_END, MENU_LOADING };
		enum Fruit
		{
			ORANGE,
			TOMATO,
			PEACH,
			DRAGON_FRUIT,
			PATATO,
			KIWI,
			PAWPAW,
			APPLE
		};

		//��2D��ɫ���ļ�
		void Build2DEffect(char * path);
		//��3D��ɫ���ļ�
		void Build3DEffect(char * path);
		//����2D���㲼��
		void Build2DVertexLayout();
		//����3D���㲼��
		void Build3DVertexLayout();
		//������ϼ���
		void BuildBlend();
		//������Ϣ
		void KeyBoardMsg();
		void LoadMainMenu();
		void LoadGame();
		void LoadPause();
		void LoadGameEnd();
		void LoadLoading();
		void CalculateVertices();
		//����Ƿ���Button������
		void ButtonClick();
		//����ˮ��
		void LoadVegetables();
		void LoadVegetables1(std::future<bool>&fut);
		void LoadVegetables2(std::future<bool>&fut);
		void LoadVegetables3(std::future<bool>&fut);
		void LoadVegetables4(std::future<bool>&fut);
		void LoadFruitChange();//��ȡ�ļ�����ˮ����ʼ�任
		//��������
		void LoadQuestionsAndAnswers();
		//���صƹ�
		void LoadLight();
		//��ˮ��
		void GameOn(float dt);
		void ThrowFruit(int fruitType, int answerSub);

	private:
		LPD3D11EFFECT m_p2DEffect;
		LPD3D11EFFECT m_p3DEffect;
		LPD3D11EFFECTTECHNIQUE m_p2DTechnique; 
		LPD3D11EFFECTTECHNIQUE m_p3DTechnique;
		LPD3D11EFFECTMATRIXVARIABLE m_p2DEffectWorldViewProj;
		LPD3D11EFFECTMATRIXVARIABLE m_p3DEffectWorldViewProj;
		LPD3D11EFFECTMATRIXVARIABLE m_p3DWorld;
		LPD3D11EFFECTMATRIXVARIABLE m_p3DEffectWorldInvTranspose;
		LPD3D11EFFECTVECTORVARIABLE m_p3DEffectEyePosW;
		LPD3D11EFFECTVARIABLE m_p3DEffectMaterial;
		ID3D11InputLayout * m_p2DInputLayout;
		ID3D11InputLayout * m_p3DInputLayout;
		ID3D11BlendState* m_pAlphaToCoverageBS;
		//Buffer
		ID3D11Buffer *m_pBfruits[16];
		//Texture
		ID3D11ShaderResourceView * m_pTfruits[11];

		XMFLOAT4X4 mWorld;
		XMFLOAT4X4 mView;
		XMFLOAT4X4 mProj;

		float mTheta;
		float mPhi;
		float mRadius;

		POINT mLastMousePos;
		//����
		std::vector<wchar_t *> m_Questions;
		//������Ŀ
		int m_QuestionNum;
		//�׳�ˮ����������
		int m_FruitArray[4]{};
		//��
		std::vector<wchar_t *> m_Answers;
		//��ȷ�����
		int m_CorrectSub;
		//�׳������߹�
		std::vector<FruitPairs *> m_FruitPairs;
		std::vector<Vertex3D> m_FruitVertices[16];
		//��Ϸ��ʹ�õı�ʶչʾ
		std::vector<Sprite2D *> m_AfterKnifeShow;
		//�ش����ʱ����ʾ
		DirectWrite m_CorrectAnswer;
		//��ʾ��ʾʱ��
		float m_CorrectAnswerTime;
		//��ȷ�����
		int m_CorrectNum;
		//�Ƿ�Ϊ��һ�μ���
		bool m_IsFirstLoad;
		std::vector<float> m_AfterKnifeShowTime;
		XMFLOAT2 m_MousePoint;
		D3D11_VIEWPORT m_FruitPort[16];
		XNA::AxisAlignedBox m_FruitBox[16];
		XMVECTOR m_FruitWordLeftTop[16];
		XMVECTOR m_FruitWordRightDown[16];
		XMVECTOR m_WordsFixed[16];
		float m_ScaleRate[16];
		XMFLOAT3 m_Translation[16];
		Material m_FruitMatrial;
		int m_NumofVertex[16];
		int m_GameStatus;//�ж�״̬
		bool m_IsKnife;		//�����Ϊˮ������
		LightControl * m_pLightControl;
		//UI����
		std::vector<DX11UI::UI *> m_UI;
		DX11UI::UI m_MainMenu;
		DX11UI::UI m_GameMenu;
		DX11UI::UI m_PauseMenu;
		DX11UI::UI m_GameEnd;
		DX11UI::UI m_Loading;
		//�߹�ģ�Ϳ���
		DX11Game::GameControl m_GameControl;
		
		Camera m_Camera;
		LightControl *m_LightControl;
		FruitPairs m_ExampleFruit;
		int m_Goal, m_Heart;
		bool m_Fut1, m_Fut2, m_Fut3, m_Fut4;
		float m_LastTime;
		XMFLOAT2 m_Window;
	};
}