Open Points for Max Kellermann
------------------------------

### Problem size_t <==> integration

Viele (die meisten) Container geben als size() (und oder ....) ein size_t zurück, in XCS wird das ganz oft aber als int weitergegeben (mit der Warnung bei 64 bit Programm: 'Möglicherweise werden die Daten 'abgeschnitten'' 
3 Lösungen sind denkbar:
* so lassen, wie es ist (Schlecht!)
* über ein cast ( (int) - oder besser  reinterpret_cast<int>() die Fehlermeldung eliminieren (das ist besser, beseitigt aber nicht ganz das mögliche Problem)
* die Count() oder Size() funktionen zu Rückgabewert 'size_t' umwandeln (viel Arbeit, aber die beste Lösung)

### Problem [gnu::const]/ [gnu::]

Diese ('Definer') gibt es außerhalb clang/gcc nicht! Sollte dafüre nicht ein Macro - z.B.
* #define GNU_CONST [gnu::const]
genommen werden - dann kann ich im MSVC dieses Macro einfach leer lassen? Oder was spricht dagegen?

Mein Problem ist immer wieder, dass Max den MSVC nicht auf dem Schirm hat - und sich dadurch immer wieder Nachbesserungen ergeben müssen oder Warnmeldungen zu unterdrücken sind...

### Problem Übersetzung
bei PRxxx wurde zwar die (italienische) Übersetzung geändert, aber der (falsche) Basistext (in Englisch) beibehalten...  Es sollte aber der 'Basistext' geändert werden -und alle Übersetzungen dem angepasst werden...
Aug: Welche Rolle spielt dabei 'WebLate' (oder so ähnlich): werden da die Übersetzungen gemanaged? Ist das auch für die 'komischen' Übersetzungen ins Deutsche verantwortlich ('GoTo' -> 'Fliege nach')?

### Problem LateX Doc 
Fehlendes Leerzeichen nach dem 'Macro' /xc lässt sich beheben mit /xc/ (Gleiches gilt für /fl - 'Flarm' usw...



#