create user 'daniel' identified by '0511';
create user 'daniel@localhost' identified by '0511';

create database practicas;
use practicas;

grant all on practicas.* to 'daniel';
grant all on practicas.* to 'daniel@localhost';

create table profesores (id int,dni varchar(9),nombre tinytext,apellidos tinytext);
create table alumnos (id int,dni varchar(9),nombre tinytext,apellidos tinytext);

insert into profesores (id,dni,nombre,apellidos) values (0, '012345678',"Profesor","1");
insert into profesores (id,dni,nombre,apellidos) values (1, '012345679',"Profesor","2");
insert into profesores (id,dni,nombre,apellidos) values (1, '012345688',"Profesor","3");

insert into alumnos (id,dni,nombre,apellidos) values (0, '012345678',"Alumno","1");
insert into alumnos (id,dni,nombre,apellidos) values (1, '012345688',"Alumno","2");
insert into alumnos (id,dni,nombre,apellidos) values (1, '012345679',"Alumno","3");

select * from profesores;
select * from alumnos;

drop table profesores;
drop table alumnos;


create table profesores (id int,dni varchar(9),nombre tinytext,apellidos tinytext,primary key (id));
create table alumnos (id int,dni varchar(9),nombre tinytext,apellidos tinytext, primary key(id));

insert into profesores (id,dni,nombre,apellidos) values (0, '012345678',"Profesor","1");
insert into profesores (id,dni,nombre,apellidos) values (1, '012345679',"Profesor","2");
insert into profesores (id,dni,nombre,apellidos) values (1, '012345688',"Profesor","3");

insert into alumnos (id,dni,nombre,apellidos) values (0, '012345678',"Alumno","1");
insert into alumnos (id,dni,nombre,apellidos) values (1, '012345688',"Alumno","2");
insert into alumnos (id,dni,nombre,apellidos) values (1, '012345679',"Alumno","3");

alter table profesores drop primary key;
alter table alumno drop primary key;

alter table profesores add primary key(id,dni);
alter table alumnos add primary key(id,dni);

insert into profesores (id,dni,nombre,apellidos) values (0, '012345678',"Profesor","1");
insert into profesores (id,dni,nombre,apellidos) values (1, '012345679',"Profesor","2");
insert into profesores (id,dni,nombre,apellidos) values (1, '012345688',"Profesor","3");
insert into profesores (id,dni,nombre,apellidos) values (1, '012345688',"Profesor","3");

insert into alumnos (id,dni,nombre,apellidos) values (0, '012345678',"Alumno","1");
insert into alumnos (id,dni,nombre,apellidos) values (1, '012345688',"Alumno","2");
insert into alumnos (id,dni,nombre,apellidos) values (1, '012345679',"Alumno","3");
insert into alumnos (id,dni,nombre,apellidos) values (1, '012345679',"Alumno","3");
