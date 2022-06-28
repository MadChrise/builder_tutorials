#ifndef dlgFileWatcherH
#define dlgFileWatcherH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------

#include <list>
//---------------------------------------------------------------------------

struct Datei
{
	String pfad;
	String name;
	FILETIME lastWriteTime;

	bool operator== (const Datei &vg)
	{
		if (pfad == vg.pfad && name == vg.name)
			return true;
		else
			return false;
	}
};
//---------------------------------------------------------------------------

class TFormFileWatcher : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TEdit *edtPfad;
	TButton *btnStart;
	TMemo *meInfo;
	TButton *btnMitThread;
	void __fastcall btnOhneThreadClick(TObject *Sender);
	void __fastcall btnMitThreadClick(TObject *Sender);
private:	// Benutzer-Deklarationen
	// Daten
	String m_strInfo = L"";

	// Funktionen
	void __fastcall InfoAusgeben(void);
	/**
	 * Liest die Dateien in einem Verzeichnis (rekursiv) und speichert die Daten
	 * in einer Liste.
	 * @param strPfad Pfad, in dem die Dateien gelesen werden soll
	 * @param lstDateien Liste in der die Dateien gespeichert werden sollen
	 */
	void DateienLesen(const String &strPfad, std::list<Datei> &lstDateien);
	/**
	 * Vergleicht zwei Listen von Dateien und gibt evtl. geaenderte Dateien aus
	 * @param lstDateienAlt Die Liste mit den "alten" Dateien
	 * @param lstDateienNeu Die Liste mit den "neuen" Dateien
	 */
	void DateienVergleichen(const std::list<Datei> &lstDateienAlt, const std::list<Datei> &lstDateienNeu);

public:		// Benutzer-Deklarationen
	// Konstruktor + Destruktor
	__fastcall TFormFileWatcher(TComponent* Owner);
	__fastcall ~TFormFileWatcher();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFileWatcher *FormFileWatcher;
//---------------------------------------------------------------------------
#endif
