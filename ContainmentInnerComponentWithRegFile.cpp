#pragma once
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ContainmentInnerComponentWithRegFile.h"


class CReadWrite : public IReadRecordFile, IWriteRecordFIle
{
private:
	long m_cRef;
public:
	//constructor
	CReadWrite();

	//Destructor
	~CReadWrite();

	//IUnknown Specific method declaration
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//IReadRecord specific method
	HRESULT __stdcall ReadRecord(void*, void*);
	HRESULT __stdcall SpotRead(void*, void*);

	//IWriteRecord specific method
	HRESULT __stdcall WriteRecord(void*, BOOL*);

	HRESULT __stdcall Validation(DATAENTRY*, BOOL*);
	HRESULT __stdcall DataEntry(DATAENTRY*);
	HRESULT __stdcall IsEqual(int, int, BOOL*);
	HRESULT __stdcall IsStringEqual(char[], char[], BOOL*);
	HRESULT __stdcall IsStringEqualSpot(char[], char[], BOOL*);

};

class CReadWriteClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	CReadWriteClassFactory();
	~CReadWriteClassFactory();

	//IUnknown specific method
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//IClassFactory specific method
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);

};

long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//Implementation of Co class

CReadWrite::CReadWrite(void) {
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CReadWrite::~CReadWrite(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);

}

HRESULT CReadWrite::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IReadRecordFile*>(this);
	else if (riid == IID_IRead)
		*ppv = static_cast<IReadRecordFile*>(this);
	else if (riid == IID_IWrite)
		*ppv = static_cast<IWriteRecordFIle*>(this);
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG CReadWrite::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CReadWrite::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}


HRESULT CReadWrite::SpotRead(void *request, void *response)
{

	//MessageBox(NULL, TEXT("Inside read recoord"), TEXT("Error"), MB_OK);


	ATTRIBUTES searchRequest = *(ATTRIBUTES*)request;
	BOOL *flag = (BOOL*)response;
	//RESULTSET *resultSet;
	FILE *fData;
	DATAENTRY dataScan = { "0", "0","0", 0, 0, 0, 0, 0, 0, 0 };


	int i = 0, j = 0, j1 = 0, k = 0, l = 0, m = 0, n = 0, numberOfRecords = 0;
	int jname, jsurname, jadress, spaceflag = 1;
	int dataEntryCounter = 0;
	char Line[100] = "0";
	char Line1[100] = "0";
	char str[100] = "0";
	char str1[100] = "0";
	char username[20] = "0";
	BOOL stringEqualFlag = FALSE;
	BOOL fName = FALSE, fUser_id = FALSE, fDepartment_id = FALSE, fName1 = FALSE, fUser_id1 = FALSE, fDepartment_id1 = FALSE;
	int entry_counter = 0;

	//wsprintf((LPWSTR)str, TEXT("search request is  : %d"), searchRequest.user_id);
	//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);

	//MessageBox(NULL, TEXT("Values assigned"), TEXT("Error"), MB_OK);

	if (searchRequest.name[0] == '0' || searchRequest.name[0] == '\0' || searchRequest.name == NULL)
	{

		fName1 = TRUE;
	}
	if (searchRequest.user_id == 0 || searchRequest.user_id == NULL)
	{
		fUser_id1 = TRUE;
	}
	if (searchRequest.department_id == 0 || searchRequest.department_id == NULL)
	{
		fDepartment_id1 = TRUE;
	}

	for (i = 0; i < 30; i++)
	{
		if (searchRequest.name[i] == '\0')
			break;
		entry_counter++;
	}

	errno_t error = fopen_s(&fData, "E:\\TestFileIO\\FileIO\\FileIO\\CheckText.txt", "a+");

	if (error)
	{
		//printf("Cannot open file\n");
		//MessageBox(NULL, TEXT("Cannot open file"), TEXT("Error"), MB_OK);
	}
	else
	{
		fseek(fData, 0L, SEEK_SET);

		//MessageBox(NULL, TEXT("Before loop"), TEXT("Error"), MB_OK);
		for (i = 0;; i++)
		{
			fscanf_s(fData, "%c", &Line[i], 1);
			
			

			if (Line[i] == '\n')
			{


				for (j = 0, j1 = 0;; j++, j1++)
				{
					Line1[j1] = Line[j];
					if (Line[j] == '\0')
						break;
				}
				for (j = 0, jname = 0, jsurname = 0, jadress = 0, spaceflag = 1;; j++)
				{

					if (spaceflag == 4)
					{
						//dataScan.surname[jsurname++] = Line1[j];
						username[jsurname++] = Line1[j];
						//wsprintf((LPWSTR)str, TEXT("request string is %d and scan string is  and entry_counter is %d  - %c"), entry_counter, jsurname, username[0]);
						//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);
						if ((entry_counter == jsurname))
						{
							searchRequest.name[entry_counter] = '\0';
							username[entry_counter] = '\0';
							
							IsStringEqual(searchRequest.name, username, flag);

							if (*flag == TRUE)
							{

								fclose(fData);
								return S_OK;
							}
						}
					}


					if (Line1[j] == ' ')
					{
						spaceflag++;
					}

					if (Line1[j] == '\n')
						break;

				}




				dataScan.name[0] = 0;
				//printf("%s\n", Line1);

				Line1[0] = 0;
				Line[0] = 0;
				i = -1;
			}

			if (Line[i] == 0)
			{
				//printf("EOF");
				break;
			}
		}
		//MessageBox(NULL, TEXT("Inside read recoord 2 loop executed"), TEXT("Error"), MB_OK);
	}
	fclose(fData);
	//wsprintf((LPWSTR)str, TEXT("result brought is  : %d"), resultSet->ResultSet[1].department_id);
	//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);

	//response = &resultSet;
	//printf("%d\n", resultSet[1].department_id);

	return S_OK;


}



HRESULT CReadWrite::ReadRecord(void *request, void *response)
{


	//MessageBox(NULL, TEXT("Inside read recoord"), TEXT("Error"), MB_OK);


	ATTRIBUTES searchRequest = *(ATTRIBUTES*)request;
	RESULTSET *resultSet = (RESULTSET*)response;
	FILE *fData;
	DATAENTRY dataScan = { "0", "0","0", 0, 0, 0, 0, 0, 0, 0 };


	int i = 0, j = 0, j1 = 0, k = 0, l = 0, m = 0, n = 0, numberOfRecords = 0, p = 0, q = 0, r = 0;
	int jname, jsurname, jadress, spaceflag = 1;
	int dataEntryCounter = 0;
	char Line[100] = "0";
	char Line1[100] = "0";
	char str[100] = "0";
	char check_equal[30] = "0";
	char NAME[20] = "0";
	char SURNAME[20] = "0";
	char CITY[20] = "0";
	BOOL stringEqualFlag = FALSE;
	int entry_counter = 0;
	BOOL fName = FALSE, fUser_id = FALSE, fDepartment_id = FALSE, fName1 = FALSE, fUser_id1 = FALSE, fDepartment_id1 = FALSE;

	//wsprintf((LPWSTR)str, TEXT("search request is  : %d"), searchRequest.user_id);
	//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);

	//MessageBox(NULL, TEXT("Values assigned"), TEXT("Error"), MB_OK);

	if (searchRequest.name[0] == '0' || searchRequest.name[0] == '\0' || searchRequest.name == NULL)
	{

		fName1 = TRUE;
	}
	if (searchRequest.user_id == 0 || searchRequest.user_id == NULL)
	{
		fUser_id1 = TRUE;
	}
	if (searchRequest.department_id == 0 || searchRequest.department_id == NULL)
	{
		fDepartment_id1 = TRUE;
	}

	for (i = 0; i < 30; i++)
	{
		if (searchRequest.name[i] == '\0')
			break;
		entry_counter++;
	}

	errno_t error = fopen_s(&fData, "E:\\TestFileIO\\FileIO\\FileIO\\CheckText.txt", "a+");

	if (error)
	{
		//printf("Cannot open file\n");
		MessageBox(NULL, TEXT("Cannot open file"), TEXT("Error"), MB_OK);
	}
	else
	{
		fseek(fData, 0L, SEEK_SET);

		//MessageBox(NULL, TEXT("Before loop"), TEXT("Error"), MB_OK);
		for (i = 0;; i++)
		{
			fscanf_s(fData, "%c", &Line[i], 1);


			if (Line[i] == '\n')
			{
				fName = fName1;
				fUser_id = fUser_id1;
				fDepartment_id = fDepartment_id1;

				dataScan.user_id = 0;
				dataScan.department_id = 0;
				dataScan.sex = 0;
				dataScan.age = 0;
				dataScan.date = 0;
				dataScan.month = 0;
				dataScan.year = 0;

				for (j = 0, j1 = 0;; j++, j1++)
				{
					Line1[j1] = Line[j];
					if (Line[j] == '\0')
						
						break;
				}
				for (j = 0, jname = 0, jsurname = 0, jadress = 0, spaceflag = 1;; j++)
				{
					if (spaceflag == 1 && Line1[j] != ' ')
					{

						dataScan.user_id = dataScan.user_id * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 2 && Line1[j] != ' ')
					{

						dataScan.department_id = dataScan.department_id * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 3)
					{
						dataScan.name[jname++] = Line1[j];
					}
					if (spaceflag == 4)
					{
						dataScan.surname[jsurname++] = Line1[j];
					}
					if (spaceflag == 5)
					{
						dataScan.city[jadress++] = Line1[j];
					}
					if (spaceflag == 6 && Line1[j] != ' ')
					{

						dataScan.sex = dataScan.sex * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 7 && Line1[j] != ' ')
					{

						dataScan.age = dataScan.age * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 8 && Line1[j] != ' ')
					{

						dataScan.date = dataScan.date * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 9 && Line1[j] != ' ')
					{

						dataScan.month = dataScan.month * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 10 && Line1[j] != '\n')
					{

						dataScan.year = dataScan.year * 10 + (Line1[j] - '0');
					}

					if (Line1[j] == ' ')
					{
						spaceflag++;
					}

					if (Line1[j] == '\n')
						break;

				}

				//printf("%d\n", dataScan.age);

				//printf("Success\n");
				//printf("%s\n\n", dataScan.name);

				//printf("%d - %d - %d - %d  -%d\n", dataScan.user_id, searchRequest.user_id, fUser_id, fDepartment_id, fName);
				if (fUser_id == FALSE && dataScan.user_id == searchRequest.user_id)
				{
					fUser_id = TRUE;
				}
				if (fDepartment_id == FALSE && dataScan.department_id == searchRequest.department_id)
				{
					fDepartment_id = TRUE;
				}

				strcpy_s(check_equal, dataScan.name);
				searchRequest.name[entry_counter] = '\0';
				check_equal[entry_counter] = '\0';
				
				IsStringEqual(check_equal, searchRequest.name, &stringEqualFlag);
				if (fName == FALSE && stringEqualFlag == TRUE)
				{
					fName = TRUE;
				}

				if (fUser_id == TRUE && fDepartment_id == TRUE && fName == TRUE)
				{
					/*for (j = 0, j1 = 0;; j++, j1++)
					{
						if(dataScan.name[j]!=' ')
						NAME[j] = dataScan.name[j];

						if (dataScan.name[j] == '\0')

							break;
					}

					for (j = 0, j1 = 0;; j++, j1++)
					{
						if (dataScan.surname[j] != ' ')
							SURNAME[j] = dataScan.surname[j];

						if (dataScan.surname[j] == '\0')

							break;
					}

					for (j = 0, j1 = 0;; j++, j1++)
					{
						if (dataScan.city[j] != ' ')
							CITY[j] = dataScan.city[j];

						if (dataScan.city[j] == '\0')

							break;
					}*/

					resultSet->ResultSet[dataEntryCounter].user_id = dataScan.user_id;
					resultSet->ResultSet[dataEntryCounter].department_id = dataScan.department_id;
					resultSet->ResultSet[dataEntryCounter].age = dataScan.age;
					resultSet->ResultSet[dataEntryCounter].sex = dataScan.sex;
					resultSet->ResultSet[dataEntryCounter].date = dataScan.date;
					resultSet->ResultSet[dataEntryCounter].month = dataScan.month;
					resultSet->ResultSet[dataEntryCounter].year = dataScan.year;
					strcpy_s(resultSet->ResultSet[dataEntryCounter].name, dataScan.name);
					strcpy_s(resultSet->ResultSet[dataEntryCounter].surname, dataScan.surname);
					strcpy_s(resultSet->ResultSet[dataEntryCounter].city, dataScan.city);

					memset(dataScan.name, '\0', sizeof(dataScan.name));
					memset(dataScan.surname, '\0', sizeof(dataScan.surname));
					memset(dataScan.city, '\0', sizeof(dataScan.city));
					//wsprintf((LPWSTR)str, TEXT("search result is  : %d"), dataScan.year);
					//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);
					dataEntryCounter++;
				}

				for (j = 0;; j++)
				{
					if (dataScan.name[j] != '\0')
						dataScan.name[j] = 0;
					else
						break;
				}
				dataScan.name[0] = 0;
				//printf("%s\n", Line1);

				Line1[0] = 0;
				Line[0] = 0;
				i = -1;
			}

			if (Line[i] == 0)
			{
				//printf("EOF");
				break;
			}
		}
		resultSet->dataEntry_counter = dataEntryCounter;
		//MessageBox(NULL, TEXT("Inside read recoord 2 loop executed"), TEXT("Error"), MB_OK);
	}
	fclose(fData);
	
	//wsprintf((LPWSTR)str, TEXT("result brought is  : %d"), resultSet->ResultSet[1].department_id);
	//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);

	//response = &resultSet;
	//printf("%d\n", resultSet[1].department_id);

	return S_OK;
}

HRESULT CReadWrite::IsStringEqualSpot(char name1[], char name2[], BOOL *stringEqualFlag)
{

	int i = 0, j = 0;
	char str[100];

	//wsprintf((LPWSTR)str, TEXT("result brought is  : %s"), name1);
	//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);
	if (name1[0] == '\0' || name2[0] == '\0')
	{
		//MessageBox(NULL, TEXT("inside if for spot check"), TEXT("Result"), MB_OK);
		*stringEqualFlag = FALSE;
		return S_OK;
	}

	for (i = 0; name1[i] != ' ' && name1[i] != '\n' && name1[i] != '\0'; i++);

	if (name2[i] == ' ' || name2[i] == '\n' || name2[i] == '\0')
	{
		for (i--; i >= 0; i--)
		{
			//printf("%d\n", i);
			//wsprintf((LPWSTR)str, TEXT(" %c -  %c "), name1[i], name2[i]);

			//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);
			if (name1[i] != name2[i]) {

				*stringEqualFlag = FALSE;
				return S_OK;
			}
		}
		//printf("%s is equal with %s\n", name1, name2);
		*stringEqualFlag = TRUE;
		return S_OK;
	}
	//printf("%s and %s is not equla2\n", name1, name2);
	*stringEqualFlag = FALSE;
	return S_OK;


}

HRESULT CReadWrite::IsStringEqual(char name1[], char name2[], BOOL *stringEqualFlag)
{

	int i = 0, j = 0;
	char str[100] = "0";
	char str1[100] = "0";

	/*if (name1[0] == '\n' || name2[0] == '\n')
	{
		*stringEqualFlag = FALSE;
		return S_OK;
	}*/

	for (i = 0; name1[i] != '\0'; i++);

	//wsprintf((LPWSTR)str1, TEXT(" %d - "), i);

	//MessageBox(NULL, (LPWSTR)str1, TEXT("Result"), MB_OK);

	if (name2[i] == '\0')
	{
		for (i--; i >= 0; i--)
		{

			//printf("%d\n", i);
			//wsprintf((LPWSTR)str, TEXT(" %c -  %c "), name1[i], name2[i]);

			//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);

			if (name1[i] != name2[i]) {

				//wsprintf((LPWSTR)str, TEXT(" %c -  %c "), name1[i], name2[i]);

				//MessageBox(NULL, (LPWSTR)str, TEXT("Result"), MB_OK);
				*stringEqualFlag = FALSE;
				return S_OK;
			}
		}
		//printf("%s is equal with %s\n", name1, name2);
		*stringEqualFlag = TRUE;
		return S_OK;
	}
	//printf("%s and %s is not equla2\n", name1, name2);
	*stringEqualFlag = FALSE;
	return S_OK;


}
HRESULT CReadWrite::WriteRecord(void *request, BOOL *resultResonseFlag)
{

	BOOL validationFlag;


	//MessageBox(NULL, TEXT("inside write record"), TEXT("Error"), MB_OK);
	//dataEntry = { "Rahul", "Shinde", "sa", 101, 12, 1, 24 };
	Validation((DATAENTRY*)request, &validationFlag);


	//DataEntry(dataEntry);
	if (validationFlag)
		DataEntry((DATAENTRY*)request);
	else
		MessageBox(NULL, TEXT("Cannot write this record"), TEXT("Error"), MB_OK);

	return S_OK;

}

HRESULT CReadWrite::DataEntry(DATAENTRY *DataEntry)
{
	FILE *dataFile;
	DATAENTRY dataEntry = *DataEntry;
	printf("%s", dataEntry.name);
	errno_t error = fopen_s(&dataFile, "E:\\TestFileIO\\FileIO\\FileIO\\CheckText.txt", "a+");

	if (error)
	{
		printf("file cannot be obtained\n");
	}
	else
	{
		fprintf_s(dataFile, "%d %d %s %s %s %d %d %d %d %d\n", dataEntry.user_id, dataEntry.department_id, dataEntry.name, dataEntry.surname,
			dataEntry.city, dataEntry.sex, dataEntry.age, dataEntry.date, dataEntry.month, dataEntry.year);
		fclose(dataFile);
	}

	return S_OK;
}

HRESULT CReadWrite::Validation(DATAENTRY *DataEntry, BOOL *validateFlag)
{
	DATAENTRY dataScan = { "0", "0", "0", 0, 0, 0 ,0, 0, 0, 0 };
	DATAENTRY dataEntry = *DataEntry;
	HRESULT hr;
	FILE *dataFile;
	errno_t error;
	char Line[100] = "0";
	char Line1[100] = "0";
	int i = 0, j = 0, jname = 0, jsurname = 0, jadress = 0, spaceflag = 1;
	int numberOfRecord = 0, record = 0, ageFlag = 1;
	char scanRecord;
	BOOL dataEntryFlag, equalFlag, iflag = FALSE;
	//dataScan = { "0", "0", "0", 0 };

	error = fopen_s(&dataFile, "E:\\TestFileIO\\FileIO\\FileIO\\CheckText.txt", "a+");

	if (error)
	{
		exit(0);
		//printf("file cannot be opened\n");

	}
	else
	{
		//printf("file is opened\n");


		fseek(dataFile, 0L, SEEK_SET);

		for (i = 0;; i++)
		{
			fscanf_s(dataFile, "%c", &Line[i], 1);


			if (Line[i] == '\n')
			{

				dataScan.user_id = 0;
				dataScan.department_id = 0;
				dataScan.sex = 0;
				dataScan.age = 0;
				dataScan.date = 0;
				dataScan.month = 0;
				dataScan.year = 0;

				for (j = 0;; j++)
				{
					Line1[j] = Line[j];
					if (Line[j] == '\0')
						break;
				}
				for (j = 0, jname = 0, jsurname = 0, jadress = 0, spaceflag = 1;; j++)
				{
					if (spaceflag == 1 && Line1[j] != ' ')
					{

						dataScan.user_id = dataScan.user_id * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 2 && Line1[j] != ' ')
					{

						dataScan.department_id = dataScan.department_id * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 3)
					{
						dataScan.name[jname++] = Line1[j];
					}
					if (spaceflag == 4)
					{
						dataScan.surname[jsurname++] = Line1[j];
					}
					if (spaceflag == 5)
					{
						dataScan.city[jadress++] = Line1[j];
					}
					if (spaceflag == 6 && Line1[j] != ' ')
					{

						dataScan.sex = dataScan.sex * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 7 && Line1[j] != '\n')
					{

						dataScan.age = dataScan.age * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 8 && Line1[j] != '\n')
					{

						dataScan.date = dataScan.date * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 9 && Line1[j] != '\n')
					{

						dataScan.month = dataScan.month * 10 + (Line1[j] - '0');
					}
					if (spaceflag == 10 && Line1[j] != '\n')
					{

						dataScan.year = dataScan.year * 10 + (Line1[j] - '0');
					}

					if (Line1[j] == ' ')
					{
						spaceflag++;
					}

					if (Line1[j] == '\n')
						break;

				}

				//printf("%d\n", dataScan.age);

				//printf("Success\n");
				//printf("%s\n\n", dataScan.name);

				hr = IsEqual(dataEntry.user_id, dataScan.user_id, &equalFlag);
				if (equalFlag)
				{
					*validateFlag = FALSE;
					return hr;
				}

				for (j = 0;; j++)
				{
					if (dataScan.name[j] != '\0')
						dataScan.name[j] = 0;
					else
						break;
				}
				dataScan.name[0] = 0;
				//printf("%s\n", Line1);

				Line1[0] = 0;
				Line[0] = 0;
				i = -1;
			}

			if (Line[i] == 0)
			{
				//printf("EOF");
				break;
			}
		}

		fclose(dataFile);

	}

	return S_OK;
}

HRESULT CReadWrite::IsEqual(int user_id_input, int user_id_scan, BOOL *equalFlag)
{
	if (user_id_input == user_id_scan)
	{
		*equalFlag = TRUE;
	}
	else
		*equalFlag = FALSE;

	return S_OK;
}


//Implementation of classFactory class

CReadWriteClassFactory::CReadWriteClassFactory(void)
{
	m_cRef = 1;
}

CReadWriteClassFactory::~CReadWriteClassFactory(void)
{
	//no code
}

HRESULT CReadWriteClassFactory::QueryInterface(REFIID riid, void **ppv)
{

	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CReadWriteClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CReadWriteClassFactory::Release(void)
{

	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

HRESULT CReadWriteClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	CReadWrite *pReadWrite = NULL;
	HRESULT hr;
	if (pUnkOuter != NULL)
		return CLASS_E_NOAGGREGATION;

	pReadWrite = new CReadWrite;

	if (pReadWrite == NULL)
		return E_OUTOFMEMORY;


	hr = pReadWrite->QueryInterface(riid, ppv);
	pReadWrite->Release();
	return hr;
}

HRESULT CReadWriteClassFactory::LockServer(BOOL fLock)
{

	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return S_OK;

}


extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	//MessageBox(NULL, TEXT("DllGetClassObject entered"), TEXT("Error"), MB_OK);
	CReadWriteClassFactory *pCReadWriteClassFactory = NULL;
	HRESULT hr;
	if (rclsid != CLSID_ReadWrite)
		return E_NOINTERFACE;

	pCReadWriteClassFactory = new CReadWriteClassFactory;
	if (pCReadWriteClassFactory == NULL)
		return E_OUTOFMEMORY;

	hr = pCReadWriteClassFactory->QueryInterface(riid, ppv);
	pCReadWriteClassFactory->Release();

	return hr;
}


extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return S_OK;
	else
		return S_FALSE;
}

