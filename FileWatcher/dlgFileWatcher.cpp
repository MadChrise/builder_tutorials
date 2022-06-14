#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "dlgFileWatcher.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

TFormFileWatcher *FormFileWatcher;
//---------------------------------------------------------------------------

__fastcall TFormFileWatcher::TFormFileWatcher(TComponent* Owner)
	: TForm(Owner)
{
	this->edtPfad->Text = ExtractFilePath(Application->ExeName);
}
//---------------------------------------------------------------------------

__fastcall TFormFileWatcher::~TFormFileWatcher()
{

}
//---------------------------------------------------------------------------

void TFormFileWatcher::DateienLesen(const String &strPfad, std::list<Datei> &lstDateien)
{
	int done = 0;
	TSearchRec rec;

	try
	{
		done = FindFirst(strPfad + L"\\*.*", faAnyFile, rec);

		while(!done)
		{
			if (rec.Name != L"." && rec.Name != L"..")
			{
				// Falls Ordner
				// -> rekusriver Aufruf
				if (rec.Attr & faDirectory)
					this->DateienLesen(strPfad + L"\\" + rec.Name, lstDateien);
				else
				{
					Datei datei;
					datei.pfad = strPfad;
					datei.name = rec.Name;
					datei.lastWriteTime = rec.FindData.ftLastWriteTime;
					lstDateien.push_back(datei);
				}
			}

			done = FindNext(rec);
		}
	}
	__finally
	{
		FindClose(rec);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormFileWatcher::btnOhneThreadClick(TObject *Sender)
{
	this->meInfo->Lines->Add(L"FileWatcher gestartet ...");

	this->DateienLesen(this->edtPfad->Text, this->m_lstDateienAlt);

	// FileWatcher registrieren
	HANDLE hChangeNotification = FindFirstChangeNotificationW(this->edtPfad->Text.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);

	// Pruefen, ob der FileWatcher richtig registriert werden konnte
	if (hChangeNotification == INVALID_HANDLE_VALUE || hChangeNotification == NULL)
	{
		this->meInfo->Lines->Add(L"Fehler beim registrieren des FileWatchers");
		return;
	}

	// Warten, bis im Ordner ein Aenderung auftritt
	WaitForSingleObject(hChangeNotification, INFINITE);

	this->meInfo->Lines->Add(L"Änderung registriert!");

	this->DateienLesen(this->edtPfad->Text, this->m_lstDateienNeu);

	for(auto dateiAlt: this->m_lstDateienAlt)
	{
		for(auto dateiNeu: this->m_lstDateienNeu)
		{
			if (dateiAlt.pfad == dateiNeu.pfad
			&& dateiAlt.name == dateiNeu.name
			&& (dateiAlt.lastWriteTime.dwHighDateTime != dateiNeu.lastWriteTime.dwHighDateTime || dateiAlt.lastWriteTime.dwLowDateTime != dateiNeu.lastWriteTime.dwLowDateTime))
			{
				this->meInfo->Lines->Add(L"Datei geändert: " + dateiAlt.name);
			}
		}
	}
}
//---------------------------------------------------------------------------
