create database cine;
use cine;

create table Actores(ID_actor int auto_increment,nombre tinytext,primary key(ID_actor));
create table Directores(ID_Director int auto_increment,nombre tinytext,primary key(ID_director));
create table Peliculas(ID_pelicula int auto_increment,titulo tinytext,primary key(ID_pelicula));

insert into Actores values(null,"Bruce Willis");
insert into Actores values(null,"Jason Statham");
insert into Actores values(null,"Matt Damon");
insert into Actores values(null,"Robert Downey Jr");
insert into Actores values(null,"Sylvester Stallone");

insert into Directores values(null,"Quentin Tarantino");
insert into Directores values(null,"James Wan");
insert into Directores values(null,"Cristopher Nolan");
insert into Directores values(null,"Jon Favreau");
insert into Directores values(null,"Ted Kotcheff");

insert into Peliculas values(null,"Pulp Fiction");
insert into Peliculas values(null,"Fast And Furious 7");
insert into Peliculas values(null,"Interstellar");
insert into Peliculas values(null,"Iron Man");
insert into Peliculas values(null,"Rambo");

alter table Peliculas add ID_Director int;#voy a añadir una relacion 1-N
alter table Peliculas add foreign key (ID_Director) references Directores(ID_Director);#foreign key

update peliculas set ID_director = 1 where ID_Pelicula = 1; #Actualizo la base de datos
update peliculas set ID_director = 2 where ID_Pelicula = 2;# para que tenga sentido
update peliculas set ID_director = 3 where ID_Pelicula = 3;
update peliculas set ID_director = 4 where ID_Pelicula = 4;
update peliculas set ID_director = 5 where ID_Pelicula = 5;

create table ActorPeliculas(ID_Pelicula int,ID_Actor int, #Relacion M-N
Primary Key(ID_Pelicula,ID_Actor),
Foreign Key(ID_Pelicula) references Peliculas(ID_Pelicula),
Foreign Key(ID_Actor) references Actores(ID_Actor));

insert into ActorPeliculas values(1,1); #ejemplos de relacion M-N
insert into ActorPeliculas values(1,5);
insert into ActorPeliculas values(2,2);
insert into ActorPeliculas values(2,3);
insert into ActorPeliculas values(3,3);
insert into ActorPeliculas values(4,5);
insert into ActorPeliculas values(5,5);

insert into ActorPeliculas values(5,7); #no va a funcionar porque no hay actor 7

alter table Actores add IMDB int(7);#ejercicio de unicidad
alter table Actores add unique (IMDB);

alter table Directores add IMDB int(7);
alter table Directores add unique (IMDB);

alter table Peliculas add IMDB int(7);
alter table Peliculas add unique (IMDB);

update Actores set IMDB=0000246 where ID_actor = 1;#los updateo
update Actores set IMDB=0005458 where ID_actor = 2;
update Actores set IMDB=0000354 where ID_actor = 3;
update Actores set IMDB=0000375 where ID_actor = 4;
update Actores set IMDB=0000230 where ID_actor = 5;

update Directores set IMDB=0000233 where ID_director = 1;
update Directores set IMDB=1490123 where ID_director = 2;
update Directores set IMDB=0634240 where ID_director = 3;
update Directores set IMDB=0269463 where ID_director = 4;
update Directores set IMDB=0467646 where ID_director = 5;

update Peliculas set IMDB=0110912 where ID_pelicula = 1;
update Peliculas set IMDB=2820852 where ID_pelicula = 2;
update Peliculas set IMDB=0816692 where ID_pelicula = 3;
update Peliculas set IMDB=0371746 where ID_pelicula = 4;
update Peliculas set IMDB=0083944 where ID_pelicula = 5;

alter table ActorPeliculas modify ID_pelicula int not null;#primero las claves foraneas
alter table ActorPeliculas modify ID_actor int not null;

alter table peliculas modify ID_pelicula int not null;#va a dar fallo porque al ser primary key ya es no nula
alter table peliculas modify titulo tinytext not null;

alter table actores modify ID_actor int not null;#va a dar fallo porque al ser primary key ya es no nula
alter table actores modify nombre tinytext not null;

alter table directores modify ID_director int not null;
alter table directores modify nombre tinytext not null;

alter table actores add edad int(5);
alter table actores add check (edad > 0);
alter table actores add check (edad < 120);


alter table directores add edad int(5);
alter table directores add check (edad > 0);
alter table directores add check (edad < 120);

#los updateo
update Actores set edad=60 where ID_actor = 1;
update Actores set edad=48 where ID_actor = 2;
update Actores set edad=45 where ID_actor = 3;
update Actores set edad=50 where ID_actor = 4;
update Actores set edad=69 where ID_actor = 5;

update Directores set edad=52 where ID_director = 1;
update Directores set edad=38 where ID_director = 2;
update Directores set edad=45 where ID_director = 3;
update Directores set edad=49 where ID_director = 4;
update Directores set edad=84 where ID_director = 5;

#va a arrojar fallo
update Actores set edad=121 where ID_actor = 5;#mysql no hace check guay

select * from actores;
select nombre from peliculas,actores,actorpeliculas where actores.ID_actor = actorpeliculas.ID_actor and peliculas.ID_pelicula = actorpeliculas.ID_pelicula and titulo = 'La Guerra de las Galaxias';
select * from actores where edad > 50;
select ID_pelicula,titulo,peliculas.IMDB,nombre from peliculas,directores where peliculas.ID_director = directores.ID_director;
select count(ID_pelicula) from peliculas,directores where peliculas.ID_director = directores.ID_director group by directores.ID_director;
select ID_actor,nombre from actores where ID_actor not in (select ID_actor from actorpeliculas);
select actores.nombre,directores.nombre from actores,actorpeliculas,peliculas,directores where actores.ID_actor = actorpeliculas.ID_actor and peliculas.ID_pelicula = actorpeliculas.ID_pelicula and peliculas.ID_director = directores.ID_director;

create database UnivPeque;
use UnivPeque;
ALTER SCHEMA `univpeque`  DEFAULT COLLATE utf8_spanish_ci ;
#\. D:\UnivDDL.sql
#\. D:\UnivInsertPeque.sql

create database UnivGrande;
use UnivGrande;
ALTER SCHEMA `univgrande`  DEFAULT COLLATE utf8_spanish_ci ;


#\. D:\UnivDDL.sql
#\. D:\UnivInsertGrande.sql

#de la parte 1 a 7 easy
select * from docente;
select nombre from docente where nombre_dpto = 'Ingeniería Telemática';
select nombre from docente where nombre_dpto = 'Ingeniería Telemática' and salario > 70000;
select ID,nombre,salario,docente.nombre_dpto,edificio,presupuesto from docente,departamento where departamento.nombre_dpto = docente.nombre_dpto;
select * from materia,departamento where materia.nombre_dpto = departamento.nombre_dpto and materia.nombre_dpto = 'Ingeniería Telemática' and creditos = 3;
select materia.id_materia,materia.nombre from materia,cursa where cursa.id_materia = materia.id_materia and ID = 00128;
select nombre from docente  union select nombre from alumno_3ciclo order by nombre;
#parte 8
select nombre,null as 'Creditos' from docente#cojo a los docentes
union (select alumno_3ciclo.nombre,sum(materia.creditos) from cursa,alumno_3ciclo,materia #los uno con los alumnos y su suma de creditos
where cursa.ID = alumno_3ciclo.ID and cursa.id_materia = materia.id_materia group by alumno_3ciclo.nombre)#eliminando dobles
union (select alumno_3ciclo.nombre,0 as 'Creditos' from cursa,alumno_3ciclo,materia#union alumnos sin asignaturas
where alumno_3ciclo.ID not in (select ID from cursa)) order by nombre;#que no estén en cursa
#parte 9 igual que la 8 pero sin la union de alumnos sin matricula
select nombre,null as 'Creditos' from docente union (select alumno_3ciclo.nombre,sum(materia.creditos) 
from cursa,alumno_3ciclo,materia where cursa.ID = alumno_3ciclo.ID and cursa.id_materia = materia.id_materia
group by alumno_3ciclo.nombre) order by nombre;
#parte10
select distinct alumno_3ciclo.nombre from alumno_3ciclo,cursa,materia#para evitar dobles
where alumno_3ciclo.ID = cursa.ID and cursa.id_materia = materia.id_materia and materia.nombre_dpto = 'Ingeniería Telemática';#los que hayan cursado una asignatura de telematica
#parte11
select distinct docente.ID from docente,imparte where docente.ID not in (select ID from imparte);#docentes que no han impartido clase
#parte12
select distinct docente.nombre from docente,imparte where docente.ID not in (select ID from imparte);#igual que la parte 11 pero sacando nombre
#parte13
(SELECT 'Grupo/s con más alumnos' AS '', id_materia, id_grupo, cuatrimestre, anho, COUNT(ID) AS 'Nº Alumnos' FROM cursa GROUP BY id_materia, id_grupo, cuatrimestre, anho#los agrupo 
HAVING COUNT(ID) >= all (SELECT COUNT(id) FROM cursa GROUP BY id_materia, id_grupo, cuatrimestre, anho)) #saco grupos con más alumnos
UNION (SELECT 'Grupo/s con menos alumnos' AS '', id_materia, id_grupo, cuatrimestre, anho, COUNT(ID) AS 'Nº Alumnos' FROM cursa GROUP BY id_materia, id_grupo, cuatrimestre, anho#union y agrupacion de la otra parte
HAVING COUNT(ID) <= all (SELECT COUNT(id) FROM cursa GROUP BY id_materia, id_grupo, cuatrimestre, anho));#saco grupos con menos alumnos
#parte14
SELECT id_materia, id_grupo, cuatrimestre, anho, MAX(number) AS 'Nº alumnos' #a las tuplas agrupadas cojo la tupla con el numero maximo de alumnos
FROM (SELECT *, COUNT(id) AS number#cojo los grupos, cuento sus alumnos y los ordeno en orden descendiente
FROM cursa GROUP BY id_materia, id_grupo, cuatrimestre, anho ORDER BY COUNT(ID) desc) AS Obligatorio GROUP BY id_materia;#agrupo las tuplas por materias
#parte15 igual que antes pero solo hay que ordenar
(SELECT id_materia, id_grupo, cuatrimestre, anho, MAX(number) AS 'Nº alumnos' 
FROM (SELECT *, COUNT(id) AS number 
FROM cursa GROUP BY id_materia, id_grupo, cuatrimestre, anho ORDER BY COUNT(ID) desc) AS x GROUP BY id_materia) 
order by max(number) desc;
#parte16
(SELECT 'Grupo/s con más alumnos' AS '', id_materia, id_grupo, cuatrimestre, anho, COUNT(ID) AS 'Nº Alumnos' #cojo todos los grupos
FROM cursa GROUP BY id_materia, id_grupo, cuatrimestre, anho) order by count(id) desc limit 10;#ordeno por numero de alumnos y limito a 10

#CONSULTAS ADICIONALES PREVIAS
#consulta para sacar alumnos y materias matriculadas
select alumno_3ciclo.nombre,materia.nombre from cursa,alumno_3ciclo,materia where cursa.ID = alumno_3ciclo.ID and cursa.id_materia = materia.id_materia order by alumno_3ciclo.nombre;
#CONSULTA PARA SACAR TODOS LOS ALUMNOS INCLUIDOS LOS NO MATRICULADOS
(select alumno_3ciclo.nombre,materia.nombre from cursa,alumno_3ciclo,materia where cursa.ID = alumno_3ciclo.ID and cursa.id_materia = materia.id_materia order by alumno_3ciclo.nombre) union (select alumno_3ciclo.nombre,NULL from cursa,alumno_3ciclo,materia where alumno_3ciclo.ID not in (select ID from cursa));
#consulta para sacar alumnos y creditos matriculados en cursa
select alumno_3ciclo.nombre,sum(materia.creditos) from cursa,alumno_3ciclo,materia where cursa.ID = alumno_3ciclo.ID and cursa.id_materia = materia.id_materia group by alumno_3ciclo.nombre order by alumno_3ciclo.nombre;