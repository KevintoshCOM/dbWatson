# dbWatson

### Zweck

Das in C++ entwicklete CMD-Tool __dbWatson__ ermöglicht es Strukturinformationen einer Datenbank auszulesen und als XML-Datei auszugeben. Dies beinhaltet Tabellennamen und -typ sowie Name, Länge, Typ und Nullbarkeit der jweiligen Spalten.
Gerade bei konmplexen Datenbanken kann dbWatson helfen, die Datenstrukturen zu dokumentieren. Dies kann eine wertvolle Hilfe für bereits existierende Datenbanken sein.

### Entwickler

* Kevin Klein [Zer0Knowledge]
* Tobias Donix [tobidon]
* Leonard Franke [leofranke95]


### Unterstützte DBMS

Momentan unterstützt dbWatson PostgreSQL-Datenbanken. Eine Unterstützung von MySQL-Datenbanken ist zukünftig angedacht.

### Kompilieren

Kompilieren lässt sich dbWatson mit __clang__ unter Linux und MacOSX. Dafür ist die erforderliche Bibliothek __libxml2__ selbst zu kompilieren und in folgendem Unterverzeichnis als __xml2.la__ abzulegen:

* MacOSX: libs/osx
* Linux: libs/lnx

Die PostgreSQL-Bibliothek __libpq__ wird in folgendem Verzeichnis erwartet:

* MacOSX: ~/PostgreSQL/pg96/lib __Header:__ ~/PostgreSQL/pg96/include
* Linux: /usr/local/pgsql/lib __Header:__ /usr/local/pgsql/include

### Ini-Datei

Die Ini-Datei muss den Namen __dbWatson.ini__ tragen und wird in folgendem Format erwartet:

    [default]
    dbType=postgres
    dbServer=localhost
    dbPort=5432
    dbName=pgtest
    dbUsr=postgres
    dbPasswd=

### Verwendung

Um dbWatson verwenden zu können, ist ein Arbeitsverzeichnis zu erstellen und die oben beschriebene Ini-Datei darin zu erstellen. Danach ist in dieses Arbeitsverzeichnis zu wechseln und dbWatson von hier zu starten.

### Lizenz

BSD 2.0