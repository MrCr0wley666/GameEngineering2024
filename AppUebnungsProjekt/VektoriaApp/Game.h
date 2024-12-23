#pragma once
//-----------------------------------------------------------------------
// CGame
// 
// Erklärung : Eine weitestgehend leere Klasse, 
//             sinnfällig in eine Windows-Applikation eingebettet,
//             um ein Spiel bzw. eine 3D-Simulation zu erzeugen
// Autor     : Prof. Dr. Tobias Breiner
// Ort       : Pfungstadt
// Zeit      : seit Aug. 2011 (mit seitdem ständigen Verbesserungs-Updates)
// Copyright : Tobias Breiner  
// Disclaimer: Nutzung auf eigene Gefahr, keine Gewähr, no warranty!
//------------------------------------------------------------------------


#ifdef _WIN64
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug64.lib")
#pragma comment (lib, "VektoriaMath_Debug64.lib")
#else
#pragma comment (lib, "Vektoria_Release64.lib")
#pragma comment (lib, "VektoriaMath_Release64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug.lib")
#pragma comment (lib, "VektoriaMath_Debug.lib")
#else
#pragma comment (lib, "Vektoria_Release.lib")
#pragma comment (lib, "VektoriaMath_Release.lib")
#endif
#endif


#include "Vektoria\Root.h"

using namespace Vektoria;

#define NR_DEATHSTARS 1
#define NR_OF_STONES 1000


class CGame
{
public:
	// Wird vor Begin einmal aufgerufen (Konstruktor):
	CGame(void);

	// Wird nach Ende einmal aufgerufen (Destruktor):
	~CGame(void);

	// Wird zu Begin einmal aufgerufen:
	void Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash* psplash);

	// Wird während der Laufzeit bei jedem Bildaufbau aufgerufen:
	void Tick(float fTime, float fTimeDelta);

	// Wird am Ende einmal aufgerufen:
	void Fini();

	// Wird immer dann aufgerufen, wenn der Benutzer die Fenstergröße verändert hat:
	void WindowReSize(int iNewWidth, int iNewHeight);

	// Holt das minimale Zeitdelta zur eventuellen Frame-Rate-Beschränkung:
	float GetTimeDeltaMin();

	// Holt die Versionsnummer:
	float GetVersion();


private:

	// Member_ZockKlasse --> m_zr

	// Die Wurzel des Szenengrafen: 
	CRoot m_zr;

	// Hier ist Platz für Deine weiteren Vektoriaobjekte:	--> Knotenobjekte des Szenengraphen
	CCamera m_zc; // Kamera

	CFrame m_zf; // Fenster, in das wir reinrendern

	CViewport m_zv;

	COverlay m_zo;

	CScene m_zs; // 3D-Raum/Szene

	CPlacement m_zp;

	CLightParallel m_zlp; // Licht

	CDeviceKeyboard m_zdk;

	//Erde
	CGeoSphere m_zgSphere;

	CPlacement m_zpSphere;

	CMaterial m_zmEarth;

	//Sonne

	CGeoSphere m_zgSun;

	CPlacement m_zpSun;

	CMaterial m_zmSun;

	//Wolken
	CGeoSphere m_zgCloud;

	CPlacement m_zpCloud;

	CMaterial m_zmCloud;

	//Weltall
	CGeoSphere m_zgWeltall;

	CPlacement m_zpWeltall;

	CMaterial m_zmWeltall;

	//Moon
	CGeoSphere m_zgMoon;

	CPlacement m_zpMoon;

	CMaterial m_zmMoon;

	//EarthMoonOrbit

	CPlacement m_zpEarthMoonOrbit;

	//Raumschiff
	CFileWavefront m_fileojb;

	CGeoTriangleTable* m_pzgRocket = nullptr;

	CPlacement m_zpRocket;

	CMaterial m_zmRocket;

	//Todesstern
	CFileWavefront m_fileojb1;

	CGeoTriangleTable* m_pzgTodesstern = nullptr;

	CPlacement m_zapTodesstern[NR_DEATHSTARS];

	CPlacement m_zpTodesstern;
	CPlacement m_zpTodesstern2;

	CMaterial m_zmTodesstern;

	CHVector m_avTodessternRandom[NR_DEATHSTARS];

	//Curor
	CDeviceCursor m_zdc;

	CImage m_ziPfeil;

	CImage m_ziHand;

	COverlay m_zoPfeil;

	COverlay m_zoHand;

	//Picking
	CGeos m_zgPick;

	CMaterial m_zmTodessternTreffer;

	//Overlay
	COverlay m_zoStartbildschirm;

	CImage m_ziStarbildschirm;

	COverlay m_zoEinzel;

	CImage m_ziEinzel;

	COverlay m_zoMulti;

	CImage m_ziMulti;

	COverlays m_zosOverlayliste;

	bool m_bMulti = false;

	bool m_bOverlay = true;

	//Nebel
	CPlacement m_azpNebula[5];

	CGeoQuad m_zgNebula;

	CMaterial m_zmNebula;

	//Astroiden
	CPlacement m_azpStone[NR_OF_STONES];

	CGeoQuad m_azgStone[3];

	CMaterial m_azmStone[3];

	CHVector m_avStoneRandom[NR_OF_STONES];

	//Cockpit
	CFileWavefront m_fileojb2;

	CGeoTriangleTable* m_pzgCockpit = nullptr;

	CPlacement m_zpCockpit;

	CMaterial m_zmCockpit;

	//Kugeln
	CPlacements m_zpsBullets;

	CPlacement m_zpBulletTemplate;

	CFileWavefront m_fileojb3;

	CGeoTriangleTable* m_pzgBullet = nullptr;

	CMaterial m_zmBullet;

	//Kugeln Gegner
	CPlacements m_zpsBulletsEnemy;

	CPlacement m_zpBulletTemplateEnemy;

	CFileWavefront m_fileojb4;

	CGeoTriangleTable* m_pzgBulletEnemy = nullptr;

	CMaterial m_zmBulletEnemy;

	int Timer = 0;

	//Splitscreen
	CCamera m_zcLeft; // Kamera links
	CCamera m_zcRight; // Kamera rechts

	CViewport m_rvLeft;
	CViewport m_rvRight;

	CPlacement m_zpRight;
	CPlacement m_zpLeft;

	CGeoTriangleTable* m_pzgCockpitLeft = nullptr;
	CPlacement m_zpCockpitLeft;
	CMaterial m_zmCockpitLeft;

	CGeoTriangleTable* m_pzgCockpitRight = nullptr;
	CPlacement m_zpCockpitRight;
	CMaterial m_zmCockpitRight;

	//Sound
	CAudio m_zaaBulletSound[20];
	CAudio m_ExolionSound;


};