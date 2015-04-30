#include "WindowMgr.h"

extern double calculateDeltaT();

WindowMgr::WindowMgr()
{
	active = TRUE;
	fullscreen = FALSE;
}


WindowMgr::~WindowMgr()
{
}



//MAIN LOOP del gioco
void WindowMgr::gameLoop(MSG &msg, double &deltaT) {
	BOOL done = false;

	//GAME LOOP
	while (!done)													// Loop That Runs While done=FALSE
	{



		//tempo passato a causa del disegno del frame
		deltaT = calculateDeltaT();


		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))					// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)								// Have We Received A Quit Message?
			{
				done = TRUE;											// If So done=TRUE
			}
			else													// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);								// Translate The Message
				DispatchMessage(&msg);								// Dispatch The Message
			}
		}
		else														// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])		// Active?  Was There A Quit Received?
			{
				done = TRUE;											// ESC or DrawGLScene Signalled A Quit
			}
			else													// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);									// Swap Buffers (Double Buffering)
			}




			//rotazione telecamera
			if (keys[VK_LEFT]) {
				camera.rotateLeft(deltaT);
			}
			if (keys[VK_RIGHT]) {
				camera.rotateRight(deltaT);
			}

			//movimento avanti/indietro
			if (keys[VK_UP] || keys[0x57]) { //W
				camera.moveForward(deltaT);
			}
			if (keys[VK_DOWN] || keys[0x53]) { //S
				camera.moveBackward(deltaT);
			}

			//altezza telecamera
			if (keys[0x51]) { //Q
				camera.moveUp(deltaT);
			}

			if (keys[0x45]) { //E
				camera.moveDown(deltaT);
			}

			//strafe laterale
			if (keys[0x41]) { //A
				camera.strafeLeft(deltaT);
			}

			if (keys[0x44]) { //D
				camera.strafeRight(deltaT);
			}

			//altri comandi
			if (keys[0x52]) { //R -> attiva/disattiva wireframe
				levelsMgr->get()->wireframeOnly = !levelsMgr->get()->wireframeOnly;
			}

			if (keys[0x42]) { //B - disegna o meno le Bounding Box oggetti collidibili
				levelsMgr->get()->drawBoundingBoxes = !levelsMgr->get()->drawBoundingBoxes;
			}

			if (keys[0x43]) { //C - attiva/disattiva collisioni camera(player) - mondo
				camera.collisionsEnabled = !camera.collisionsEnabled;
			}

			if (keys[0x4E]) { //N - carica prox. livello
				if (levelsMgr->getActiveLevelNum() == 1 && levelsMgr->get()->isLevelLoaded()) {
					levelsMgr->loadLevel(levelsMgr->LEVEL_2);
				}
			}


			//riposiz. il puntatore al centro della finestra
			RECT r;
			GetWindowRect(hWnd, &r);
			camera.resetCursorPos(r, p, SCREEN_W, SCREEN_H);

		}//else
	} //while
}




//gestisce i messaggi passati alla finestra
LRESULT  WindowMgr::WndProc(HWND	hWnd,							// Handle For This Window
	UINT	uMsg,							// Message For This Window
	WPARAM	wParam,							// Additional Message Information
	LPARAM	lParam)							// Additional Message Information
{
	switch (uMsg)													// Check For Windows Messages
	{
	case WM_ACTIVATE:											// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))									// Check Minimization State
		{
			active = TRUE;										// Program Is Active
		}
		else
		{
			active = FALSE;										// Program Is No Longer Active
		}

		return 0;												// Return To The Message Loop
	}

	case WM_SYSCOMMAND:											// Intercept System Commands
	{
		switch (wParam)											// Check System Calls
		{
		case SC_SCREENSAVE:									// Screensaver Trying To Start?
		case SC_MONITORPOWER:								// Monitor Trying To Enter Powersave?
			return 0;											// Prevent From Happening
		}
		break;													// Exit
	}

	case WM_CLOSE:												// Did We Receive A Close Message?
	{
		PostQuitMessage(0);										// Send A Quit Message
		return 0;												// Jump Back
	}

	case WM_KEYDOWN:											// Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;									// If So, Mark It As TRUE

		//tiene premuto left shift -> corsa
		if (wParam == VK_SHIFT)
			camera.run();




		return 0;												// Jump Back
	}

	case WM_KEYUP:												// Has A Key Been Released?
	{
		keys[wParam] = FALSE;									// If So, Mark It As FALSE

		//left shift rilasciato -> walk
		if (wParam == VK_SHIFT)
			camera.walk();

		return 0;												// Jump Back
	}

	case WM_SIZE:												// Resize The OpenGL Window
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));			// LoWord=Width, HiWord=Height
		return 0;												// Jump Back
	}

	case WM_MOUSEMOVE:
	{
		p = MAKEPOINTS(lParam);

		//calcolo di quanto si � spostato il mouse in un frame
		camera.updateMouseDeltaPos(p.x, p.y);

		return 0;
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}






//chiamata quando ridim. finestra
GLvoid WindowMgr::ReSizeGLScene(GLsizei width, GLsizei height)					// Resize And Initialize The GL Window
{
	if (height == 0)													// Prevent A Divide By Zero By
	{
		height = 1;													// Making Height Equal One
	}

	glViewport(0, 0, width, height);									// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);									// Select The Projection Matrix
	glLoadIdentity();												// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	//i clipping plane sono in 1 e 500 -> view depth di 500
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 2000.0f);	// View Depth of 500

	glMatrixMode(GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity();												// Reset The Modelview Matrix
}







//inizializza opengl
int WindowMgr::InitGL(GLvoid)													// All Setup For OpenGL Goes Here
{
	levelsMgr->get()->reloadTextures();							//ricarica le texture di tutti i modelli


	glEnable(GL_TEXTURE_2D);										// Enable Texture Mapping ( NEW )
	glShadeModel(GL_FLAT);										// Enable flat(low poly style) Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);							// Black Background
	glClearDepth(1.0f);												// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);										// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);											// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Really Nice Perspective Calculations


	//illuminazione
	glEnable(GL_LIGHTING);

	GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 30.0, 0.0f, 1.0f };



	//una luce
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glEnable( GL_LIGHT0 );

	glEnable(GL_NORMALIZE);								//prima di applicare luci normalizza i vettori se necessario


	//nebbia
	if (levelsMgr->get()->fogEnabled) {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogfv(GL_FOG_COLOR, levelsMgr->get()->fogColor);
		glFogf(GL_FOG_DENSITY, levelsMgr->get()->fogDensity);
		glHint(GL_FOG_HINT, GL_NICEST);
	}



	//direzione iniziale verso la quale � rivolta la telecamera
	camera.lx = cos(camera.angley);
	camera.lz = -sin(camera.angley);

	//posiziona la telecamera
	gluLookAt(camera.xpos, camera.ypos, camera.zpos, 0, camera.angley, camera.anglex, 0, 1, 0);


	//glEnable(GL_CULL_FACE);		//se lo metto non disegna le penne dell aku da dietro


	return TRUE;													// Initialization Went OK
}






int WindowMgr::DrawGLScene(GLvoid)												// Here's Where We Do All The Drawing
{

	glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
	glLoadIdentity();



	//telecamera, xpos + lx; zpos+lz
	gluLookAt(camera.xpos, camera.ypos, camera.zpos, camera.xpos + camera.lx, camera.ypos + camera.ly, camera.zpos + camera.lz, 0.0f, 1.0f, 0.0f);


	//disegno filled/wireframe
	if (levelsMgr->get()->wireframeOnly) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//disegno skybox + mappa + gemme + maschere
	levelsMgr->get()->drawLevel(deltaT);


	glEnable(GL_DEPTH_TEST); //enable the depth testing
	glEnable(GL_LIGHTING); //enable the lighting
	glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
	glShadeModel(GL_FLAT); //set the shader to flat(per low poly effect) shader




	//SCRITTE A SCHERMO - overlyay, gui, ...
	//scritte a schermo(OVERLAY e GUI varie)
	//barra vita

	//calcolo fps ogni 100 frame
	if (frameCounter >= 10) {
		fps = "FPS = " + to_string(int(frameCounter / cumulativeDeltaT));

		frameCounter = 0;
		cumulativeDeltaT = 0.0f;
	}
	else {
		frameCounter++;
		cumulativeDeltaT += deltaT;
	}

	//scrivo la posiz. (x,y,z) della telecamera(player)
	string camPos = "POS = " + to_string(camera.xpos) + ", " + to_string(camera.ypos) + ", " + to_string(camera.zpos);
	string points = "Punteggio: " + to_string(pStats.getPoints());


	if( showFPS ) txt->drawText(fps, 10, 20, SCREEN_W, SCREEN_H);					//FPS
	if( showCameraPosition ) txt->drawText(camPos, 10, 35, SCREEN_W, SCREEN_H);		//(x,y,z) di Camera
	txt->drawText(points, SCREEN_W - 140, 75, SCREEN_W, SCREEN_H);					//punteggio


	//aggiorna punteggio
	pStats.updateLifeAmount(deltaT);	//aggiorna qnt� vita rimasta
	pStats.drawLifeBar();				//disegna barra vita
	pStats.drawPickedMasks();			//disegna # maschere raccolte

	return TRUE;
}







GLvoid WindowMgr::KillGLWindow(GLvoid)											// Properly Kill The Window
{
	if (fullscreen)													// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);								// If So Switch Back To The Desktop
		ShowCursor(TRUE);											// Show Mouse Pointer
	}

	if (hRC)														// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))								// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))									// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;													// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))								// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;													// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))								// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;													// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))						// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;												// Set hInstance To NULL
	}
}








/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL WindowMgr::CreateGLWindow(char* title)
{
	GLuint		PixelFormat;										// Holds The Results After Searching For A Match
	WNDCLASS	wc;													// Windows Class Structure
	DWORD		dwExStyle;											// Window Extended Style
	DWORD		dwStyle;											// Window Style
	RECT		WindowRect;											// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;										// Set Left Value To 0
	WindowRect.right = (long)SCREEN_W;									// Set Right Value To Requested Width
	WindowRect.top = (long)0;											// Set Top Value To 0
	WindowRect.bottom = (long)SCREEN_H;									// Set Bottom Value To Requested Height

	//fullscreen = this->fullscreen										// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);					// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;						// WndProc Handles Messages
	wc.cbClsExtra = 0;										// No Extra Window Data
	wc.cbWndExtra = 0;										// No Extra Window Data
	wc.hInstance = hInstance;								// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);				// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				// Load The Arrow Pointer
	wc.hbrBackground = NULL;										// No Background Required For GL
	wc.lpszMenuName = NULL;										// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";									// Set The Class Name

	if (!RegisterClass(&wc))										// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if (fullscreen)													// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;									// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = SCREEN_W;					// Selected Screen Width
		dmScreenSettings.dmPelsHeight = SCREEN_H;					// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = SCREEN_BIT;						// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;									// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;										// Return FALSE
			}
		}
	}

	if (fullscreen)													// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;									// Window Extended Style
		dwStyle = WS_POPUP;											// Windows Style
		ShowCursor(FALSE);											// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;				// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;								// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =								// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),								// Size Of This Pixel Format Descriptor
		1,															// Version Number
		PFD_DRAW_TO_WINDOW |										// Format Must Support Window
		PFD_SUPPORT_OPENGL |										// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,											// Must Support Double Buffering
		PFD_TYPE_RGBA,												// Request An RGBA Format
		SCREEN_BIT,														// Select Our Color Depth
		0, 0, 0, 0, 0, 0,											// Color Bits Ignored
		0,															// No Alpha Buffer
		0,															// Shift Bit Ignored
		0,															// No Accumulation Buffer
		0, 0, 0, 0,													// Accumulation Bits Ignored
		16,															// 16Bit Z-Buffer (Depth Buffer)  
		0,															// No Stencil Buffer
		0,															// No Auxiliary Buffer
		PFD_MAIN_PLANE,												// Main Drawing Layer
		0,															// Reserved
		0, 0, 0														// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))											// Did We Get A Device Context?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))					// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))						// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))								// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))									// Try To Activate The Rendering Context
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);										// Show The Window
	SetForegroundWindow(hWnd);										// Slightly Higher Priority
	SetFocus(hWnd);													// Sets Keyboard Focus To The Window
	ReSizeGLScene(SCREEN_W, SCREEN_H);									// Set Up Our Perspective GL Screen

	if (!InitGL())													// Initialize Our Newly Created GL Window
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	return TRUE;													// Success
}