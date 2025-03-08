//PROYECTO DE PROGRAMACION AVANZADA
//JORGE PABLO FLORES BLANCO 
//MATRICULA 1923061
//GRUPO 001

#include<Windows.h>
#include<iostream>
#include<fstream>
#include"resource.h"
#include<CommDlg.h>
#include<string.h>

using namespace std;

struct usuarios {
	char nomComp[52];
	char nomUsu[22];
	char pswrd[12];
	int admin = 0;	//  1 admin | 2 no admin
	char sucur[52];
	char foto[300];
	usuarios* sig;
};

struct sucursal {

	char nomSuc[52];
	char direc[50];
	sucursal* sig;
};

struct clientes {

	clientes* ant;
	char nomComp[52];
	char phone[12];
	char email[50];
	char sucur[52];
	int puntos = 0; //0 por defecto
	int montototal = 0;
	clientes* sig;

};

struct promocion {

	promocion* ant;
	char nomProm[52];
	int puntReq = 0;
	int porcent = 0;
	int status = 0; // 1 activa | 2 inactiva
	int vecesCanjeada = 0;
	promocion* sig;
};

//punteros y funciones USUARIOS
usuarios* inicioUs, * auxUs, * Sesion = nullptr;
void registarUsuario(usuarios* nuevo);
void leerBinUsuario();
void escribirBinUsuario();
void eliminarUsuario(char usuarioEliminar[50], HWND hwnd, int indice);
void modificarUsuario(usuarios* UsuMod, HWND hwnd, char buscaUsu[50]);


//puntero y funciones CLIENTES
clientes* inicioCl, * auxCl, * finalCl = nullptr;
void registrarCliente(clientes* nuevo);
void leerBinCliente();
void escribirBinCliente();
void eliminarCliente(char clienteEliminar[50], HWND hwnd, int indice);
void modificarCliente(clientes* CliMod, HWND hwnd, char buscaCli[50]);

//punteros y funciones SUCURSALES
sucursal* inicioSu, * auxSu = nullptr;
void registrarSucur(sucursal* nuevo);
void leerBinSucur();
void escribirBinSucur();
void eliminarSucursal(char sucursalEliminar[50], HWND hwnd, int indice);
void modificarSucursal(sucursal* SucMod, HWND hwnd, char buscarUsu[50]);

//punteros y funciones PROMOCIONES
promocion* inicioPr, * auxPr, * finalPr = nullptr;
void registrarPromo(promocion* nuevo);
void leerBinPromo();
void escribirBinPromo();
void eliminarPromocion(char promoEliminar[50], HWND hwnd, int indice);
void modificarPromocion(promocion* PromMod, HWND hwnd, char buscaProm[50]);

//reportes
void ReportePuntosCSV();
void ReporteMontosCSV();
void ReporteCanjesCSV();

//ordenamientos
void OrdenarPuntos();
void OrdenarMontos();
void OrdenarCanjes();

//procedimientos
BOOL CALLBACK InicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK MenuPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK procRegUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK procListUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK procRegSuc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK procListSuc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK procRegCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK procListCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK procRegProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK procListProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK procCapturaPuntos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK procCanjePuntos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ReportePuntos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReporteMontos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReportePromociones(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//menu
void menu(WPARAM wParam, HWND hwnd);

//validaciones
bool validarNomCom(char nombre[52]);
bool validarContra(char password[12]);
bool validarUsuario(char usuario[22]);
bool validarTelefono(char telefono[12]);
bool validarCorreo(char correo[50]);
bool validarDireccion(char direccion[52]);

//limitar cajero
void LimitarCajero(HWND hwnd);

//variables globales
char zFile[MAX_PATH];
HINSTANCE hInstGlobal;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {

	leerBinUsuario();
	leerBinSucur();
	leerBinCliente();
	leerBinPromo();

	HWND Programa;
	hInstGlobal = hInst;
	Programa = CreateDialog(hInst, MAKEINTRESOURCE(INICIO_SESION), NULL, InicioSesion);

	ShowWindow(Programa, SW_SHOW);

	MSG mensajes;
	ZeroMemory(&mensajes, sizeof(mensajes));

	while (GetMessage(&mensajes, 0, 0, 0)) {
		TranslateMessage(&mensajes);
		DispatchMessage(&mensajes);
	}

	escribirBinUsuario();
	escribirBinSucur();
	escribirBinCliente();
	escribirBinPromo();

	return mensajes.wParam;
}

//procedimientos
BOOL CALLBACK InicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {
		auxSu = inicioSu;
		while (auxSu != nullptr) {
			SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxSu->nomSuc);
			auxSu = auxSu->sig;
		}

	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDOK: { //IniciarSesion

			int indice = 0;
			char usuario[22] = { 0 };
			char contrasena[12] = { 0 };
			char sucursal[52] = { 0 };

			GetDlgItemText(hwnd, IDC_EDIT1, usuario, 22);
			GetDlgItemText(hwnd, IDC_EDIT2, contrasena, 10);
			GetDlgItemText(hwnd, IDC_COMBO1, sucursal, 52);
			indice = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);

			if (inicioUs == nullptr) {

				MessageBox(hwnd, "No hay usuarios registrados. Registrese.", "AVISO", MB_OK | MB_ICONERROR);
				ShowWindow(GetDlgItem(hwnd, IDC_BUTTON1), SW_SHOW);

			}
			else {

				auxUs = inicioUs;

				while (auxUs != nullptr && strcmp(usuario, auxUs->nomUsu) != 0) {
					auxUs = auxUs->sig;
				}

				if (auxUs == nullptr) {
					MessageBox(hwnd, "El nombre de usuario es incorrecto o no esta registrado.", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (strcmp(contrasena, auxUs->pswrd) == 0) {

						if (strcmp(sucursal, auxUs->sucur) == 0 && indice >= 0) {
							EndDialog(hwnd, 0);
							Sesion = auxUs;
							DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);
						}
						else {
							if (strcmp(sucursal, auxUs->sucur) != 0) { //no selecciono la sucursal correcta
								MessageBox(hwnd, "Seleccione la sucursal registrada para este usuario.", "AVISO", MB_OK | MB_ICONERROR);
							}

						}

					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta, intentelo nuevamente", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}
		}break;

		case IDC_BUTTON1: { //registrar
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(USU_REGISTRO), hwnd, procRegUsu);
		}break;
		}
	}break;
	}
	return 0;

}

BOOL CALLBACK MenuPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_INITDIALOG: {

		if (Sesion->admin == 2) {

			LimitarCajero(hwnd);
		}

		SetDlgItemText(hwnd, IDC_EDIT1, Sesion->nomComp); //mostramos nombre

		if (Sesion->admin == 1) { //muestra el tipo de usuario
			SetDlgItemText(hwnd, IDC_EDIT2, "Administrador");
		}
		else {
			SetDlgItemText(hwnd, IDC_EDIT2, "Cajero");
		}

		SetDlgItemText(hwnd, IDC_EDIT3, Sesion->sucur); //muestra la sucursal

		//carga la foto de perfil
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, Sesion->foto, IMAGE_BITMAP, 190, 170, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 

		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));

		GetLocalTime(&fechaHoy);

		char fechaComHoy[11] = { 0 };
		char varchar[5] = { 0 };

		_itoa_s(fechaHoy.wDay, varchar, 10);
		strcat_s(fechaComHoy, varchar);

		strcat_s(fechaComHoy, "/");

		_itoa_s(fechaHoy.wMonth, varchar, 10);
		strcat_s(fechaComHoy, varchar);

		strcat_s(fechaComHoy, "/");

		_itoa_s(fechaHoy.wYear, varchar, 10);
		strcat_s(fechaComHoy, varchar);

		SetDlgItemText(hwnd, IDC_EDIT4, fechaComHoy);

	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

	}break;

	}

	return 0;

}

BOOL CALLBACK procRegUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

		//if (inicioUs != nullptr) { //si esta vacia y se inicia el if consiguiente causaria un error pues sesion seria null
		//	if (Sesion->admin == 2) {
		//		LimitarCajero(hwnd);
		//	}
		//}

		if (inicioUs == nullptr) { //si es el primero usuario a registar no se permite elegir entre admin o cajero ni ver lista de usuarios

			EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), false);
			EnableWindow(GetDlgItem(hwnd, ID_LISTA), false);
			SetMenu(hwnd, NULL);
		}

		if (inicioUs == nullptr) { //si es el primer usuario a registrar forzadamente sera admin
			SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
		}

		auxSu = inicioSu;
		while (auxSu != nullptr) {
			//SendMessage(GetDlgItem(hwnd, IDC_COMBO1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxSu->nomSuc);
			SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxSu->nomSuc);
			auxSu = auxSu->sig;
		}

		strcpy_s(zFile, "");//vaciamos el zfile para que pida foto siempre

	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDOK: {

			usuarios* tempUs = new usuarios;
			bool aceptarUsuario = true;
			int indice;

			GetDlgItemText(hwnd, IDC_EDIT1, tempUs->nomComp, sizeof(tempUs->nomComp)); //nombre Completo
			GetDlgItemText(hwnd, IDC_EDIT2, tempUs->nomUsu, sizeof(tempUs->nomUsu)); //nombre Usuario
			GetDlgItemText(hwnd, IDC_EDIT3, tempUs->pswrd, sizeof(tempUs->pswrd)); //contraseña

			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {

				tempUs->admin = 1; //administrador
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
				tempUs->admin = 2; //cajero
			}


			GetDlgItemText(hwnd, IDC_COMBO1, tempUs->sucur, sizeof(tempUs->sucur)); //elegir sucursal
			indice = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);

			strcpy_s(tempUs->foto, zFile);

			if (validarNomCom(tempUs->nomComp) == false) { //validacion nombre completo
				aceptarUsuario = false;
				MessageBox(hwnd, "Su nombre completo debe tener de 5 a 50 caracteres, y unicamente caracteres alfabeticos.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (inicioUs != nullptr) {

				auxUs = inicioUs;

				while (auxUs != nullptr) {
					if (strcmp(tempUs->nomUsu, auxUs->nomUsu) == 0) {

						aceptarUsuario = false;
						MessageBox(hwnd, "El nombre de usuario ya está en uso.", "AVISO", MB_OK | MB_ICONERROR);
					}
					auxUs = auxUs->sig;

				}
			}

			if (validarUsuario(tempUs->nomUsu) == false) { //validacion nombre de usuario
				aceptarUsuario = false;
				MessageBox(hwnd, "Su nombre de usuario debe tener de 5 a 20 caracteres, y unicamente caracteres alfanumericos.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (validarContra(tempUs->pswrd) == false) { //validar contraseña
				aceptarUsuario = false;
				MessageBox(hwnd, "Su contraseña debe tener de 3 a 10 caracteres, tener al menos una letra, un numero, un caracter especial y no tener espacios.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (tempUs->admin == 0) { //validar tipo de usuario
				aceptarUsuario = false;
				MessageBox(hwnd, "Seleccione un tipo de usuario", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (strlen(tempUs->sucur) == 0 || indice < 0) { //validar que eligio una sucursal
				aceptarUsuario = false;
				MessageBox(hwnd, "Seleccione una sucursal", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (strlen(tempUs->foto) == 0) { //validar que haya elegido foto
				aceptarUsuario = false;
				MessageBox(hwnd, "Seleccione una fotografia", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (aceptarUsuario) {
				registarUsuario(tempUs);

				/*auxUs = inicioUs;
				while(auxUs->sig != nullptr) {
					auxUs = auxUs->sig;
				}*/

				if (auxUs == inicioUs) {
					Sesion = auxUs;
					MessageBox(hwnd, "Usuario registrado exitosamente y se inicio sesion con el", "AVISO", MB_OK | MB_ICONINFORMATION);
					EndDialog(hwnd, 0);
					DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);

				}
				else {
					MessageBox(hwnd, "Usuario registrado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);
				}

				//borramos los apartados
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT3, "");
				SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SetDlgItemText(hwnd, IDC_COMBO1, "");
				SendDlgItemMessage(hwnd, IDC_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				strcpy_s(zFile, "");//vaciamos el zfile para que pida foto siempre
			}

		}break;

		case IDC_BUTTON1: { //subir fotografia

			OPENFILENAME ofn;

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Todos\0 * .*\0ImagenBMP\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 150, 130, LR_LOADFROMFILE); //2

				SendDlgItemMessage(hwnd, IDC_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 


			}
			else {
				MessageBox(hwnd, "No seleccionó una fotografia", "AVISO", MB_OK | MB_ICONERROR);
			}

		}break;

		case ID_LISTA: {

			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(USU_LISTA), hwnd, procListUsu);

		}break;
		}

	}break;

	}

	return 0;
}
BOOL CALLBACK procListUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		if (Sesion->admin == 2) {
			LimitarCajero(hwnd);
		}

		auxUs = inicioUs; //llenamos listbox de nombres completos de usuarios
		while (auxUs != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxUs->nomComp);
			auxUs = auxUs->sig;
		}

		auxSu = inicioSu; //llenamos listbox de sucursales
		while (auxSu != nullptr) {
			SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxSu->nomSuc);
			auxSu = auxSu->sig;
		}

	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa

		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case ID_REGRESAR: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(USU_REGISTRO), hwnd, procRegUsu);
		}break;

		case IDC_LIST1: {

			switch (HIWORD(wParam)) {

			case LBN_SELCHANGE: {

				if (Sesion->admin == 1) {
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), true);
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), true);
				}

				int indiceUsu = 0;
				char usuList[50] = { 0 }; //buscamos por nombre completo del usuario

				indiceUsu = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indiceUsu, (LPARAM)usuList);

				auxUs = inicioUs;

				while (auxUs->sig != nullptr && strcmp(usuList, auxUs->nomComp) != 0) {
					auxUs = auxUs->sig;
				}

				SetDlgItemText(hwnd, IDC_EDIT1, auxUs->nomComp);
				SetDlgItemText(hwnd, IDC_EDIT2, auxUs->nomUsu);
				SetDlgItemText(hwnd, IDC_EDIT3, auxUs->pswrd);

				if (auxUs->admin == 1) { //es admin
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}
				else if (auxUs->admin == 2) { //es cajero
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}

				SetDlgItemText(hwnd, IDC_COMBO1, auxUs->sucur);

				HBITMAP bmp; //1
				bmp = (HBITMAP)LoadImage(NULL, auxUs->foto, IMAGE_BITMAP, 100, 80, LR_LOADFROMFILE); //2

				SendDlgItemMessage(hwnd, IDC_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 


			}break; //case
			}//switch

		}break;

		case IDC_BUTTON1: { //modificar

			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_SHOW); //aparecemos boton de guardar
			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON4), SW_SHOW); //aparecemos boton de foto
			EnableWindow(GetDlgItem(hwnd, IDC_LIST1), false); //desactivas la list box para que no elija otra cosa
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), false);//desactiva boton modificar
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false); //desactiva boton de eliminar

			//EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), true); no se activa porque nombre completo no se modifica
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), true); //activamos lo demas
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBO1), true);
			SendDlgItemMessage(hwnd, IDC_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
			strcpy_s(zFile, "");


		}break;

		case IDC_BUTTON3: { //guardar cambios

			int indice = 0;
			char textoUsuario[50] = { 0 };
			bool aceptarUsuario = true;

			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoUsuario);

			usuarios* modificar = new usuarios;

			GetDlgItemText(hwnd, IDC_EDIT2, modificar->nomUsu, sizeof(modificar->nomUsu));
			GetDlgItemText(hwnd, IDC_EDIT3, modificar->pswrd, sizeof(modificar->pswrd));

			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {

				modificar->admin = 1;
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
				modificar->admin = 2;
			}

			GetDlgItemText(hwnd, IDC_COMBO1, modificar->sucur, sizeof(modificar->sucur));
			int indiceSucursal = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);

			strcpy_s(modificar->foto, zFile);

			if (inicioUs != nullptr) {

				auxUs = inicioUs;

				while (auxUs != nullptr) {
					if (strcmp(modificar->nomUsu, auxUs->nomUsu) == 0) {

						aceptarUsuario = false;
						MessageBox(hwnd, "El nombre de usuario no puede ser el mismo.", "AVISO", MB_OK | MB_ICONERROR);
					}
					auxUs = auxUs->sig;

				}
			}

			if (validarUsuario(modificar->nomUsu) == false) { //validacion nombre de usuario
				aceptarUsuario = false;
				MessageBox(hwnd, "Su nombre de usuario debe tener de 5 a 20 caracteres, y unicamente caracteres alfanumericos.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (validarContra(modificar->pswrd) == false) { //validar contraseña
				aceptarUsuario = false;
				MessageBox(hwnd, "Su contraseña debe tener de 3 a 10 caracteres, tener al menos una letra, un numero, un caracter especial y no tener espacios.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (modificar->admin == 0) { //validar tipo de usuario
				aceptarUsuario = false;
				MessageBox(hwnd, "Seleccione un tipo de usuario", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (strlen(modificar->sucur) == 0) { //validar que eligio una sucursal
				aceptarUsuario = false;
				MessageBox(hwnd, "Seleccione una sucursal", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (strlen(modificar->foto) == 0) { //validar que haya elegido foto
				aceptarUsuario = false;
				MessageBox(hwnd, "Seleccione una fotografia", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (aceptarUsuario) {

				modificarUsuario(modificar, hwnd, textoUsuario);

				EnableWindow(GetDlgItem(hwnd, IDC_LIST1), true); //reactivas listbox
				ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_HIDE); //desapareces boton guardar
				ShowWindow(GetDlgItem(hwnd, IDC_BUTTON4), SW_HIDE); //desaparece boton de foto
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
				EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBO1), false);
			}

		}break;

		case IDC_BUTTON2: { //eliminar

			int opc = MessageBox(hwnd, "¿Seguro que desea eliminar este usuario?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				int indUsuario = 0;
				char textUsuDel[50] = { 0 };

				indUsuario = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indUsuario, (LPARAM)textUsuDel);

				eliminarUsuario(textUsuDel, hwnd, indUsuario);

				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false);


			}break;
			}

		}break;

		case IDC_BUTTON4: { //subir foto

			OPENFILENAME ofn;

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Todos\0 * .*\0ImagenBMP\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 100, 80, LR_LOADFROMFILE); //2

				SendDlgItemMessage(hwnd, IDC_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 


			}
			else {
				MessageBox(hwnd, "No seleccionó una fotografia", "AVISO", MB_OK | MB_ICONERROR);
			}

		}break;

		}//switch de loword

	}break;

	}
	return 0;
}

BOOL CALLBACK procRegSuc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDOK: {

			sucursal* tempSu = new sucursal;
			bool aceptarSucursal = true;

			GetDlgItemText(hwnd, IDC_EDIT1, tempSu->nomSuc, sizeof(tempSu->nomSuc)); //nombre de sucursal
			GetDlgItemText(hwnd, IDC_EDIT2, tempSu->direc, sizeof(tempSu->direc)); //nombre Usuario

			if (validarNomCom(tempSu->nomSuc) == false) {
				aceptarSucursal = false;
				MessageBox(hwnd, "El nombre de la sucursal debe tener de 5 a 50 caracteres, y unicamente caracteres alfabeticos.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (validarDireccion(tempSu->direc) == false) {

				aceptarSucursal = false;
				MessageBox(hwnd, "La direccion de la sucursal debe tener unicamente caracteres alfanumericos.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (aceptarSucursal) {
				registrarSucur(tempSu);

				MessageBox(hwnd, "Sucursal registrada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				//borramos los apartados
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
			}

		}break;

		case IDC_BUTTON1: {

			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(SUC_LISTA), hwnd, procListSuc);

		}break;
		}

	}break;

	}

	return 0;
}
BOOL CALLBACK procListSuc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		if (Sesion->admin == 2) {

			LimitarCajero(hwnd);
		}

		auxSu = inicioSu;
		while (auxSu != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxSu->nomSuc);
			auxSu = auxSu->sig;
		}
	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa

		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case ID_REGRESAR: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(SUC_REGISTRO), hwnd, procRegSuc);
		}break;

		case IDC_LIST1: {

			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: {

				if (Sesion->admin == 1) {
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), true);
				}


				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), true);


				int indiceSuc = 0;
				char SucList[50] = { 0 }; //buscamos por nombre completo

				indiceSuc = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indiceSuc, (LPARAM)SucList);

				auxSu = inicioSu;

				while (auxSu->sig != nullptr && strcmp(SucList, auxSu->nomSuc) != 0) {
					auxSu = auxSu->sig;
				}

				SetDlgItemText(hwnd, IDC_EDIT1, auxSu->nomSuc);
				SetDlgItemText(hwnd, IDC_EDIT2, auxSu->direc);

			}break;
			}


		}break;

		case IDC_BUTTON2: { //eliminar

			int opc = MessageBox(hwnd, "¿Seguro que desea eliminar esta sucursal?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				int indSucursal = 0;
				char textSucDel[50] = { 0 };

				indSucursal = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indSucursal, (LPARAM)textSucDel);

				eliminarSucursal(textSucDel, hwnd, indSucursal);

			}break;
			}

		}break;

		case IDC_BUTTON1: { //modificar

			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_SHOW); //aparecemos boton de guardar
			EnableWindow(GetDlgItem(hwnd, IDC_LIST1), false); //desactivas la list box para que no elija otra cosa
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), false);//desactiva boton modificar
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false); //desactiva boton de eliminar

			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), true); //activas direccion

		}break;

		case IDC_BUTTON3: { //guardar

			int indice = 0;
			char textoSucursal[50] = { 0 };
			bool aceptarSucursal = true;


			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoSucursal);

			sucursal* modificar = new sucursal;

			GetDlgItemText(hwnd, IDC_EDIT2, modificar->direc, sizeof(modificar->direc));

			if (validarDireccion(modificar->direc) == false) {

				aceptarSucursal = false;
				MessageBox(hwnd, "La direccion de la sucursal debe tener unicamente caracteres alfanumericos.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (aceptarSucursal) {
				modificarSucursal(modificar, hwnd, textoSucursal);

				EnableWindow(GetDlgItem(hwnd, IDC_LIST1), true); //reactivas list box
				ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_HIDE); // ocultar guardar
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false); //desactivas direccion
			}

		}break;

		}//switch loword
	}break;

	} //switch msg
	return 0;
}

BOOL CALLBACK procRegCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

		if (Sesion->admin == 2) {

			LimitarCajero(hwnd);
		}


		auxSu = inicioSu;
		while (auxSu != nullptr) {
			SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxSu->nomSuc);
			auxSu = auxSu->sig;
		}

		SetDlgItemText(hwnd, IDC_EDIT4, "0");
		SetDlgItemText(hwnd, IDC_COMBO1, Sesion->sucur);


	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDOK: {

			clientes* tempCl = new clientes;
			bool aceptarCliente = true;

			GetDlgItemText(hwnd, IDC_EDIT1, tempCl->nomComp, sizeof(tempCl->nomComp)); //nombre Completo
			GetDlgItemText(hwnd, IDC_EDIT2, tempCl->phone, sizeof(tempCl->phone)); // telefono
			GetDlgItemText(hwnd, IDC_EDIT3, tempCl->email, sizeof(tempCl->email)); // correo

			GetDlgItemText(hwnd, IDC_COMBO1, tempCl->sucur, sizeof(tempCl->sucur)); //elegir sucursal

			if (validarNomCom(tempCl->nomComp) == false) {
				aceptarCliente = false;
				MessageBox(hwnd, "El nombre completo debe tener de 5 a 50 caracteres, y unicamente caracteres alfabeticos.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (inicioCl != nullptr) {

				auxCl = inicioCl;

				while (auxCl != nullptr) {
					if (strcmp(tempCl->nomComp, auxCl->nomComp) == 0) {
						aceptarCliente = false;
						MessageBox(hwnd, "El nombre ya está en uso.", "AVISO", MB_OK | MB_ICONERROR);
					}
					auxCl = auxCl->sig;

				}
			}

			if (validarTelefono(tempCl->phone) == false) {

				aceptarCliente = false;
				MessageBox(hwnd, "El telefono debe tener 10 caracteres y ser unicamente numeros.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (validarCorreo(tempCl->email) == false) {

				aceptarCliente = false;
				MessageBox(hwnd, "El correo debe tener de 5 a 50 caracteres, ademas de contener un @.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (aceptarCliente) {

				registrarCliente(tempCl);

				MessageBox(hwnd, "Cliente registrado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				//borramos los apartados
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT3, "");

			}

		}break;


		case ID_LISTA: {

			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(CLI_LISTA), hwnd, procListCli);

		}break;
		}

	}break;

	}

	return 0;



}
BOOL CALLBACK procListCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		if (Sesion->admin == 2) {

			LimitarCajero(hwnd);
		}

		auxCl = inicioCl;
		while (auxCl != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxCl->nomComp);
			auxCl = auxCl->sig;
		}
	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa

		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case ID_REGRESAR: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(CLI_REGISTRO), hwnd, procRegCli);
		}break;

		case IDC_LIST1: {

			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: {

				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), true);
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), true);

				int indiceCli = 0;
				char cliList[50] = { 0 }; //buscamos por nombre completo del cliente

				indiceCli = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indiceCli, (LPARAM)cliList);

				auxCl = inicioCl;

				while (auxCl->sig != nullptr && strcmp(cliList, auxCl->nomComp) != 0) {
					auxCl = auxCl->sig;
				}

				SetDlgItemText(hwnd, IDC_EDIT1, auxCl->nomComp);
				SetDlgItemText(hwnd, IDC_EDIT2, auxCl->phone);
				SetDlgItemText(hwnd, IDC_EDIT3, auxCl->email);
				SetDlgItemText(hwnd, IDC_COMBO1, auxCl->sucur);
				SetDlgItemInt(hwnd, IDC_EDIT4, auxCl->puntos, false);

			}break; //case
			}//switch


		}break;

		case IDC_BUTTON2: { //eliminar

			int opc = MessageBox(hwnd, "¿Seguro que desea eliminar este cliente?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				int indCliente = 0;
				char textCliDel[50] = { 0 };

				indCliente = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indCliente, (LPARAM)textCliDel);

				eliminarCliente(textCliDel, hwnd, indCliente);

				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false);

			}break;

			}
		}break;

		case IDC_BUTTON1: { //modificar
			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_SHOW); //aparecemos boton de guardar
			EnableWindow(GetDlgItem(hwnd, IDC_LIST1), false); //desactivas la list box para que no elija otra cosa
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), false);//desactiva boton modificar
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false); //desactiva boton de eliminar

			EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);

		}break;

		case IDC_BUTTON3: { //guardar cambios

			int indice = 0;
			char textoCliente[50] = { 0 };
			bool aceptarCliente = true;

			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoCliente);

			clientes* modificar = new clientes;

			GetDlgItemText(hwnd, IDC_EDIT1, modificar->nomComp, sizeof(modificar->nomComp));
			GetDlgItemText(hwnd, IDC_EDIT2, modificar->phone, sizeof(modificar->phone));
			GetDlgItemText(hwnd, IDC_EDIT3, modificar->email, sizeof(modificar->email));

			if (validarNomCom(modificar->nomComp) == false) {
				aceptarCliente = false;
				MessageBox(hwnd, "El nombre completo debe tener de 5 a 50 caracteres, y unicamente caracteres alfabeticos.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (inicioCl != nullptr) {

				auxCl = inicioCl;

				while (auxCl != nullptr) {
					if (strcmp(modificar->nomComp, auxCl->nomComp) == 0) {
						aceptarCliente = false;
						MessageBox(hwnd, "El nombre ya está en uso.", "AVISO", MB_OK | MB_ICONERROR);
					}
					auxCl = auxCl->sig;

				}
			}

			if (validarTelefono(modificar->phone) == false) {

				aceptarCliente = false;
				MessageBox(hwnd, "El telefono debe tener 10 caracteres y ser unicamente numeros.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (validarCorreo(modificar->email) == false) {

				aceptarCliente = false;
				MessageBox(hwnd, "El correo debe tener de 5 a 50 caracteres, ademas de contener un @.", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (aceptarCliente) {

				modificarCliente(modificar, hwnd, textoCliente);

				EnableWindow(GetDlgItem(hwnd, IDC_LIST1), true); //reactivas listbox
				ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_HIDE); //desapareces boton guardar
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
			}

		}break;

		}//switch loword
	}break;
	}
	return 0;

}

BOOL CALLBACK procRegProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDOK: {

			promocion* tempProm = new promocion;
			bool aceptarPromocion = true;

			GetDlgItemText(hwnd, IDC_EDIT1, tempProm->nomProm, sizeof(tempProm->nomProm)); //nombre promocion
			tempProm->puntReq = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, true); //puntos minimos requeridos
			tempProm->porcent = GetDlgItemInt(hwnd, IDC_EDIT3, NULL, true); // porcentaje

			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {

				tempProm->status = 1; //activa
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
				tempProm->status = 2; //inactiva
			}

			if (validarNomCom(tempProm->nomProm) == false) {
				aceptarPromocion = false;
				MessageBox(hwnd, "El nombre debe tener de 5 a 50 caracteres, y unicamente caracteres alfabeticos.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (inicioPr != nullptr) {

				auxPr = inicioPr;

				while (auxPr != nullptr) {
					if (strcmp(tempProm->nomProm, auxPr->nomProm) == 0) {
						aceptarPromocion = false;
						MessageBox(hwnd, "El nombre ya está en uso.", "AVISO", MB_OK | MB_ICONERROR);
					}
					auxPr = auxPr->sig;

				}
			}

			if (tempProm->puntReq < 0) {

				aceptarPromocion = false;
				MessageBox(hwnd, "Los puntos minimos requeridos no pueden ser menor a cero.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (tempProm->porcent <= 0 || tempProm->porcent >= 100) {
				aceptarPromocion = false;
				MessageBox(hwnd, "El porcentaje de descuento debe ser mayor a  0 y menor a 100.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (tempProm->status == 0) {
				aceptarPromocion = false;
				MessageBox(hwnd, "Seleccione el estatus de la promocion.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (aceptarPromocion) {
				registrarPromo(tempProm);

				MessageBox(hwnd, "Promocion registrada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				//borramos los apartados
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT3, "");
				SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			}

		}break;

		case ID_LISTA: {

			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(PROM_LISTA), hwnd, procListProm);

		}break;
		}

	}break;

	}

	return 0;

}
BOOL CALLBACK procListProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		if (Sesion->admin == 2) {

			LimitarCajero(hwnd);
		}

		auxPr = inicioPr;
		while (auxPr != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxPr->nomProm);
			auxPr = auxPr->sig;
		}
	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa

		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case ID_REGRESAR: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(PROM_REGISTRO), hwnd, procRegProm);
		}break;

		case IDC_LIST1: {
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: {

				if (Sesion->admin == 1) {
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), true);
				}

				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), true);

				int indiceProm = 0;
				char promList[52] = { 0 }; //buscamos por nombre de la promo

				indiceProm = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indiceProm, (LPARAM)promList);

				auxPr = inicioPr;

				while (auxPr->sig != nullptr && strcmp(promList, auxPr->nomProm) != 0) {
					auxPr = auxPr->sig;
				}

				SetDlgItemText(hwnd, IDC_EDIT1, auxPr->nomProm);
				SetDlgItemInt(hwnd, IDC_EDIT2, auxPr->puntReq, false);
				SetDlgItemInt(hwnd, IDC_EDIT3, auxPr->porcent, false);

				if (auxPr->status == 1) { //esta activa
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}
				else if (auxPr->status == 2) { //esta inactiva
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}

			}break; //case
			}//switch
		}break;

		case IDC_BUTTON2: { //eliminar

			int opc = MessageBox(hwnd, "¿Seguro que desea eliminar esta promocion?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				int indPromo = 0;
				char textPromDel[50] = { 0 };

				indPromo = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indPromo, (LPARAM)textPromDel);

				eliminarPromocion(textPromDel, hwnd, indPromo);

				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false);

			}break;

			}

		}break;

		case IDC_BUTTON1: { //modificar

			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_SHOW); //aparecemos boton de guardar
			EnableWindow(GetDlgItem(hwnd, IDC_LIST1), false); //desactivas la list box para que no elija otra cosa
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), false);//desactiva boton modificar
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false); //desactiva boton de eliminar

			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), true);


		}break;

		case IDC_BUTTON3: { //guardar

			int indice = 0;
			char textoPromocion[50] = { 0 };
			bool aceptarPromocion = true;

			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoPromocion);

			promocion* modificar = new promocion;

			modificar->puntReq = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, true); //puntos minimos requeridos
			modificar->porcent = GetDlgItemInt(hwnd, IDC_EDIT3, NULL, true); // porcentaje

			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {

				modificar->status = 1; //activa
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
				modificar->status = 2; //inactiva
			}

			if (modificar->puntReq < 0) {

				aceptarPromocion = false;
				MessageBox(hwnd, "Los puntos minimos requeridos no pueden ser menor a cero.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (modificar->porcent <= 0 || modificar->porcent >= 100) {
				aceptarPromocion = false;
				MessageBox(hwnd, "El porcentaje de descuento debe ser mayor a  0 y menor a 100.", "AVISO", MB_OK | MB_ICONERROR);

			}

			if (aceptarPromocion) {

				modificarPromocion(modificar, hwnd, textoPromocion);

				EnableWindow(GetDlgItem(hwnd, IDC_LIST1), true); //reactivas listbox
				ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_HIDE);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
				EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), false);

			}

		}break;

		}
	}break;

	}
	return 0;
}

BOOL CALLBACK procCapturaPuntos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		auxCl = inicioCl;
		while (auxCl != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxCl->nomComp);
			auxCl = auxCl->sig;
		}
	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: { //cancelar
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);
		}break;

		case IDC_LIST1: {

			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: {

				EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), true);
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), true);

				int indiceCli = 0;
				char cliList[50] = { 0 }; //buscamos por nombre completo del cliente

				indiceCli = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indiceCli, (LPARAM)cliList);

				auxCl = inicioCl;

				while (auxCl->sig != nullptr && strcmp(cliList, auxCl->nomComp) != 0) {
					auxCl = auxCl->sig;
				}

			}break;
			}

		}break;

		case IDC_BUTTON2: { //calcular puntos

			int puntos = 0;
			int monto = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, true);

			if (monto > 0) {

				auxCl->montototal = auxCl->montototal + monto;
				EnableWindow(GetDlgItem(hwnd, ID_OK), true);

				puntos = monto * .1;

				SetDlgItemInt(hwnd, IDC_EDIT2, puntos, true);
			}
			else {

				MessageBox(hwnd, "El monto debe ser mayor a 0.", "AVISO", MB_OK | MB_ICONERROR);

			}


		}break;

		case ID_OK: {

			int opc = MessageBox(hwnd, "¿Desea registrar los puntos obtenidos por esta compra?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				int monto = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, true);
				auxCl->puntos = auxCl->puntos + monto;

				MessageBox(hwnd, "Puntos registrados correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);

				EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false);
				EnableWindow(GetDlgItem(hwnd, ID_OK), false);

				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");


			}break;
			}

		}break;

		}
	}break;

	}
	return 0;
}
BOOL CALLBACK procCanjePuntos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		auxCl = inicioCl;
		while (auxCl != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxCl->nomComp);
			auxCl = auxCl->sig;
		}
	}break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_LIST2: {

			switch (HIWORD(wParam)) {

			case LBN_SELCHANGE: {

				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
				EnableWindow(GetDlgItem(hwnd, ID_OK), false);
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT3, "");

				int indiceCli = 0;
				char cliList[50] = { 0 }; //buscamos por nombre completo del cliente

				indiceCli = SendDlgItemMessage(hwnd, IDC_LIST2, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST2, LB_GETTEXT, indiceCli, (LPARAM)cliList);

				auxCl = inicioCl;

				while (auxCl->sig != nullptr && strcmp(cliList, auxCl->nomComp) != 0) {
					auxCl = auxCl->sig;
				}

				SetDlgItemInt(hwnd, IDC_EDIT1, auxCl->puntos, false);

				auxPr = inicioPr;

				while (auxPr != nullptr) {

					if (auxPr->puntReq <= auxCl->puntos) {
						SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxPr->nomProm);
					}

					auxPr = auxPr->sig;
				}

			}break;
			}

		}break;

		case IDC_LIST1: {

			switch (HIWORD(wParam)) {

			case LBN_SELCHANGE: {

				EnableWindow(GetDlgItem(hwnd, ID_OK), true);

				int indiceProm = 0;
				char promList[50] = { 0 };

				indiceProm = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indiceProm, (LPARAM)promList);

				auxPr = inicioPr;

				while (auxPr->sig != nullptr && strcmp(promList, auxPr->nomProm) != 0) {
					auxPr = auxPr->sig;
				}

				SetDlgItemInt(hwnd, IDC_EDIT2, auxPr->puntReq, false);
				SetDlgItemInt(hwnd, IDC_EDIT3, auxCl->puntos - auxPr->puntReq, false);

			}break;
			}

		}break;

		case ID_OK: {

			int opc = MessageBox(hwnd, "¿Desea canjear esta promocion?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				auxCl->puntos = auxCl->puntos - auxPr->puntReq;
				auxPr->vecesCanjeada++; //le agregamos una vez canjeada

				MessageBox(hwnd, "Promocion canjeada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
				EnableWindow(GetDlgItem(hwnd, ID_OK), false);
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT3, "");

			}break;
			}



		}break;

		case IDC_BUTTON1: { //regresar
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);
		}break;


		}


	}break;

	}

	return 0;
}

BOOL CALLBACK ReportePuntos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_INITDIALOG: {

		OrdenarPuntos();

		auxCl = inicioCl;
		while (auxCl != nullptr) {

			if (auxCl->puntos > 0) {
				int puntosInt = 0;
				char puntosChar[10] = { 0 };
				puntosInt = auxCl->puntos;
				_itoa_s(puntosInt, puntosChar, 10);
				SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxCl->nomComp);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)puntosChar);
			}

			auxCl = auxCl->sig;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_BUTTON1: { //regresar
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);
		}break;

		case IDC_BUTTON2: { //generar excel
			int opc = MessageBox(hwnd, "¿Desea generar un archivo en Excel de estos datos?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {
				ReportePuntosCSV();
				MessageBox(hwnd, "Archivo generado exitosamente.", "AVISO", MB_OK | MB_ICONINFORMATION);

			}break;
			}


		}break;

		}

	}break;

	}

	return 0;
}
BOOL CALLBACK ReporteMontos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_INITDIALOG: {

		OrdenarMontos();

		auxCl = inicioCl;
		while (auxCl != nullptr) {
			int montosInt = 0;
			char montosChar[10] = { 0 };
			montosInt = auxCl->montototal;
			_itoa_s(montosInt, montosChar, 10);
			SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxCl->nomComp);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)montosChar);

			auxCl = auxCl->sig;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_BUTTON1: { //regresar
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);
		}break;

		case IDC_BUTTON2: { //generar excel
			int opc = MessageBox(hwnd, "¿Desea generar un archivo en Excel de estos datos?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				ReporteMontosCSV();
				MessageBox(hwnd, "Archivo generado exitosamente.", "AVISO", MB_OK | MB_ICONINFORMATION);

			}break;
			}


		}break;

		}

	}break;

	}

	return 0;
}
BOOL CALLBACK ReportePromociones(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break;

	case WM_INITDIALOG: {

		OrdenarCanjes();

		auxPr = inicioPr;
		while (auxPr != nullptr) {
			int canjesInt = 0;
			char canjesChar[10] = { 0 };
			canjesInt = auxPr->vecesCanjeada;
			_itoa_s(canjesInt, canjesChar, 10);
			SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxPr->nomProm);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)canjesChar);

			auxPr = auxPr->sig;
		}
	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_BUTTON1: { //regresar
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);
		}break;

		case IDC_BUTTON2: { //generar excel
			int opc = MessageBox(hwnd, "¿Desea generar un archivo en Excel de estos datos?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {
				ReporteCanjesCSV();
				MessageBox(hwnd, "Archivo generado exitosamente.", "AVISO", MB_OK | MB_ICONINFORMATION);

			}break;
			}
		}break;
		}

	}break;

	}

	return 0;
}

//funciones de usuario
void registarUsuario(usuarios* nuevo) {

	if (inicioUs == nullptr) {

		inicioUs = new usuarios;

		strcpy_s(inicioUs->nomComp, nuevo->nomComp);
		strcpy_s(inicioUs->nomUsu, nuevo->nomUsu);
		strcpy_s(inicioUs->pswrd, nuevo->pswrd);
		inicioUs->admin = nuevo->admin;
		strcpy_s(inicioUs->sucur, nuevo->sucur);
		strcpy_s(inicioUs->foto, nuevo->foto);
		inicioUs->sig = nullptr;

		auxUs = inicioUs;
	}
	else {


		auxUs = inicioUs;


		while (auxUs->sig != nullptr) {

			auxUs = auxUs->sig;

		}

		auxUs->sig = new usuarios;

		auxUs = auxUs->sig;

		strcpy_s(auxUs->nomComp, nuevo->nomComp);
		strcpy_s(auxUs->nomUsu, nuevo->nomUsu);
		strcpy_s(auxUs->pswrd, nuevo->pswrd);
		auxUs->admin = nuevo->admin;
		strcpy_s(auxUs->sucur, nuevo->sucur);
		strcpy_s(auxUs->foto, nuevo->foto);
		auxUs->sig = nullptr;

	}
}
void leerBinUsuario() {

	ifstream leerUsuario;

	leerUsuario.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\usuarios.bin", ios::in | ios::binary);

	if (leerUsuario.is_open()) {

		usuarios* usuarioLeido = new usuarios;

		while (!leerUsuario.read((char*)usuarioLeido, sizeof(usuarios)).eof()) {

			if (inicioUs == nullptr) {

				inicioUs = usuarioLeido;
				inicioUs->sig = nullptr;
				auxUs = inicioUs;

			}
			else {
				auxUs->sig = usuarioLeido;
				auxUs = auxUs->sig;
				auxUs->sig = nullptr;

			}

			usuarioLeido = new usuarios;

		}
		leerUsuario.close();
		delete usuarioLeido;

	}


}
void escribirBinUsuario() {

	auxUs = inicioUs;

	ofstream escrUsuario;

	escrUsuario.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\usuarios.bin", ios::out | ios::binary | ios::trunc);

	if (escrUsuario.is_open()) {

		while (auxUs != nullptr) {

			escrUsuario.write((char*)auxUs, sizeof(usuarios));
			auxUs = auxUs->sig;
		}

		escrUsuario.close();
	}
}
void eliminarUsuario(char usuarioEliminar[50], HWND hwnd, int indice) {

	auxUs = inicioUs;

	usuarios* anterior = nullptr;

	if (inicioUs == nullptr) {
		MessageBox(hwnd, "Su lista de usuarios esta vacia", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else {
		while (auxUs != nullptr && strcmp(auxUs->nomComp, usuarioEliminar) != 0) { //se mueve por el arreglo

			anterior = auxUs;
			auxUs = auxUs->sig;

		}

		if (auxUs == nullptr) {
			MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONQUESTION);
		}
		else if (auxUs == inicioUs) { //se encontro al inicio de la lista

			inicioUs = inicioUs->sig; //el inicio se pasa al siguiente

			delete auxUs;

			auxUs = inicioUs;

			MessageBox(hwnd, "Usuario eliminado correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);

			while (auxUs != nullptr) { //actualizamos la listbox
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxUs->nomComp);
				auxUs = auxUs->sig;
			}

			auxUs = inicioUs;

			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");
			SetDlgItemText(hwnd, IDC_EDIT3, "");
			SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SetDlgItemText(hwnd, IDC_COMBO1, "");
			SendDlgItemMessage(hwnd, IDC_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);

		}
		else {

			anterior->sig = auxUs->sig;

			delete auxUs;

			auxUs = inicioUs;

			MessageBox(hwnd, "Usuario eliminado correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);

			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");
			SetDlgItemText(hwnd, IDC_EDIT3, "");
			SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SetDlgItemText(hwnd, IDC_COMBO1, "");
			SendDlgItemMessage(hwnd, IDC_PHOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);


		}


	}

}
void modificarUsuario(usuarios* UsuMod, HWND hwnd, char buscaUsu[50]) {

	auxUs = inicioUs;

	while (auxUs != nullptr && strcmp(buscaUsu, auxUs->nomComp) != 0) {
		auxUs = auxUs->sig;

	}

	if (auxUs == nullptr) {

		MessageBox(hwnd, "Usuario no encontrado.", "AVISO", MB_OK | MB_ICONQUESTION);

	}
	else {

		strcpy_s(auxUs->nomUsu, UsuMod->nomUsu);
		strcpy_s(auxUs->pswrd, UsuMod->pswrd);
		auxUs->admin = UsuMod->admin;
		strcpy_s(auxUs->sucur, UsuMod->sucur);
		strcpy_s(auxUs->foto, UsuMod->foto);

		MessageBox(hwnd, "Usuario modificado correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);

		auxUs = inicioUs;

		while (auxUs != nullptr) { //actualizamos la listbox
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxUs->nomComp);
			auxUs = auxUs->sig;
		}
	}



}

//funciones de sucursal
void registrarSucur(sucursal* nuevo) {

	if (inicioSu == nullptr) {

		inicioSu = new sucursal;

		strcpy_s(inicioSu->nomSuc, nuevo->nomSuc);
		strcpy_s(inicioSu->direc, nuevo->direc);
		inicioSu->sig = nullptr;

	}
	else {

		auxSu = inicioSu;

		while (auxSu->sig != nullptr) {

			auxSu = auxSu->sig;

		}

		auxSu->sig = new sucursal;

		auxSu = auxSu->sig;

		strcpy_s(auxSu->nomSuc, nuevo->nomSuc);
		strcpy_s(auxSu->direc, nuevo->direc);
		auxSu->sig = nullptr;

	}

}
void leerBinSucur() {

	fstream leerSucur;

	leerSucur.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\sucursales.bin", ios::in | ios::binary);

	if (leerSucur.is_open()) {

		sucursal* sucurLeido = new sucursal;

		while (!leerSucur.read((char*)sucurLeido, sizeof(sucursal)).eof()) {

			if (inicioSu == nullptr) {

				inicioSu = sucurLeido;
				inicioSu->sig = nullptr;
				auxSu = inicioSu;

			}
			else {
				auxSu->sig = sucurLeido;
				auxSu = auxSu->sig;
				auxSu->sig = nullptr;

			}

			sucurLeido = new sucursal;

		}
		leerSucur.close();
		delete sucurLeido;

	}
}
void escribirBinSucur() {

	auxSu = inicioSu;

	ofstream escrSucur;

	escrSucur.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\sucursales.bin", ios::out | ios::binary | ios::trunc);

	if (escrSucur.is_open()) {

		while (auxSu != nullptr) {

			escrSucur.write((char*)auxSu, sizeof(sucursal));
			auxSu = auxSu->sig;
		}

		escrSucur.close();

	}
}
void eliminarSucursal(char sucursalEliminar[50], HWND hwnd, int indice) {

	auxSu = inicioSu;

	sucursal* anterior = nullptr;

	if (inicioSu == nullptr) {

		MessageBox(hwnd, "Su lista de sucursales esta vacia", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else {

		while (auxSu != nullptr && strcmp(auxSu->nomSuc, sucursalEliminar) != 0) {

			anterior = auxSu;
			auxSu = auxSu->sig;
		}

		if (auxSu == nullptr) {
			MessageBox(hwnd, "Sucursal no encontrada", "AVISO", MB_OK | MB_ICONQUESTION);
		}
		else if (auxSu == inicioSu) {

			inicioSu = inicioSu->sig;

			delete auxSu;

			auxSu = inicioSu;

			MessageBox(hwnd, "Sucursal eliminada correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);

			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");

		}
		else {

			anterior->sig = auxSu->sig;

			delete auxSu;

			auxSu = inicioSu;

			MessageBox(hwnd, "Sucursal eliminada correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);

			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");

		}
	}
}
void modificarSucursal(sucursal* SucMod, HWND hwnd, char buscaSuc[50]) {

	auxSu = inicioSu;

	while (auxSu != nullptr && strcmp(buscaSuc, auxSu->nomSuc) != 0) {
		auxSu = auxSu->sig;
	}

	if (auxSu == nullptr) {
		MessageBox(hwnd, "Sucursal no encontrada.", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else {

		strcpy_s(auxSu->direc, SucMod->direc);
		MessageBox(hwnd, "Sucursal modificada correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);

		auxSu = inicioSu;

		while (auxSu != nullptr) { //actualizamos la listbox
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxSu->nomSuc);
			auxSu = auxSu->sig;
		}
	}

}

//funciones de cliente
void registrarCliente(clientes* nuevo) {

	if (inicioCl == nullptr) {

		inicioCl = new clientes;

		auxCl = inicioCl;

		strcpy_s(inicioCl->nomComp, nuevo->nomComp);
		strcpy_s(inicioCl->phone, nuevo->phone);
		strcpy_s(inicioCl->email, nuevo->email);
		strcpy_s(inicioCl->sucur, nuevo->sucur);
		inicioCl->puntos = nuevo->puntos;

		finalCl = auxCl;
		inicioCl->ant = nullptr;
		inicioCl->sig = nullptr;

	}
	else {

		auxCl = inicioCl;

		while (auxCl->sig != nullptr) {

			auxCl = auxCl->sig;
		}

		auxCl->sig = new clientes;

		auxCl->sig->sig = nullptr;
		auxCl->sig->ant = auxCl;

		auxCl = auxCl->sig;
		finalCl = auxCl;


		strcpy_s(auxCl->nomComp, nuevo->nomComp);
		strcpy_s(auxCl->phone, nuevo->phone);
		strcpy_s(auxCl->email, nuevo->email);
		strcpy_s(auxCl->sucur, nuevo->sucur);
		auxCl->puntos = nuevo->puntos;

	}

}
void leerBinCliente() {

	ifstream leerCliente;

	leerCliente.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\clientes.bin", ios::in | ios::binary);

	if (leerCliente.is_open()) {

		clientes* clienteLeido = new clientes;

		while (!leerCliente.read((char*)clienteLeido, sizeof(clientes)).eof()) {

			if (inicioCl == nullptr) {

				inicioCl = clienteLeido;
				inicioCl->sig = nullptr;
				inicioCl->ant = nullptr;
				auxCl = inicioCl;

			}
			else {
				auxCl->sig = clienteLeido;
				auxCl->sig->ant = auxCl;
				auxCl = auxCl->sig;
				auxCl->sig = nullptr;

			}

			clienteLeido = new clientes;

		}
		leerCliente.close();
		delete clienteLeido;

	}

}
void escribirBinCliente() {

	auxCl = inicioCl;

	ofstream escrCliente;

	escrCliente.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\clientes.bin", ios::out | ios::binary | ios::trunc);

	if (escrCliente.is_open()) {

		while (auxCl != nullptr) {

			escrCliente.write((char*)auxCl, sizeof(clientes));
			auxCl = auxCl->sig;
		}

		escrCliente.close();
	}
}
void eliminarCliente(char clienteEliminar[50], HWND hwnd, int indice) {

	auxCl = inicioCl;

	if (inicioCl == nullptr) {
		MessageBox(hwnd, "Su lista de clientes esta vacia", "AVISO", MB_OK | MB_ICONQUESTION);

	}
	else {

		while (auxCl != nullptr && strcmp(auxCl->nomComp, clienteEliminar) != 0) {
			auxCl = auxCl->sig;
		}

		if (auxCl == nullptr) {
			MessageBox(hwnd, "Cliente no encontrado.", "AVISO", MB_OK | MB_ICONQUESTION);
		}
		else if (auxCl == inicioCl) { //es el primero de la lista

			if (auxCl->sig == nullptr) { //solo hay 1

				delete auxCl;

				inicioCl = nullptr;
				auxCl = inicioCl;
				finalCl = inicioCl;

			}
			else { //hay mas nodos

				inicioCl = auxCl->sig;

				delete auxCl;

				auxCl = inicioCl;
				auxCl->ant = nullptr;

			}
			MessageBox(hwnd, "Cliente eliminado correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);
			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");
			SetDlgItemText(hwnd, IDC_EDIT3, "");
			SetDlgItemText(hwnd, IDC_COMBO1, "");
			SetDlgItemText(hwnd, IDC_EDIT4, "");
		}
		else {

			if (auxCl->sig == nullptr) { //si el nodo es el ultimo

				finalCl = auxCl->ant; //pasamos el puntero del final al nuevo ultimo
				auxCl->ant->sig = nullptr;

			}
			else {

				auxCl->sig->ant = auxCl->ant;
				auxCl->ant->sig = auxCl->sig;

			}

			delete auxCl;

			auxCl = inicioCl;

			MessageBox(hwnd, "Cliente eliminado correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);
			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");
			SetDlgItemText(hwnd, IDC_EDIT3, "");
			SetDlgItemText(hwnd, IDC_COMBO1, "");
			SetDlgItemText(hwnd, IDC_EDIT4, "");


		}

	}

}
void modificarCliente(clientes* CliMod, HWND hwnd, char buscaCli[50]) {

	auxCl = inicioCl;

	while (auxCl != nullptr && strcmp(buscaCli, auxCl->nomComp) != 0) {

		auxCl = auxCl->sig;
	}

	if (auxCl == nullptr) {

		MessageBox(hwnd, "Cliente no encontrado.", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else {

		strcpy_s(auxCl->nomComp, CliMod->nomComp);
		strcpy_s(auxCl->phone, CliMod->phone);
		strcpy_s(auxCl->email, CliMod->email);

		MessageBox(hwnd, "Cliente modificado correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);

		auxCl = inicioCl;

		while (auxCl != nullptr) { //actualizamos la listbox
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxCl->nomComp);
			auxCl = auxCl->sig;
		}

	}


}

//funciones de promociones
void registrarPromo(promocion* nuevo) {

	if (inicioPr == nullptr) {

		inicioPr = new promocion;

		auxPr = inicioPr;

		strcpy_s(inicioPr->nomProm, nuevo->nomProm);
		inicioPr->puntReq = nuevo->puntReq;
		inicioPr->porcent = nuevo->porcent;
		inicioPr->status = nuevo->status;

		finalPr = auxPr;
		inicioPr->ant = nullptr;
		inicioPr->sig = nullptr;

	}
	else {

		auxPr = inicioPr;

		while (auxPr->sig != nullptr) {

			auxPr = auxPr->sig;

		}

		auxPr->sig = new promocion;

		auxPr->sig->sig = nullptr;
		auxPr->sig->ant = auxPr;

		auxPr = auxPr->sig;
		finalPr = auxPr;

		strcpy_s(auxPr->nomProm, nuevo->nomProm);
		auxPr->puntReq = nuevo->puntReq;
		auxPr->porcent = nuevo->porcent;
		auxPr->status = nuevo->status;

	}


}
void leerBinPromo() {

	fstream leerPromo;

	leerPromo.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\promociones.bin", ios::in | ios::binary);

	if (leerPromo.is_open()) {

		promocion* promoLeido = new promocion;

		while (!leerPromo.read((char*)promoLeido, sizeof(promocion)).eof()) {

			if (inicioPr == nullptr) {

				inicioPr = promoLeido;
				inicioPr->sig = nullptr;
				inicioPr->ant = nullptr;
				auxPr = inicioPr;

			}
			else {
				auxPr->sig = promoLeido;
				auxPr->sig->ant = auxPr;
				auxPr = auxPr->sig;
				auxPr->sig = nullptr;

			}

			promoLeido = new promocion;

		}
		leerPromo.close();
		delete promoLeido;

	}

}
void escribirBinPromo() {

	auxPr = inicioPr;

	ofstream escrPromo;

	escrPromo.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\promociones.bin", ios::out | ios::binary | ios::trunc);

	if (escrPromo.is_open()) {

		while (auxPr != nullptr) {
			escrPromo.write((char*)auxPr, sizeof(promocion));
			auxPr = auxPr->sig;
		}

		escrPromo.close();
	}

}
void eliminarPromocion(char promoEliminar[50], HWND hwnd, int indice) {

	auxPr = inicioPr;

	if (inicioPr == nullptr) {
		MessageBox(hwnd, "Su lista de promociones esta vacia", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else {

		while (auxPr != nullptr && strcmp(auxPr->nomProm, promoEliminar) != 0) {
			auxPr = auxPr->sig;
		}

		if (auxPr == nullptr) {
			MessageBox(hwnd, "Promocion no encontrada.", "AVISO", MB_OK | MB_ICONQUESTION);
		}
		else if (auxPr == inicioPr) { //primero de lista

			if (auxPr->sig == nullptr) { //solo 1

				delete auxPr;

				inicioPr = nullptr;
				auxPr = inicioPr;
				finalPr = inicioPr;

			}
			else { //hay mas nodos

				inicioPr = auxPr->sig;

				delete auxPr;

				auxPr = inicioPr;
				auxPr->ant = nullptr;

			}
			MessageBox(hwnd, "Promocion eliminada correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);
			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");
			SetDlgItemText(hwnd, IDC_EDIT3, "");
			SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

		}
		else {

			if (auxPr->sig == nullptr) { //si es el ultimo

				finalPr = auxPr->ant;
				auxPr->ant->sig = nullptr;

			}
			else {

				auxPr->sig->ant = auxPr->ant;
				auxPr->ant->sig = auxPr->sig;

			}

			delete auxPr;

			auxPr = inicioPr;

			MessageBox(hwnd, "Promocion eliminada correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);
			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");
			SetDlgItemText(hwnd, IDC_EDIT3, "");
			SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

		}

	}

}
void modificarPromocion(promocion* PromMod, HWND hwnd, char buscaProm[50]) {

	auxPr = inicioPr;

	while (auxCl != nullptr && strcmp(buscaProm, auxPr->nomProm) != 0) {

		auxPr = auxPr->sig;
	}

	if (auxPr == nullptr) {
		MessageBox(hwnd, "Promocion no encontrada.", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else {

		auxPr->puntReq = PromMod->puntReq;
		auxPr->porcent = PromMod->porcent;
		auxPr->status = PromMod->status;

		MessageBox(hwnd, "Promocion modificada correctamente.", "AVISO", MB_OK | MB_ICONINFORMATION);
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);

		auxPr = inicioPr;

		while (auxPr != nullptr) { //actualizamos la listbox
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (LPARAM)0, (LPARAM)auxPr->nomProm);
			auxPr = auxPr->sig;
		}

	}


}

//funciones de generar reportes
void ReportePuntosCSV() {

	auxCl = inicioCl;

	ofstream escribir;

	escribir.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\ReportePuntos.csv", ios::out | ios::trunc);

	if (escribir.is_open()) {

		escribir << "Nombre del cliente" << "," << "Puntos" << endl;

		while (auxCl != nullptr) {

			if (auxCl->puntos > 0) {
				escribir << auxCl->nomComp << "," << auxCl->puntos << endl;
			}
			auxCl = auxCl->sig;
		}

		escribir.close();

	}
}
void ReporteMontosCSV() {

	auxCl = inicioCl;

	ofstream escribir;

	escribir.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\ReporteMontos.csv", ios::out | ios::trunc);

	if (escribir.is_open()) {

		escribir << "Nombre del cliente" << "," << "Monto total" << endl;

		while (auxCl != nullptr) {

			escribir << auxCl->nomComp << "," << auxCl->montototal << endl;
			auxCl = auxCl->sig;
		}

		escribir.close();

	}
}
void ReporteCanjesCSV() {

	auxPr = inicioPr;

	ofstream escribir;

	escribir.open("C:\\Users\\jpfb0\\source\\repos\\PROYECTO PROG AVANZADA\\ReporteCanjes.csv", ios::out | ios::trunc);

	if (escribir.is_open()) {

		escribir << "Nombre de la promocion" << "," << "Veces canjeada" << endl;

		while (auxPr != nullptr) {

			escribir << auxPr->nomProm << "," << auxPr->vecesCanjeada << endl;
			auxPr = auxPr->sig;
		}

		escribir.close();

	}
}

//ordenamientos
void OrdenarPuntos() {

	auxCl = inicioCl;
	clientes* next = nullptr;
	clientes temp;
	clientes* i = nullptr;

	while (auxCl != nullptr && i != inicioCl) {

		next = auxCl->sig;
		while (next != nullptr) {

			if (auxCl->puntos < next->puntos) {
				temp = *next;
				*next = *auxCl;
				*auxCl = temp;

				next->sig = auxCl->sig;
				auxCl->sig = next;

				auxCl->ant = next->ant;
				next->ant = auxCl;

			}
			next = next->sig;
			auxCl = auxCl->sig;
		}
		if (i == nullptr) {
			i = auxCl;
		}
		else {
			i = i->ant;
		}
		auxCl = inicioCl;

	}

}
void OrdenarMontos() {

	auxCl = inicioCl;
	clientes* next = nullptr;
	clientes temp;
	clientes* i = nullptr;

	while (auxCl != nullptr && i != inicioCl) {

		next = auxCl->sig;
		while (next != nullptr) {

			if (auxCl->montototal < next->montototal) {
				temp = *next;
				*next = *auxCl;
				*auxCl = temp;

				next->sig = auxCl->sig;
				auxCl->sig = next;

				auxCl->ant = next->ant;
				next->ant = auxCl;

			}
			next = next->sig;
			auxCl = auxCl->sig;
		}
		if (i == nullptr) {
			i = auxCl;
		}
		else {
			i = i->ant;
		}
		auxCl = inicioCl;

	}

}
void OrdenarCanjes() {

	auxPr = inicioPr;
	promocion* next = nullptr;
	promocion temp;
	promocion* i = nullptr;

	while (auxPr != nullptr && i != inicioPr) {

		next = auxPr->sig;
		while (next != nullptr) {

			if (auxPr->vecesCanjeada < next->vecesCanjeada) {
				temp = *next;
				*next = *auxPr;
				*auxPr = temp;

				next->sig = auxPr->sig;
				auxPr->sig = next;

				auxPr->ant = next->ant;
				next->ant = auxPr;

			}
			next = next->sig;
			auxPr = auxPr->sig;
		}
		if (i == nullptr) {
			i = auxPr;
		}
		else {
			i = i->ant;
		}
		auxPr = inicioPr;

	}

}

//validaciones
bool validarNomCom(char nombre[52]) {

	bool largo = false;
	bool letra = false;


	if (strlen(nombre) >= 5 && strlen(nombre) <= 50) {

		largo = true;

	}

	for (int i = 0; i < strlen(nombre); i++) {

		if (isalpha(nombre[i])) {

			letra = true;

		}
	}

	for (int i = 0; i < strlen(nombre); i++) {

		if (isdigit(nombre[i])) {

			return false;

		}
	}

	for (int i = 0; i < strlen(nombre); i++) {

		if (ispunct(nombre[i])) {
			return false;
		}
	}

	if (largo && letra) {

		return true;
	}
	else {

		return false;
	}


}
bool validarContra(char password[12]) {
	bool largo = false;
	bool letra = false;
	bool numero = false;
	bool caracter = false;


	if (strlen(password) >= 3 && strlen(password) <= 10) {

		largo = true;

	}

	for (int i = 0; i < strlen(password); i++) {

		if (isalpha(password[i])) {

			letra = true;

		}
	}

	for (int i = 0; i < strlen(password); i++) {

		if (isdigit(password[i])) {

			numero = true;

		}
	}

	for (int i = 0; i < strlen(password); i++) {

		if (ispunct(password[i])) {

			caracter = true;

		}
	}

	if (largo && letra && numero && caracter) {

		return true;
	}
	else {

		return false;
	}

}
bool validarUsuario(char usuario[22]) {

	bool largo = false;

	if (strlen(usuario) >= 5 && strlen(usuario) <= 20) {

		largo = true;

	}

	for (int i = 0; i < strlen(usuario); i++) {

		if (ispunct(usuario[i])) {
			return false;
		}
	}

	if (largo) {

		return true;
	}
	else {

		return false;
	}

}
bool validarTelefono(char telefono[12]) {

	bool largo = false;
	bool numero = false;

	if (strlen(telefono) == 10) {
		largo = true;
	}

	for (int i = 0; i < strlen(telefono); i++) {

		if (ispunct(telefono[i])) {

			return false;

		}
	}

	for (int i = 0; i < strlen(telefono); i++) {

		if (isalpha(telefono[i])) {

			return false;

		}
	}

	for (int i = 0; i < strlen(telefono); i++) {

		if (isdigit(telefono[i])) {

			numero = true;

		}
	}

	if (largo && numero) {
		return true;

	}
	else {

		return false;
	}


}
bool validarCorreo(char correo[50]) {

	bool largo = false;
	bool arroba = false;

	if (strlen(correo) >= 5 && strlen(correo) <= 50) {

		largo = true;

	}

	for (int i = 0; i < strlen(correo); i++) {

		if (correo[i] == 64) {

			arroba = true;
		}

	}

	if (largo && arroba) {

		return true;
	}
	else {
		return false;

	}

}
bool validarDireccion(char direccion[52]) {

	for (int i = 0; i < strlen(direccion); i++) {

		for (int i = 0; i < strlen(direccion); i++) {

			if (ispunct(direccion[i])) {
				return false;
			}
		}

	}

	return true;

}

//limitacion de cajero
void LimitarCajero(HWND hwnd) {
	DeleteMenu(GetMenu(hwnd), ID_USUARIOS_REGISTRO, MF_BYCOMMAND);
	DeleteMenu(GetMenu(hwnd), ID_PROMOCIONES_REGISTRO, MF_BYCOMMAND);
	DeleteMenu(GetMenu(hwnd), ID_SUCURSALES_REGISTRO, MF_BYCOMMAND);
	DeleteMenu(GetMenu(hwnd), ID_REPORTES_PUNTOS, MF_BYCOMMAND);
	DeleteMenu(GetMenu(hwnd), ID_REPORTES_PROMOCIONES, MF_BYCOMMAND);
	DeleteMenu(GetMenu(hwnd), ID_REPORTES_MONTOTOTAL, MF_BYCOMMAND);
}

//menu
void menu(WPARAM wParam, HWND hwnd) {

	switch (LOWORD(wParam)) {

	case ID_REPORTES_PROMOCIONES: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(REPORTE_PROMCANJ), hwnd, ReportePromociones);
	}break;

	case ID_REPORTES_MONTOTOTAL: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(REPORTE_MONTOS), hwnd, ReporteMontos);
	}break;

	case ID_REPORTES_PUNTOS: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(REPORTE_PUNTOS), hwnd, ReportePuntos);
	}break;

	case ID_MENUPRINCIPAL_CERRARSESION: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(INICIO_SESION), hwnd, InicioSesion);

	}break;

	case ID_USUARIOS_REGISTRO: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(USU_REGISTRO), hwnd, procRegUsu);
	}break;

	case ID_USUARIOS_LISTA: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(USU_LISTA), hwnd, procListUsu);
	}break;

	case ID_SUCURSALES_REGISTRO: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(SUC_REGISTRO), hwnd, procRegSuc);
	}break;

	case ID_SUCURSALES_LISTA: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(SUC_LISTA), hwnd, procListSuc);
	}break;

	case ID_CLIENTES_REGISTRO: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CLI_REGISTRO), hwnd, procRegCli);
	}break;

	case ID_CLIENTES_LISTA: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CLI_LISTA), hwnd, procListCli);
	}break;

	case ID_PROMOCIONES_REGISTRO: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(PROM_REGISTRO), hwnd, procRegProm);
	}break;

	case ID_PROMOCIONES_LISTA: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(PROM_LISTA), hwnd, procListProm);

	}break;

	case ID_MENUPRINCIPAL_IR: {

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(MENU_PRINCIPAL), hwnd, MenuPrincipal);

	}break;

	case ID_PUNTOS_CAPTURA: {

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CAPTURA_PUNTOS), hwnd, procCapturaPuntos);

	}break;

	case ID_PUNTOS_CANJE: {

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CANJE_PUNTOS), hwnd, procCanjePuntos);

	}break;


	}//switch

}