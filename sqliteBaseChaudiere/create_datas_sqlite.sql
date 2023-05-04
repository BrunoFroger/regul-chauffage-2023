    
#
#  construction de la table des plages
#
.open chaudiere.db
drop table if exists plages;
CREATE TABLE IF NOT EXISTS plages(
    ID integer primary key autoincrement not null,
    TYPE text CHECK(TYPE in ("MODELE","USER", "SEMAINE")),
    NAME text,
    JOUR integer check(JOUR >=0 and JOUR <= 6),
    IDX_PLA integer check(IDX_PLA >= 0 and IDX_PLA <= 6),
    H_DEB integer check(H_DEB >= 0 and H_DEB <= 23),
    M_DEB integer check(M_DEB >= 0 and M_DEB <= 59),
    H_FIN integer check(H_FIN >= 0 and H_FIN <= 23),
    M_FIN integer check(M_FIN >= 0 and M_FIN <= 59),
    CONSIGNE int,
    ON_OFF integer check(ON_OFF = 0 or ON_OFF = 1));
insert into plages(TYPE, NAME, JOUR, IDX_PLA, H_DEB, M_DEB, H_FIN, M_FIN, CONSIGNE, ON_OFF) values
    ("MODELE", "Journee complete", 0, 0, 0, 0, 23, 59, 200, 0),
    ("MODELE", "matin", 0, 0, 7, 0, 8, 59, 200, 0),
    ("MODELE", "soiree", 0, 0, 17, 0, 22, 59, 200, 0),
    ("MODELE", "midi", 0, 0, 12, 0, 13, 59, 160, 0),
    ("MODELE", "apres midi", 0, 0, 13, 0, 18, 59, 160, 0),
    ("MODELE", "matinee", 0, 0, 8, 0, 11, 59, 160, 0),
    ("MODELE", "nuit", 0, 0, 22, 0, 6, 59, 160, 0),
    ("SEMAINE", "dimanche 0", 0, 0, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "dimanche 1", 0, 1, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "dimanche 2", 0, 2, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "dimanche 3", 0, 3, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "dimanche 4", 0, 4, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "lundi 0", 1, 0, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "lundi 1", 1, 1, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "lundi 2", 1, 2, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "lundi 3", 1, 3, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "lundi 4", 1, 4, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mardi 0", 2, 0, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mardi 1", 2, 1, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mardi 2", 2, 2, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mardi 3", 2, 3, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mardi 4", 2, 4, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mercredi 0", 3, 0, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mercredi 1", 3, 1, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mercredi 2", 3, 2, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mercredi 3", 3, 3, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "mercredi 4", 3, 4, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "jeudi 0", 4, 0, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "jeudi 1", 4, 1, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "jeudi 2", 4, 2, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "jeudi 3", 4, 3, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "jeudi 4", 4, 4, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "vendredi 0", 5, 0, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "vendredi 1", 5, 1, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "vendredi 2", 5, 2, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "vendredi 3", 5, 3, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "vendredi 4", 5, 4, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "samedi 0", 6, 0, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "samedi 1", 6, 1, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "samedi 2", 6, 2, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "samedi 3", 6, 3, 0, 0, 23, 59, 200, 0),
    ("SEMAINE", "samedi 4", 6, 4, 0, 0, 23, 59, 200, 0),
    ("USER", "user0", 0, 0, 0, 0, 23, 59, 160, 0),
    ("USER", "user1", 0, 0, 0, 0, 23, 59, 160, 0),
    ("USER", "user2", 0, 0, 0, 0, 23, 59, 160, 0),
    ("USER", "user3", 0, 0, 0, 0, 23, 59, 160, 0),
    ("USER", "user4", 0, 0, 0, 0, 23, 59, 160, 0);
select * from plages;

drop table if exists modeles;
create table if not exists modeles(
    ID integer primary key autoincrement not null,
    PLAGE ID);
insert into modeles(ID) 
    select ID from plages where TYPE = "MODELE";
select * from modeles;

drop table if exists users;
create table if not exists users(
    ID integer primary key autoincrement not null);
insert into users(ID) 
    select ID from plages where TYPE = "USER";
select * from users;

drop table if exists calendrier;
create table if not exists calendrier(
    ID integer primary key autoincrement not null);
insert into calendrier(ID) 
    select ID from plages where TYPE = "SEMAINE";
select * from calendrier;

drop table if exists parametres;
create table if not exists parametres(
    ID integer primary key autoincrement not null,
    nom text not null,
    valeur  text);
insert into parametres (nom, valeur) values 
    ("on-off", "false"),
    ("acces_point", "1"),
    ("nb_plages_jour", "5"),
    ("nb_plages_modele", "7"),
    ("nb_plages_modele", "5");
select * from parametres;

drop table if exists accesPoints;
create table if not exists accesPoints(
    ID integer primary key autoincrement not null,
    ssid text,
    pwd text);
insert into accesPoints (ssid, pwd) values
    ("Livebox-webcom", "webcom2014"),
    ("Livebox-A0EA","eHQSXY2DHrrGb3AXsz"),
    ("TP-LINK_88E2","75546007");
select * from accesPoints;
