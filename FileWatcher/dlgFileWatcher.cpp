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

void __fastcall TFormFileWatcher::InfoAusgeben(void)
{
	this->meInfo->Lines->Add(this->m_strInfo);
}
//---------------------------------------------------------------------------

void TFormFileWatcher::DateienLesen(const String &strPfad, std::list<Datei> &lstDateien)
{
	int done = 0;
	TSearchRec rec;

	try
	{
		done = FindFirst(strPfad + L"\\*.*", faAnyFile, rec);

		// Solange noch Dateien vorhanden sind
		while(!done)
		{
			if (rec.Name != L"." && rec.Name != L"..")
			{
				// Falls Ordner
				// -> rekusriver Aufruf
				if (rec.Attr & faDirectory)
					this->DateienLesen(strPfad + L"\\" + rec.Name, lstDateien);
				// Falls Datei
				else
				{
					// Daten ablegen
					Datei datei;
					datei.pfad = strPfad;
					datei.name = rec.Name;
					datei.lastWriteTime = rec.FindData.ftLastWriteTime;
					// Und in einer Liste speichern
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

void TFormFileWatcher::DateienVergleichen(const std::list<Datei> &lstDateienAlt, const std::list<Datei> &lstDateienNeu)
{
	// Pruefen, welche Dateien hinzugefuegt wurden
	for(auto dateiNeu: lstDateienNeu)
	{
		bool bGefunden = false;

		for(auto dateiAlt: lstDateienAlt)
		{
			if (dateiNeu == dateiAlt)
			{
				bGefunden = true;
				break;
			}
		}

		// Falls die Datei nicht gefunden wurde
		// -> neue Datei
		if (!bGefunden)
		{
			this->m_strInfo = L"Neue Datei: " + dateiNeu.name;
			TThread::Synchronize(nullptr, &this->InfoAusgeben);
		}
	}

	// Pruefen, welche Dateien geloescht wurden
	for(auto dateiAlt: lstDateienAlt)
	{
		bool bGefunden = false;

		for(auto dateiNeu: lstDateienNeu)
		{
			if (dateiAlt == dateiNeu)
			{
				bGefunden = true;
				break;
			}
		}

		// Falls die Datei nicht gefunden wurde
		// -> geloeschte Datei
		if (!bGefunden)
		{
			this->m_strInfo = L"Gelöschte Datei: " + dateiAlt.name;
			TThread::Synchronize(nullptr, &this->InfoAusgeben);
		}
	}

	// Pruefen welche Dateien geaendert wurden
	for(auto dateiAlt: lstDateienAlt)
	{
		for(auto dateiNeu: lstDateienNeu)
		{
			if (dateiAlt == dateiNeu)
			{
				if (dateiAlt.lastWriteTime.dwLowDateTime != dateiNeu.lastWriteTime.dwLowDateTime
				 || dateiAlt.lastWriteTime.dwHighDateTime != dateiNeu.lastWriteTime.dwHighDateTime)
				{
					this->m_strInfo = "Geänderte Datei: " + dateiNeu.name;
					TThread::Synchronize(nullptr, &this->InfoAusgeben);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormFileWatcher::btnOhneThreadClick(TObject *Sender)
{
	std::list<Datei> lstDateienAlt, lstDateienNeu;

	this->meInfo->Lines->Add(L"FileWatcher gestartet ...");

	// Den aktuellen Stand der Dateien lesen
	this->DateienLesen(this->edtPfad->Text, lstDateienAlt);

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

	// Meldung ausgeben
	this->meInfo->Lines->Add(L"Änderung registriert!");

	// Den aktuellen Stand der Dateien lesen
	this->DateienLesen(this->edtPfad->Text, lstDateienNeu);

	// Alte und neue Dateien vergleichen und evtl. gemachte Aenderungen ausgeben
	this->DateienVergleichen(lstDateienAlt, lstDateienNeu);
}
//---------------------------------------------------------------------------

void __fastcall TFormFileWatcher::btnMitThreadClick(TObject *Sender)
{
	auto pThread = TThread::CreateAnonymousThread([this]() -> void {
		std::list<Datei> lstDateienAlt, lstDateienNeu;
		this->meInfo->Lines->Add(L"FileWatcher gestartet ...");

		// FileWatcher registrieren
		HANDLE hChangeNotification = FindFirstChangeNotificationW(this->edtPfad->Text.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);

		// Pruefen, ob der FileWatcher richtig registriert werden konnte
		if (hChangeNotification == INVALID_HANDLE_VALUE || hChangeNotification == NULL)
		{
			this->meInfo->Lines->Add(L"Fehler beim registrieren des FileWatchers");
			return;
		}

		while(true)
		{
			lstDateienAlt.clear();
			lstDateienNeu.clear();

			// Den aktuellen Stand der Dateien lesen
			this->DateienLesen(this->edtPfad->Text, lstDateienAlt);

			// Warten, bis im Ordner ein Aenderung auftritt
			WaitForSingleObject(hChangeNotification, INFINITE);

			// Den aktuellen Stand der Dateien lesen
			this->DateienLesen(this->edtPfad->Text, lstDateienNeu);

			// Alte und neue Dateien vergleichen und evtl. gemachte Aenderungen ausgeben
			this->DateienVergleichen(lstDateienAlt, lstDateienNeu);

			FindNextChangeNotification(hChangeNotification);
		}
	});

	pThread->Start();
}
//---------------------------------------------------------------------------

