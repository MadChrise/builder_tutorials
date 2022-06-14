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
};
//---------------------------------------------------------------------------

class TFormFileWatcher : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TEdit *edtPfad;
	TButton *btnStart;
	TMemo *meInfo;
	void __fastcall btnOhneThreadClick(TObject *Sender);
private:	// Benutzer-Deklarationen
	// Daten
	std::list<Datei> m_lstDateienAlt;
	std::list<Datei> m_lstDateienNeu;

	// Funktionen
	void DateienLesen(const String &strPfad, std::list<Datei> &lstDateien);

public:		// Benutzer-Deklarationen
	// Konstruktor + Destruktor
	__fastcall TFormFileWatcher(TComponent* Owner);
	__fastcall ~TFormFileWatcher();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFileWatcher *FormFileWatcher;
//---------------------------------------------------------------------------
#endif
