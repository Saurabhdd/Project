#include<windows.h>
#include<windowsx.h>
#include<stdio.h>
#include<stdlib.h>
#include"Header.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DataEntryDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SearchDataDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SearchResultDlgProc(HWND, UINT, WPARAM, LPARAM);


IReadRecordFile *pIRead = NULL;
IWriteRecordFIle *pIWrite = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	MSG msg;
	HWND hwnd;
	WNDCLASSEX wndClass;
	TCHAR szAppName[] = TEXT("MyApp");
	HRESULT hr;


	hr = CoInitialize(NULL);

	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("Com Library Can Not Be Initialized"), TEXT("Error"), MB_OK);
		exit(0);
	}

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(100));
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.hInstance = hInstance;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(szAppName, "MyIcon", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, hwnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	CoUninitialize();

	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HDC hmdc;
	static HBITMAP hBitmap;
	BITMAP bitmap;
	RECT rc;
	HRESULT hr;
	HINSTANCE hInstance;


	switch (iMsg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		hmdc = CreateCompatibleDC(NULL);
		SelectObject(hmdc, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		StretchBlt(hdc, rc.left, rc.top, rc.right, rc.bottom, hmdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
		DeleteObject(hmdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_CREATE:

		hr = CoCreateInstance(CLSID_ReadWrite, NULL, CLSCTX_INPROC_SERVER, IID_IRead, (void **)&pIRead);


		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("IRead Interface can not be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		//hr = pIRead->ReadRecord((void*)&searchResult, (void*)&resultSet);

		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("read record has issues"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		

		hr = pIRead->QueryInterface(IID_IWrite, (void**)&pIWrite);

		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("IWrite Interface can not be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTATOM(MYBITMAP));

		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_SPACE:
			ShowWindow(hwnd, SW_HIDE);
			//hIntance =(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);

			hInstance = GetModuleHandle(NULL);
			DialogBox(hInstance, "Select", hwnd, (DLGPROC)MyDlgProc);

			//DestroyWindow(hwnd);

			break;

		}
		
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndclass;
	TCHAR szAppName[] = TEXT("COM");
	HWND hwnd;
	MSG msg;

	HRESULT hr;


	hr = CoInitialize(NULL);

	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("Com Library Can Not Be Initialized"), TEXT("Error"), MB_OK);
		exit(0);
	}


	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.lpszMenuName = NULL;

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName,
		TEXT("ClassFactory Client"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();
	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {



	int Num1, Num2, Num3;
	FILE* fData;


	static HBITMAP hbtmp;
	RECT rc;
	PAINTSTRUCT ps;
	HDC hdc1;
	HDC hdc2;
	BITMAP bitmap;

	HINSTANCE hInstance;

	HRESULT hr;
	RESULTSET resultSet;
	HANDLE handle;
	//ATTRIBUTES searchResult = { "saurabh", NULL, NULL };
	//searchResult.user_id = 101;
	TCHAR str[255];
	//DATAENTRY dataEntry = { "saurabh", "deshpande", "sa", 106, 102, 103, 104, 0, 0, 0 };
	//BOOL writeRecord;

	switch (iMsg) {
	case WM_CREATE:
		hr = CoCreateInstance(CLSID_ReadWrite, NULL, CLSCTX_INPROC_SERVER, IID_IRead, (void **)&pIRead);


		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("IRead Interface can not be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		//hr = pIRead->ReadRecord((void*)&searchResult, (void*)&resultSet);

		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("read record has issues"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		wsprintf(str, "result brought is  : %d", resultSet.ResultSet[0].department_id);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);


		//hbtmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(MYBITMAP));

		hr = pIRead->QueryInterface(IID_IWrite, (void**)&pIWrite);

		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("IWrite Interface can not be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		hbtmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTATOM(MYBITMAP));
		//pIWrite->WriteRecord((void*)&dataEntry, &writeRecord);
		//pIRead->Release();
		//pIRead = NULL;
		//pIWrite->Release();
		//pIWrite = NULL;


		break;




	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_SPACE:
			ShowWindow(hwnd, SW_HIDE);
			//hIntance =(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);

			hInstance = GetModuleHandle(NULL);
			DialogBox(hInstance, "Select", hwnd, (DLGPROC)MyDlgProc);

			//DestroyWindow(hwnd);

			break;


		case WM_PAINT:
			GetClientRect(hwnd, &rc);
			hdc1 = BeginPaint(hwnd, &ps);
			hdc2 = CreateCompatibleDC(NULL);
			
			handle = SelectObject(hdc2, hbtmp);
			GetObject(hbtmp, sizeof(BITMAP), &bitmap);
			StretchBlt(hdc1, rc.left, rc.top, rc.right, rc.bottom, hdc2, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

			DeleteObject(hdc2);
			EndPaint(hwnd, &ps);

		

			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
*/

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd;
	HINSTANCE hInstance;
	TCHAR firstWindowMessage[] = TEXT("dataEntry");
	hwnd = (HWND)GetWindowLongPtr(hDlg, GWLP_HWNDPARENT);
	hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);

	switch (iMsg)
	{


	case WM_INITDIALOG:

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			//SendMessage(hwnd, WM_PAINT, 0, (LPARAM)(LPCSTR)firstWindowMessage);

			ShowWindow(hwnd, SW_HIDE);
			hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);
			DialogBox(hInstance, "DATAENTRY", hwnd, (DLGPROC)DataEntryDlgProc);
			EndDialog(hDlg, 0);
			break;



		case IDCANCEL:

			DialogBox(hInstance, "SearchData", hwnd, (DLGPROC)SearchDataDlgProc);
			//DestroyWindow(hDlg);
			EndDialog(hDlg, 0);
			break;

		}
		return TRUE;

	case WM_DESTROY:
		DestroyWindow((HWND)GetWindowLong(hDlg, GWLP_HWNDPARENT));
		break;
	}
	return FALSE;


}

BOOL CALLBACK DataEntryDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	DATAENTRY dataEntry;
	DATAENTRY dataSpotEntry;
	ATTRIBUTES searchResult, spotResult, searchResult_username;
	static char sex;
	static int Male, Female;
	RESULTSET resultSet;
	HRESULT hr;
	int i, counter_checkEntries = 0;
	BOOL department_flag, userid_flag, name_flag, surname_flag, sex_flag, age_flag, address_flag, date_flag, month_flag, year_flag;
	BOOL dataEntry_flag;
	BOOL spotEntry_flag;
	BOOL dataENtry_flag_username;
	static HBRUSH hbr1;

	HWND hwnd = NULL;

	department_flag = FALSE;
	userid_flag = FALSE;
	name_flag = FALSE;
	surname_flag = FALSE;
	sex_flag = FALSE;
	age_flag = FALSE;
	address_flag = FALSE;
	dataEntry_flag = FALSE;
	date_flag = FALSE;
	month_flag = FALSE;
	year_flag = FALSE;
	counter_checkEntries = 0;


	switch (iMsg)
	{
	case WM_CTLCOLORSTATIC:
		hbr1 = CreateSolidBrush(RGB(255, 255, 0));

		SetBkColor((HDC)wParam, RGB(255, 255, 0));

		if ((HWND)lParam == GetDlgItem(hDlg, ID_DATEOFJOIN)) {
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
		}
		break;

	case WM_INITDIALOG:

		//MessageBox(NULL, TEXT("init is pressed"), TEXT("result"), MB_OK);

		return TRUE;



	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case ID_ETSURNAME:
			switch (HIWORD(wParam))
			{
			case EN_UPDATE:
				//memset(spotResult.name, '\0', sizeof(spotResult.name));
				searchResult.department_id = NULL;
				spotResult.user_id = NULL;
				spotResult.department_id = NULL;
				SetDlgItemText(hDlg, ID_SPOTMESSAGE, "");
				GetDlgItemText(hDlg, ID_ETSURNAME, spotResult.name, 20);
				//strcpy_s(spotResult.name, dataSpotEntry.surname);
				hr = pIRead->SpotRead((void*)&spotResult, (void*)&spotEntry_flag);

				if (FAILED(hr))
				{
					MessageBox(NULL, TEXT("cannot spotify"), TEXT("result"), MB_OK);
				}

				if (spotEntry_flag == TRUE) {

					SendDlgItemMessage(hDlg, IDOK, WS_DISABLED, 1, 0);
					hwnd = CreateWindow("static", "Record Already Exists", WS_CHILD | WS_VISIBLE, 450, 90, 150, 20, hDlg, (HMENU)ID_SPOTMESSAGE, GetModuleHandle(NULL), NULL);

					//MessageBox(NULL, TEXT("already existed record"), TEXT("result"), MB_OK);
				}
				break;
			}

			break;

		

		case ID_OK:
			
			//SendDlgItemMessage(hDlg, IDOK, WS_DISABLED, 1, 0);
			dataEntry.age = GetDlgItemInt(hDlg, ID_ETAGE, NULL, TRUE);
			age_flag = TRUE;
			if (dataEntry.age == NULL)
			{
				MessageBox(NULL, TEXT("You need to fill all Fields"), TEXT("Incomplete details"), MB_OK);
				age_flag = FALSE;
				break;
			}


			dataEntry.user_id = GetDlgItemInt(hDlg, ID_USERID, NULL, TRUE);
			userid_flag = TRUE;
			searchResult.user_id = dataEntry.user_id;
			searchResult.department_id = NULL;
			memset(searchResult.name, '\0', sizeof(searchResult.name));

			hr = pIRead->ReadRecord((void*)&searchResult, (void*)&resultSet);

			if (FAILED(hr)) {
				MessageBox(NULL, TEXT("read record has issues"), TEXT("Error"), MB_OK);
				DestroyWindow(hDlg);
			}

			for (i = 0; i < (sizeof(resultSet.ResultSet) / sizeof(resultSet.ResultSet[0])); i++)
			{
				if (resultSet.ResultSet[i].user_id == dataEntry.user_id)
				{
					counter_checkEntries++;
					break;
				}
			}
			if (counter_checkEntries != 0)
			{
				MessageBox(NULL, TEXT("Record Already exist please enter someting else!"), TEXT("Error"), MB_OK);
			}

			if (dataEntry.user_id == NULL)
			{
				MessageBox(NULL, TEXT("You need to fill all Fields"), TEXT("Incomplete details"), MB_OK);
				userid_flag = FALSE;
				break;
			}



			GetDlgItemText(hDlg, ID_ETNAME, dataEntry.name, 20);
			name_flag = TRUE;

			//pIRead->ReadRecord()
			if (dataEntry.name == NULL)
			{
				MessageBox(NULL, TEXT("You need to all Fields"), TEXT("Incomplete details"), MB_OK);
				name_flag = FALSE;
				break;
			}


			GetDlgItemText(hDlg, ID_ETSURNAME, dataEntry.surname, 20);
			GetDlgItemText(hDlg, ID_ETSURNAME, searchResult_username.name, 20);
			surname_flag = TRUE;
			hr = pIRead->SpotRead((void*)&searchResult_username, (void*)&dataENtry_flag_username);

			if (FAILED(hr))
			{
				MessageBox(NULL, TEXT("failed search"), TEXT("error"), MB_OK);
			}
			if (dataEntry.surname == NULL)
			{
				MessageBox(NULL, TEXT("You need to all Fields"), TEXT("Incomplete details"), MB_OK);
				surname_flag = FALSE;
				break;
			}


			GetDlgItemText(hDlg, ID_ETADDRESS, dataEntry.city, 30);
			address_flag = TRUE;
			if (dataEntry.city == NULL)
			{
				MessageBox(NULL, TEXT("You need to all Fields"), TEXT("Incomplete details"), MB_OK);
				address_flag = FALSE;
				break;
			}


			dataEntry.department_id = GetDlgItemInt(hDlg, ID_DEPARTMENT, NULL, TRUE);
			department_flag = TRUE;
			if (dataEntry.department_id == NULL)
			{
				MessageBox(NULL, TEXT("You need to all Fields"), TEXT("Incomplete details"), MB_OK);
				department_flag = FALSE;
				break;
			}


			dataEntry.date = GetDlgItemInt(hDlg, ID_DD, NULL, TRUE);
			if (dataEntry.date > 31 || dataEntry.month < 0)
			{
				MessageBox(NULL, TEXT("Incorrect date"), TEXT("Incomplete details"), MB_OK);
				date_flag = FALSE;
				break;
			}
			date_flag = TRUE;
			if (dataEntry.date == NULL)
			{
				MessageBox(NULL, TEXT("You need to all Fields"), TEXT("Incomplete details"), MB_OK);
				date_flag = FALSE;
				break;
			}



			dataEntry.month = GetDlgItemInt(hDlg, ID_MM, NULL, TRUE);
			if (dataEntry.month > 12 || dataEntry.month < 0)
			{
				MessageBox(NULL, TEXT("Incorrect month"), TEXT("Incomplete details"), MB_OK);
				month_flag = FALSE;
				break;
			}
			month_flag = TRUE;
			if (dataEntry.month == NULL)
			{
				MessageBox(NULL, TEXT("You need to all Fields"), TEXT("Incomplete details"), MB_OK);
				month_flag = FALSE;
				break;
			}

			dataEntry.year = GetDlgItemInt(hDlg, ID_YYYY, NULL, TRUE);
			year_flag = TRUE;
			if (dataEntry.year == NULL)
			{
				MessageBox(NULL, TEXT("You need to all Fields"), TEXT("Incomplete details"), MB_OK);
				year_flag = FALSE;
				break;
			}

			Male = SendDlgItemMessage(hDlg, ID_MALE, BM_GETCHECK, 1, 0);
			Female = SendDlgItemMessage(hDlg, ID_FEMALE, BM_GETCHECK, 1, 0);
			//ShowWindow(GetDlgItem(hDlg, ID_MALE), 1);
			//ShowWindow(GetDlgItem(hDlg, ID_FEMALE), 1);
			sex_flag = TRUE;

			if (Male != 0 || Female != 0) {
				if (Male == 1 && Female == 0) {

					dataEntry.sex = 1;
				}
				else {

					dataEntry.sex = 0;
				}
			}
			else {
				MessageBox(NULL, TEXT("Please Fill All Fields"), TEXT("Incomplete Form"), MB_OK);
				sex_flag = FALSE;
				break;
			}

			MessageBox(NULL, TEXT("check before entry"), TEXT("Error"), MB_OK);
			if (userid_flag == TRUE && dataENtry_flag_username == FALSE)
			{
				//MessageBox(NULL, TEXT("inside flag"), TEXT("Error"), MB_OK);
				if (counter_checkEntries == 0)
					hr = pIWrite->WriteRecord((void*)&dataEntry, (BOOL*)&dataEntry_flag);

				if (FAILED(hr)) {
					MessageBox(NULL, TEXT("Not able to write this record"), TEXT("Error"), MB_OK);
					DestroyWindow(hDlg);
				}

				SetDlgItemText(hDlg, ID_ETNAME, "");
				SetDlgItemText(hDlg, ID_USERID, "");
				SetDlgItemText(hDlg, ID_DEPARTMENT, "");
				SetDlgItemText(hDlg, ID_ETSURNAME, "");
				SetDlgItemText(hDlg, ID_ETADDRESS, "");
				SetDlgItemText(hDlg, ID_ETAGE, "");
				SetDlgItemText(hDlg, ID_DD, "");
				SetDlgItemText(hDlg, ID_MM, "");
				SetDlgItemText(hDlg, ID_YYYY, "");
				SendDlgItemMessage(hDlg, ID_MALE, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hDlg, ID_FEMALE, BM_SETCHECK, 0, 0);

			}
			else {
				MessageBox(NULL, TEXT("Usename already exists please change this!"), TEXT("Error"), MB_OK);
			}
			break;

		case ID_PBCONTINUE:

			SetDlgItemText(hDlg, ID_ETNAME, "");
			SetDlgItemText(hDlg, ID_USERID, "");
			SetDlgItemText(hDlg, ID_DEPARTMENT, "");
			SetDlgItemText(hDlg, ID_ETSURNAME, "");
			SetDlgItemText(hDlg, ID_ETADDRESS, "");
			SetDlgItemText(hDlg, ID_ETAGE, "");
			SendDlgItemMessage(hDlg, ID_MALE, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hDlg, ID_FEMALE, BM_SETCHECK, 0, 0);
			SetDlgItemText(hDlg, ID_DD, "");
			SetDlgItemText(hDlg, ID_MM, "");
			SetDlgItemText(hDlg, ID_YYYY, "");
			//ShowWindow(GetDlgItem(hDlg, ID_MALE), 1);
			//ShowWindow(GetDlgItem(hDlg, ID_FEMALE), 1);
			break;

		case IDCANCEL:
			hwnd = (HWND)GetWindowLongPtr(hDlg, GWLP_HWNDPARENT);
			DestroyWindow(hwnd);
			break;

		}

		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK SearchDataDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	RESULTSET resultSet;
	ATTRIBUTES search_Attributes;
	BOOL userId_flag, name_flag, departmentId_flag;
	HRESULT hr;
	userId_flag = FALSE;
	departmentId_flag = FALSE;
	name_flag = FALSE;
	HWND hwnd, hs;
	int counter_checkEntries = 0, i = 0;
	HINSTANCE hInstance;
	const char MALE[10] = "Male";
	const char FEMALE[10] = "Female";
	TCHAR firstWindowMessage[] = TEXT("dataSearch");
	hwnd = (HWND)GetWindowLongPtr(hDlg, GWLP_HWNDPARENT);
	hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);
	int x = 10;
	int y = 20;
	int w = 750;
	int h = 20, j;
	char str[200];
	char gender[10] = "0";


	search_Attributes.department_id = NULL;
	search_Attributes.user_id = NULL;
	memset(search_Attributes.name, '\0', sizeof(search_Attributes.name));

	switch (iMsg)
	{
	case WM_INITDIALOG:
		EnableWindow(GetDlgItem(hDlg, ID_STCONTINUE), FALSE);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_STOK:
			
			GetDlgItemText(hDlg, ID_STNAME, search_Attributes.name, 30);
			name_flag = TRUE;
			if (search_Attributes.name == NULL)
			{
				name_flag = FALSE;
				
			}


			search_Attributes.user_id = GetDlgItemInt(hDlg, ID_STUSERID, NULL, TRUE);
			userId_flag = TRUE;
			if (search_Attributes.user_id == NULL)
			{
				userId_flag = FALSE;
				
			}


			search_Attributes.department_id = GetDlgItemInt(hDlg, ID_STDEPARTMENT, NULL, TRUE);
			departmentId_flag = TRUE;
			if (search_Attributes.department_id == NULL)
			{
				departmentId_flag = FALSE;
				
			}

			if (userId_flag == TRUE || departmentId_flag == TRUE || name_flag == TRUE)
			{
				hr = pIRead->ReadRecord((void*)&search_Attributes, (void*)&resultSet);
				if (FAILED(hr))
				{
					MessageBox(NULL, TEXT("Cannot read record!"), TEXT("error"), MB_OK);
					DestroyWindow(hDlg);
				}

				//disabling windows for search result display
				ShowWindow(GetDlgItem(hDlg, ID_STNAME), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_STUSERID), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_STDEPARTMENT), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_SEARCHCRIT), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_SLNAME), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_SLUSERID), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_SLDEPARTMENTID), SW_HIDE);
				EnableWindow(GetDlgItem(hDlg, ID_STOK), FALSE);

				//EnableWindow(GetDlgItem(hDlg, ID_STNAME), FALSE);
				EnableWindow(GetDlgItem(hDlg, ID_STCONTINUE), TRUE);


				counter_checkEntries = resultSet.dataEntry_counter;
				if (counter_checkEntries > 0)
				{
					if (counter_checkEntries >= 1)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s       %s       %s       %d       %d        %d      %d-%d-%d       %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD1, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 2)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (FEMALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s        %s        %s        %d        %d       %d       %d-%d-%d       %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD2, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));

						i++;
					}
					if (counter_checkEntries >= 3)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s        %s        %s        %d         %d        %d        %d-%d-%d       %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD3, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 4)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s        %s        %S        %d         %d        %d        %d-%d-%d        %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD4, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 5)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s         %s       %S        %d        %d        %d        %d-%d-%d        %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD5, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 6)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s           %s           %S           %d         %d         %d        %d-%d-%d         %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD6, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 7)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD7, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 8)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD8, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 9)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD9, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 10)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD10, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 11)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD11, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 12)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD12, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 13)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD13, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 14)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD14, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 15)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD15, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 16)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD16, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 17)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD17, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 18)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD18, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}
					if (counter_checkEntries >= 19)
					{
						if (resultSet.ResultSet[i].sex == 1)
						{
							for (j = 0;; j++)
							{
								gender[j] = MALE[j];
								if (MALE[j] == '\0')

									break;
							}
						}
						else
							for (j = 0;; j++)
							{
								gender[j] = FEMALE[j];
								if (MALE[j] == '\0')

									break;
							}
						wsprintf(str, "%s     %s     %S     %d      %d     %d     %d-%d-%d      %s ", resultSet.ResultSet[i].name, resultSet.ResultSet[i].surname, gender, resultSet.ResultSet[i].user_id, resultSet.ResultSet[i].department_id, resultSet.ResultSet[i].age, resultSet.ResultSet[i].date, resultSet.ResultSet[i].month, resultSet.ResultSet[i].year, resultSet.ResultSet[i].city);
						hwnd = CreateWindow("static", str, WS_CHILD | WS_VISIBLE, x, y, w, h, hDlg, (HMENU)ID_RECORD19, GetModuleHandle(NULL), NULL);
						y = y + 20;
						memset(gender, '\0', sizeof(gender));
						memset(str, '\0', sizeof(str));
						i++;
					}

				}
				else
				{
					MessageBox(NULL, TEXT("No records to display"), TEXT("info"), MB_OK);
				}


				for (i = 0; i < (sizeof(resultSet.ResultSet) / sizeof(resultSet.ResultSet[0])); i++)
				{
					resultSet.ResultSet[i].age = NULL;
					resultSet.ResultSet[i].date = NULL;
					resultSet.ResultSet[i].month = NULL;
					resultSet.ResultSet[i].year = NULL;
					resultSet.ResultSet[i].sex = NULL;
					resultSet.ResultSet[i].department_id = NULL;
					resultSet.ResultSet[i].user_id = NULL;
					memset(resultSet.ResultSet[i].name, '\0', sizeof(resultSet.ResultSet[i].name));
					memset(resultSet.ResultSet[i].city, '\0', sizeof(resultSet.ResultSet[i].city));
					memset(resultSet.ResultSet[1].surname, '\0', sizeof(resultSet.ResultSet[1].surname));

				}


				//CreateWindow()
				//DialogBox(hInstance, "SearchResult", hwnd, (DLGPROC)SearchResultDlgProc);
				//hs = (HWND)SendMessage(GetWindow(hwnd, GW_CHILD),WM_CREATE, NULL, (LPARAM)&resultSet);


			}
			else
			{
				MessageBox(NULL, TEXT("Please enter at least one search attribute!"), TEXT("Incomplete request"), MB_OK);
			}

			break;

			case ID_STCONTINUE:
				ShowWindow(GetDlgItem(hDlg, ID_STNAME), SW_SHOW);
				ShowWindow(GetDlgItem(hDlg, ID_STUSERID), SW_SHOW);
				ShowWindow(GetDlgItem(hDlg, ID_STDEPARTMENT), SW_SHOW);
				ShowWindow(GetDlgItem(hDlg, ID_SEARCHCRIT), SW_SHOW);
				ShowWindow(GetDlgItem(hDlg, ID_SLNAME), SW_SHOW);
				ShowWindow(GetDlgItem(hDlg, ID_SLUSERID), SW_SHOW);
				ShowWindow(GetDlgItem(hDlg, ID_SLDEPARTMENTID), SW_SHOW);
				//ShowWindow(GetDlgItem(hDlg, ID_STOK), SW_SHOW);

				DestroyWindow(GetDlgItem(hDlg, ID_RECORD1));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD2));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD3));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD4));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD5));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD6));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD7));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD8));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD9));
				DestroyWindow(GetDlgItem(hDlg, ID_RECORD10));

				/*ShowWindow(GetDlgItem(hDlg, ID_RECORD1), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD2), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD3), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD4), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD5), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD6), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD7), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD8), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD9), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD10), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD11), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD12), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD13), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD14), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD15), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD16), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD17), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD18), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ID_RECORD19), SW_HIDE);
				*/

				EnableWindow(GetDlgItem(hDlg, ID_STOK), TRUE);
				EnableWindow(GetDlgItem(hDlg, ID_STCONTINUE), FALSE);

				SetDlgItemText(hDlg, ID_STNAME, "");
				SetDlgItemText(hDlg, ID_STUSERID, "");
				SetDlgItemText(hDlg, ID_STDEPARTMENT, "");
				break;


			case ID_STCANCEL:
				hwnd = (HWND)GetWindowLongPtr(hDlg, GWLP_HWNDPARENT);
				DestroyWindow(hwnd);
				break;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CALLBACK SearchResultDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	RESULTSET resultSet;
	RESULTSET RS;
	char str[100];
	switch (iMsg)
	{
	case WM_CREATE:


		resultSet = *(RESULTSET*)lParam;

		wsprintf(str, "result brought is  : %d", resultSet.ResultSet[0].department_id);
		MessageBox(NULL, str, TEXT("Result"), MB_OK);
		Sleep(1000);
		break;
	}

	return FALSE;
}

