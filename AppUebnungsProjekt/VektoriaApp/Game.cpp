#include "Game.h"

CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash)
{
	//---------------------------------
	// Initialisiere die Knotenobjekte:
	//---------------------------------

	// Zuallererst muss die Root mit dem Splash-Screen initialisiert werden, 
	// damit die Engine freigeschaltet wird:
	m_zr.Init(psplash);

	// Hier kommen alle weiteren Initialisierungen hinein: 

	m_zf.Init(hwnd, procOS);
	m_zv.InitFull(&m_zc);	// Viewport über ganze Größe initialisieren
	m_zo.InitFull(&m_zmEarth);


	// Vernetzung:
	m_zr.AddFrame(&m_zf);						// Frame an Root hängen
	m_zf.AddViewport(&m_zv);					// Viewport an Frame hängena

	//Scene
	m_zr.AddScene(&m_zs);						// Szene an Root hängen
	m_zs.AddPlacement(&m_zp);					// Placement an Szene hängen

	//Licht
	m_zs.AddLightParallel(&m_zlp);				//Parallel Light
	m_zlp.Init(CHVector(-1, -1, -1));			// Licht initialisieren

	//Camera
	m_zp.AddCamera(&m_zc);						// Kamera an Placement hängen
	m_zp.TranslateZ(250);				   		//Translate Camera
	m_zc.Init(0.3f);

	//Controlls
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zp.SetTranslationSensitivity(10);

	//-----------Sun------------

	//Sonnenmaterial
	m_zmSun.MakeTextureDiffuse("textures/sun_image.jpg");

	//Sonnen Sphere
	m_zgSun.Init(6.0f, &m_zmSun,50,50);
	m_zs.AddPlacement(&m_zpSun);
	m_zpSun.AddGeo(&m_zgSun);


	//-----------Erde------------

	//Erdmaterial
	m_zmEarth.MakeTextureDiffuse("textures/earth_image.jpg");
	m_zmEarth.MakeTextureBump("textures/earth_bump.jpg");
	m_zmEarth.MakeTextureGlow("textures/earth_glow.jpg");
	m_zmEarth.MakeTextureSpecular("textures/earth_reflektion.jpg");

	//Erde Sphere
	m_zgSphere.Init(1.0f, &m_zmEarth,30,30);				// 1.0 = 1m Radius, Material
	//m_zs.AddPlacement(&m_zpSphere);
	m_zpSphere.AddGeo(&m_zgSphere);
	
	//-----------Mond------------

	//Mondmaterial
	m_zmMoon.MakeTextureDiffuse("textures/Moon_image.jpg");
	m_zmMoon.MakeTextureBump("textures/moon_bump.jpg");

	//Mond Sphere
	m_zgMoon.Init(0.5f, &m_zmMoon,30,30);
	//m_zs.AddPlacement(&m_zpMoon);
	m_zpMoon.AddGeo(&m_zgMoon);
	

	//-----------Wolken-----------

	//Wolkenmaterial
	m_zmCloud.MakeTextureDiffuse("textures/clouds_alpha_l1.gif");
	m_zmCloud.SetTransparencyOn();

	//Wolken Sphere
	m_zgCloud.Init(1.1f, &m_zmCloud);			// 1.2 = 1.2m Radius, Material
	m_zpSphere.AddPlacement(&m_zpCloud);
	m_zpCloud.AddGeo(&m_zgCloud);
	m_zpCloud.FixDistance(1.9f);

	//-----------EarthMoonOrbit----------

	m_zpSun.AddPlacement(&m_zpEarthMoonOrbit);
	m_zpEarthMoonOrbit.AddPlacement(&m_zpSphere);
	m_zpEarthMoonOrbit.AddPlacement(&m_zpMoon);

	//-----------Weltall----------

	//Weltallmaterial
	m_zmWeltall.MakeTextureSky("textures/weltall_image.jpg");

	//Weltall Sphere
	m_zgWeltall.Init(1000.0f,&m_zmWeltall);
	m_zs.AddPlacement(&m_zpWeltall);
	m_zpWeltall.AddGeo(&m_zgWeltall);
	m_zpWeltall.SetSky();
	m_zgWeltall.Flip();

	//-----------Raumschiff----------
	m_pzgRocket = m_fileojb.LoadGeoTriangleTable("models/rocket.obj");
	m_zpRocket.AddGeo(m_pzgRocket);
	m_zpSphere.AddPlacement(&m_zpRocket);

	//-----------Todesstern----------

	//Todesstern Material
	m_zmTodesstern.MakeTextureDiffuse("textures/Todesstern_color.jpeg");
	m_zmTodesstern.MakeTextureBump("textures/Todesstern_Bump.jpeg");


	//Todesstern Placement
	m_pzgTodesstern = m_fileojb.LoadGeoTriangleTable("models/deathstar.obj");
	m_pzgTodesstern->SetMaterial(&m_zmTodesstern);
	m_zpTodesstern.AddGeo(m_pzgTodesstern);
	m_zpSun.AddPlacement(&m_zpTodesstern);
	m_zpTodesstern2.AddGeo(m_pzgTodesstern);
	m_zpSun.AddPlacement(&m_zpTodesstern2);

	//Array Version erstmal Raus genommen wegen Bulletkonzept
	//for (int i = 0; i < NR_DEATHSTARS; i++)
	//{
	//	m_zapTodesstern[i].AddGeo(m_pzgTodesstern);
	//	m_zpSun.AddPlacement(&m_zapTodesstern[i]);
	//	int iRandom = rand() % 10;
	//	if (i % 2 == 0)
	//	{
	//		m_avTodessternRandom[i] = CHVector(
	//			iRandom + 20.f,				// x; + 10 für min abstand
	//			iRandom,					// y; 
	//			iRandom,					// z
	//			rand() % 1000 / 1000.0f		// w; Geschwindigkeit der Sterne zwischen 0 & 1
	//		);
	//	}
	//	else
	//	{
	//		m_avTodessternRandom[i] = CHVector(
	//			iRandom + 20.f,				// x; + 10 für min abstand
	//			-iRandom,					// y; 
	//			iRandom,					// z
	//			rand() % 1000 / 1000.0f		// w; Geschwindigkeit der Sterne zwischen 0 & 1
	//		);
	//	}
	//	
	//}


	//-----------Cursor----------

	m_ziPfeil.Init("textures/rocket_mouse.png");
	m_ziHand.Init("textures/rocket_hand.png");
	m_zoPfeil.Init(&m_ziPfeil,
		C2dRect(0.05F, 0.05F), false);
	m_zoHand.Init(&m_ziHand,
		C2dRect(0.05F, 0.05F), false);

	m_zf.AddDeviceCursor(&m_zdc);
	m_zv.AddOverlay(&m_zoPfeil);
	m_zv.AddOverlay(&m_zoHand);
	m_zoPfeil.SetLayer(0.1f);
	m_zoHand.SetLayer(0.1f);
	


	//-----------Pick----------
	m_zgPick.Add(m_pzgTodesstern);
	m_zmTodessternTreffer.MakeTextureDiffuse("textures/Todesstern_color_dead.jpeg");
	m_zr.AddMaterial(&m_zmTodessternTreffer);

	//-----------Menue----------
	m_zv.AddOverlay(&m_zoStartbildschirm);
	m_ziStarbildschirm.Init("textures/menue.png");
	m_zoStartbildschirm.InitFull(&m_ziStarbildschirm);
	m_zoStartbildschirm.SetLayer(1);

	//-----------Einzel----------
	m_zv.AddOverlay(&m_zoEinzel);
	m_ziEinzel.Init("textures/einzel.png");
	m_zoEinzel.Init("textures/einzel.png",C2dRect(0.1f,0.1f,0.8f,0.5f));
	m_zoEinzel.SetLayer(0.2f);

	//-----------Multi----------
	m_zv.AddOverlay(&m_zoMulti);
	m_ziMulti.Init("textures/multi.png");
	m_zoMulti.Init("textures/multi.png", C2dRect(0.1f, 0.1f, 0.1f, 0.5f));
	m_zoMulti.SetLayer(0.2f);

	m_zosOverlayliste.Add(&m_zoEinzel);
	m_zosOverlayliste.Add(&m_zoMulti);

	//-----------Nebel----------
	m_zmNebula.MakeTextureBillboard("textures/nebular.tif");
	m_zmNebula.SetTransparencyOn();
	m_zgNebula.Init(1.0f, &m_zmNebula);
	m_zs.AddPlacement(&m_azpNebula[0]);
	m_azpNebula[0].SetBillboard();
	m_azpNebula[0].TranslateY(10.0f);
	m_azpNebula[0].TranslateXDelta(10.0f);

	for (int i = 1; i < 5; i++)
	{
		m_azpNebula[0].AddPlacement(&m_azpNebula[i]);
		m_azpNebula[i].AddGeo(&m_zgNebula);
		m_azpNebula[i].TranslateZ(i / 5.0f);
	}

	//-----------Asteroiden----------
	for (int i = 0; i < 3; i++)
	{
		char ac[100];
		sprintf(ac, "textures/Astroid%i.png", i);
		m_azmStone[i].MakeTextureBillboard(ac);
		m_azmStone[i].SetTransparencyOn();
		m_azgStone[i].Init(0.5f, &m_azmStone[i]);
	}


	for (int i = 0; i < NR_OF_STONES; i++)
	{
		m_zpSun.AddPlacement(&m_azpStone[i]);
		m_azpStone[i].SetBillboard(eBillboardType_Parallel);
		int iRandHit = rand() % 3;
		m_azpStone[i].AddGeo(&m_azgStone[iRandHit]);

		//T0bis Funktion für bessere verteilung
		float fRadius;
		float fHeight;
		float fa = rand() % 10000 / 10000.0f * TWOPI;
		if (rand() % 2)
			fRadius = 0.6f + 1.0f / (rand() % 100 + 1);
		else
			fRadius = 0.6f - 1.0f / (rand() % 100 + 1);

		if (rand() % 2)
			fHeight = +1.0f / (rand() % 100 + 1);
		else
			fHeight = -1.0f / (rand() % 100 + 1);
		fRadius *= 20.0f;

		m_avStoneRandom[i] = CHVector(
			sinf(fa) * fRadius,
			fHeight,
			cosf(fa) * fRadius,
			0.1f / fRadius);

		m_azpStone[i].SetBillboardAngle((rand() % 3600) / 1000.0f);
		m_azpStone[i].SetBillboardScaling(0.2f + (rand() % 1000) / 1000.0f);
	}

	//-----------Cockpit----------
	m_pzgCockpit = m_fileojb.LoadGeoTriangleTable("models/cockpit.obj",true);
	m_zmCockpit.MakeTextureDiffuse("textures/cockpit.png");
	m_zpCockpit.AddGeo(m_pzgCockpit);
	m_zp.AddPlacement(&m_zpCockpit);
	m_pzgCockpit->SetMaterial(&m_zmCockpit);

	//Platzierung
	m_zpCockpit.RotateY(UM_DEG2RAD(-90));
	m_zpCockpit.TranslateZDelta(-4.4f);
	m_zpCockpit.TranslateYDelta(-0.57f);


	//-----------Kugeln----------
	m_zmBullet.LoadPreset("AshesGlowing");
	m_pzgBullet = m_fileojb.LoadGeoTriangleTable("models/Projektil.obj", true);
	m_pzgBullet->SetMaterial(&m_zmBullet);
	m_zpBulletTemplate.AddGeo(m_pzgBullet);
	m_zpBulletTemplate.SetPhysics(2.0f, 0.1f,
		0.0f, 200000.1f, true);
	m_zpBulletTemplate.SwitchOff();
	m_zs.AddPlacement(&m_zpBulletTemplate);
	// Initialisiere den Schalt-Ringpuffer:
	m_zpsBullets.RingMake(20, m_zpBulletTemplate);
	m_zs.AddPlacements(m_zpsBullets);
	m_zs.SetPhysicsSpace();

	//-----------Kugeln----------
	m_pzgBulletEnemy = m_fileojb.LoadGeoTriangleTable("models/EnemyB.obj", true);
	m_zpBulletTemplateEnemy.AddGeo(m_pzgBulletEnemy);
	m_zmBulletEnemy.MakeTextureDiffuse("textures/EnemyB.png");
	m_pzgBulletEnemy->SetMaterial(&m_zmBulletEnemy);
	m_zpBulletTemplateEnemy.SetPhysics(2.0f, 0.1f,
		0.0f, 200000.1f, true);
	m_zpBulletTemplateEnemy.SwitchOff();
	m_zpTodesstern.AddPlacement(&m_zpBulletTemplateEnemy);
	// Initialisiere den Schalt-Ringpuffer:
	m_zpsBulletsEnemy.RingMake(5, m_zpBulletTemplateEnemy);
	m_zs.AddPlacements(m_zpsBulletsEnemy);
	m_zs.AddPlacement(&m_zpBulletTemplateEnemy);

	//Splittscreen
	//m_rvLeft.Init(&m_zcLeft, 0.0f, 0.0f, 0.5f, 1.0f);
	//m_rvRight.Init(&m_zcRight, 0.5f, 0.0f, 0.5f, 1.0f);

	m_rvLeft.InitHalfLeft(&m_zcLeft);
	m_rvRight.InitHalfRight(&m_zcRight);


	m_rvLeft.SwitchOff();
	m_rvRight.SwitchOff();

	//Viewport Multi an Frame
	m_zf.AddViewport(&m_rvLeft);
	m_zf.AddViewport(&m_rvRight);

	m_zs.AddPlacement(&m_zpLeft);					// Placement an Szene hängen Mutliplayer
	m_zs.AddPlacement(&m_zpRight);					// Placement an Szene hängen Mutliplayer

	//CameraMulti
	m_zpLeft.AddCamera(&m_zcLeft);					// Kamera an Placement hängen Multiplayer
	m_zpRight.AddCamera(&m_zcRight);				// Kamera an Placement hängen Multiplayer

	m_zpLeft.TranslateZ(250);				   		//Translate Camera
	m_zpRight.TranslateZ(250);				   		//Translate Camera

	m_zcLeft.Init(0.3f);
	m_zcRight.Init(0.3f);

	//Splitscreen Cockpit nötig wegen Translation da größe des Sichtfelds anders
	m_pzgCockpitLeft = m_fileojb.LoadGeoTriangleTable("models/cockpit.obj", true);
	m_zmCockpitLeft.MakeTextureDiffuse("textures/cockpit.png");
	m_zpCockpitLeft.AddGeo(m_pzgCockpitLeft);
	m_zpLeft.AddPlacement(&m_zpCockpitLeft);
	m_pzgCockpitLeft->SetMaterial(&m_zmCockpitLeft);

	m_pzgCockpitRight = m_fileojb.LoadGeoTriangleTable("models/cockpit.obj", true);
	m_zmCockpitRight.MakeTextureDiffuse("textures/cockpit.png");
	m_zpCockpitRight.AddGeo(m_pzgCockpitRight);
	m_zpRight.AddPlacement(&m_zpCockpitRight);
	m_pzgCockpitRight->SetMaterial(&m_zmCockpitRight);

	//Platzierung
	m_zpCockpitLeft.RotateY(UM_DEG2RAD(-90));
	m_zpCockpitLeft.TranslateZDelta(-2.4f);
	m_zpCockpitLeft.TranslateYDelta(-0.57f);

	m_zpCockpitRight.RotateY(UM_DEG2RAD(-90));
	m_zpCockpitRight.TranslateZDelta(-2.4f);
	m_zpCockpitRight.TranslateYDelta(-0.57f);

	//Sound
	for (int i = 0; i < 20; i++)
	{
		m_zs.AddAudio(&m_zaaBulletSound[i]);
		m_zaaBulletSound[i].Init("sounds/Gun.wav");
	}

	
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------
	
	//Startscreen

	//Cursor
	float fx, fy;
	m_zdc.GetFractional(fx, fy, true);
	if (m_zdc.ButtonPressedLeft())
	{
		m_zoPfeil.SwitchOff(); m_zoHand.SwitchOn();
		m_zoHand.SetPos(fx - 0.01F, fy - 0.01F);
	}
	else
	{
		m_zoPfeil.SwitchOn(); m_zoHand.SwitchOff();
		m_zoPfeil.SetPos(fx - 0.01F, fy - 0.01F);
	}

	//Button Klick Abfrage
	if (m_zdc.ButtonPressedLeft())
	{
		COverlay* pzo = m_zdc.PickOverlayPreselected(m_zosOverlayliste);
		if (pzo == &m_zoEinzel)
		{
			m_zoStartbildschirm.SwitchOff();
			m_zoEinzel.SwitchOff();
			m_zoMulti.SwitchOff();
			m_bMulti = false;
			m_bOverlay = false;
		}
		if (pzo == &m_zoMulti)
		{
			m_zoStartbildschirm.SwitchOff();
			m_zoEinzel.SwitchOff();
			m_zoMulti.SwitchOff();
			m_bMulti = true;
			m_bOverlay = false;
		}
	}


	//Gameplay 
	if (m_bOverlay == false && m_bMulti == false)
	{
		m_zpLeft.SwitchOff();
		m_zpRight.SwitchOff();
		m_zpTodesstern2.SwitchOff();

		//Controlls
		m_zdk.PlaceWASD(m_zp, fTimeDelta, false);

		//Sonne
		m_zpSun.RotateY(fTime / 2);

		//ErdMondOrbit
		m_zpEarthMoonOrbit.TranslateX(30.0f);

		//Erddrehung
		m_zpSphere.RotateY(fTime / 2);
		m_zpSphere.RotateXDelta(0.24f);

		//Monddrehung
		m_zpMoon.TranslateX(4.0f);
		m_zpMoon.RotateYDelta(fTime / 2);

		//Raumschiffsteurung
		m_zpRocket.TranslateX(-2.0f);
		m_zpRocket.RotateZDelta(fTime);

		//Todesstern Placement
		m_zpTodesstern.TranslateX(15.0f);
		m_zpTodesstern.RotateZDelta(fTime / 4);

		//Array Version erstmal Raus genommen wegen Bulletkonzept
		//for (int i = 0; i < NR_DEATHSTARS; i++)
		//{
		//	m_zapTodesstern[i].Translate(m_avTodessternRandom[i]);
		//	m_zapTodesstern[i].RotateYDelta(m_avTodessternRandom[i].w * fTime/2);	// damit sich Stein um Achse der Erde dreht; w ist die Geschwindigkeit (siehe oben Initialisierung)
		//}

		//Todesstern Anklicken
		if (m_zdc.ButtonDownLeft())
		{
			CGeo* gPickedGeo = m_zdc.PickGeoPreselected(m_zgPick);

			if (gPickedGeo != nullptr && m_zdc.ButtonPressedLeft()) {
				gPickedGeo->SetMaterial(&m_zmTodessternTreffer);
			}
		}

		//Schimmel im Weltall
		for (int i = 0; i < 5; i++)
		{
			m_azpNebula[i].SetBillboardAngle(fTime * i * 0.05f);
			m_azpNebula[i].SetBillboardScaling((1.5f + sinf(fTime * i * 0.7), (1.5f + sinf(fTime * i * 0.7), (1.5f + sinf(fTime * i * 0.7)))));
		}

		//Asteroiden
		for (int i = 0; i < NR_OF_STONES; i++)
		{
			m_azpStone[i].Translate(m_avStoneRandom[i]);
			m_azpStone[i].RotateYDelta(m_avStoneRandom[i].w * fTime / 2);
			m_azpStone[i].RotateZDelta(UM_DEG2RAD(0));
		}

		//Kugel Ticks
		if (m_zdk.KeyDown(DIK_SPACE))
		{
			if (m_zpsBullets.RingIsNotFull()) {
				CPlacement* pzp = m_zpsBullets.RingInc();
				// Kopiere die Matrix (und damit die Startposition)
				pzp->SetMat(m_zp.GetMatGlobal());
				pzp->TranslateDelta(0.0f, -0.5f, -1.0f);
				// Und schieß die Kugel frontal
				CHVector v;
				v = m_zp.GetDirectionGlobal();
				v.Normal();
				pzp->SetPhysicsVelocity(v * 800.0f);

			}
		}


		//Löschen von Kugeln auf Distanz
		CPlacement* pzpOldestBullet = m_zpsBullets.RingAskLast();
		if (pzpOldestBullet)
			if (pzpOldestBullet->GetPos().Length() > 250.0f)
				m_zpsBullets.RingDec();

		//Kugel Ticks Enemy
		Timer++;

		if (Timer % 1000 == 0) {
			if (m_zpsBulletsEnemy.RingIsNotFull()) {
				CPlacement* pzpE = m_zpsBulletsEnemy.RingInc();
				// Kopiere die Matrix (und damit die Startposition)
				pzpE->SetMat(m_zpTodesstern.GetMatGlobal());
				// Und schieß die Kugel auf Spieler
				CHVector vE;
				vE = m_zp.GetPosGlobal() - m_zpTodesstern.GetPosGlobal();
				vE.Normal();
				pzpE->SetPhysicsVelocity(vE * 25.0f);
			}
		}

		if (Timer == 100000)
			Timer = 0;

		//Löschen von Kugeln auf Distanz
		CPlacement* pzpOldestBulletEnemy = m_zpsBulletsEnemy.RingAskLast();
		if (pzpOldestBulletEnemy)
			if (pzpOldestBulletEnemy->GetPos().Length() > 300.0f)
				m_zpsBulletsEnemy.RingDec();



	}
	else if (m_bOverlay == false && m_bMulti == true)
	{
		m_zv.SwitchOff();
		m_rvLeft.SwitchOn();
		m_rvRight.SwitchOn();
		m_zp.SwitchOff();

		//Controlls
		m_zdk.PlaceWASD(m_zpLeft, fTimeDelta, false);
		m_zdk.PlaceWASD(m_zpRight, fTimeDelta, false);

		//Sonne
		m_zpSun.RotateY(fTime / 2);

		//ErdMondOrbit
		m_zpEarthMoonOrbit.TranslateX(30.0f);

		//Erddrehung
		m_zpSphere.RotateY(fTime / 2);
		m_zpSphere.RotateXDelta(0.24f);

		//Monddrehung
		m_zpMoon.TranslateX(4.0f);
		m_zpMoon.RotateYDelta(fTime / 2);

		//Raumschiffsteurung
		m_zpRocket.TranslateX(-2.0f);
		m_zpRocket.RotateZDelta(fTime);

		//Todesstern Placement
		m_zpTodesstern.TranslateX(15.0f);
		m_zpTodesstern.RotateZDelta(fTime / 4);
		m_zpTodesstern2.TranslateX(-15.0f);
		m_zpTodesstern2.RotateZDelta(fTime / 4);


		//Array Version erstmal Raus genommen wegen Bulletkonzept
		//for (int i = 0; i < NR_DEATHSTARS; i++)
		//{
		//	m_zapTodesstern[i].Translate(m_avTodessternRandom[i]);
		//	m_zapTodesstern[i].RotateYDelta(m_avTodessternRandom[i].w * fTime/2);	// damit sich Stein um Achse der Erde dreht; w ist die Geschwindigkeit (siehe oben Initialisierung)
		//}

		//Todesstern Anklicken
		if (m_zdc.ButtonDownLeft())
		{
			CGeo* gPickedGeo = m_zdc.PickGeoPreselected(m_zgPick);

			if (gPickedGeo != nullptr && m_zdc.ButtonPressedLeft()) {
				gPickedGeo->SetMaterial(&m_zmTodessternTreffer);
			}
		}

		//Schimmel im Weltall
		for (int i = 0; i < 5; i++)
		{
			m_azpNebula[i].SetBillboardAngle(fTime * i * 0.05f);
			m_azpNebula[i].SetBillboardScaling((1.5f + sinf(fTime * i * 0.7), (1.5f + sinf(fTime * i * 0.7), (1.5f + sinf(fTime * i * 0.7)))));
		}

		//Asteroiden
		for (int i = 0; i < NR_OF_STONES; i++)
		{
			m_azpStone[i].Translate(m_avStoneRandom[i]);
			m_azpStone[i].RotateYDelta(m_avStoneRandom[i].w * fTime / 2);
			m_azpStone[i].RotateZDelta(UM_DEG2RAD(0));
		}

		//Kugel Ticks
		if (m_zdk.KeyDown(DIK_SPACE))
		{
			if (m_zpsBullets.RingIsNotFull()) {
				CPlacement* pzpLeft = m_zpsBullets.RingInc();
				// Kopiere die Matrix (und damit die Startposition)
				pzpLeft->SetMat(m_zpLeft.GetMatGlobal());
				pzpLeft->TranslateDelta(0.0f, -0.5f, -1.0f);
				// Und schieß die Kugel frontal
				CHVector vLeft;
				vLeft = m_zpLeft.GetDirectionGlobal();
				vLeft.Normal();
				pzpLeft->SetPhysicsVelocity(vLeft * 800.0f);
			}
		}
		if (m_zdk.KeyDown(DIK_SPACE))
		{
			if (m_zpsBullets.RingIsNotFull()) {
				CPlacement* pzpRight = m_zpsBullets.RingInc();
				// Kopiere die Matrix (und damit die Startposition)
				pzpRight->SetMat(m_zpRight.GetMatGlobal());
				pzpRight->TranslateDelta(0.0f, -0.5f, -1.0f);
				// Und schieß die Kugel frontal
				CHVector vRight;
				vRight = m_zpRight.GetDirectionGlobal();
				vRight.Normal();
				pzpRight->SetPhysicsVelocity(vRight * 800.0f);
			}
		}


		//Löschen von Kugeln auf Distanz
		CPlacement* pzpOldestBullet = m_zpsBullets.RingAskLast();
		if (pzpOldestBullet)
			if (pzpOldestBullet->GetPos().Length() > 250.0f)
				m_zpsBullets.RingDec();

		//Kugel Ticks Enemy
		Timer++;

		if (Timer % 1000 == 0) {
			if (m_zpsBulletsEnemy.RingIsNotFull()) {
				CPlacement* pzpELeft = m_zpsBulletsEnemy.RingInc();
				// Kopiere die Matrix (und damit die Startposition)
				pzpELeft->SetMat(m_zpTodesstern.GetMatGlobal());
				// Und schieß die Kugel auf Spieler
				CHVector vELeft;
				vELeft = m_zpLeft.GetPosGlobal() - m_zpTodesstern.GetPosGlobal();
				vELeft.Normal();
				pzpELeft->SetPhysicsVelocity(vELeft * 25.0f);
			}
		}

		if (Timer % 1000 == 0) {
			if (m_zpsBulletsEnemy.RingIsNotFull()) {
				CPlacement* pzpERight = m_zpsBulletsEnemy.RingInc();
				// Kopiere die Matrix (und damit die Startposition)
				pzpERight->SetMat(m_zpTodesstern2.GetMatGlobal());
				// Und schieß die Kugel auf Spieler
				CHVector vERight;
				vERight = m_zpLeft.GetPosGlobal() - m_zpTodesstern2.GetPosGlobal();
				vERight.Normal();
				pzpERight->SetPhysicsVelocity(vERight * 25.0f);
			}
		}

		if (Timer == 100000)
			Timer = 0;

		//Löschen von Kugeln auf Distanz
		CPlacement* pzpOldestBulletEnemy = m_zpsBulletsEnemy.RingAskLast();
		if (pzpOldestBulletEnemy)
			if (pzpOldestBulletEnemy->GetPos().Length() > 300.0f)
				m_zpsBulletsEnemy.RingDec();



	}
		
	// Traversiere am Schluss den Szenengraf und rendere:
	m_zr.Tick(fTimeDelta);
}

void CGame::Fini()
{
	//-------------------------------
	// Finalisiere die Knotenobjekte:
	//-------------------------------

	// Hier die Finalisierung Deiner Vektoria-Objekte einfügen:

	// Auch die Root sollte finalisiert werden:   
	m_zr.Fini();
}

void CGame::WindowReSize(int iNewWidth, int iNewHeight)
{
	// Windows ReSize wird immer automatisch aufgerufen, wenn die Fenstergröße verändert wurde.
	// Hier kannst Du dann die Auflösung des Viewports neu einstellen:
	// z.B. mittels: m_zf.ReSize(iNewWidth, iNewHeight);
	m_zf.ReSize(iNewWidth, iNewHeight);
}

float CGame::GetTimeDeltaMin()
{
	return m_zr.GetTimeDeltaMin();
}

float CGame::GetVersion()
{
	return m_zr.GetVersion();
}


